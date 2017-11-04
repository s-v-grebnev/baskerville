#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <regex>
#include "soptions.hpp"
#include "serverfileop.hpp"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

bool FileOperator::PutFile(const std::string& filename,
		const std::string& basketid, const void *content,
		const int content_len) {

	try {
// check that basket id is ok
		bool flag = false;
		for (auto v : options.baskets) {
			if (v == basketid)
				flag = true;
		}
		if (!flag) {
			throw InvalidBasket();
		}

		std::string abs_filename = options.path + '/' + basketid;

		mkdir(abs_filename.c_str(), 0777); // error check

		abs_filename = options.path + '/' + basketid + '/' + filename;

		std::ofstream fout;
		fout.open(abs_filename, std::ios::binary | std::ios::out);
		if (fout) {
			fout.write(static_cast<const char*>(content), content_len);
			if (fout.bad()) {
				fout.close();
				throw std::string("Error saving file");
			}
			fout.close();

		} else
			throw std::string("Error saving file");

	} catch (const InvalidBasket & ex) {
		return false;
	} catch (const std::string & ex) {
		std::cout << "Error: " << ex << std::endl;
		exit(1);
	}
	return true;
}

std::set<std::string> FileOperator::BasketLS(const std::string& basketid) {
	std::set<std::string> result;
	for (auto v : options.baskets)
		if (v == basketid) {
			dirent *de;
			DIR *dp;
			errno = 0;
			std::string abs_path = options.path + '/' + basketid;
			dp = opendir(abs_path.empty() ? "." : abs_path.c_str());
			if (dp) {
				while (true) {
					errno = 0;
					de = readdir(dp);
					if (de == NULL)
						break;
					if ((std::string(de->d_name) != ".")
							&& (std::string(de->d_name) != ".."))
						result.insert(std::string(de->d_name));

				}
				closedir(dp);
			} else {
				mkdir(abs_path.c_str(), 0777);
			}

		}
	return result;
}
