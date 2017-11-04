#pragma once
#ifndef __RC4_HPP__
#define __RC4_HPP__

#include "rc4.h"

#define KEY_LEN 32

class Rc4 {
	private:
		struct rc4_state *s;
		unsigned char key[KEY_LEN];

	public:
		static const unsigned int KEYLEN = KEY_LEN;

		Rc4();
		Rc4(unsigned char *key);
		~Rc4();
		void GetKey(unsigned char *key);
		void Encrypt(unsigned char *data, unsigned int len);
		void Decrypt(unsigned char *data, unsigned int len);
		bool GenerateKey();
};

#endif // !__RC4_HPP__
