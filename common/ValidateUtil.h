#pragma once
#include "stdafx.h"

namespace ValidateUtil
{
	BOOL IsIPv4(LPCWSTR lpszIP);
	BOOL IsMask(LPCWSTR lpszMask);
	BOOL IsDns(LPCWSTR lpszDns);
	BOOL IsGateway(LPCWSTR lpszGateway);
	BOOL IsMatch(LPCSTR lpszPattern, LPCWSTR lpszText);
}