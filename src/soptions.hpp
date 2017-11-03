/*
 * Classes and methods for parsing the server config file
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
	std::set<std::string> baskets;
	std::string pubkey_file;

	bool parseFile(const std::string& filename);
};

struct FileError {
};
