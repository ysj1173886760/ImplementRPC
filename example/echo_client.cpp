#include "rpc_controller.h"
#include "rpc_channel.h"
#include "echo.pb.h"
#include "logger.h"

int main() {

    EchoRequest request;
    request.set_message("hello world");

    TinyChannel channel("127.0.0.1", 8080);
    if (!channel.Init()) {
        LOG_ERROR("failed to initialize channel");
        return 0;
    }

    auto stub = EchoService_Stub(&channel);

    while (true) {
        EchoResponse response;
        TinyRpcController rpc_controller;
        stub.Echo(&rpc_controller, &request, &response, nullptr);
        LOG_INFO("received response %s", response.message().c_str());
    }
    
    return 0;
}