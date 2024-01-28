#include "Sender.h"
#include "Direct3D11Capturer.h"
#include "MFH264Encoder.h"

using namespace remotexs;

int main() {
    std::string sender_adress("0.0.0.0:50051");
    std::unique_ptr<IFrameEncoder> encoder = std::make_unique<MFH264Encoder>();
    std::unique_ptr<IScreenCapturer> capturer = std::make_unique<Direct3D11Capturer>(encoder);
    Sender sender(capturer);
    grpc::ServerBuilder builder;

    builder.AddListeningPort(sender_adress, grpc::InsecureServerCredentials());
    builder.RegisterService(&sender);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    std::cout << "Sender listening on " << sender_adress << std::endl;
    server->Wait();
    return 0;
}
