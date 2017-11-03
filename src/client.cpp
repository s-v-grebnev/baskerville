#include <iostream>
#include <memory>
#include <string>
#include <set>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/support/channel_arguments.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "basket.grpc.pb.h"

using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
using grpc::Status;
//using grpc::InsecureChannelCredentials;

using BasketApi::BasketListRequest;
using BasketApi::BasketListResponse;
using BasketApi::BasketPutFileRequest;
using BasketApi::BasketPutFileResponse;
using BasketApi::BaskApi;

class BasketClient {
 public:
  BasketClient(std::shared_ptr<Channel> channel)
      : stub_(BaskApi::NewStub(channel)) {}

  std::set<std::string> BasketList(const std::string& basketid) {
	BasketListRequest request;
    request.set_basketid(basketid);
    BasketListResponse reply;
    ClientContext context;

    Status status = stub_->BasketList(&context, request, &reply);
	std::set<std::string> result;
    if (status.ok()) {
    	  for(int i = 0; i < reply.filenames_size(); i++)
    		  result.insert(reply.filenames(i));
    }
    return result;
  }

  bool BasketPutFile(const std::string& basketid,
		  	  	  	  const std::string& filename,
					  const void * content,
					  size_t content_len,
					  const std::string &signature)
  {
  	BasketPutFileRequest request;
      request.set_basketid(basketid);
      request.set_filename(filename);
      request.set_signature(signature);
      request.set_content(content, content_len);
      BasketPutFileResponse reply;
      ClientContext context;

      Status status = stub_->BasketPutFile(&context, request, &reply);
      return reply.success();
    }

 private:
  std::unique_ptr<BaskApi::Stub> stub_;
};

int main(int argc, char** argv) {
  BasketClient client(
      grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));// ,ChannelArguments()));
  std::set<std::string> reply = client.BasketList("user");

  return 0;
}
