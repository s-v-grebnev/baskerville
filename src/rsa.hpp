#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "base64.hpp"


/*
class RSAProvider{
private:
    RSA * rsa = NULL;
public:

    ~RSAProvider(){
        if(rsa!=NULL)
            free(rsa);
    }
};
*/



class RSASignProvider{
private:
	RSASignProvider() {};
    RSA * rsa = NULL;
    bool createPrivateRSA(std::string key);
    bool RSASign( const char* Msg,
                  size_t MsgLen,
                  unsigned char** EncMsg,
                  size_t* MsgLenEnc);
public:
    RSASignProvider(const std::string& keyfile);
    ~RSASignProvider(){
    	if(rsa!=NULL)
        free(rsa);
    }
    std::string RSASignBase64(const char* Msg, size_t MsgLen);
};

class RSAVerifyProvider{
private:
    RSAVerifyProvider() {};
	RSA * rsa = NULL;
    bool createPublicRSA(std::string key);
    bool RSAVerify( unsigned char* MsgHash,
                             size_t MsgHashLen,
                             const char* Msg,
                             size_t MsgLen,
                             bool* Authentic);
public:
    RSAVerifyProvider(const std::string& keyfile);
    ~RSAVerifyProvider(){
        	if(rsa!=NULL)
            free(rsa);
        }
    bool RSAVerifyBase64(const std::string signature, const char* Msg,
            size_t MsgLen);
};
