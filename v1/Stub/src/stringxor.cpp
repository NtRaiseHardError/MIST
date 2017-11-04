#include <cstring>
#include <string>

#include "stringxor.hpp"

StringXor::StringXor(unsigned char *key, unsigned int keylen) {
	this->key = new unsigned char[keylen];
	memcpy(this->key, key, keylen);
	this->keylen = keylen;
}

StringXor::~StringXor() {
}

void StringXor::Xor(char *output, unsigned char *input, unsigned int len) {
	for (int i = 0; i < len; i++)
		output[i] = input[i] ^ this->key[i % this->keylen];
}

std::string StringXor::MultibyteXor(unsigned char *input, unsigned int len) {
	char *output = new char[len];
	Xor(output, input, len);
	
	std::string s(output);
	delete output;

	return s;
}

std::wstring StringXor::WideCharXor(unsigned char *input, unsigned int len) {
	char *output = new char[len];
	Xor(output, input, len);

	std::string s(output);
	std::wstring ws(s.begin(), s.end());
	delete output;

	return ws;
}
