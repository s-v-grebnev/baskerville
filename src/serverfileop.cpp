/*
 * Работа с файлами на стороне сервера
 */
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <regex>
#include <memory>
#include "soptions.hpp"
#include "serverfileop.hpp"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>

/*
 * Запись массива const void *content, длины const int content_len
 * в файл const std::string& filename в корзине const std::string& basketid
 */

bool FileOperator::PutFile(const std::string& filename,
		const std::string& basketid, const std::string& content) {

	try {
// Проверяем id корзины
		bool flag = false;
		for (auto v : options.baskets) {
			if (v == basketid)
				flag = true;
		}
		if (!flag) {
			throw InvalidBasket();
		}
// Формируем абсолютный путь до корзины
		std::string abs_filename = options.path + '/' + basketid;
// Пытаемся создать корзину
		if (mkdir(abs_filename.c_str(), 0777) == -1) {
			if (errno != EEXIST) {
				throw std::runtime_error("Error creating basket");
			}
		}
// Формируем абсолютный путь до файла, убрав на всякий случай все лишние слэши
		abs_filename = options.path + '/' + basketid + '/' +
				std::string(basename(const_cast<char*>(filename.c_str())));
// Перезаписываем контент, проверяем ошибки
		std::ofstream fout;
		fout.open(abs_filename,
				std::ios::binary | std::ios::out | std::ios::trunc);
		if (fout) {
			fout.write(static_cast<const char*>(content.c_str()), content.size());
			if (fout.bad()) {
				fout.close();
				throw std::runtime_error("Error saving file");
			}
			fout.close();

		} else
			throw std::runtime_error("Error saving file");

	} catch (const InvalidBasket & ex) {
		return false;
	} catch (const std::runtime_error & ex) {
		std::cout << "Error: " << ex.what() << std::endl;
		exit(1);
	}
	return true;
}

/*
 * Листинг корзины const std::string& basketid,
 * результат выводится в виде std::set<std::string>.
 */

std::vector<std::string> FileOperator::BasketLS(const std::string& basketid) {
	std::vector<std::string> result;
	try {
		for (auto v : options.baskets)
			if (v == basketid) {
				dirent *de;
				DIR *dp;
				errno = 0;
// Формируем абсолютный путь до корзины
				std::string abs_path = options.path + '/' + basketid;
				dp = opendir(abs_path.c_str());
// Если корзина существует -- пролистываем ее и записываем содержимое в set
				if (dp) {
					while (true) {
						errno = 0;
						de = readdir(dp);
						if (de == NULL)
							break;
						if ((std::string(de->d_name) != ".")
								&& (std::string(de->d_name) != ".."))
							result.push_back(std::string(de->d_name));
					}
					closedir(dp);
				}
			}
	} catch (const std::runtime_error & ex) {
		std::cout << ex.what() << std::endl;
	}
	return result;
}
