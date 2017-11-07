#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <thread>

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
#include <mutex>
#include <unistd.h>

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

std::mutex m_;

/*
 * Реализация серверной части протокола с помощью gRPC
 */

class BasketServiceImpl final : public BaskApi::Service {

	/*
	 * Режим листания, серверная часть
	 */

	Status BasketList(ServerContext* context, const BasketListRequest* request,
			BasketListResponse* reply) override {
		std::vector<std::string> answer;
		FileOperator fop(options);
		m_.lock();
		answer = fop.BasketLS(request->basketid());
		m_.unlock();
		for (auto v : answer)
			reply->add_filenames(v);
		return Status::OK;
	}

	/*
	 * Режим отправки, серверная часть
	 */

	Status BasketPutFile(ServerContext* context,
			const BasketPutFileRequest* request, BasketPutFileResponse* reply)
					override {
		bool result = false;
		FileOperator fop(options);
		m_.lock();
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
		m_.unlock();
		return Status::OK;
	}
private:
	ServerOptions options;

public:
	explicit BasketServiceImpl() {
// В конструкторе надо распарсить конфиг-файл
		options.ParseFile("config.txt");
		if (mkdir(options.path.c_str(), 0777) == -1) {
			if (errno != EEXIST) {
				std::cout << "Cannot create basket directory" << std::endl;
				exit(1);
			}
		}
	}
	/*
	 * Метод, возвращающий рабочий порт -- чтобы не таскать глобальные переменные
	 */
	std::string GetPort() {
		return options.port;
	}
};

/*
 * Функция, запускающая сервер
 */

void RunServer() {
	std::string server_address("0.0.0.0");
	BasketServiceImpl service;
// формируем параметры для создания сокета
	server_address = server_address + ":" + service.GetPort();
	ServerBuilder builder;
// Магия gRPC
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

