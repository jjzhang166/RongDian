#include "Util.h"
#include <Shlwapi.h>
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Shlwapi.lib")

#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4996)

// Generates unique identifier (GUID)
HRESULT Utility::CreateGuid(LPWSTR lpszGuid)
{
#if 0
#pragma comment(lib, "rpcrt4.lib")
#include <ServProv.h>
#include <iostream>
	UUID uuid = {0};
	UuidCreate(&uuid);
	RPC_WSTR Buffer;
	UuidToStringW(&uuid, &Buffer);
	if(lpszGuid)
		wcscpy(lpszGuid, Buffer);
#endif

	GUID guid = {0};
	HRESULT hRes = CoCreateGuid(&guid);
	if(hRes!=S_OK)
		return hRes;
	wchar_t szGuid[MAX_PATH] = {0};
	int dwSize = StringFromGUID2(guid, szGuid, _countof(szGuid));
	if(dwSize && lpszGuid)
		wcscpy(lpszGuid, szGuid);
	return S_OK;
}

HRESULT Utility::GetEngineGuid(LPCWSTR lpszExt, LPGUID lpGuid)
{
	HRESULT hRes = S_FALSE;
	HKEY hKey = NULL;
	DWORD dwSize = 0;
	HKEY hSubKey = NULL;
	DWORD dwType = REG_SZ;
	wchar_t szBuffer[MAX_PATH] = {0};
	// See if this file extension is associated with an ActiveX Script Engine
	hRes = RegOpenKeyExW(HKEY_CLASSES_ROOT, lpszExt, 0, KEY_QUERY_VALUE | KEY_READ, &hKey);
	if(hRes!=S_OK)
		return hRes;
	dwSize = sizeof(szBuffer);
	dwSize = RegQueryValueExW(hKey, 0, 0, &dwType, (LPBYTE)&szBuffer[0], &dwSize);
	RegCloseKey(hKey);
	if(dwSize!=ERROR_SUCCESS)
		return S_FALSE;
	// The engine set an application.
	// We got the Language string in buffer[].
	// Now We can use it to look up the engine's GUID.
again:
	dwSize = sizeof(szBuffer);
	hRes = RegOpenKeyExW(HKEY_CLASSES_ROOT, (LPCWSTR)szBuffer, 0, KEY_QUERY_VALUE | KEY_READ, &hKey);
	if(hRes!=S_OK)
	{
		// Read the GUID(in string format)
		// into buffer[] by querying the value of CLSID
		hRes = RegOpenKeyExW(hKey, L"CLSID", 0, KEY_QUERY_VALUE | KEY_READ, &hSubKey);
		if(hRes==S_OK)
		{
			dwSize = RegQueryValueExW(hSubKey, 0, 0, &dwType, (LPBYTE)&szBuffer[0], &dwSize);
			RegCloseKey(hSubKey);
		}
		else if(lpszExt)
		{
			// If an error, see if we have a "ScriptEngine"
			// key under here that contains
			// the real language name
			hRes = RegOpenKeyExW(hKey, L"ScriptEngine", 0, KEY_QUERY_VALUE | KEY_READ, &hSubKey);
			if(hRes==S_OK)
			{
				dwSize = RegQueryValueExW(hSubKey, 0, 0, &dwType, (LPBYTE)&szBuffer[0], &dwSize);
				RegCloseKey(hSubKey);
				if(!dwSize)
				{
					RegCloseKey(hKey);
					lpszExt = NULL;
					goto again;
				}
			}
		}
	}
	RegCloseKey(hKey);
	if(!dwSize)
		hRes = CLSIDFromString(szBuffer, lpGuid);

	return hRes;
}

// Returns TRUE if Windows is 64-bit
BOOL Utility::IsOS64Bit()
{
	BOOL b64Bit = FALSE;

#ifdef _WIN64
	// 64-bit applications always run under 64-bit Windows
	return TRUE;
#endif

	// Check for 32-bit applications
	typedef BOOL (WINAPI *PFNISWOW64PROCESS)(HANDLE, PBOOL);

	HMODULE hKernel32 = LoadLibraryW(L"kernel32.dll");
	if(hKernel32!=NULL)
	{
		PFNISWOW64PROCESS pfnIsWow64Process = 
			(PFNISWOW64PROCESS)GetProcAddress(hKernel32, "IsWow64Process");
		if(pfnIsWow64Process==NULL)
		{
			// If there is no IsWow64Process() API, than Windows is 32-bit for sure
			FreeLibrary(hKernel32);
			return FALSE;
		}

		pfnIsWow64Process(GetCurrentProcess(), &b64Bit);
		FreeLibrary(hKernel32);
	}

	return b64Bit;
}

// Retrieves product version info from resources embedded into EXE or DLL
BOOL Utility::GetVersion(LPCWSTR lpszModule, LPWSTR lpszVer)
{ 
	BOOL bRet = FALSE;
	if(!lpszVer)
		return bRet;
	DWORD dwBuffSize = GetFileVersionInfoSize(lpszModule, 0);
	LPBYTE pBuff = (LPBYTE)GlobalAlloc(GPTR, dwBuffSize);  
	if(NULL!=pBuff && 0!=GetFileVersionInfo(lpszModule, 0, dwBuffSize, pBuff))
	{
		VS_FIXEDFILEINFO* fi = NULL;
		UINT uLen = 0;
		VerQueryValue(pBuff, L"\\", (LPVOID*)&fi, &uLen);

		WORD dwVerMajor = HIWORD(fi->dwProductVersionMS);
		WORD dwVerMinor = LOWORD(fi->dwProductVersionMS);
		WORD dwPatchLevel = HIWORD(fi->dwProductVersionLS);
		WORD dwVerBuild = LOWORD(fi->dwProductVersionLS);
		swprintf(lpszVer, L"%u.%u.%u.%u", dwVerMajor, dwVerMinor, dwPatchLevel, dwVerBuild);  
	} 
	GlobalFree((HGLOBAL)pBuff);

	return bRet;
}

BOOL Utility::OpenFile(HWND hWnd, LPCWSTR lpszFile)
{
	ShellExecuteW(hWnd, L"open", lpszFile, NULL, NULL, SW_SHOW);
	return TRUE;
}


/*************************************************************************
 * Method:    		OpenFolder
 * Description:		打开文件所在的文件夹，并选中该文件
 * ParameterList:	HWND hWnd, LPCTSTR lpszPath
 * Parameter:       hWnd为调用该函数的窗口句柄
 * Parameter:       lpszPath为文件路径
 * Return Value:	BOOL打开成功返回TRUE，失败返回FALSE
 * Date:        	13:08:29 12:22:25
 * Author:			
 * CopyRight:		
 ************************************************************************/
BOOL Utility::OpenFolder(HWND hWnd, LPCWSTR lpszPath)
{
	if(!lpszPath)
		return FALSE;
	wchar_t szParam[1024] = {0};
	wsprintf(szParam, L"/select,%s", lpszPath);
	return (int)ShellExecute(hWnd, NULL, L"explorer.exe", szParam, 0, SW_SHOW)>32;
};

/*************************************************************************
 * Method:    		OpenUrl
 * Description:		调用默认浏览器打开url地址
 * ParameterList:	LPCWSTR lpszUrl
 * Parameter:       lpszUrl为url地址
 * Return Value:	BOOL
 * Date:        	13:08:29 12:37:04
 * Author:			
 * CopyRight:		
 ************************************************************************/
BOOL Utility::OpenUrl(HWND hWnd, LPCWSTR lpszUrl)
{
	ShellExecuteW(hWnd, L"open", lpszUrl, NULL, NULL, SW_SHOW);
	return TRUE;
}

BOOL Utility::OpenMail(HWND hWnd, LPCWSTR lpszMail)
{
	wchar_t szMailText[1024];
	swprintf(szMailText, L"mailto:%", lpszMail);
	ShellExecuteW(hWnd, L"open", szMailText, NULL, NULL, SW_SHOW);
	return TRUE;
}

BOOL Utility::GetINIStr(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, LPWSTR lpszValue)
{
	wchar_t szBuffer[1024] = {0};
	GetPrivateProfileString(lpszSection, lpszName, L"", szBuffer, 1024, lpszFileName);
	if(lpszValue)
	{
		wcscpy(lpszValue, szBuffer);
		return TRUE;
	}

	return FALSE;
}

BOOL Utility::SetINIStr(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, LPCWSTR lpszValue)
{
	return WritePrivateProfileString(lpszSection, lpszName, lpszValue, lpszFileName);
}

BOOL Utility::GetINIInt(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, int *pnValue)
{
	(*pnValue) = GetPrivateProfileInt(lpszSection, lpszName, -1, lpszFileName);
	return ((*pnValue)!=-1);
}

BOOL Utility::SetINIInt(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszName, int nValue)
{
	wchar_t szValue[32];
	swprintf(szValue, L"%d", nValue);
	return SetINIStr(lpszFileName, lpszSection, lpszName, szValue);
}

BOOL Utility::DelINISection(LPCWSTR lpszFileName, LPCWSTR lpszSection)
{
	return WritePrivateProfileSection(lpszSection, NULL, lpszFileName);
}

BOOL Utility::DelINIKey(LPCWSTR lpszFileName, LPCWSTR lpszSection, LPCWSTR lpszKey)
{
	return WritePrivateProfileString(lpszSection, lpszKey, NULL, lpszFileName);
}

BOOL Utility::TimeToTm(const time_t &t, tm * tt)
{
	if (localtime_s(tt, &t) == 0)
		return TRUE;
	return FALSE;
}

BOOL Utility::GetScreenColor(int x, int y, DWORD &dwColor)
{
	BOOL bRet = FALSE;
	dwColor = ::GetPixel(::GetDC(NULL), x, y);
	return bRet;
}

BOOL Utility::CopyTextToClipBoard(LPCSTR lpszText)
{
	if(OpenClipboard(NULL) )
	{
		HGLOBAL hGlobal = NULL;
		EmptyClipboard();
		hGlobal = GlobalAlloc(GMEM_DDESHARE, strlen(lpszText) + 1);
		char *lpData = (char*)GlobalLock(hGlobal);
		memset(lpData, 0, strlen(lpszText)+1);
		strcpy(lpData, lpszText);
		GlobalUnlock(hGlobal);
		SetClipboardData(CF_TEXT, hGlobal);
		CloseClipboard();
	}
	return TRUE;
}

void Utility::GetErrMsg(DWORD dwCode, LPWSTR lpszMsg)
{
	SetLastError(0);
	wchar_t *lpBuffer = NULL;
	/*int nByte = */FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpBuffer,
		0,
		NULL);
	DWORD dwErr = GetLastError();
	if(dwErr==0)
	{
		wcscpy(lpszMsg, lpBuffer);
		LocalFree((HLOCAL)lpBuffer);
	}
}

BOOL Utility::IsAdminPrivilege()
{
	BOOL bIsAdmin = FALSE;
	BOOL bIsVista = FALSE;
	HANDLE hToken = NULL;
	PSID pSid = NULL;
	OSVERSIONINFOEX osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL bRes = GetVersionEx((OSVERSIONINFO *)&osvi);
	if(!bRes)
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		bRes = GetVersionEx((OSVERSIONINFO *)&osvi);
		if(!bRes)
			return bRes;
	}
	if(osvi.dwMajorVersion>=6)
		bIsVista = TRUE;
	if(bIsVista)
	{
		DWORD dwType = REG_DWORD;
		DWORD dwValue = 0;
		DWORD dwData = sizeof(REG_DWORD);
		LONG lStatus = SHGetValueW(HKEY_LOCAL_MACHINE,
			L"SOLFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\",
			L"EnableLUA",
			&dwType,
			&dwValue,
			&dwData);
		if(lStatus==ERROR_SUCCESS && dwValue==0) // 当前没有打开UAC，具有管理员权限
			return TRUE;
		DWORD dwElevationType;
		DWORD dwSize = 0;
		bIsAdmin = FALSE;
		if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
			return FALSE;
		if(!hToken)
			goto cleanup;
#ifndef TokenElevationType
#define TokenElevationType 18
#endif
#ifndef TokenElevationTypeFull
#define TokenElevationTypeFull 2
#endif
		if(!GetTokenInformation(hToken, (TOKEN_INFORMATION_CLASS)TokenElevationType, &dwElevationType, sizeof(dwElevationType), &dwSize))
			goto cleanup;
		if(dwElevationType==TokenElevationTypeFull)
			bIsAdmin = TRUE;
	}
	else // Not Vista
	{
		SID_IDENTIFIER_AUTHORITY sidAuthority = SECURITY_NT_AUTHORITY;
		bIsAdmin = AllocateAndInitializeSid(
						&sidAuthority,
						2,
						SECURITY_BUILTIN_DOMAIN_RID,
						DOMAIN_ALIAS_RID_ADMINS,
						0, 0, 0, 0, 0, 0,
						&pSid);
		if(bIsAdmin && CheckTokenMembership(NULL, pSid, &bIsAdmin))
			bIsAdmin = TRUE;
	}
cleanup:
	if(hToken)
		CloseHandle(hToken);
	if(pSid)
		FreeSid(pSid);
	return bIsAdmin;
}

BOOL Utility::IsAdminUser()
{
	BOOL bIsAdminUser = FALSE;
	SID_IDENTIFIER_AUTHORITY Authority = {SECURITY_NT_AUTHORITY};
	HANDLE hToken = NULL;
	DWORD dwSize = 0;
	PTOKEN_GROUPS lpGroups = NULL;
	PSID lpSid = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return bIsAdminUser;

	if (!GetTokenInformation(hToken, TokenGroups, NULL, 0, &dwSize))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			goto cleanup;
	}
	lpGroups = (PTOKEN_GROUPS)HeapAlloc(GetProcessHeap(), 0, dwSize);
	if (lpGroups == NULL)
		goto cleanup;
	if (!GetTokenInformation(hToken, TokenGroups, lpGroups, dwSize, &dwSize))
		goto cleanup;
	CloseHandle(hToken);
	hToken = NULL;
	if (!AllocateAndInitializeSid(
			&Authority,
			2,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS, 
			0, 0, 0, 0, 0, 0,
			&lpSid))
		goto cleanup;

	for (DWORD i = 0; i < lpGroups->GroupCount; i++)
	{
		if (EqualSid(lpSid, lpGroups->Groups[i].Sid))
		{
			bIsAdminUser = TRUE;
			break;
		}
	}
cleanup:
	if(lpSid)
		FreeSid(lpSid);
	if(lpGroups)
		HeapFree(GetProcessHeap(), 0, lpGroups);
	if(hToken)
		CloseHandle(hToken);
	return bIsAdminUser;
}

BOOL Utility::GetFolderSize(LPCWSTR lpszFolderPath, DWORD &dwSize)
{
	BOOL bRet = TRUE;
	DWORD dwFolderSize = 0;
	DWORD dwFileSize = 0;
	wchar_t temp[1024] = {0};
	wcscpy_s(temp, lpszFolderPath);
	wcscat_s(temp, L"\\*.*");
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = ::FindFirstFile(temp, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return bRet;
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(_wcsicmp(FindFileData.cFileName, L".")!=0 && _wcsicmp(FindFileData.cFileName, L"..")!=0)
			{
				BOOL bSubRet = TRUE;
				DWORD dwSubSize = 0;
				wchar_t szDir[1024];
				_stprintf(szDir, L"%s\\%s", lpszFolderPath, FindFileData.cFileName);
				bSubRet = GetFolderSize(szDir, dwSubSize);
				bRet = bSubRet;
				if(bSubRet)
					dwFolderSize += dwSubSize;
				else
					break;
			}
		}
		else
		{
			dwFileSize = (FindFileData.nFileSizeHigh * (MAXDWORD+1)) + FindFileData.nFileSizeLow;
			dwFolderSize += dwFileSize;
		}
		if(!FindNextFile(hFind, &FindFileData))
			break;
	}
	dwSize += dwFolderSize;

	return bRet;
}

BOOL Utility::ExcuteCommand(LPWSTR pCommandParam)
{
	//初始化shellexe信息
	SHELLEXECUTEINFO   ExeInfo; 
	ZeroMemory(&ExeInfo, sizeof(SHELLEXECUTEINFO)); 
	ExeInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
	ExeInfo.lpFile = L"cmd.exe"; 
	ExeInfo.lpParameters = pCommandParam;
	ExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ExeInfo.nShow = SW_HIDE; 
	ExeInfo.hwnd = NULL;
	ExeInfo.lpVerb = L"runas";
	ExeInfo.lpDirectory = NULL;
	ExeInfo.hInstApp = NULL;
	//执行命令
	ShellExecuteEx(&ExeInfo);
	//等待进程结束
	WaitForSingleObject(ExeInfo.hProcess, INFINITE);//INFINITE	
	return TRUE;
}
#pragma warning(pop)
