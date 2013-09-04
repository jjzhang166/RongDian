#pragma once

#if defined(_WIN64) && !defined(_WIN32)
#define _WIN32
#endif

#include "RDWin32.h"

HMODULE RDLoadLibrary(const char* szFileName);
bool RDFreeLibrary(HMODULE hModule);

