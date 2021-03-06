// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: basket.proto
#ifndef GRPC_basket_2eproto__INCLUDED
#define GRPC_basket_2eproto__INCLUDED

#include "basket.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace BasketApi {

class BaskApi final {
 public:
  static constexpr char const* service_full_name() {
    return "BasketApi.BaskApi";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status BasketList(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::BasketApi::BasketListResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketListResponse>> AsyncBasketList(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketListResponse>>(AsyncBasketListRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketListResponse>> PrepareAsyncBasketList(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketListResponse>>(PrepareAsyncBasketListRaw(context, request, cq));
    }
    virtual ::grpc::Status BasketPutFile(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::BasketApi::BasketPutFileResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketPutFileResponse>> AsyncBasketPutFile(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketPutFileResponse>>(AsyncBasketPutFileRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketPutFileResponse>> PrepareAsyncBasketPutFile(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketPutFileResponse>>(PrepareAsyncBasketPutFileRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketListResponse>* AsyncBasketListRaw(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketListResponse>* PrepareAsyncBasketListRaw(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketPutFileResponse>* AsyncBasketPutFileRaw(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::BasketApi::BasketPutFileResponse>* PrepareAsyncBasketPutFileRaw(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status BasketList(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::BasketApi::BasketListResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketListResponse>> AsyncBasketList(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketListResponse>>(AsyncBasketListRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketListResponse>> PrepareAsyncBasketList(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketListResponse>>(PrepareAsyncBasketListRaw(context, request, cq));
    }
    ::grpc::Status BasketPutFile(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::BasketApi::BasketPutFileResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketPutFileResponse>> AsyncBasketPutFile(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketPutFileResponse>>(AsyncBasketPutFileRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketPutFileResponse>> PrepareAsyncBasketPutFile(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketPutFileResponse>>(PrepareAsyncBasketPutFileRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketListResponse>* AsyncBasketListRaw(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketListResponse>* PrepareAsyncBasketListRaw(::grpc::ClientContext* context, const ::BasketApi::BasketListRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketPutFileResponse>* AsyncBasketPutFileRaw(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::BasketApi::BasketPutFileResponse>* PrepareAsyncBasketPutFileRaw(::grpc::ClientContext* context, const ::BasketApi::BasketPutFileRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::RpcMethod rpcmethod_BasketList_;
    const ::grpc::RpcMethod rpcmethod_BasketPutFile_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status BasketList(::grpc::ServerContext* context, const ::BasketApi::BasketListRequest* request, ::BasketApi::BasketListResponse* response);
    virtual ::grpc::Status BasketPutFile(::grpc::ServerContext* context, const ::BasketApi::BasketPutFileRequest* request, ::BasketApi::BasketPutFileResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_BasketList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_BasketList() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_BasketList() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BasketList(::grpc::ServerContext* context, const ::BasketApi::BasketListRequest* request, ::BasketApi::BasketListResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestBasketList(::grpc::ServerContext* context, ::BasketApi::BasketListRequest* request, ::grpc::ServerAsyncResponseWriter< ::BasketApi::BasketListResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_BasketPutFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_BasketPutFile() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_BasketPutFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BasketPutFile(::grpc::ServerContext* context, const ::BasketApi::BasketPutFileRequest* request, ::BasketApi::BasketPutFileResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestBasketPutFile(::grpc::ServerContext* context, ::BasketApi::BasketPutFileRequest* request, ::grpc::ServerAsyncResponseWriter< ::BasketApi::BasketPutFileResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_BasketList<WithAsyncMethod_BasketPutFile<Service > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_BasketList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_BasketList() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_BasketList() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BasketList(::grpc::ServerContext* context, const ::BasketApi::BasketListRequest* request, ::BasketApi::BasketListResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_BasketPutFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_BasketPutFile() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_BasketPutFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BasketPutFile(::grpc::ServerContext* context, const ::BasketApi::BasketPutFileRequest* request, ::BasketApi::BasketPutFileResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_BasketList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_BasketList() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::StreamedUnaryHandler< ::BasketApi::BasketListRequest, ::BasketApi::BasketListResponse>(std::bind(&WithStreamedUnaryMethod_BasketList<BaseClass>::StreamedBasketList, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_BasketList() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status BasketList(::grpc::ServerContext* context, const ::BasketApi::BasketListRequest* request, ::BasketApi::BasketListResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedBasketList(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::BasketApi::BasketListRequest,::BasketApi::BasketListResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_BasketPutFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_BasketPutFile() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::StreamedUnaryHandler< ::BasketApi::BasketPutFileRequest, ::BasketApi::BasketPutFileResponse>(std::bind(&WithStreamedUnaryMethod_BasketPutFile<BaseClass>::StreamedBasketPutFile, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_BasketPutFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status BasketPutFile(::grpc::ServerContext* context, const ::BasketApi::BasketPutFileRequest* request, ::BasketApi::BasketPutFileResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedBasketPutFile(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::BasketApi::BasketPutFileRequest,::BasketApi::BasketPutFileResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_BasketList<WithStreamedUnaryMethod_BasketPutFile<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_BasketList<WithStreamedUnaryMethod_BasketPutFile<Service > > StreamedService;
};

}  // namespace BasketApi


#endif  // GRPC_basket_2eproto__INCLUDED
