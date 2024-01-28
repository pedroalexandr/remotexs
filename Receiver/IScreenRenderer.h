#pragma once

#include <vector>
#include <wtypes.h>

namespace remotexs {

class IScreenRenderer
{
public:
    virtual ~IScreenRenderer() = default;

    virtual bool RenderFrameOntoWindow(const std::vector<uint8_t>& encoded_frame, HWND window) = 0;
};

} // namespace remotexs