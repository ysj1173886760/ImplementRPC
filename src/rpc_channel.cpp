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

}