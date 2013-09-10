#include "ValidateUtil.h"

BOOL ValidateUtil::IsIPv4(LPCWSTR lpszIP)
{
	return IsMatch("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?).){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$",lpszIP);
}

BOOL ValidateUtil::IsMask(LPCWSTR lpszMask)
{
	return IsMatch("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])(.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])){3}$",lpszMask);
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