#include <PCREUtil.h>
#include <StrUtil.h>

#define PCRE_STATIC // 静态库编译选项

#include "../thirdparty/pcre/pcre.h"

BOOL PCREUtil::IsIPv4(LPCWSTR lpszText)
{
#pragma message("PCREUtil::IsDomain - 正则表达式 - 检测IP地址是否有效 - 127.256.0.1返回TRUE")
	return IsMatch("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?).){4}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$", lpszText);
}

BOOL PCREUtil::IsMask(LPCWSTR lpszText)
{
	return IsMatch("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])(.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])){3}$", lpszText);
}

BOOL PCREUtil::IsDomain(LPCWSTR lpszText)
{
#pragma message("PCREUtil::IsDomain - 正则表达式 - 此正则表达式有问题")
	return IsMatch("^([A-Z0-9][A-Z0-9_-]*(?:.[A-Z0-9][A-Z0-9_-]*)+):?(d+)?/?/i", lpszText);
}

BOOL PCREUtil::IsMatch(LPCSTR lpszPattern, LPCWSTR lpszText)
{
	const char *error;
	int erroffset;
	int ovector[30] = {0};
	int rc;
	char *pszTextA = StrUtil::w2a(lpszText);
	if(!pszTextA)
		return FALSE;
	pcre *re = pcre_compile(lpszPattern, 0, &error, &erroffset, NULL);
	if (re == NULL)
	{
		delete pszTextA;
		return FALSE;
	}
	rc = pcre_exec(re, NULL, pszTextA, strlen(pszTextA), 0, 0, ovector, _countof(ovector));
	pcre_free(re);
	delete pszTextA;
	if (rc<0)
		return FALSE;
	return TRUE;
}