#pragma once

#include <Windows.h>

namespace PCREUtil
{
	BOOL IsIPv4(LPCWSTR lpszIP);
	BOOL IsMask(LPCWSTR lpszMask);
	BOOL IsDomain(LPCWSTR lpszDomain);
	BOOL IsHTMLColor(LPCWSTR lpszStr);
	BOOL IsMatch(LPCWSTR lpszPattern, LPCWSTR lpszText);
}