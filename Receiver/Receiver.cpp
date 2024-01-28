#include "Receiver.h"

using namespace remotexs;

Receiver::Receiver(std::shared_ptr<grpc::Channel> channel, std::unique_ptr<IScreenRenderer>& renderer)
    : m_stub(SenderService::NewStub(channel)), m_renderer(std::move(renderer))
{
}

bool Receiver::StartCapturingFrames() const {
    grpc::ClientContext context;
    google::protobuf::Empty request;
    google::protobuf::Empty response;

    grpc::Status status = m_stub->StartCapturingFrames(&context, request, &response);

    if (!status.ok()) {
        MessageBox(NULL, L"Error while requesting capture start.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }
    
    return true;
}

bool Receiver::HandleCapturedFrame(HWND window) const {
    grpc::ClientContext context{};
    google::protobuf::Empty request{};
    Frame captured_frame{};

    grpc::Status status = m_stub->AcquireNextFrame(&context, request, &captured_frame);

    if (!status.ok()) {
        MessageBox(NULL, L"Error while handling frame.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    std::vector<uint8_t> encoded_frame_data(captured_frame.data().begin(), captured_frame.data().end());

    return m_renderer->RenderFrameOntoWindow(encoded_frame_data, window);
}
