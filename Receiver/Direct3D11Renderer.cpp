#include <iostream>

#include "Direct3D11Renderer.h"

using namespace remotexs;

Direct3D11Renderer::Direct3D11Renderer(std::unique_ptr<IFrameDecoder>& decoder)
    :   m_decoder(std::move(decoder)), m_d3d_device(nullptr), m_d3d_device_context(nullptr),
        m_swap_chain(nullptr)
{
}

bool Direct3D11Renderer::RenderFrameOntoWindow(const std::vector<uint8_t>& encoded_frame, HWND window)
{
    // Render captured frame onto the window
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.BufferDesc.Width = 1920;  // Set the appropriate width
    swap_chain_desc.BufferDesc.Height = 1080; // Set the appropriate height
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.OutputWindow = window;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.Windowed = TRUE;

    if (FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_d3d_device, nullptr, &m_d3d_device_context))) {
        std::cerr << "Failed to create D3D11 device and swap chain." << std::endl;
        return false;
    }

    ID3D11Texture2D* back_buffer = nullptr;
    if (SUCCEEDED(m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)))) {
        ID3D11Texture2D* texture = nullptr;
        m_decoder->Decode(encoded_frame, texture);

        ID3D11RenderTargetView* render_target_view = nullptr;
        if (FAILED(m_d3d_device->CreateRenderTargetView(texture, nullptr, &render_target_view))) {
            std::cerr << "Failed to create render target view." << std::endl;
            return false;
        }

        m_d3d_device_context->OMSetRenderTargets(1, &render_target_view, nullptr);

        D3D11_VIEWPORT vp;
        ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

        vp.Width = (float)1920;
        vp.Height = (float)1080;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;

        m_d3d_device_context->RSSetViewports(1, &vp);

        if (render_target_view) {
            m_d3d_device_context->ClearRenderTargetView(render_target_view, DirectX::Colors::Black);

            // Copy captured frame onto the back buffer using CPU
            D3D11_BOX box;
            box.left = 0;
            box.right = 1920;  // Use the acquired frame dimensions
            box.top = 0;
            box.bottom = 1080;
            box.front = 0;
            box.back = 1;

            m_d3d_device_context->CopySubresourceRegion(back_buffer, 0, 0, 0, 0, texture, 0, &box);

            render_target_view->Release();

            back_buffer->Release();
        }
    }

    // Present the rendered frame
    m_d3d_device_context->Flush();
    m_d3d_device_context->ClearState();

    if (m_swap_chain)
        m_swap_chain->Present(1, 0);
}
