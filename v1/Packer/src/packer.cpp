#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

#include "packer.hpp"

Packer::Packer(std::wstring infile, std::wstring outfile) {
	DWORD dwBinaryType = 0;
	if (!::GetBinaryType(infile.c_str(), &dwBinaryType))
		throw L"Invalid input file\n";

	this->infile = infile;
	this->outfile = outfile;
	this->useIcon = false;
	this->lpPayload = new PAYLOAD;
	qlz = new Quicklz();
	rc4 = new Rc4();

	LoadFile();
}

Packer::Packer(std::wstring infile, std::wstring outfile, std::wstring iconfile) {
	this->infile = infile;
	this->outfile = outfile;
	this->iconfile = iconfile;
	this->useIcon = true;
	this->lpPayload = new PAYLOAD;
	qlz = new Quicklz();
	rc4 = new Rc4();

	LoadFile();
}

Packer::~Packer() {
	delete qlz;
	delete rc4;
	delete[] this->lpPayload->lpBuffer;
	delete[] this->lpPayload->lpKey;
	delete this->lpPayload;
}

bool Packer::LoadFile() {
	// get file handle to file
	HANDLE hFile = ::CreateFile(this->infile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// get file size
	this->lpPayload->dwFileSize = ::GetFileSize(hFile, NULL);
	if (this->lpPayload->dwFileSize == INVALID_FILE_SIZE) {
		CloseHandle(hFile);
		return false;
	}
	this->lpPayload->dwBufSize = this->lpPayload->dwFileSize;

	// create heap buffer to hold file contents
	this->lpPayload->lpBuffer = new BYTE[this->lpPayload->dwFileSize];

	// read file contents
	DWORD dwRead = 0;
	if (!ReadFile(hFile, this->lpPayload->lpBuffer, this->lpPayload->dwFileSize, &dwRead, NULL)) {
		CloseHandle(hFile);
		return false;
	}

	// clean up
	CloseHandle(hFile);
	return true;
}

//bool Packer::GenerateKey() {
//	this->lpPayload->lpKey = new BYTE[KEY_LEN];
//	if (this->lpPayload->lpKey == NULL) return false;
//
//	// initialize crypto service provider
//	HCRYPTPROV hProv = NULL;
//	if (::CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
//		::CryptReleaseContext(hProv, 0);
//		return false;
//	}
//
//	// generate secure bytes
//	if (::CryptGenRandom(hProv, KEY_LEN, this->lpPayload->lpKey))
//		return false;
//
//	std::wcout << L"Using key: ";
//	for (int i = 0; i < KEY_LEN; i++)
//		std::wcout << this->lpPayload->lpKey[i] << "\n";
//
//	// clean up
//	::CryptReleaseContext(hProv, 0);
//
//	return true;
//}

bool Packer::EncryptPayload() {
	LPBYTE lpTempBuffer = new BYTE[this->lpPayload->dwBufSize];
	memcpy(lpTempBuffer, this->lpPayload->lpBuffer, this->lpPayload->dwBufSize);

	rc4->Encrypt(lpTempBuffer, this->lpPayload->dwBufSize);

	delete[] this->lpPayload->lpBuffer;
	this->lpPayload->lpBuffer = lpTempBuffer;

	this->lpPayload->lpKey = new BYTE[rc4->KEYLEN];
	rc4->GetKey(this->lpPayload->lpKey);

	return true;
}

bool Packer::CompressPayload() {
	LPBYTE lpCompressedBuffer = new BYTE[this->lpPayload->dwBufSize];

	size_t nOutlen;
	this->qlz->Compress(lpCompressedBuffer, this->lpPayload->lpBuffer, nOutlen, this->lpPayload->dwBufSize);

	this->lpPayload->lpBuffer = lpCompressedBuffer;
	this->lpPayload->dwBufSize = nOutlen;

	return true;
}

bool Packer::BuildStub() {
	// get stub program as a resource
	HRSRC hRsrc = ::FindResource(GetModuleHandle(L"packer.dll"), MAKEINTRESOURCE(1), L"FILE");
	if (hRsrc == NULL)
		return false;
	DWORD dwSize = ::SizeofResource(GetModuleHandle(L"packer.dll"), hRsrc);

	HGLOBAL hGlobal = ::LoadResource(GetModuleHandle(L"packer.dll"), hRsrc);
	if (hGlobal == NULL)
		return false;

	// get stub's file content
	LPBYTE lpStubBuffer = (LPBYTE)::LockResource(hGlobal);
	if (lpStubBuffer == NULL)
		return false;

	// copy stub to memory for editing
	LPBYTE lpBuffer = new BYTE[dwSize];
	memcpy(lpBuffer, lpStubBuffer, dwSize);

	// create output file
	HANDLE hFile = CreateFile(this->outfile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		delete lpBuffer;
		return false;
	}

	// write stub content to output file
	DWORD dwWritten = 0;
	if (!WriteFile(hFile, lpBuffer, dwSize, &dwWritten, NULL)) {
		CloseHandle(hFile);
		delete lpBuffer;
		return false;
	}

	CloseHandle(hFile);

	// add payload to stub
	if (!UpdateStub())
		return false;

	return true;
}

bool Packer::BuildSecondaryStub() {
	Pe *pe = new Pe(this->outfile.c_str());
	unsigned char shellcode[] = { 0x60, 0x8D, 0x1D, 0xAA, 0xAA, 0xAA, 0xAA, 0xB9, 0xBB, 0xBB, 0xBB, 0xBB, 0x83, 0xF9, 0x00,
								0x74, 0x05, 0x80, 0xF3, 0xC9, 0xEB, 0xF6, 0x61, 0x68, 0xCC, 0xCC, 0xCC, 0xCC, 0xC3 };

	// encrypt .text section
	//for (int i = 0; i < pe->GetSectionHeaderByIndex(0)->Misc.VirtualSize; i++)
	//	*(LPBYTE)(pe->GetFile + i) ^= XOR_KEY;

	BYTE b = 0xCC;
	DWORD dwNewAddress = pe->InjectCode(0, &b, 0);

	DWORD dwShellcodeSize = sizeof(shellcode);
	//std::vector<unsigned char> shellcodeVector(shellcode, shellcode + dwShellcodeSize);

	//std::replace(shellcodeVector.begin(), shellcodeVector.end(), 0xAAAAAAAA, pe->GetSectionHeaderByIndex(0)->PointerToRawData);
	
	DWORD dwPlaceholder = 0xAAAAAAAA;
	InsertStubValue(shellcode, dwShellcodeSize, &pe->GetSectionHeaderByIndex(0)->PointerToRawData, sizeof(DWORD), &dwPlaceholder);
	dwPlaceholder = 0xBBBBBBBB;
	InsertStubValue(shellcode, dwShellcodeSize, &pe->GetSectionHeaderByIndex(0)->Misc.VirtualSize, sizeof(DWORD), &dwPlaceholder);
	
	DWORD dwNewEntryPoint = pe->SetNewEntryPoint(dwNewAddress, TRUE);

	dwPlaceholder = 0xCCCCCCCC;
	InsertStubValue(shellcode, dwShellcodeSize, &dwNewEntryPoint, sizeof(DWORD), &dwPlaceholder);

	pe->InjectCode(0, shellcode, dwShellcodeSize);

	delete pe;

	return false;
}

bool Packer::UpdateStub() {
	// start updating stub's resources
	HANDLE hUpdate = BeginUpdateResource(this->outfile.c_str(), FALSE);
	// add file as a resource to stub
	if (!::UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(BUFFER_RSRC_ID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), this->lpPayload->lpBuffer, this->lpPayload->dwBufSize))
		return false;

	// add file size as a resource to stub
	if (!::UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(FILE_SIZE_RSRC_ID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (PVOID)&this->lpPayload->dwFileSize, sizeof(DWORD)))
		return false;

	// add decryption key as a resource
	if (!::UpdateResource(hUpdate, RT_RCDATA, MAKEINTRESOURCE(KEY_RSRC_ID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), this->lpPayload->lpKey, KEY_LEN))
		return false;

	if (this->useIcon == false) {
		::EndUpdateResource(hUpdate, FALSE);
		return true;
	}

	HANDLE hIconFile = ::CreateFile(this->iconfile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hIconFile == INVALID_HANDLE_VALUE)
		return false;

	// calculate buffer length and load file into buffer
	DWORD dwIconSize = ::GetFileSize(hIconFile, NULL);
	LPBYTE lpIconBuffer = new BYTE[dwIconSize];

	DWORD dwRead = 0;
	if (!::ReadFile(hIconFile, lpIconBuffer, dwIconSize, &dwRead, NULL)) {
		CloseHandle(hIconFile);
		delete[] lpIconBuffer;
		return false;
	}
	CloseHandle(hIconFile);

	if (!::UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(1), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (lpIconBuffer + 22), dwIconSize - 22)) {
		delete[] lpIconBuffer;
		return false;
	}

	GROUPICON grData;

	// This is the header
	grData.Reserved1 = 0;					// reserved, must be 0
	grData.ResourceType = 1;				// type is 1 for icons
	grData.ImageCount = 1;					// number of icons in structure (1)

	// This is the directory entry
	grData.Width = 32;						// icon width (32)
	grData.Height = 32;						// icon height (32)
	grData.Colors = 0;						// colors (256)
	grData.Reserved2 = 0;					// reserved, must be 0
	grData.Planes = 2;						// color planes
	grData.BitsPerPixel = 32;				// bit depth
	grData.ImageSize = dwIconSize - 22;		// size of image
	grData.ResourceID = 1;				// resource ID is 1

	if (!::UpdateResource(hUpdate, RT_GROUP_ICON, L"MAINICON", MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), &grData, sizeof(GROUPICON))) {
		delete[] lpIconBuffer;
		return false;
	}

	delete[] lpIconBuffer;

	::EndUpdateResource(hUpdate, FALSE);

	return true;
}

bool Packer::InsertStubValue(LPBYTE lpShellcode, SIZE_T nStubSize, LPDWORD lpBuffer, SIZE_T nSize, LPDWORD lpPlaceholderValue) {
	// modify stub shellcode data
	DWORD dwIncrementor = 0;
	for (; dwIncrementor < nStubSize; dwIncrementor++) {
		if (*((LPDWORD)lpShellcode + dwIncrementor) == *lpPlaceholderValue) {
			// insert data
			*((LPDWORD)lpShellcode + dwIncrementor) = (DWORD)lpBuffer;
			return true;
		}
	}

	return false;
}

__declspec(naked) void Packer::SecondaryStubStart() {
	__asm {
			pushad
			lea		ebx,	ds:[0xAAAAAAAA]
			mov		ecx,	0xBBBBBBBB

		loop1:
			cmp		ecx,	0
			jz		fin
			xor		bl,		XOR_KEY
			jmp		loop1

		fin:
			popad
			push	0xCCCCCCCC
			ret
	}
}

void Packer::SecondaryStubEnd() {
}
