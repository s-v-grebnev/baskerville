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

#include "soptions.hpp"
#include "serverfileop.hpp"
#include "rsa.hpp"

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

		std::cout << request->basketid();
		std::set<std::string> answer;
		FileOperator fop(options);
		answer = fop.BasketLS(request->basketid());
		for (auto v : answer)
			reply->add_filenames(v);
		return Status::OK;
	}

	Status BasketPutFile(ServerContext* context,
			const BasketPutFileRequest* request, BasketPutFileResponse* reply)
					override {
		bool result = false;
		FileOperator fop(options);
		RSAspace::RSAVerifyProvider rsa(options.pubkey_file);
		std::string signature = request->signature();
		const char * content = request->content().c_str();
		int content_len = request->content().size();
		bool Authentic = rsa.RSAVerifyBase64(signature, content, content_len);

		if (Authentic) {
			result = fop.PutFile(request->filename(), request->basketid(),
					static_cast<const void*>(content), content_len);
			reply->set_success(
					result ? "Saved file successfully" : "Failed to save file");
		} else {
			reply->set_success("Failed to save file: incorrect signature");
		}
		return Status::OK;
	}
private:
	ServerOptions options;
public:
	explicit BasketServiceImpl() {
		options.ParseFile("config.txt");
	}
	std::string GetPort() {
		return options.port;
	}
};

void RunServer() {
	std::string server_address("0.0.0.0");
	BasketServiceImpl service;
	server_address = server_address + ":" + service.GetPort();

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
}

int main(int argc, char** argv) {

	RunServer();
	return 0;

}

