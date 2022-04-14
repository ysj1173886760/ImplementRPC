/**
 * @file rpc_server.cpp
 * @author sheep
 * @brief implementation of TinyRpcServer
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "rpc_server.h"
#include "logger.h"

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>

#include <thread>
#include <condition_variable>

void TinyRpcServer::start(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int workers = 3;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        LOG_INFO("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt))) {
        LOG_INFO("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
       
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        LOG_INFO("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, workers) < 0) {
        LOG_INFO("listen failed");
        exit(EXIT_FAILURE);
    }

    // spawn worker thread
    std::vector<std::thread> list;
    for (int i = 0; i < workers; i++) {
        list.emplace_back(std::thread(&TinyRpcServer::worker_thread, this));
    }

    LOG_INFO("server listening");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            continue;
        }
        async_queue_.push(new_socket);
    }
}

bool TinyRpcServer::add(::google::protobuf::Service *service) {
    ServiceInfo service_info;
    const ::google::protobuf::ServiceDescriptor *service_descriptor = service->GetDescriptor();
    for (int i = 0; i < service_descriptor->method_count(); i++) {
        service_info.method_lookup_tbl_[service_descriptor->method(i)->name()] = 
            service_descriptor->method(i);
    }

    service_info.service_ = service;
    service_lookup_tbl_[service_descriptor->name()] = service_info;
    return true;
}

void TinyRpcServer::worker_thread() {
    while (true) {
        int fd = async_queue_.pop();
        work(fd);
        close(fd);
    }
}

void TinyRpcServer::work(int fd) {
    
}