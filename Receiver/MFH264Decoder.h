#pragma once

#include "IFrameDecoder.h"

namespace remotexs {

class MFH264Decoder : public IFrameDecoder
{
    public:
    MFH264Decoder() = default;
    ~MFH264Decoder() = default;

    bool Decode(const std::vector<uint8_t>& encoded_frame, void* decoded_frame) override;
};

} // namespace remotexs


