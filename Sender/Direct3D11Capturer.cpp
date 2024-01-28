#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "Winmm.lib")

// std
#include <iostream>
#include <string>

// Windows
#include <windows.h>
#include <atlbase.h>

// DirectX
#include <d3d11.h>
#include <dxgi1_2.h>

#include "Direct3D11Capturer.h"

// Simplified error handling via macros
#ifdef DEBUG
#define ON_ERROR __debugbreak(); return false;
#else
#define ON_ERROR system("pause"); return false;
#endif
#define CHECK(x, err) if (!x) { std::cerr << err << std::endl; ON_ERROR; }
#define CHECK_HR(x, err) if (FAILED(x)) { std::cerr << err << std::endl; ON_ERROR; }

const int kFrameRate = 60;
const int kSurfaceWidth = 1920;
const int kSurfaceHeight = 1080;

CComPtr<IMFDXGIDeviceManager> m_device_manager;
CComPtr<ID3D11Device> m_device;
CComPtr<ID3D11DeviceContext> m_device_context;
CComPtr<IDXGIOutputDuplication> m_desktop_dup;

using namespace remotexs;

Direct3D11Capturer::Direct3D11Capturer(std::unique_ptr<IFrameEncoder>& encoder)
    : m_encoder(std::move(encoder))
{
}

bool Direct3D11Capturer::InitializeCapturer()
{
    HRESULT hr;

    D3D_DRIVER_TYPE DriverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

    D3D_FEATURE_LEVEL FeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_1
    };
    UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

    D3D_FEATURE_LEVEL FeatureLevel;

    CComPtr<IDXGIFactory1> dxgiFactory;
    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory);
    CHECK_HR(hr, "Failed to create DXGI factory");

    // Get system's default adapter
    CComPtr<IDXGIAdapter> dxgiAdapter;
    hr = dxgiFactory->EnumAdapters(0, &dxgiAdapter);
    CHECK_HR(hr, "Failed to enumerate adapters.");

    for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex)
    {
        hr = D3D11CreateDevice(nullptr, DriverTypes[DriverTypeIndex], nullptr,
            D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
            FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, &m_device, &FeatureLevel, &m_device_context);

        if (SUCCEEDED(hr))
            break;
    }

    CHECK_HR(hr, "Failed to create device");

    // ------------------------------------------------------------------------
    // Prepare DXGI Desktop Duplication for screen capturing
    // ------------------------------------------------------------------------
    CComPtr<IDXGIOutput> dxgiOutput;
    hr = dxgiAdapter->EnumOutputs(0, &dxgiOutput);
    CHECK_HR(hr, "Failed to enumerate outputs.");

    // Create DXGI Output Duplication
    CComPtr<IDXGIOutput1> dxgiOutput1;
    hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), (void**)&dxgiOutput1);
    CHECK_HR(hr, "Failed to query interface.");

    hr = dxgiOutput1->DuplicateOutput(m_device, &m_desktop_dup);
    CHECK_HR(hr, "Failed to duplicate output.");

    // ------------------------------------------------------------------------
    // Initialize encoder
    // ------------------------------------------------------------------------
    if (!m_encoder->Initialize(m_device, m_device_manager, kSurfaceWidth, kSurfaceHeight))
        return false;

    return true;
}

bool Direct3D11Capturer::CaptureFrame(BYTE** encoded_frame_buffer)
{
    std::cout << "Capturing frame..." << std::endl;

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = kSurfaceWidth;
    textureDesc.Height = kSurfaceHeight;
    textureDesc.Format = DXGI_FORMAT_NV12;
    textureDesc.ArraySize = 1;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.MipLevels = 1;

    HRESULT hr;
    CComPtr<ID3D11Texture2D> frame_surface;
    hr = m_device->CreateTexture2D(&textureDesc, NULL, &frame_surface);
    CHECK_HR(hr, "Could not create surface");

    // Acquire the next frame using DXGI Desktop Duplication API
    IDXGIResource* desktop_resource = nullptr;
    DXGI_OUTDUPL_FRAME_INFO frame_info;

    hr = m_desktop_dup->AcquireNextFrame(500, &frame_info, &desktop_resource);
    CHECK_HR(hr, "Failed to acquire next frame.");

    // If contains valid data, copy it to the frame surface
    if (frame_info.LastPresentTime.QuadPart != 0) {
        CComPtr<ID3D11Texture2D> desktopTexture;
        hr = desktop_resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&desktopTexture));
        m_device_context->CopyResource(frame_surface, desktopTexture);
    }

    CHECK_HR(hr, "Failed to query interface.");

    // Release the frame
    desktop_resource->Release();
    m_desktop_dup->ReleaseFrame();

    // ------------------------------------------------------------------------
    // Encode the frame surface
    // ------------------------------------------------------------------------
    if (!m_encoder->Encode(frame_surface, encoded_frame_buffer)) {
        std::cerr << "Could not encode frame." << std::endl;
        return false;
    }

    return true;
}
