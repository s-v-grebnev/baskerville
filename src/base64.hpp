/*
 *Работа с base64
 */

#pragma once

void Base64Encode(const unsigned char* buffer,  size_t length,  char** base64Text);
void Base64Decode(const char *b64message, unsigned char **buffer, size_t *length);
