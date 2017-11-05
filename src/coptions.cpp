/*
 * Классы и методы для разбора опций командной строки
 */

#include <iostream>
#include <getopt.h>
#include <string>
#include "coptions.hpp"

/*
 * Разбор опций командной строки
 * Аргументы -- передаются из аргументов main()
 */

void ClientOptions::ParseOptions(int argc, char **argv) {

	int opt;
	struct option longopts[] =
			{ { "host", 1, NULL, 'h' }, { "browse", 0, NULL, 'b' }, { "send", 0,
					NULL, 's' }, { "basketid", 1, NULL, 'i' }, { "filename", 1,
					NULL, 'f' }, { "key", 1, NULL, 'k' }, { 0, 0, 0, 0 } };
	if (argc <= 1) {
		std::cout << "Usage:  " << std::endl;
		std::cout
				<< "Send mode:   -s -h host:port -f filename -i basketid -k key "
				<< std::endl;
		std::cout << "Browse mode: -b -h host:port -i basketid " << std::endl;
		exit(1);

	}
	while ((opt = getopt_long(argc, argv, "bf:h:i:k:s", longopts, NULL)) != -1) {
		switch (opt) {
		case 'h':
			hostname = optarg;
			break;
		case 'b':
			mode = BROWSE;
			break;
		case 's':
			mode = SEND;
			break;
		case 'i':
			basketid = optarg;
			break;
		case 'f':
			filename = optarg;
			break;
		case 'k':
			key = optarg;
			break;
		case '?':
// Непонятная опция -- выводим подсказку и выходим
			std::cout << "Unknown option: " << opt << std::endl;
			std::cout << "Usage:  " << std::endl;
			std::cout << "Send mode:   -s -h host:port -f filename -i basketid -k key "
					<< std::endl;
			std::cout << "Browse mode: -b -h host:port -i basketid "
					<< std::endl;
			exit(1);
			//break;
		}

	}
}
