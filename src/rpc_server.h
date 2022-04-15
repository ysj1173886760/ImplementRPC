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
#include <queue>
#include <condition_variable>
#include <mutex>
#include <unordered_map>

#include "google/protobuf/service.h"

class AsyncQueue {
public:
    std::queue<int> q_;
    std::condition_variable consumer_;
    std::mutex mu_;

    AsyncQueue() {}

    void push(int fd) {
        std::unique_lock<std::mutex> lock_guard(mu_);
        q_.push(fd);
        consumer_.notify_one();
    }

    int pop() {
        std::unique_lock<std::mutex> lock_guard(mu_);
        while (q_.empty()) {
            consumer_.wait(lock_guard, [&]() { return !q_.empty(); });
        }
        int fd = q_.front();
        q_.pop();
        return fd;
    }
};

class TinyRpcServer {
public:
    TinyRpcServer() {}
    ~TinyRpcServer() {}

    void start(int port);
    bool add(::google::protobuf::Service *service);

private:
    AsyncQueue async_queue_;
    struct ServiceInfo {
        ::google::protobuf::Service *service_;
        // method name -> method descriptor
        std::unordered_map<std::string, 
            const ::google::protobuf::MethodDescriptor *> method_lookup_tbl_;
    };
    std::unordered_map<std::string, ServiceInfo> service_lookup_tbl_;

    void worker_thread();
    void work(int fd);
    void callback(int fd, ::google::protobuf::Message *response_msg);
};

#endif