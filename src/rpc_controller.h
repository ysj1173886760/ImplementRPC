/**
 * @file rpc_controller.h
 * @author header of TinyRpcController
 * @brief 
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef RPC_CONTROLLER_H
#define RPC_CONTROLLER_H

#include "google/protobuf/service.h"

// An RpcController mediates a single method call.  The primary purpose of
// the controller is to provide a way to manipulate settings specific to the
// RPC implementation and to find out about RPC-level errors.
//
// The methods provided by the RpcController interface are intended to be a
// "least common denominator" set of features which we expect all
// implementations to support.  Specific implementations may provide more
// advanced features (e.g. deadline propagation).
class TinyRpcController: public ::google::protobuf::RpcController {
public:
    TinyRpcController();
    virtual ~TinyRpcController() {}

    // Client-side methods ---------------------------------------------
    // These calls may be made from the client side only.  Their results
    // are undefined on the server side (may crash).

    // Resets the RpcController to its initial state so that it may be reused in
    // a new call.  Must not be called while an RPC is in progress.
    void Reset();

    // After a call has finished, returns true if the call failed.  The possible
    // reasons for failure depend on the RPC implementation.  Failed() must not
    // be called before a call has finished.  If Failed() returns true, the
    // contents of the response message are undefined.
    bool Failed() const;

    // If Failed() is true, returns a human-readable description of the error.
    std::string ErrorText() const;

    // Advises the RPC system that the caller desires that the RPC call be
    // canceled.  The RPC system may cancel it immediately, may wait awhile and
    // then cancel it, or may not even cancel the call at all.  If the call is
    // canceled, the "done" callback will still be called and the RpcController
    // will indicate that the call failed at that time.
    void StartCancel();

    // Server-side methods ---------------------------------------------
    // These calls may be made from the server side only.  Their results
    // are undefined on the client side (may crash).

    // Causes Failed() to return true on the client side.  "reason" will be
    // incorporated into the message returned by ErrorText().  If you find
    // you need to return machine-readable information about failures, you
    // should incorporate it into your response protocol buffer and should
    // NOT call SetFailed().
    void SetFailed(const std::string& reason);

    // If true, indicates that the client canceled the RPC, so the server may
    // as well give up on replying to it.  The server should still call the
    // final "done" callback.
    bool IsCanceled() const;

    // Asks that the given callback be called when the RPC is canceled.  The
    // callback will always be called exactly once.  If the RPC completes without
    // being canceled, the callback will be called after completion.  If the RPC
    // has already been canceled when NotifyOnCancel() is called, the callback
    // will be called immediately.
    //
    // NotifyOnCancel() must be called no more than once per request.
    void NotifyOnCancel(::google::protobuf::Closure* callback);

private:
    bool is_failed_;
    std::string error_code_;

};

#endif