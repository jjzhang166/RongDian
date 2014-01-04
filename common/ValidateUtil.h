#pragma once
#include "stdafx.h"

namespace ValidateUtil
{
	BOOL IsIPv4(LPCWSTR lpszIP);
	BOOL IsMask(LPCWSTR lpszMask);
	BOOL IsDomain(LPCWSTR lpszDomain);
	BOOL IsMatch(LPCSTR lpszPattern, LPCWSTR lpszText);
}