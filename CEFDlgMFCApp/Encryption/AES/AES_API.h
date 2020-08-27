#pragma once
void AES_encrypt(unsigned char* in, int inl, unsigned char *out, int* len, unsigned char * key);
int AES_decrypt(unsigned char* in, int inl, unsigned char *out, unsigned char *key);