#include <PCREUtil.h>

#define PCRE_STATIC // ��̬�����ѡ��
#define COMPILE_PCRE16

#include "../thirdparty/pcre/pcre.h"

BOOL PCREUtil::IsIPv4(LPCWSTR lpszText)
{
#pragma message("PCREUtil::IsDomain - ������ʽ - ���IP��ַ�Ƿ���Ч - 127.256.0.1����TRUE")
	return IsMatch(L"^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?).){4}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$", lpszText);
}

BOOL PCREUtil::IsMask(LPCWSTR lpszText)
{
	return IsMatch(L"^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])(.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])){3}$", lpszText);
}

BOOL PCREUtil::IsDomain(LPCWSTR lpszText)
{
#pragma message("PCREUtil::IsDomain - ������ʽ - ��������ʽ������")
	return IsMatch(L"^([A-Z0-9][A-Z0-9_-]*(?:.[A-Z0-9][A-Z0-9_-]*)+):?(d+)?/?/i", lpszText);
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