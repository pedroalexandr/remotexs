#pragma once

#include "IFrameEncoder.h"

namespace remotexs {

class MFH264Encoder : public IFrameEncoder
{
public:
    bool Initialize(
        CComPtr<ID3D11Device>& device,
        CComPtr<IMFDXGIDeviceManager>& device_manager,
        UINT32 frame_width,
        UINT32 frame_height
    ) override;
    bool Encode(const CComPtr<ID3D11Texture2D>& surface, BYTE** encoded_frame_buffer) override;
    bool StopEncoding() override;
};

} // namespace remotexs

