/*
 * Классы и методы для разбора опций командной строки
 */

#pragma once

#include <iostream>
#include <getopt.h>
#include <string>

// Для удобства дадим режимам названия

constexpr bool BROWSE = false;
constexpr bool SEND = true;

// Опции клиента

struct ClientOptions {
	std::string hostname = "";
	bool mode = BROWSE; // true -- send, false -- browse
	std::string basketid = "";
	std::string filename = "";
	std::string key = "";
	void ParseOptions(int argc, char **argv);
};
