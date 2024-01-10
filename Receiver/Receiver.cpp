#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <iostream>
#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Global variables
IDXGIOutputDuplication* dxgiOutputDuplication = nullptr;
ID3D11Device* d3dDevice = nullptr;
ID3D11DeviceContext* d3dDeviceContext = nullptr;
ID3D11Texture2D* texture = nullptr;
IDXGISwapChain* swapChain = nullptr;

// Function to initialize DirectX and the screen capture
bool InitializeDirectX(HWND hWnd) {
    // Create DXGI Factory
    IDXGIFactory1* dxgiFactory;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory)))
        return false;

    // Get DXGI Adapter
    IDXGIAdapter* dxgiAdapter;
    if (FAILED(dxgiFactory->EnumAdapters(0, &dxgiAdapter))) {
        dxgiFactory->Release();
        return false;
    }

    // Get DXGI Device
    if (FAILED(D3D11CreateDevice(dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &d3dDevice, NULL, &d3dDeviceContext))) {
        dxgiAdapter->Release();
        dxgiFactory->Release();
        return false;
    }

    // Get DXGI Output
    IDXGIOutput* dxgiOutput;
    if (FAILED(dxgiAdapter->EnumOutputs(0, &dxgiOutput))) {
        dxgiAdapter->Release();
        dxgiFactory->Release();
        return false;
    }

    // Get DXGI Output Description
    DXGI_OUTPUT_DESC outputDesc;
    if (FAILED(dxgiOutput->GetDesc(&outputDesc))) {
        dxgiOutput->Release();
        dxgiAdapter->Release();
        dxgiFactory->Release();
        return false;
    }

    // Create DXGI Output Duplication
    IDXGIOutput1* dxgiOutput1;
    if (FAILED(dxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), (void**)&dxgiOutput1))) {
        dxgiOutput->Release();
        dxgiAdapter->Release();
        dxgiFactory->Release();
        return false;
    }

    if (FAILED(dxgiOutput1->DuplicateOutput(d3dDevice, &dxgiOutputDuplication))) {
        dxgiOutput1->Release();
        dxgiOutput->Release();
        dxgiAdapter->Release();
        dxgiFactory->Release();
        return false;
    }

    // DXGI swap chain and texture creation
    DXGI_OUTDUPL_DESC outputDescDupl;
    dxgiOutputDuplication->GetDesc(&outputDescDupl);

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = outputDescDupl.ModeDesc.Width;
    textureDesc.Height = outputDescDupl.ModeDesc.Height;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.ArraySize = 1;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.MipLevels = 1;

    if (FAILED(d3dDevice->CreateTexture2D(&textureDesc, NULL, &texture))) {
        dxgiOutputDuplication->Release();
        dxgiOutput1->Release();
        dxgiOutput->Release();
        dxgiAdapter->Release();
        dxgiFactory->Release();
        return false;
    }

    // DXGI swap chain creation
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = 1920; // Replace with appropriate width
    swapChainDesc.BufferDesc.Height = 1080; // Replace with appropriate height
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    if (FAILED(dxgiFactory->CreateSwapChain(d3dDevice, &swapChainDesc, &swapChain))) {
        // Error handling
        return false;
    }

    dxgiOutput1->Release();
    dxgiOutput->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();

    return true;
}

void RenderCapturedFrame(HWND hWnd) {
    // Get the captured frame
    IDXGIResource* desktopResource = nullptr;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;

    if (SUCCEEDED(dxgiOutputDuplication->AcquireNextFrame(500, &frameInfo, &desktopResource))) {
        if (frameInfo.LastPresentTime.QuadPart != 0) {
            ID3D11Texture2D* desktopTexture = nullptr;
            if (SUCCEEDED(desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&desktopTexture)))) {
                d3dDeviceContext->CopyResource(texture, desktopTexture);
                desktopTexture->Release();
            }
        }

        // Release the frame
        desktopResource->Release();
        dxgiOutputDuplication->ReleaseFrame();

        std::cout << "Frame captured" << std::endl;

        // Render captured frame onto the window
        if (texture) {
            ID3D11Texture2D* backBuffer = nullptr;
            if (SUCCEEDED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))) {
                ID3D11RenderTargetView* renderTargetView = nullptr;

                d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

                d3dDeviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

                D3D11_VIEWPORT vp;
                ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

                vp.Width = (float)1920;
                vp.Height = (float)1080;
                vp.MinDepth = 0.0f;
                vp.MaxDepth = 1.0f;
                vp.TopLeftX = 0;
                vp.TopLeftY = 0;

                d3dDeviceContext->RSSetViewports(1, &vp);

                if (renderTargetView) {
                    d3dDeviceContext->ClearRenderTargetView(renderTargetView, DirectX::Colors::Black);

                    // Copy captured frame onto the back buffer using CPU
                    D3D11_BOX box;
                    box.left = 0;
                    box.right = 1920;  // Use the acquired frame dimensions
                    box.top = 0;
                    box.bottom = 1080;
                    box.front = 0;
                    box.back = 1;

                    d3dDeviceContext->CopySubresourceRegion(backBuffer, 0, 0, 0, 0, texture, 0, &box);

                    renderTargetView->Release();

                    backBuffer->Release();

                    /*D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
                    ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
                    shaderResourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
                    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                    shaderResourceViewDesc.Texture2D.MipLevels = 1;

                    ID3D11ShaderResourceView* shaderResourceView = nullptr;

                    if (SUCCEEDED(d3dDevice->CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourceView))) {

                        std::cout << "Shader resource view created" << std::endl;

                        if (shaderResourceView) {
                            d3dDeviceContext->PSSetShaderResources(0, 1, &shaderResourceView);
                            d3dDeviceContext->Draw(3, 0);
                            shaderResourceView->Release();
                        }

                        std::cout << "Frame rendered" << std::endl;

                        backBuffer->Release();
                    }*/
                }
            }
        }
    }

    // Present the rendered frame
    d3dDeviceContext->Flush();
    d3dDeviceContext->ClearState();
    if (swapChain)
        swapChain->Present(1, 0);
}

// Window procedure for message handling
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    // Window creation
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ScreenCaptureWindowClass";

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(0, L"ScreenCaptureWindowClass", L"Screen Capture", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, NULL, NULL, hInstance, NULL);
    if (!hWnd) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Initialize DirectX and screen capture
    if (!InitializeDirectX(hWnd)) {
        MessageBox(NULL, L"DirectX Initialization Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Main message loop
    MSG msg = {};
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            RenderCapturedFrame(hWnd);
        }
    }

    // Release resources
    if (swapChain) swapChain->Release();
    if (texture) texture->Release();
    if (d3dDeviceContext) d3dDeviceContext->Release();
    if (d3dDevice) d3dDevice->Release();
    if (dxgiOutputDuplication) dxgiOutputDuplication->Release();

    return static_cast<int>(msg.wParam);
}
