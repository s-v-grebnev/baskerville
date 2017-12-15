#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/support/channel_arguments.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "basket.grpc.pb.h"

#include "coptions.hpp"
#include "rsa.hpp"

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

/* Реализация клиентской части протокола с помощью gRPC
 */

class BasketClient {
public:
	BasketClient(std::shared_ptr<Channel> channel) :
			stub_(BaskApi::NewStub(channel)) {
	}

/*
 * Клиентская часть процедуры листания
 */

	std::vector<std::string> BasketList(const std::string& basketid) {
		BasketListRequest request;
		request.set_basketid(basketid);
		BasketListResponse reply;
		ClientContext context;

		Status status = stub_->BasketList(&context, request, &reply);
		std::vector<std::string> result;
		if (status.ok()) {
			for (int i = 0; i < reply.filenames_size(); i++)
				result.push_back(reply.filenames(i));
		} else {
			std::cout << "GRPC error: " << status.error_message() << std::endl;
		}
		return result;
	}

/*
 * Клиентская часть процедуры отправки файла
 */

	std::string BasketPutFile(const std::string& basketid,
			const std::string& filename, const std::vector<char> content, const std::string &signature) {
		BasketPutFileRequest request;
		request.set_basketid(basketid);
		request.set_filename(filename);
		request.set_signature(signature);
		request.set_content(content.data(), content.size());
		BasketPutFileResponse reply;
		ClientContext context;

		Status status = stub_->BasketPutFile(&context, request, &reply);
		if (status.ok()) {
			return reply.success();
		} else {
			std::cout << "GRPC error: " << status.error_message() << std::endl;
			return "";
		}

	}

private:
	std::unique_ptr<BaskApi::Stub> stub_;
};

/* Вспомогательная функция для чтения содержимого файла в память
* Аргументы:
* const std::string& filename -- имя файла
* В случае ошибки чтения вызывает exit(1)
*/

std::vector<char> ReadFileContents(const std::string& filename) {
	size_t length;
	std::vector<char> res;
	try {
		struct stat st;
		stat(filename.c_str(), &st);
		length = st.st_size;
		std::ifstream input(filename, std::ios::binary);
		if (!input)
			throw std::runtime_error("cannot open file " + filename);
		res.resize(length);
		input.read(res.data(), length);
		if (input.fail())
			throw std::runtime_error("error reading contents of " + filename);
		input.close();
	} catch (const std::runtime_error& ex) {
		std::cout << "Error: " << ex.what() << std::endl;
		exit(1);
	}
	return res;
}

int main(int argc, char** argv) {
	ClientOptions options;
// Разбираем командную строку
	options.ParseOptions(argc, argv);
// Магия gRPC
	BasketClient client(
			grpc::CreateChannel(options.hostname,
					grpc::InsecureChannelCredentials()));
	if (options.mode == BROWSE) {  // режим листания
// Вызываем метод gRPC
		std::vector<std::string> lst = client.BasketList(options.basketid);
// Выводим ответ
		for (auto v : lst)
			std::cout << v << std::endl;
	} else if (options.mode == SEND) { // режим отправки
// Создаем криптопровайдера, загружаем секретный ключ
		RSAspace::RSASignProvider signer;
		signer.LoadKey(options.key);
// Загружаем и подписываем контент
		std::vector<char> content = ReadFileContents(options.filename);
		std::string signature = signer.RSASignBase64(content);
		if (signature == "") {
			std::cout << "Internal error in signature" << std::endl;
			exit(1);
		}
// Вызываем метод gRPC
// Предварительно преобразуем имя файла, убрав пути
		std::string res = client.BasketPutFile(options.basketid,
				std::string(basename(const_cast<char*>(options.filename.c_str()))),
				content, signature);
		if(res != ""){
			std::cout << res << std::endl;
// Выводим результат
		}

	}

	return 0;
}
