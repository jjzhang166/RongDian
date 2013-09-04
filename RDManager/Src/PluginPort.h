#pragma once

#if defined(_WIN64) && !defined(_WIN32)
#define _WIN32
#endif

#ifdef _WIN32
#endif

#if !defined(_MSC_VER) || _MSC_VER<1400
#endif

HMODULE RDLoadLibrary(const char* lpszFileName);
bool RDFreeLibrary(HMODULE hModule);