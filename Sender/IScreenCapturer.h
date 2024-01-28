#pragma once

#include <wtypes.h>
#include <vector>

namespace remotexs {

class IScreenCapturer
{
public:
    virtual ~IScreenCapturer() = default;

    virtual bool InitializeCapturer() = 0;
    virtual bool CaptureFrame(BYTE** encoded_frame_buffer) = 0;
};

} // namespace remotexs

