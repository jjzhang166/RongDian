#include <PCREUtil.h>

#define PCRE_STATIC // 静态库编译选项
#define COMPILE_PCRE16

#include "../thirdparty/pcre/pcre.h"

BOOL PCREUtil::IsIPv4(LPCWSTR lpszText)
{
//#pragma message("PCREUtil::IsIPv4 - 正则表达式 - 检测IP地址是否有效 - 127.256.0.1返回TRUE")
#pragma message("PCREUtil::IsIPv4 - 待测试 - 来源 - <<Regular Expressions Cookbook>> - 第7章")
	return IsMatch(L"^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?).){3}(25[0-4]\\d|25[0-5]|[01]?\\d\\d?)$", lpszText);
	//return IsMatch(L"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$", lpszText);
}

BOOL PCREUtil::IsMask(LPCWSTR lpszText)
{
	return IsMatch(L"^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])(.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])){3}$", lpszText);
}

BOOL PCREUtil::IsDomain(LPCWSTR lpszText)
{
//#pragma message("PCREUtil::IsDomain - 正则表达式 - 此正则表达式有问题")
#pragma message("PCREUtil::IsDomain - 待测试 - 来源 - <<Regular Expressions Cookbook>> - 第7章")
	return IsMatch(L"^([a-z0-9]+(-[a-z0-9]+)*\\.)+[a-z]{2,}$", lpszText);
}

BOOL PCREUtil::IsMatch(LPCWSTR lpszPattern, LPCWSTR lpszText)
{
	const char *error;
	int erroffset;
	int ovector[30] = {0};
	int rc;
	pcre16 *re = pcre16_compile((PCRE_SPTR16)lpszPattern, 0, &error, &erroffset, NULL);
	if (re == NULL)
		return FALSE;
	rc = pcre16_exec(re, NULL, (PCRE_SPTR16)lpszText, wcslen(lpszText), 0, 0, ovector, _countof(ovector));
	pcre16_free(re);
	if (rc<0)
		return FALSE;
	return TRUE;
}