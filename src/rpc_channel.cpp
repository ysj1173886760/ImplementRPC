#include "rpc_channel.h"


TinyChannel::TinyChannel(std::string &server_addr, int port) {

}

void TinyChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::RpcController* controller,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure* done) {

}