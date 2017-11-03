/*
 * Classes and methods for parsing the client command line
 */

#include <iostream>
#include <getopt.h>
#include <string>
#include "coptions.hpp"

void ClientOptions::ParseOptions(int argc, char **argv) {

	int opt;
	struct option longopts[] = {
		{	"host", 1, NULL, 'h'},
		{	"browse", 0, NULL, 'b'},
		{	"send", 0, NULL, 's'},
		{	"basketid", 1, NULL, 'i'},
		{	"filename", 1, NULL, 'f'},
		{	"key", 1, NULL, 'k'},
		{	0,0,0,0}
	};
	if(argc <=1){
		std::cout << "Usage:  " << std::endl;
		std::cout << "Send mode:   -s -h host:port -f filename -i basketid -k key " << std::endl;
		std::cout << "Browse mode: -b -h host:port -i basketid " << std::endl;
		exit(1);

	}
	while((opt = getopt_long(argc, argv, "bf:h:i:k:s", longopts, NULL)) != -1) {
		switch(opt) {
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
				std::cout << "Unknown option: "<< opt << std::endl;
				std::cout << "Usage:  " << std::endl;
				std::cout << "Send mode:   -s -h host:port -f filename -i basketid -k key " << std::endl;
				std::cout << "Browse mode: -b -h host:port -i basketid " << std::endl;
				exit(1);
				//break;
		}

	}
}
/*
using namespace std;

int umain(int argc, char ** argv) {
		ClientOptions options;
		options.ParseOptions(argc, argv);

		cout << "hostname:" <<  options.hostname << endl;
		cout << "mode:" << (options.mode == BROWSE?"browse":"send") << endl;
		cout << "basketid:" << options.basketid << endl;
		cout << "filename: "<< options.filename  << endl;
		cout << "key:" << options.key << endl;

		return 0;
	}
*/
