/*
 * Классы и методы для работы с цифровой подписью RSA
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include "rsa.hpp"
#include "base64.hpp"
#include <memory>

using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using EVP_MD_CTX_ptr = std::unique_ptr<EVP_MD_CTX, decltype(&::EVP_MD_CTX_free)>;
using BIO_MEM_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;

namespace RSAspace {

// Размер буфера для чтения ключей
static constexpr size_t KEYREAD_BUFSIZE = 10000;

/*
 * Конструктор, инициализирующий криптопровайдер подписи
 * ключом из const std::string& keyfile
 */

void RSASignProvider::LoadKey(const std::string& keyfile) {
	try {
		std::ifstream fh;
		char buf[KEYREAD_BUFSIZE];
		std::string s;
		fh.open(keyfile);
		if (!fh) {
			throw std::runtime_error("error opening keyfile");
		}
		fh.getline(buf, KEYREAD_BUFSIZE, '\t');
		fh.close();
		s = buf;
		BIO_MEM_ptr keybio (BIO_new_mem_buf((void*) s.c_str(), -1), ::BIO_free);
		if (keybio.get() == NULL) {
			throw std::runtime_error("error loading RSA private key from file");
		}
		rsa = std::move(RSA_ptr(PEM_read_bio_RSAPrivateKey(keybio.get(), NULL, NULL, NULL), ::RSA_free));
		if (rsa.get() == NULL) {
			throw std::runtime_error("error loading RSA private key from file");
		}
	} catch (const std::runtime_error & ex) {
		std::cout << "Cannot continue: " << ex.what() << std::endl;
		exit(1);
	}
}

/*
 * Метод, реализующий подпись сообщения const char* Msg длины size_t MsgLen,
 * подпись помещается в буфер unsigned char** EncMsg длины size_t* MsgLenEnc
 */

std::vector<char> RSASignProvider::RSASign(const std::vector<char> Msg) {
	std::vector<char> res;
	size_t MsgLenEnc;
	EVP_MD_CTX_ptr m_RSASignCtx(EVP_MD_CTX_create(), ::EVP_MD_CTX_free);
	EVP_KEY_ptr priKey (EVP_PKEY_new(), ::EVP_PKEY_free);
	EVP_PKEY_set1_RSA(priKey.get(), rsa.get());
	if (EVP_DigestSignInit(m_RSASignCtx.get(), NULL, EVP_sha512(), NULL, priKey.get())
			<= 0) {
		return res;
	}
	if (EVP_DigestSignUpdate(m_RSASignCtx.get(), Msg.data(), Msg.size()) <= 0) {
		return res;
	}
	if (EVP_DigestSignFinal(m_RSASignCtx.get(), NULL, &MsgLenEnc) <= 0) {
		return res;
	}
	res.resize(MsgLenEnc);
	if (EVP_DigestSignFinal(m_RSASignCtx.get(), reinterpret_cast<unsigned char*>(res.data()), &MsgLenEnc) <= 0) {
		res.clear();
	}
	return res;
}

/*
 * Метод, реализующий выработку подписи сообщения const char* Msg длины size_t MsgLen
 * в строку в формате base64
 */

std::string RSASignProvider::RSASignBase64(const std::vector<char> Msg) {

	std::vector<char> signature = RSASign(Msg);

	if (signature.empty()) {
		return "";
	}
	std::string result = base64_encode(reinterpret_cast<const unsigned char*>(signature.data()), signature.size());
	return result;
}

/*
 * Конструктор, инициализирующий криптопровайдер проверки
 * ключом из const std::string& keyfile
 */

void RSAVerifyProvider::LoadKey(const std::string & keyfile){
	try {
		std::ifstream fh;
		char buf[KEYREAD_BUFSIZE];
		std::string s;
		fh.open(keyfile);
		if (!fh) {
			throw std::runtime_error("error opening keyfile");
		}
		fh.getline(buf, KEYREAD_BUFSIZE, '\t');
		fh.close();
		s = buf;

		BIO_MEM_ptr keybio (BIO_new_mem_buf((void*) s.c_str(), -1), ::BIO_free);
		if (keybio.get() == NULL) {
			throw std::runtime_error("error loading RSA public key from file");
			}
		rsa = std::move(RSA_ptr(PEM_read_bio_RSA_PUBKEY(keybio.get(), NULL, NULL, NULL), ::RSA_free));
		if (rsa.get() == NULL) {
				throw std::runtime_error("error loading RSA public key from file");
		}
	} catch (const std::runtime_error & ex) {
		std::cout << "Cannot continue: " << ex.what() << std::endl;
		exit(1);
	}
}

/*
 * Метод, реализующий проверку подписи unsigned char* MsgSig длины size_t MsgSigLen,
 * от сообщения const char* Msg длины size_t MsgLen,
 * результат записывается в bool* Authentic
 */

bool RSAVerifyProvider::RSAVerify(const std::string& Sig, const std::string&  Msg) {
	try {
		EVP_KEY_ptr pubKey (EVP_PKEY_new(), ::EVP_PKEY_free);
		EVP_PKEY_set1_RSA(pubKey.get(), rsa.get());
		EVP_MD_CTX_ptr m_RSAVerifyCtx ( EVP_MD_CTX_create(), ::EVP_MD_CTX_free);
		if (EVP_DigestVerifyInit(m_RSAVerifyCtx.get(), NULL, EVP_sha512(), NULL,
				pubKey.get()) <= 0) {
			throw std::runtime_error("internal error in signature");
		}
		if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx.get(), (const unsigned char*)(Msg.c_str()),
				Msg.size()) <= 0) {
			throw std::runtime_error("internal error in signature");
		}
		int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx.get(), (const unsigned char*)Sig.c_str(),
				Sig.size());
		if (AuthStatus == 1) {
			return true;
		} else if (AuthStatus == 0) {
			return false;
		} else {
			throw std::runtime_error("internal error in signature");
		}
	} catch (std::runtime_error & ex) {
		std::cout << ex.what() << std::endl;
		exit(1);
	}
}

/*
 * Метод, реализующий проверку base64-кодированной подписи const std::string signature
 * от сообщения const char* Msg длины size_t MsgLen
 */

bool RSAVerifyProvider::RSAVerifyBase64(const std::string & signature,
		const std::string & content) {

	std::string sig64  = base64_decode(signature);

	return (RSAVerify(sig64, content));
}


} // namespace
