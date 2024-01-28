#pragma once

#include <vector>

namespace remotexs {

class IFrameDecoder {
public:
    virtual ~IFrameDecoder() = default;
    virtual bool Decode(const std::vector<uint8_t>& encoded_frame, void* decoded_frame) = 0;
};

} // namespace remotexs
