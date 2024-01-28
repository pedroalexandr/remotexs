#pragma once

#include <memory>

#include "IScreenCapturer.h"
#include "IFrameEncoder.h"
#include <mfobjects.h>

namespace remotexs {

class Direct3D11Capturer : public IScreenCapturer
{
public:
    Direct3D11Capturer(std::unique_ptr<IFrameEncoder>& encoder);
    ~Direct3D11Capturer() = default;
    Direct3D11Capturer(const Direct3D11Capturer&) = delete;
    Direct3D11Capturer(Direct3D11Capturer&&) = delete;
    Direct3D11Capturer& operator=(const Direct3D11Capturer&) = delete;
    Direct3D11Capturer& operator=(Direct3D11Capturer&&) = delete;

    bool InitializeCapturer() override;
    bool CaptureFrame(BYTE** encoded_frame_buffer) override;
private:
    std::unique_ptr<IFrameEncoder> m_encoder;
};

} // namespace remotexs


