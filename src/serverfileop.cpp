#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <regex>
#include "soptions.hpp"
#include "serverfileop.hpp"

#include <dirent.h>
#include <sys/types.h>


bool FileOperator::putFile(const std::string& filename,
		const std::string& basketid, const void *content,
		const int content_len) {

	try {
// check for appropriate basket
		bool flag = false;
		for (auto v : options.baskets) {
			if (v == basketid)
				flag = true;
		}
		if (!flag) {
			throw InvalidBasket();
		}

		std::string abs_filename = options.path + '/' +  basketid + '/' + filename;

		std::ofstream fout;
		fout.open(abs_filename, std::ios::binary | std::ios::out);
		if(fout){
			fout.write(static_cast<const char*> (content), content_len);
			if(fout.bad()){
				fout.close();
				throw FileError();
			}
// XXX check for output!
			fout.close();

		}
		else throw FileError();

	}
	catch(const InvalidBasket & ex){
		return false;
	}
	catch (const FileError & ex) {
		std::cout << "Error doing something\n";
		exit(1);
	}
	return true;
}

std::set<std::string> FileOperator::BasketLS(const std::string& basketid){
	std::set<std::string> result;
	dirent *de;
	DIR *dp;
	errno = 0;
	std::string abs_path = options.path + '/' +  basketid;
	dp = opendir(abs_path.empty()?".":abs_path.c_str());
	if(dp){
		while(true)
		{
			errno  = 0;
			de = readdir(dp);
			if(de == NULL) break;
			if((std::string(de->d_name) != "." ) && (std::string(de->d_name) != ".." ))
				result.insert(std::string(de->d_name));

		}
		closedir(dp);
	}
	return result;
}
