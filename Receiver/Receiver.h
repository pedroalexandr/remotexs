#pragma once

#include <memory>
#include <grpcpp/grpcpp.h>

#include "resource.h"

#include "sender_service.grpc.pb.h"
#include "IScreenRenderer.h"

namespace remotexs {

class Receiver {
public:
    Receiver(std::shared_ptr<grpc::Channel> channel, std::unique_ptr<IScreenRenderer>& renderer);
    bool StartCapturingFrames() const;
    bool HandleCapturedFrame(HWND window) const;
private:
    std::unique_ptr<SenderService::Stub> m_stub;
    std::unique_ptr<IScreenRenderer> m_renderer;
};

} // namespace remotexs
