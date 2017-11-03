#pragma once

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <regex>
#include <cstdlib>
#include "soptions.hpp"

class FileOperator {
public:
	FileOperator(const ServerOptions& ioptions) {
		options = ioptions;
	};
	bool PutFile(const std::string& filename, const std::string& basketid,
			const void *content, const int content_len);
	std::set<std::string> BasketLS(const std::string& basketid);

private:
	ServerOptions options;
};

struct InvalidBasket {};
struct DirectoryError{};
