#pragma once

#include "RDWin.h"

bool RDFreeLibrary(HMODULE hModule)
{
	return !FreeLibrary(hModule);
}

HMODULE RDLoadLibrary(const char* lpszFileName)
{
	HMODULE hModule = LoadLibraryExA(lpszFileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	return hModule;
}