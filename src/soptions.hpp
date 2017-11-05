/*
 * Разбор конфиг-файла сервера
 */

#pragma once
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <regex>
#include <cstdlib>

struct ServerOptions {
	std::string path;
	std::string port = "10050";
	std::set<std::string> baskets;
	std::string pubkey_file;
	ServerOptions() {
	}
	ServerOptions& operator =(const ServerOptions& right) {
		if (this == &right) {
			return *this;
		}
		path = right.path;
		pubkey_file = right.pubkey_file;
		baskets = right.baskets;
		port = right.port;
		return *this;
	}
	ServerOptions(const std::string& filename) {
		ParseFile(filename);
	}
	bool ParseFile(const std::string& filename);
};

