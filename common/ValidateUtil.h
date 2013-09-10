#pragma once
#include "stdafx.h"

namespace ValidateUtil
{
	BOOL IsIPv4(LPCWSTR lpszIP);
	BOOL IsMask(LPCWSTR lpszMask);
	BOOL IsMatch(LPCSTR lpszPattern, LPCWSTR lpszText);
}