/*
 * Classes and methods for parsing the server config file
 */
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <regex>
#include <cstdlib>
#include "soptions.hpp"

bool ServerOptions::ParseFile(const std::string& filename) {
	try {
		path = "";
		pubkey_file = "";
		baskets.clear();
		std::string buf;
		std::ifstream fh(filename);
		if (!fh) {
			throw std::string("cannot open config file");
		}
		while (!fh.eof()) {
			getline(fh, buf);
			std::regex r("(.*) = (.*)");
			std::smatch m;
			auto pos = buf.cbegin();
			auto end = buf.cend();
			if (!std::regex_search(pos, end, m, r)) { // Trash in config file
				continue;
			}
			if (m.str(1) == "Path") {
				path = m.str(2);
			} else if (m.str(1) == "Pubkey") {
				pubkey_file = m.str(2);
			} else if (m.str(1) == "Basket") {
				baskets.insert(m.str(2));
			}
		}
		fh.close();
		if ((path == "") || (pubkey_file == "") || (baskets.empty()))
			throw std::string("incomplete config file");
	} catch (const std::string & ex) {
		std::cout << "Error reading server config:" << ex << std::endl;
		exit(1);
	}
	return true;
}

