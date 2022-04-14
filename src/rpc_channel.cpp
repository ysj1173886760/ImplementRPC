/**
 * @file rpc_channel.cpp
 * @author sheep
 * @brief implementation of TinyChannel
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "rpc_channel.h"
#include "logger.h"

#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>


TinyChannel::TinyChannel(std::string &server_addr, int port) {
    server_addr_ = server_addr;
    port_ = port;
}

TinyChannel::~TinyChannel() {
    close(socket_);
}

bool TinyChannel::Init() {
	struct sockaddr_in serv_addr;
	char recv_buffer[1024] = {0};
    char send_buffer[1024] = {0};

	if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("Socket Creation Error");
		return false;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, server_addr_.c_str(), &serv_addr.sin_addr) <= 0) {
        LOG_ERROR("Invalid address/ Address not supported");
		return false;
	}

	if (connect(socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        LOG_ERROR("Connection Failed");
		return false;
	}
}

void TinyChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::RpcController* controller,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure* done) {
    // serialize the request message
    std::string request_buffer = request->SerializeAsString();

    RpcMeta rpc_meta;
    rpc_meta.set_service_name(method->service()->name());
    rpc_meta.set_method_name(method->name());
    rpc_meta.set_data_size(request_buffer.size());

    std::string meta_buffer = rpc_meta.SerializeAsString();

    int rpc_meta_size = meta_buffer.size();

    std::string buffer;
    // serialize the rpc_meta_size
    buffer += std::string((char *)&rpc_meta_size, sizeof(int));
    // append meta data
    buffer += meta_buffer;
    // append request data, we don't need to append the length of request_buffer
    // since it has been stored in rpc_meta
    buffer += request_buffer;

    LOG_INFO("sending rpc request");
    LOG_INFO("service name: %s", rpc_meta.service_name().c_str());
    LOG_INFO("method name: %s", rpc_meta.method_name().c_str());
    LOG_INFO("meta size: %d", rpc_meta_size);
    LOG_INFO("request size: %d", rpc_meta.data_size());

    // since we are using bio here, so it's safe to put send buffer on stack
    send(socket_, buffer.c_str(), buffer.size(), 0);

    // receive response
    // first 4 byte is response size, then comes response data
    char resp_data_size[sizeof(int)];
    // bio here, recv the header
    recv(socket_, resp_data_size, 4, MSG_WAITALL);
    
    int resp_len = *((int *)resp_data_size);
    std::string resp_buffer;
    resp_buffer.resize(resp_len);

    // bio here, recv the data
    recv(socket_, (void *)resp_buffer.c_str(), resp_len, MSG_WAITALL);
    response->ParseFromString(resp_buffer);
}