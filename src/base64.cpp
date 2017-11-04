/*
 * Methods for base64
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
//#include <openssl/evp.h>

void Base64Encode(const unsigned char* buffer, size_t length,
		char** base64Text) {
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*base64Text = (*bufferPtr).data;
}

size_t calcDecodelength(const char * b64input) {
	size_t len = std::strlen(b64input), padding = 0;
	if (b64input[len - 1] == '=' && b64input[len - 2] == '=') {
		padding = 2;
	} else if (b64input[len - 1] == '=') {
		padding = 1;
	}
	return (len * 3) / 4 - padding;
}

void Base64Decode(const char *b64message, unsigned char **buffer,
		size_t *length) {
	BIO *bio, *b64;
	int decodeLen = calcDecodelength(b64message);
	*buffer = (unsigned char*) malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf((void*) (b64message), -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	*length = BIO_read(bio, *buffer, strlen(b64message));
	BIO_free_all(bio);

}
