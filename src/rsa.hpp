#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "base64.hpp"

namespace RSAspace {

class RSASignProvider {
private:

	RSA * rsa = NULL;
	bool RSASign(const char* Msg, size_t MsgLen, unsigned char** EncMsg,
			size_t* MsgLenEnc);
public:
	RSASignProvider() {
	}
	;
	bool createPrivateRSA(std::string key);
	RSASignProvider(const std::string& keyfile);
	~RSASignProvider() {
		if (rsa != NULL)
			free(rsa);
	}
	std::string RSASignBase64(const char* Msg, size_t MsgLen);
};

class RSAVerifyProvider {
private:

	RSA * rsa = NULL;
	bool RSAVerify(unsigned char* MsgHash, size_t MsgHashLen, const char* Msg,
			size_t MsgLen, bool* Authentic);
public:

	RSAVerifyProvider() {
	}
	;
	RSAVerifyProvider(const std::string& keyfile);
	~RSAVerifyProvider() {
		if (rsa != NULL)
			free(rsa);
	}
	bool createPublicRSA(std::string key);

	bool RSAVerifyBase64(const std::string signature, const char* Msg,
			size_t MsgLen);
};

}
