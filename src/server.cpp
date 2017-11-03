#include <iostream>
#include <memory>
#include <string>
#include <set>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/server_builder.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/support/channel_arguments.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/server_credentials.h>
#include "basket.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
//using grpc::InsecureChannelCredentials;

using BasketApi::BasketListRequest;
using BasketApi::BasketListResponse;
using BasketApi::BasketPutFileRequest;
using BasketApi::BasketPutFileResponse;
using BasketApi::BaskApi;

class BasketServiceImpl final : public BaskApi::Service {
  Status BasketList(ServerContext* context, const BasketListRequest* request,
                  BasketListResponse* reply) override {
// request -> вытащить аргументы
	  std::set<std::string> answer;

//do the stuff!

	  for (auto v : answer) reply->add_filenames(v);
    return Status::OK;
  }

  Status BasketPutFile(ServerContext* context, const BasketPutFileRequest* request,
		  BasketPutFileResponse* reply) override {
	  bool result;
	  reply->set_success(result);

    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  BasketServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}


