#pragma once  

#include <Windows.h>
#include <tchar.h>
#include <time.h>

#include <stdio.h>
#include <IPHlpApi.h>
#pragma comment(lib, "IphlpApi.lib")

typedef struct _tagADDR_INFO
{
	int port;
	char ip[256];
	char mac[256];
} ADDR_INFO, *PADDR_INFO, *LPADDR_INFO;

namespace Utility  
{
// Generates unique identifier (GUID)
HRESULT CreateGuid(LPWSTR lpszGuid);

HRESULT GetEngineGuid(LPCWSTR lpszExt, LPGUID lpGuid);

// Returns TRUE if Windows is 64-bit
BOOL IsOS64Bit();

// Retrieves product version info from resources embedded into EXE or DLL
BOOL GetVersion(LPCWSTR lpszModule, LPWSTR lpszVer);

BOOL OpenFile(HWND hWnd, LPCWSTR lpszFile);

// 打开文件所在的文件夹，并选中该文件
BOOL OpenFolder(HWND hWnd, LPCWSTR lpszPath);

// 调用默认浏览器打开url地址
BOOL OpenUrl(HWND hWnd, LPCWSTR lpszUrl);

BOOL OpenMail(HWND hWnd, LPCWSTR lpszMail);

BOOL GetINIStr(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, LPWSTR lpszValue);

BOOL SetINIStr(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, LPCWSTR lpszValue);

BOOL GetINIInt(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, int *pnValue);

BOOL SetINIInt(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, int nValue);

BOOL TimeToTm(const time_t &t, tm * tt);

BOOL GetScreenColor(int x, int y, DWORD &dwColor);

BOOL CopyTextToClipBoard(LPCSTR lpszText);

void GetErrMsg(DWORD dwCode, LPWSTR lpszMsg);

BOOL IsAdminPrivilege();

BOOL IsAdminUser();

BOOL GetFolderSize(LPCWSTR lpszFolderPath, DWORD &dwSize);

BOOL GetMacAddr(LPADDR_INFO lpAddrInfo);
};