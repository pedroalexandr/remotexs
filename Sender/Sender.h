#pragma once

#include <iostream>
#include <string>

#include "IScreenCapturer.h"

#include <grpcpp/grpcpp.h>
#include "sender_service.grpc.pb.h"

namespace remotexs {

class Sender final : public SenderService::Service {
public:
    Sender(std::unique_ptr<IScreenCapturer>& screen_capturer);

    grpc::Status StartCapturingFrames(
        grpc::ServerContext* context,
        const google::protobuf::Empty* request,
        google::protobuf::Empty* response
    ) override;

    grpc::Status AcquireNextFrame(
        grpc::ServerContext* context,
        const google::protobuf::Empty* request,
        Frame* response
    ) override;

    grpc::Status StopCapturingFrames(
        grpc::ServerContext* context,
        const google::protobuf::Empty* request,
        google::protobuf::Empty* response
    ) override;
private:
    std::unique_ptr<IScreenCapturer> m_capturer;
};

} // namespace remotexs
