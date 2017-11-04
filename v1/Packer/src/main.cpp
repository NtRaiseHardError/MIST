#include <iostream>
#include <Windows.h>

#include "packer.hpp"

extern "C" {
	__declspec(dllexport) DWORD PackFile(LPWSTR lpTargetFile, LPWSTR lpOutputFile) {
		std::wstring infile(lpTargetFile);
		std::wstring outfile(lpOutputFile);

		DWORD dwSuccess;
		Packer *p = new Packer(infile, outfile);
		if (p->CompressPayload())
			if (p->EncryptPayload())
				if (p->BuildStub())
					dwSuccess = 0;
				else
					dwSuccess = (int)GetLastError();
			else
				dwSuccess = (int)GetLastError();
		else
			dwSuccess = (int)GetLastError();

		delete p;

		return dwSuccess;
	}
}

int wmain(int argc, wchar_t *argv[]) {
	if (argc < 3) {
		std::wcerr << L"Syntax: " << argv[0] << L" [input file] [output file] [icon file]\n";
		return 1;
	}

	std::wstring infile(argv[1]);
	std::wstring outfile(argv[2]);

	Packer *p;
	if (argc > 3) {
		std::wstring iconfile(argv[3]);
		p = new Packer(infile, outfile, iconfile);
	} else
		p = new Packer(infile, outfile);
	if (p->CompressPayload())
		if (p->EncryptPayload())
			if (p->BuildStub())
				if (p->BuildSecondaryStub()) {
				std::wcout << L"Packing sucessfully completed\n";
				delete p;
				return 0;
			}

	delete p;

	std::wcerr << L"Failed to pack file\n";

	return 1;
}