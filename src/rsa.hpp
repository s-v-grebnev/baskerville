#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <memory>
using std::unique_ptr;

#include "base64.hpp"

using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;

namespace RSAspace {

// Криптопровайдер подписи

class RSASignProvider {
private:
	RSA_ptr rsa;
	bool RSASign(const char* Msg, size_t MsgLen, unsigned char** EncMsg,
			size_t* MsgLenEnc);
public:
	RSASignProvider() : rsa(nullptr, ::RSA_free) {};
	void LoadKey(const std::string& keyfile);
 	~RSASignProvider() {}
	std::string RSASignBase64(const char* Msg, size_t MsgLen);
};

// Криптопровайдер проверки

class RSAVerifyProvider {
private:
	RSA_ptr rsa;
	bool RSAVerify(char* MsgHash, size_t MsgHashLen, const char* Msg,
			size_t MsgLen, bool* Authentic);
public:
	RSAVerifyProvider() : rsa(nullptr, ::RSA_free) {};
	void LoadKey(const std::string& keyfile);
	~RSAVerifyProvider() {}
	bool RSAVerifyBase64(const std::string & signature, const char* Msg,
			size_t MsgLen);
};

} // namespace
