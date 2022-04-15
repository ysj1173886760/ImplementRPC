/**
 * @file rpc_controller.cpp
 * @author sheep
 * @brief implementation of TinyRpcController
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "rpc_controller.h"

// reset the error code
TinyRpcController::TinyRpcController() {
    Reset();
}

void TinyRpcController::Reset() {
    is_failed_ = false;
    error_code_ = "";
}

bool TinyRpcController::Failed() const {
    return is_failed_;
}

std::string TinyRpcController::ErrorText() const {
    return error_code_;
}

// currently, we don't support this operation. 
// i.e. canceling the RPC
void TinyRpcController::StartCancel() {
    // do nothing
}

// Server-side methods ---------------------------------------------
void TinyRpcController::SetFailed(const std::string& reason) {
    is_failed_ = true;
    error_code_ = reason;
}

// not supported
bool TinyRpcController::IsCanceled() const {
    return false;
}

// notify the callback is canceled, not supported
void TinyRpcController::NotifyOnCancel(::google::protobuf::Closure* callback) {
    // do nothing
}