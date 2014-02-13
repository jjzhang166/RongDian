#include "UrlEncode.h"
#include <wchar.h>
#include <stdio.h>
#include <Windows.h>

wchar_t* UrlEncode::escape(const wchar_t* wStr)
{
	int len = wcslen(wStr);
	int zoomLen = len*2;
	wchar_t* encodedStr = new wchar_t[zoomLen];
	wmemset(encodedStr,0,zoomLen);
	for(int i=0;i<len;i++)
	{
		if((wStr[i]>='a'&&wStr[i]<='z')||(wStr[i]>='A'&&wStr[i]<='Z')||(wStr[i]>='0'&&wStr[i]<='9')||wStr[i] == '*'||wStr[i] == '@'||wStr[i] == '+'||wStr[i] == '-'||wStr[i] == '_'||wStr[i] == '.'||wStr[i] == '/')
		{
			wchar_t tmp[2]={0};
			wsprintf(tmp,L"%c",wStr[i]);
			wcscat(encodedStr,tmp);
		}
		else if(wStr[i]>0xFF)
		{
			wchar_t tmpcn[7];
			wsprintf(tmpcn,L"%%u%X",wStr[i]);
			if(zoomLen-wcslen(encodedStr)<7){
				zoomLen = wcslen(encodedStr)*2+7;
				wchar_t* zoomStr = new wchar_t[zoomLen];
				wmemset(zoomStr,0,zoomLen);
				wcscpy_s(zoomStr,zoomLen,encodedStr);
				delete[] encodedStr;
				encodedStr = zoomStr;
			}
			wcscat(encodedStr,tmpcn);
		}
		else
		{
			wchar_t tmp[4];
			wsprintf(tmp,L"%%%02X",wStr[i]);
			if(zoomLen-wcslen(encodedStr)<4){
				zoomLen = wcslen(encodedStr)*2+4;
				wchar_t* zoomStr = new wchar_t[zoomLen];
				wmemset(zoomStr,0,zoomLen);
				wcscpy_s(zoomStr,zoomLen,encodedStr);
				delete[] encodedStr;
				encodedStr = zoomStr;
			}
			wcscat(encodedStr,tmp);
		}
	}
	return encodedStr;
}

wchar_t* UrlEncode::unescape(const wchar_t* str)
{
	int len = wcslen(str);
	int zoomLen = len*2;
	int j = 0;
	wchar_t* encodedStr = new wchar_t[zoomLen];
	wmemset(encodedStr,0,zoomLen);
	for(int i=0;i<len;i++)
	{
		if(str[i]=='%'&&str[i+1]=='u')
		{
			wchar_t tmp[5]={0};
			wsprintf(tmp,L"%c%c%c%c",str[i+2],str[i+3],str[i+4],str[i+5]);
			long oneword = wcstol(tmp,0,16);
			encodedStr[j]=oneword;
			i = i+5;
		}
		else if(str[i]=='%'&&str[i+1]!='u')
		{
			wchar_t tmp[3]={0};
			wsprintf(tmp,L"%c%c",str[i+1],str[i+2]);
			long oneword = wcstol(tmp,0,16);
			encodedStr[j]=oneword;
			i = i+2;
		}
		else
		{
			wchar_t tmp[2]={0};
			wsprintf(tmp,L"%c",str[i]);
			wcscat(encodedStr,tmp);
		}
		j++;
	}
	return encodedStr;
}

char* UrlEncode::url_encode(const char* sourceText)
{
	int len = strlen(sourceText);
	int zoomLen = len*2;
	char* encodedStr = new char[zoomLen];
	memset(encodedStr,0,zoomLen);
	for(int i=0;i<len;i++)
	{
		if((sourceText[i]>='a'&&sourceText[i]<='z')||(sourceText[i]>='A'&&sourceText[i]<='Z')||(sourceText[i]>='0'&&sourceText[i]<='9')||sourceText[i] == '-'||sourceText[i] == '_'||sourceText[i] == '.')
		{
			char tmp[2]={0};
			sprintf(tmp,"%c",sourceText[i]);
			strcat(encodedStr,tmp);
		}
		else// if(sourceText[i]>0xFF)
		{
			char tmp[4];
			sprintf(tmp,"%%%02X",sourceText[i]&0xFF);
			if(zoomLen-strlen(encodedStr)<4){
				zoomLen = strlen(encodedStr)*2+4;
				char* zoomStr = new char[zoomLen];
				memset(zoomStr,0,zoomLen);
				strcpy_s(zoomStr,zoomLen,encodedStr);
				delete[] encodedStr;
				encodedStr = zoomStr;
			}
			strcat(encodedStr,tmp);
		}
	}
	return encodedStr;
}

char* UrlEncode::url_unencode(const char* sourceText)
{
	int len = strlen(sourceText)+1;
	int j = 0;
	char* encodedStr = new char[len];
	memset(encodedStr,0,len);
	int oneword = 0;
	for(int i=0;i<len;i++)
	{
		if(sourceText[i]=='%')
		{
			char tmp[3]={0};
			sprintf(tmp,"%c%c",sourceText[i+1],sourceText[i+2]);
			oneword = strtol(tmp,0,16);
			encodedStr[j]=oneword;
			i = i+2;
		}
		else
		{
			char tmp[2]={0};
			sprintf(tmp,"%c",sourceText[i]);
			strcat(encodedStr,tmp);
		}
		j++;
	}
	return encodedStr;
}