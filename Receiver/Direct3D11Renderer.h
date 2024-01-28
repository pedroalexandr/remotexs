#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include <DirectXColors.h>
#include <memory>
#include <vector>

#include "IScreenRenderer.h"
#include "IFrameDecoder.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace remotexs {

class Direct3D11Renderer : public IScreenRenderer
{
public:
    Direct3D11Renderer(std::unique_ptr<IFrameDecoder>& decoder);

    bool RenderFrameOntoWindow(const std::vector<uint8_t>& encoded_frame, HWND window) override;
private:
    std::unique_ptr<IFrameDecoder> m_decoder;
    ID3D11Device* m_d3d_device;
    ID3D11DeviceContext* m_d3d_device_context;
    IDXGISwapChain* m_swap_chain;
};

} // namespace remotexs

