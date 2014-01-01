#pragma once

#ifdef NET_DLL
#define NET_EXPORT extern "C" __declspec(dllexport)
#else
#define NET_EXPORT extern "C" __declspec(dllimport)
#endif

typedef struct _tagRESPONE_INFO
{
	unsigned long ulSize; // ������pszData��С
	unsigned long ulSeek; // ��������ǰoffset
	int nRet; // �����������Ƿ��쳣:0Ϊ���쳣
	char *pszData; // ������
} RESPONE_INFO, *PRESPONE_INFO, *LPRESPONE_INFO;

NET_EXPORT LONG __cdecl CheckVersion(HWND hInvoker, const char* pszUrl, char* pszRespone, unsigned long* pnSize);

