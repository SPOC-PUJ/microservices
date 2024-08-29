// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: proto/signal_runing_sum.proto

#include "proto/signal_runing_sum.pb.h"
#include "proto/signal_runing_sum.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace signal {

static const char* SignalService_method_names[] = {
  "/signal.SignalService/ComputeRuningSum",
};

std::unique_ptr< SignalService::Stub> SignalService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< SignalService::Stub> stub(new SignalService::Stub(channel, options));
  return stub;
}

SignalService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_ComputeRuningSum_(SignalService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status SignalService::Stub::ComputeRuningSum(::grpc::ClientContext* context, const ::signal::RuningSumRequest& request, ::signal::RuningSumResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::signal::RuningSumRequest, ::signal::RuningSumResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ComputeRuningSum_, context, request, response);
}

void SignalService::Stub::async::ComputeRuningSum(::grpc::ClientContext* context, const ::signal::RuningSumRequest* request, ::signal::RuningSumResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::signal::RuningSumRequest, ::signal::RuningSumResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ComputeRuningSum_, context, request, response, std::move(f));
}

void SignalService::Stub::async::ComputeRuningSum(::grpc::ClientContext* context, const ::signal::RuningSumRequest* request, ::signal::RuningSumResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ComputeRuningSum_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::signal::RuningSumResponse>* SignalService::Stub::PrepareAsyncComputeRuningSumRaw(::grpc::ClientContext* context, const ::signal::RuningSumRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::signal::RuningSumResponse, ::signal::RuningSumRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ComputeRuningSum_, context, request);
}

::grpc::ClientAsyncResponseReader< ::signal::RuningSumResponse>* SignalService::Stub::AsyncComputeRuningSumRaw(::grpc::ClientContext* context, const ::signal::RuningSumRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncComputeRuningSumRaw(context, request, cq);
  result->StartCall();
  return result;
}

SignalService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SignalService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SignalService::Service, ::signal::RuningSumRequest, ::signal::RuningSumResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](SignalService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::signal::RuningSumRequest* req,
             ::signal::RuningSumResponse* resp) {
               return service->ComputeRuningSum(ctx, req, resp);
             }, this)));
}

SignalService::Service::~Service() {
}

::grpc::Status SignalService::Service::ComputeRuningSum(::grpc::ServerContext* context, const ::signal::RuningSumRequest* request, ::signal::RuningSumResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace signal

