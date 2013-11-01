#include "SHHelper.h"

#pragma warning(push)
#pragma warning(disable:4996)

BOOL SHHelper::DelFile(HWND hWnd, LPCWSTR lpszFile, BOOL bShowUI /*= TRUE*/)
{
	if(GetFileAttributesW(lpszFile)==INVALID_FILE_ATTRIBUTES)
		return FALSE;

	wchar_t szPath[MAX_PATH] = {0};
	wcscpy(szPath, lpszFile);
	SHFILEOPSTRUCTW sh = { 0 };
	sh.hwnd = hWnd;
	if(bShowUI)
		sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SIMPLEPROGRESS;// FOF_SILENT;
	else
		sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SILENT;
	sh.pFrom = szPath;
	sh.pTo = NULL;
	sh.wFunc = FO_DELETE;
	int nRet = SHFileOperationW(&sh);
	return (nRet == 0);
}

BOOL SHHelper::CopyFile(HWND hWnd, LPCWSTR  lpszFromPath, LPCWSTR  lpszToPath, BOOL bShowUI /*= TRUE*/)
{
	wchar_t szFromPath[MAX_PATH] = {0};
	wcscpy(szFromPath, lpszFromPath);
	SHFILEOPSTRUCTW sh = { 0 };
	sh.hwnd = hWnd;
	if(bShowUI)
		sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SIMPLEPROGRESS;// FOF_SILENT;
	else
		sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SILENT;
	sh.pFrom = szFromPath; // 必须保证以两个"\0"结尾
	sh.pTo = lpszToPath;
	sh.wFunc = FO_COPY;
	int nRet = SHFileOperationW(&sh);
	return (nRet == 0);
}

BOOL SHHelper::MoveFile(HWND hWnd, LPCWSTR  lpszFromPath, LPCWSTR  lpszToPath, BOOL bShowUI /*= TRUE*/)
{
	wchar_t szFromPath[MAX_PATH] = {0};
	wcscpy(szFromPath, lpszFromPath);
	SHFILEOPSTRUCTW sh = { 0 };
	sh.hwnd = hWnd;
	if(bShowUI)
		sh.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SIMPLEPROGRESS;// FOF_SILENT;
	else
		sh.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
	sh.pFrom = szFromPath; // 必须保证以两个"\0"结尾
	sh.pTo = lpszToPath;
	sh.wFunc = FO_MOVE;
	int nRet = SHFileOperationW(&sh);
	return (nRet == 0);
}

BOOL SHHelper::RenameFile(HWND hWnd, LPCWSTR lpszFromPath, LPCWSTR lpszToPath)
{
	wchar_t szFromPath[MAX_PATH] = {0};
	wcscpy(szFromPath, lpszFromPath);
	SHFILEOPSTRUCTW sh = { 0 };
	sh.fFlags = FOF_NOCONFIRMATION;
	sh.pFrom = szFromPath; // 必须保证以两个"\0"结尾
	sh.pTo = lpszToPath;
	sh.wFunc = FO_RENAME;
	sh.hwnd= hWnd;
	int nRet = SHFileOperationW(&sh);
	return (nRet == 0);
}

BOOL SHHelper::OpenFile(HWND hWnd, LPCWSTR lpszFile)
{
	return (int)ShellExecuteW(hWnd, _T("open"), lpszFile, NULL, NULL, SW_SHOW)>32;
}

BOOL SHHelper::OpenFolder(HWND hWnd, LPCWSTR lpszPath)
{
	if(!lpszPath)
		return FALSE;
	wchar_t szParams[1024];
	swprintf(szParams, L"/select,%s", lpszPath);
#pragma warning(suppress:4311)
	return (int)ShellExecuteW(hWnd, NULL, L"explorer.exe", szParams, 0, SW_SHOW)>32;
}

/*************************************************************************
 * Method:    		SelectFile
 * Description:		选择文件对话框
 * ParameterList:	HWND hWnd, LPTSTR lpszFile, LPCTSTR lpszFilter
 * Parameter:       hWnd为调用该函数的窗口句柄
 * Parameter:       lpszFile为被选文件的文件路径
 * Parameter:       lpszFilter为文件选择框的文件类型过滤字符串
 * Return Value:	成功选取文件返回TRUE，取消选择返货FALSE
 * Date:        	13:08:29 12:27:39
 * Author:			
 * CopyRight:		
 ************************************************************************/
BOOL SHHelper::SelectFile(HWND hWnd, LPWSTR lpszFile, LPCWSTR lpszFilter)
{
	BOOL bRet = FALSE;
	OPENFILENAMEW ofn;
	wchar_t szFile[1024] = {0};  
	ZeroMemory(&ofn, sizeof(OPENFILENAME));    
	ofn.lStructSize = sizeof(OPENFILENAME);    
	ofn.hwndOwner = hWnd;    
	ofn.lpstrFile = szFile;    
	ofn.lpstrFile[0] = L'\0';   
	ofn.nMaxFile = sizeof(szFile);    
	ofn.lpstrFilter = lpszFilter;//"All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";    
	ofn.nFilterIndex = 1;    
	ofn.lpstrFileTitle = NULL;    
	ofn.nMaxFileTitle = 0;    
	ofn.lpstrInitialDir = NULL;    
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;    
	bRet = GetOpenFileNameW(&ofn);
	if(bRet)
		wcscpy(lpszFile, szFile);

	return bRet;
}

BOOL SHHelper::SelectFolder(HWND hWnd, LPWSTR lpszFolder, LPCWSTR lpszTitle /*= NULL*/)
{
	BOOL bRet = FALSE;
	wchar_t szFolder[1024] = {0};
	wchar_t szTitle[1024];
	if(lpszTitle)
		wcscpy(szTitle, lpszTitle);
	BROWSEINFO bi;
	ITEMIDLIST *pidl = NULL;
	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szFolder;
	bi.lpszTitle = szTitle;
	bi.ulFlags = BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolderW(&bi);
	if(pidl == NULL)
		return bRet;
	if(!SHGetPathFromIDListW(pidl, szFolder))
		return bRet;
	wcscpy(lpszFolder, szFolder);

	return TRUE;
}
#pragma warning(pop)
