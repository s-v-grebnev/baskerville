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

// Размер буфера для чтения ключей
static constexpr size_t KEYREAD_BUFSIZE = 10000;

/*
 * Конструктор, инициализирующий криптопровайдер подписи
 * ключом из const std::string& keyfile
 */

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

/*
 * Загрузка секретного ключа RSA из строки (формата PEM)
 */

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

/*
 * Метод, реализующий подпись сообщения const char* Msg длины size_t MsgLen,
 * подпись помещается в буфер unsigned char** EncMsg длины size_t* MsgLenEnc
 */

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

/*
 * Метод, реализующий выработку подписи сообщения const char* Msg длины size_t MsgLen
 * в строку в формате base64
 */

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

/*
 * Конструктор, инициализирующий криптопровайдер проверки
 * ключом из const std::string& keyfile
 */


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

/*
 * Загрузка открытого ключа RSA из строки (формата PEM)
 */

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

/*
 * Метод, реализующий проверку подписи unsigned char* MsgSig длины size_t MsgSigLen,
 * от сообщения const char* Msg длины size_t MsgLen,
 * результат записывается в bool* Authentic
 */

bool RSAVerifyProvider::RSAVerify(unsigned char* MsgSig, size_t MsgSigLen,
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
		int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgSig,
				MsgSigLen);
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

/*
 * Метод, реализующий проверку ифыу64-кодированной подписи const std::string signature
 * от сообщения const char* Msg длины size_t MsgLen
 */

bool RSAVerifyProvider::RSAVerifyBase64(const std::string signature,
		const char* Msg, size_t MsgLen) {
	unsigned char* MsgSig;
	size_t MsgSigLen;
	bool Authentic;
	Base64Decode(signature.c_str(), &MsgSig, &MsgSigLen);
	RSAVerify(MsgSig, MsgSigLen, Msg, MsgLen, &Authentic);
	free(MsgSig);
	return (Authentic);
}


} // namespace
