/*
 * Classes and methods for parsing the client command line
 */

#pragma once

#include <iostream>
#include <getopt.h>
#include <string>

constexpr bool BROWSE = false;
constexpr bool SEND = true;

struct ClientOptions {
	std::string hostname = "";
	bool mode = BROWSE; // true -- send, false -- browse
	std::string basketid = "";
	std::string filename = "";
	std::string key = "";
	void ParseOptions(int argc, char **argv);
};
