#include "ValidateUtil.h"

BOOL ValidateUtil::IsIPv4(LPCWSTR lpszIP)
{
	return IsMatch("^[1-9]{1,3}.\\d{1,3}.\\d{1,3}.[1-9]{1,3}$",lpszIP);
}

BOOL ValidateUtil::IsMask(LPCWSTR lpszMask)
{
	
	return TRUE;
}

BOOL ValidateUtil::IsDns(LPCWSTR lpszDns)
{

	return TRUE;
}

BOOL ValidateUtil::IsGateway(LPCWSTR lpszGateway)
{

	return TRUE;
}

BOOL ValidateUtil::IsMatch(LPCSTR lpszPattern, LPCWSTR lpszText)
{
	const char *error;
	int erroffset;
	int ovector[30]={0};
	int rc;
	pcre *re = pcre_compile(lpszPattern, 0, &error, &erroffset, NULL);
	if (re == NULL)
		return FALSE;

	char* lpszText_a = StrUtil::w2a(lpszText);
	rc = pcre_exec(re, NULL, lpszText_a, strlen(lpszText_a), 0, 0, ovector, 30);
	delete[] lpszText_a;

	if (rc<0)
	{
		return FALSE;
	}
	return TRUE;
}