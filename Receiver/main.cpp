#include "Receiver.h"
#include "MFH264Decoder.h"
#include "Direct3D11Renderer.h"

using namespace remotexs;

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

    HWND hWnd = CreateWindowEx(0, L"ScreenCaptureWindowClass", L"Screen Capture", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    if (!hWnd) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    std::unique_ptr<IFrameDecoder> decoder = std::make_unique<MFH264Decoder>();
    std::unique_ptr<IScreenRenderer> renderer = std::make_unique<Direct3D11Renderer>(decoder);
    Receiver client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()), renderer);

    if (!client.StartCapturingFrames())
        return -1;

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
            client.HandleCapturedFrame(hWnd);
        }
    }

    return static_cast<int>(msg.wParam);
}
