#pragma once
#ifndef __PACKER_HPP__
#define __PACKER_HPP__

#include "pe.hpp"
#include "quicklz.hpp"
#include "rc4.hpp"
#define KEY_LEN 32

#define BUFFER_RSRC_ID 10
#define FILE_SIZE_RSRC_ID 20
#define KEY_RSRC_ID 30

#define XOR_KEY 0xC9

#define PUSHAD 0x60
#define POPAD 0x61
#define XOR_2r8 0x30
#define XOR_2r 0x31
#define XOR_1r 0x81

#define PUSH 0x50
#define POP 0x58
#define MOV 0xB8
#define NOP 0x90

#pragma pack(push, 2)
typedef struct {
	WORD Reserved1;       // reserved, must be 0
	WORD ResourceType;    // type is 1 for icons
	WORD ImageCount;      // number of icons in structure (1)
	BYTE Width;           // icon width (32)
	BYTE Height;          // icon height (32)
	BYTE Colors;          // colors (0 means more than 8 bits per pixel)
	BYTE Reserved2;       // reserved, must be 0
	WORD Planes;          // color planes
	WORD BitsPerPixel;    // bit depth
	DWORD ImageSize;      // size of structure
	WORD ResourceID;      // resource ID
} GROUPICON;
#pragma pack(pop)

typedef struct _PAYLOAD {
	LPBYTE lpBuffer;
	DWORD dwBufSize;
	DWORD dwFileSize;
	LPBYTE lpKey;
} PAYLOAD, *LPPAYLOAD;

class Packer {
	private:
		std::wstring infile;
		std::wstring outfile;
		std::wstring iconfile;
		bool useIcon;
		LPPAYLOAD lpPayload;
		Quicklz *qlz;
		Rc4 *rc4;
		//unsigned char shellcode[] = { 0x60, 0x8D, 0x1D, 0xAA, 0xAA, 0xAA, 0xAA, 0xB9, 0xBB, 0xBB, 0xBB, 0xBB, 0x83, 0xF9, 0x00, \
		//							0x74, 0x05, 0x80, 0xF3, 0xC9, 0xEB, 0xF6, 0x61, 0x68, 0xCC, 0xCC, 0xCC, 0xCC, 0xC3 };

		//bool GenerateKey();
		bool LoadFile();
		bool UpdateStub();
		bool InsertStubValue(LPBYTE lpShellcode, SIZE_T nStubSize, LPDWORD lpBuffer, SIZE_T nSize, LPDWORD lpPlaceholderValue);
		void SecondaryStubStart();
		void SecondaryStubEnd();

	public:
		Packer(std::wstring infile, std::wstring outfile);
		Packer::Packer(std::wstring infile, std::wstring outfile, std::wstring iconfile);
		~Packer();
		bool EncryptPayload();
		bool CompressPayload();
		bool BuildStub();
		bool BuildSecondaryStub();
};

#endif // !__PACKER_HPP__
