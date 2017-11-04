#pragma once
#ifndef __PE_HPP__
#define __PE_HPP__

#include <Windows.h>
#include <stdexcept>

class Pe {
	private:
		LPBYTE lpFile;
		BOOL SectionHasSpace(int nSectionIndex, SIZE_T nSize);
		BOOL HasNewSectionSpace(DWORD dwNewSectionHeaderOffset);
		DWORD AlignBoundary(DWORD dwAddress, DWORD dwSectionAlignment);
		BOOL UsesTls(PIMAGE_OPTIONAL_HEADER pioh);
		DWORD OverrideTlsCallback0(PIMAGE_OPTIONAL_HEADER pioh, DWORD dwAddress);

	public:
		Pe(LPCWSTR lpFileName);
		~Pe();
		LPBYTE GetFile();
		PIMAGE_DOS_HEADER GetDosHeader();
		PIMAGE_NT_HEADERS GetPeHeader();
		PIMAGE_FILE_HEADER GetFileHeader();
		PIMAGE_OPTIONAL_HEADER GetOptionalHeader();
		PIMAGE_SECTION_HEADER GetFirstSectionHeader();
		PIMAGE_SECTION_HEADER GetSectionHeaderByIndex(int nSectionIndex);
		PIMAGE_SECTION_HEADER GetSectionHeaderByName(LPCSTR lpName);
		PIMAGE_SECTION_HEADER GetLastSectionHeader();
		BOOL IsDos(PIMAGE_DOS_HEADER pidh);
		BOOL IsPe(PIMAGE_NT_HEADERS pinh);
		DWORD GetEntryPoint();
		DWORD InjectCode(int nSectionIndex, LPBYTE lpBuffer, SIZE_T nSize);
		DWORD AddSection(LPCSTR lpName, DWORD dwCharacteristics);
		DWORD SetNewEntryPoint(DWORD dwAddress, BOOL bOverrideTls);
};

#endif // !__PE_HPP__
