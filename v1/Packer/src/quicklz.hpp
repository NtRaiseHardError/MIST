#pragma once
#ifndef __QUICKLZ_HPP__
#define __QUICKLZ_HPP__

#include "quicklz.h"

class Quicklz {
	private:
		qlz_state_compress *state_compress;
		qlz_state_decompress *state_decompress;

	public:
		Quicklz();
		~Quicklz();
		size_t GetCompressedSize(unsigned char *input);
		size_t GetDecompressedSize(unsigned char *input);
		void Compress(unsigned char *output, const unsigned char *input, size_t &outlen, size_t inlen);
		void Decompress(unsigned char *output, unsigned char *input, size_t &outlen);
};

#endif // !__QUICKLZ_HPP__
