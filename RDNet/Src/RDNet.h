#pragma once

#ifdef NET_DLL
#define NET_EXPORT extern "C" __declspec(dllexport)
#else
#define NET_EXPORT extern "C" __declspec(dllimport)
#endif

typedef struct _tagRESPONE_INFO
{
	unsigned long ulSize; // 缓冲区pszData大小
	unsigned long ulSeek; // 缓冲区当前offset
	int nRet; // 缓冲区操作是否异常:0为无异常
	char *pszData; // 缓冲区
} RESPONE_INFO, *PRESPONE_INFO, *LPRESPONE_INFO;

NET_EXPORT LONG __cdecl CheckVersion(HWND hInvoker, const char* pszUrl, char* pszRespone, unsigned long* pnSize);

