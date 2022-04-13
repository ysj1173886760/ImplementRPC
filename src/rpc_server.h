/**
 * @file rpc_server.h
 * @author sheep
 * @brief header if TinyRpcServer
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef RPC_SERVER_H
#define RPC_SERVER_H

#include <string>
#include "google/protobuf/service.h"

class TinyRpcServer {
public:
    TinyRpcServer() {}
    ~TinyRpcServer() {}

    void start(std::string &server_addr);
    bool add(::google::protobuf::Service *service);
};

#endif