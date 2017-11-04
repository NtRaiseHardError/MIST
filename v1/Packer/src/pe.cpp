#include "pe.hpp"

BOOL Pe::SectionHasSpace(int nSectionIndex, SIZE_T nSize) {
	PIMAGE_SECTION_HEADER pish = GetSectionHeaderByIndex(nSectionIndex);
	DWORD dwCaveSize = pish->SizeOfRawData - pish->Misc.VirtualSize;

	return dwCaveSize >= nSize;
}

BOOL Pe::HasNewSectionSpace(DWORD dwNewSectionHeaderOffset) {
	DWORD dwAddressOfFirstSection = GetFirstSectionHeader()->PointerToRawData;
	
	return (dwAddressOfFirstSection - dwNewSectionHeaderOffset >= IMAGE_SIZEOF_SECTION_HEADER);
}

DWORD Pe::AlignBoundary(DWORD dwAddress, DWORD dwSectionAlignment) {
	return ((dwAddress + dwSectionAlignment - 1) / dwSectionAlignment) * dwSectionAlignment;
}

BOOL Pe::UsesTls(PIMAGE_OPTIONAL_HEADER pioh) {
	return pioh->DataDirectory[9].Size && pioh->DataDirectory[9].VirtualAddress ? TRUE : FALSE;
}

DWORD Pe::OverrideTlsCallback0(PIMAGE_OPTIONAL_HEADER pioh, DWORD dwAddress) {
	// get TLS directory
	PIMAGE_TLS_DIRECTORY ptls = (PIMAGE_TLS_DIRECTORY)pioh->DataDirectory[9].VirtualAddress;
	
	// get original tls_callback_0 address
	DWORD dwTlsCallback0 = ptls->AddressOfCallBacks;
	// overwrite with new address
	ptls->AddressOfCallBacks = dwAddress;

	// return original tls_callback_0 address
	return dwTlsCallback0;
}

Pe::Pe(LPCWSTR lpFileName) {
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		throw std::exception("Error opening file");

	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMapping == NULL)
		throw std::exception("Error creating file map");

	this->lpFile = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	if (this->lpFile == NULL)
		throw std::exception("Error mapping file");

	if (!IsDos(GetDosHeader()) || !IsPe(GetPeHeader())) {
		::SetLastError(ERROR_BAD_EXE_FORMAT);
		throw std::invalid_argument("Invalid PE file");
	}

	CloseHandle(hFileMapping);
	CloseHandle(hFile);
}

Pe::~Pe() {
	if (this->lpFile) UnmapViewOfFile(this->lpFile);
}

LPBYTE Pe::GetFile() {
	return this->lpFile;
}

PIMAGE_DOS_HEADER Pe::GetDosHeader() {
	return PIMAGE_DOS_HEADER(this->lpFile);
}

PIMAGE_NT_HEADERS Pe::GetPeHeader() {
	PIMAGE_DOS_HEADER pidh = GetDosHeader();

	return (PIMAGE_NT_HEADERS)((DWORD)pidh + pidh->e_lfanew);
}

PIMAGE_FILE_HEADER Pe::GetFileHeader() {
	PIMAGE_NT_HEADERS pinh = GetPeHeader();

	return (PIMAGE_FILE_HEADER)&pinh->FileHeader;
}

PIMAGE_OPTIONAL_HEADER Pe::GetOptionalHeader() {
	PIMAGE_NT_HEADERS pinh = GetPeHeader();

	return (PIMAGE_OPTIONAL_HEADER)&pinh->OptionalHeader;
}

PIMAGE_SECTION_HEADER Pe::GetFirstSectionHeader() {
	PIMAGE_NT_HEADERS pinh = GetPeHeader();

	return IMAGE_FIRST_SECTION(pinh);
}

PIMAGE_SECTION_HEADER Pe::GetSectionHeaderByIndex(int nSectionIndex) {
	return (PIMAGE_SECTION_HEADER)(GetFirstSectionHeader() + (sizeof(IMAGE_SECTION_HEADER) * nSectionIndex));
}

PIMAGE_SECTION_HEADER Pe::GetSectionHeaderByName(LPCSTR lpName) {
	for (int i = 0; i < GetPeHeader()->FileHeader.NumberOfSections; i++) {
		int len = 0;
		PIMAGE_SECTION_HEADER pish = GetSectionHeaderByIndex(i);

		// check if name is 8 characters (max), else strlen
		pish->Name[7] == '\0' ? len = strlen(lpName) : len = 8;

		if (memcmp(pish->Name, lpName, len) == 0)
			return pish;
	}

	return NULL;
}

PIMAGE_SECTION_HEADER Pe::GetLastSectionHeader() {
	return (PIMAGE_SECTION_HEADER)(GetFirstSectionHeader() + (GetPeHeader()->FileHeader.NumberOfSections - 1));
}

BOOL Pe::IsDos(PIMAGE_DOS_HEADER pidh) {
	return pidh->e_magic == IMAGE_DOS_SIGNATURE;
}

BOOL Pe::IsPe(PIMAGE_NT_HEADERS pinh) {
	return pinh->Signature == IMAGE_NT_SIGNATURE;
}

DWORD Pe::GetEntryPoint() {
	return GetOptionalHeader()->AddressOfEntryPoint;
}

DWORD Pe::InjectCode(int nSectionIndex, LPBYTE lpBuffer, SIZE_T nSize) {
	if (!SectionHasSpace(nSectionIndex, nSize)) return 0;

	PIMAGE_SECTION_HEADER pish = GetSectionHeaderByIndex(nSectionIndex);
	DWORD dwFileOffset = pish->PointerToRawData + pish->Misc.VirtualSize;

	memcpy(this->lpFile + dwFileOffset, lpBuffer, nSize);

	pish->Misc.VirtualSize += nSize;

	// RVA = file offset + virtual offset - raw offset
	return dwFileOffset + pish->VirtualAddress - pish->PointerToRawData;
}

DWORD Pe::AddSection(LPCSTR lpName, DWORD dwCharacteristics) {
	PIMAGE_SECTION_HEADER lastPish = GetLastSectionHeader();
	PIMAGE_SECTION_HEADER newPish = (PIMAGE_SECTION_HEADER)((DWORD)lastPish + IMAGE_SIZEOF_SECTION_HEADER);
	PIMAGE_OPTIONAL_HEADER pioh = GetOptionalHeader();

	if (!HasNewSectionSpace((DWORD)newPish))
		::SetLastError(INVALID_FILE_SIZE); return 0;

	memcpy(newPish->Name, lpName, strlen(lpName));

	newPish->Characteristics = dwCharacteristics;

	newPish->VirtualAddress = AlignBoundary(lastPish->VirtualAddress + lastPish->Misc.VirtualSize, pioh->SectionAlignment);
	newPish->PointerToRawData = AlignBoundary(lastPish->PointerToRawData + lastPish->SizeOfRawData, pioh->FileAlignment);
	newPish->Misc.VirtualSize = 10;
	pioh->SizeOfImage = AlignBoundary(pioh->SizeOfImage + 10, pioh->SectionAlignment);
	pioh->SizeOfHeaders = AlignBoundary(pioh->SizeOfHeaders + IMAGE_SIZEOF_SECTION_HEADER, pioh->FileAlignment);
	GetFileHeader()->NumberOfSections++;

	return (DWORD)newPish;
}

DWORD Pe::SetNewEntryPoint(DWORD dwAddress, BOOL bOverrideTls) {
	PIMAGE_OPTIONAL_HEADER pioh = GetOptionalHeader();

	if (bOverrideTls && UsesTls(pioh))
		return OverrideTlsCallback0(pioh, dwAddress);

	DWORD dwOEP = pioh->AddressOfEntryPoint;
	pioh->AddressOfEntryPoint = dwAddress;

	return dwOEP;
}
