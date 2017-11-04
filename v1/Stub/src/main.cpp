#include <cstdio>
#include <stdarg.h>
#include <Windows.h>

#include "packerstub.hpp"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Packerstub *ps = new Packerstub();

	ps->ExtractPayload();
	ps->DecryptPayload();
	ps->DecompressPayload();
	ps->ExecutePayload();

	delete ps;

	return 0;
}