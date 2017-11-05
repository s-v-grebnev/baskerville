/*
 * Classes and methods for digital signature context creation, generation, verification
 */
#include <iostream>
#include <fstream>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "rsa.hpp"
#include "base64.hpp"

namespace RSAspace {

static constexpr size_t KEYREAD_BUFSIZE = 10000;

RSASignProvider::RSASignProvider(const std::string& keyfile) {
	try {
		std::ifstream fh;
		char buf[KEYREAD_BUFSIZE];
		std::string s;
		fh.open(keyfile);
		if (!fh) {
			throw std::string("error opening keyfile");
		}
		fh.getline(buf, KEYREAD_BUFSIZE, '\t');
		fh.close();
		s = buf;
		if (!createPrivateRSA(s)) {
			throw std::string("error loading RSA private key from file");
		}
	} catch (const std::string ex) {
		std::cout << "Cannot continue: " << ex << std::endl;
		exit(1);
	}
}

bool RSASignProvider::createPrivateRSA(std::string key) {
	rsa = NULL;
	const char* c_string = key.c_str();
	BIO * keybio = BIO_new_mem_buf((void*) c_string, -1);
	if (keybio == NULL) {
		return false;
	}
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	return (rsa != NULL);
}

bool RSASignProvider::RSASign(const char* Msg, size_t MsgLen,
		unsigned char** EncMsg, size_t* MsgLenEnc) {
	EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
	EVP_PKEY* priKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(priKey, rsa);
	if (EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha512(), NULL, priKey)
			<= 0) {
		return false;
	}
	if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
		return false;
	}
	if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <= 0) {
		return false;
	}
	*EncMsg = (unsigned char*) malloc(*MsgLenEnc);
	if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
		return false;
	}
//  EVP_MD_CTX_cleanup(m_RSASignCtx);
	return true;
}

std::string RSASignProvider::RSASignBase64(const char* Msg, size_t MsgLen) {
	unsigned char *sigbuf;
	char *b64;
	size_t siglen;
	if (!RSASign(Msg, MsgLen, &sigbuf, &siglen)) {
		return "";
	}
	Base64Encode(sigbuf, siglen, &b64);
	std::string result = b64;
	free(b64);
	return result;
}

RSAVerifyProvider::RSAVerifyProvider(const std::string& keyfile) {
	try {
		std::ifstream fh;
		char buf[KEYREAD_BUFSIZE];
		std::string s;
		fh.open(keyfile);
		if (!fh) {
			throw std::string("error opening keyfile");
		}
		fh.getline(buf, KEYREAD_BUFSIZE, '\t');
		fh.close();
		s = buf;
		if (!createPublicRSA(s)) {
			throw std::string("error loading RSA public key from file");
		}
	} catch (const std::string ex) {
		std::cout << "Cannot continue: " << ex << std::endl;
		exit(1);
	}
}

bool RSAVerifyProvider::createPublicRSA(std::string key) {
	rsa = NULL;
	BIO *keybio;
	const char* c_string = key.c_str();
	keybio = BIO_new_mem_buf((void*) c_string, -1);
	if (keybio == NULL) {
		return false;
	}
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	return (rsa != NULL);
}

bool RSAVerifyProvider::RSAVerify(unsigned char* MsgHash, size_t MsgHashLen,
		const char* Msg, size_t MsgLen, bool* Authentic) {
	try {
		*Authentic = false;
		EVP_PKEY* pubKey = EVP_PKEY_new();
		EVP_PKEY_assign_RSA(pubKey, rsa);
		EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();
		if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha512(), NULL,
				pubKey) <= 0) {
			throw std::string("internal error in signature");
		}
		if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
			throw std::string("internal error in signature");
		}
		int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash,
				MsgHashLen);
		if (AuthStatus == 1) {
			*Authentic = true;
			//   EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
			return true;
		} else if (AuthStatus == 0) {
			*Authentic = false;
			//   EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
			return true;
		} else {
			*Authentic = false;
			//   EVP_MD_CTX_cleanup(m_RSAVerifyCtx);
			throw std::string("internal error in signature");
		}

	} catch (std::string & ex) {
		std::cout << "Internal error in signature";
		exit(1);
	}

}

bool RSAVerifyProvider::RSAVerifyBase64(const std::string signature,
		const char* Msg, size_t MsgLen) {
	unsigned char* MsgHash;
	size_t MsgHashLen;
	bool Authentic;
	Base64Decode(signature.c_str(), &MsgHash, &MsgHashLen);
	RSAVerify(MsgHash, MsgHashLen, Msg, MsgLen, &Authentic);
	return (Authentic);
}

/*
 int zmain(){
 std::ifstream fh;
 char buf[2000];
 std::string s;
 bool correct;

 RSASignProvider rsp("secret.key.pem");
 RSAVerifyProvider rvp("public.key.pem");

 char data[1024*1024];

 for(int i = 0; i < 1024*1024; i++) {data[i] = i % 256;}

 std::string siga = rsp.RSASignBase64(data, 1024*1024);
 std::cout << siga << std::endl;

 correct = rvp.RSAVerifyBase64(siga, reinterpret_cast<const char*>(data), 1024*1024);

 std::cout << correct << std::endl;

 data[1] += 1;

 correct = rvp.RSAVerifyBase64(siga, reinterpret_cast<const char*>(data), 1024*1024);

 std::cout << correct << std::endl;

 siga = rsp.RSASignBase64(data, 1024*1024);
 std::cout << siga << std::endl;
 correct = rvp.RSAVerifyBase64(siga, reinterpret_cast<const char*>(data), 1024*1024);

 std::cout << correct << std::endl;

 return 0;
 }
 */
}
