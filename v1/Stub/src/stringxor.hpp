#pragma once
#ifndef __STRINGXOR_H__
#define __STRINGXOR_H__

class StringXor {
	private:
		unsigned char *key;
		unsigned int keylen;

	public:
		StringXor(unsigned char *key, unsigned int keylen);
		~StringXor();
		void Xor(char *output, unsigned char *input, unsigned int len);
		std::string MultibyteXor(unsigned char *input, unsigned int len);
		std::wstring WideCharXor(unsigned char *input, unsigned int len);
};

#endif // !__STRINGXOR_H__
