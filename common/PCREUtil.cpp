#include "stdafx.h"
#include "PCREUtil.h"

#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/pcred.lib")
#else
#pragma comment(lib, "../RongDian/bin/pcre.lib")
#endif 

bool PCREUtil::IsMatch(const char* lpszPattern, const char* lpszText)
{
	int overcount = 30;
	const char *error;
	int erroffset;
	int ovector[30];
	int rc;
	pcre *re = pcre_compile(lpszPattern, 0, &error, &erroffset, NULL);
	if (re == NULL)
		return false;
	
	rc = pcre_exec(re, NULL, lpszText, strlen(lpszText), 0, 0, ovector, overcount);
	
	return (rc>=0);
}

bool PCREUtil::find(const char* lpszPattern,char* lpszText,list<char*>* result)
{
	if (!lpszText)
		return false;

	pcre *re;
	const char *error;
	int erroffset;
	int ovector[30];
	int rc;

	re = pcre_compile(lpszPattern, PCRE_MULTILINE, &error, &erroffset, NULL);
	if (re == NULL) //编译失败，返回错误信息
	{
		return false;
	}
	UINT cur = 0;
	while(cur<strlen(lpszText) && (rc = pcre_exec(re, NULL, lpszText, strlen(lpszText), cur, PCRE_NOTEMPTY, ovector, 30)) >= 0)
	{
		for(int j=1; j<rc; j++)
		{
			int substring_length = ovector[2*j+1] - ovector[2*j];//ansi类型的字符串长度
			if (substring_length>0)
			{
				char *subStr_start = lpszText + ovector[2*j];

				int subLen = substring_length+1;
				char *substring = new char[sizeof(char)*subLen];
				memset(substring, 0, sizeof(char)*subLen);
				memcpy_s(substring, subLen, subStr_start, substring_length);
				result->push_back(substring);
			}

		}
		cur = ovector[1];
	}
	pcre_free(re);
	if (rc>0)
	{
		return true;
	}
	return false;
}