/*
 * Классы и методы для работы с цифровой подписью RSA
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <memory>
using std::unique_ptr;

#include "base64.hpp"

using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;

namespace RSAspace {

// Криптопровайдер подписи

class RSAProvider{
protected:
	RSA_ptr rsa;
public:
	RSAProvider() : rsa(nullptr, ::RSA_free) {};
//	virtual void LoadKey(const std::string& keyfile);
	~RSAProvider() {}
};

class RSASignProvider : private RSAProvider{
private:
	std::vector<char> RSASign(const std::vector<char> Msg);
public:
	void LoadKey(const std::string& keyfile);
 	~RSASignProvider() {}
	std::string RSASignBase64(const std::vector<char> Msg);
};

// Криптопровайдер проверки

class RSAVerifyProvider : private RSAProvider{
private:
	bool RSAVerify(const std::string& Sig, const std::string& Msg);
public:
	void LoadKey(const std::string& keyfile);
	~RSAVerifyProvider() {}
	bool RSAVerifyBase64(const std::string & signature, const std::string& content);
};

} // namespace
