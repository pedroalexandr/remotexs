#pragma once

#include <wtypes.h>
#include <mfobjects.h>
#include <atlbase.h>
#include <vector>
#include <d3d11.h>

namespace remotexs {

class IFrameEncoder {
public:
    virtual ~IFrameEncoder() = default;

    virtual bool Initialize(
        CComPtr<ID3D11Device>& device,
        CComPtr<IMFDXGIDeviceManager>& device_manager,
        UINT32 frame_width,
        UINT32 frame_height
    ) = 0;
    virtual bool Encode(const CComPtr<ID3D11Texture2D>& surface, BYTE** encoded_frame_buffer) = 0;
    virtual bool StopEncoding() = 0;
};

} // namespace remotexs