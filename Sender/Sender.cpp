#include <wincodec.h>

#include "Sender.h"

using namespace remotexs;

Sender::Sender(std::unique_ptr<IScreenCapturer>& screen_capturer)
    : m_capturer(std::move(screen_capturer))
{
}

grpc::Status Sender::StartCapturingFrames(
    grpc::ServerContext* context,
    const google::protobuf::Empty* request,
    google::protobuf::Empty* response
) {
    std::cout << "Initializing screen capturer..." << std::endl;
    return m_capturer->InitializeCapturer() ? grpc::Status::OK : grpc::Status::CANCELLED;
}

grpc::Status Sender::AcquireNextFrame(
    grpc::ServerContext* context,
    const google::protobuf::Empty* request,
    Frame* response
) {
    std::cout << "Acquiring next frame..." << std::endl;

    BYTE* captured_encoded_frame_buffer = nullptr;
    if (!m_capturer->CaptureFrame(&captured_encoded_frame_buffer))
        return grpc::Status::CANCELLED;

    response->set_data(captured_encoded_frame_buffer, 1920 * 1080 * 4);

    std::cout << "Frame processed." << std::endl;

    return grpc::Status::OK;
}

grpc::Status remotexs::Sender::StopCapturingFrames(
    grpc::ServerContext* context,
    const google::protobuf::Empty* request,
    google::protobuf::Empty* response
) {
    return grpc::Status();
}
