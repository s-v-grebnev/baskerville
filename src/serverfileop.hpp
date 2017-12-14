/*
 * Работа с файлами на стороне сервера
 */
#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <regex>
#include <cstdlib>
#include "soptions.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

class FileOperator {
public:
	FileOperator(const ServerOptions& ioptions) {
		options = ioptions;
	}
	;
	bool PutFile(const std::string& filename, const std::string& basketid,
			const char * content, const int content_len);
	std::vector<std::string> BasketLS(const std::string& basketid);

private:
	ServerOptions options;
};

// Специальные исключения
struct InvalidBasket {
};
struct DirectoryError {
};
