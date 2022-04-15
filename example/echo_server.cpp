#include "logger.h"
#include "echo.pb.h"
#include "rpc_server.h"

class EchoServiceImpl : public EchoService {
public:
    EchoServiceImpl() {};
    virtual ~EchoServiceImpl() {};
    virtual void Echo(::google::protobuf::RpcController* controller,
                        const ::EchoRequest* request,
                        ::EchoResponse* response,
                        ::google::protobuf::Closure* done) {
        LOG_INFO("received request %s", request->message().c_str());
        response->set_message(request->message());
        done->Run();
    }
};

int main() {
    auto service = new EchoServiceImpl;
    TinyRpcServer server;
    server.add(service);
    server.start(8080);
    return 0;
}