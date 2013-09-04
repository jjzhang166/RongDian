#include "FileFinder.h"

#pragma warning(push)
#pragma warning(disable:4996)

//////////////////////////////////////////////////////////////////////////
// CFileFinder
CFinderOpts::CFinderOpts()
{	
	Reset();
}

CFinderOpts::~CFinderOpts()
{

}

void CFinderOpts::Set(const CFinderOpts *lpOpts)
{
	if(!lpOpts)
		return;
	Reset();
	wcscpy(szBaseFolder, lpOpts->szBaseFolder);
	wcscpy(szFileMask, lpOpts->szFileMask);
	wcscpy(szFindName, lpOpts->szFindName);
	wcscpy(szFindText, lpOpts->szFindText);
	bSubfolders = lpOpts->bSubfolders;
	nMinSize = lpOpts->nMinSize;
	nMaxSize = lpOpts->nMaxSize;
	dwFileAttributes = lpOpts->dwFileAttributes;
	dwOptionsFlags = lpOpts->dwOptionsFlags;
	tMinCreated = lpOpts->tMinCreated;
	tMaxCreated = lpOpts->tMaxCreated;
	tMinModified = lpOpts->tMinModified;
	tMaxModified = lpOpts->tMaxModified;
	tMinAccessed = lpOpts->tMinAccessed;
	tMaxAccessed = lpOpts->tMaxAccessed;
}

void CFinderOpts::Reset()
{
	memset(szBaseFolder, 0, sizeof(szBaseFolder));
	wcscpy(szFileMask, L"*.*");
	memset(szFindText, 0, sizeof(szFindText));
	memset(szFindName, 0, sizeof(szFindName));
	bSubfolders = FALSE;
	nMinSize = nMaxSize = 0;
	dwFileAttributes = 0;
	dwOptionsFlags = 0;
	tMinCreated = 0;
	tMaxCreated = 0;
	tMinModified = 0;
	tMaxModified = 0;
	tMinAccessed = 0;
	tMaxAccessed = 0;
}

CFinderOpts::CFinderOpts(const CFinderOpts &opts)
{
	Set(&opts);
}

CFinderOpts& CFinderOpts::operator=(const CFinderOpts &opts)
{
	Set(&opts);
	return *this;
}

void CFinderOpts::FindNormalFiles()
{
	dwOptionsFlags |= FINDER_ATTRIBUTES;
	dwFileAttributes |= FILE_ATTRIBUTE_ARCHIVE;
}

void CFinderOpts::FindAllFiles()
{
	dwOptionsFlags |= FINDER_ATTRIBUTES;
	dwFileAttributes |= FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_COMPRESSED | 
		FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | 
		FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY;
}

void CFinderOpts::FindDirectories()
{
	dwOptionsFlags |= FINDER_ATTRIBUTES;
	dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
}

void CFinderOpts::SetFolder(LPCWSTR lpszFolder)
{
	if(!lpszFolder)
		return;
	memset(szBaseFolder, 0, sizeof(szBaseFolder));
	wcscpy(szBaseFolder, lpszFolder);
}

void CFinderOpts::SetName(LPCWSTR lpszName)
{
	if(!lpszName)
		return;
	dwOptionsFlags |= FINDER_NAME;
	memset(szFindName, 0, sizeof(szFindName));
	wcscpy(szFindName, lpszName);
}

void CFinderOpts::SetText(LPCWSTR lpszText)
{
	if(!lpszText)
		return;
	dwOptionsFlags |= FINDER_TEXT;
	memset(szFindText, 0, sizeof(szFindText));
	wcscpy(szFindText, lpszText);
}

void CFinderOpts::SetTime(int nMinYear, int nMinMonth, int nMinDay, int nMaxYear, int nMaxMonth, int nMaxDay, DWORD dwOptionsFlags)
{
	SYSTEMTIME stMinTime = {0}, stMaxTime = {0};
	FILETIME ftMinTime = {0}, ftMaxTime = {0};
	stMinTime.wYear = (WORD)nMinYear;
	stMinTime.wMonth = (WORD)nMinMonth;
	stMinTime.wDay = (WORD)nMinDay;
	SystemTimeToFileTime(&stMinTime, &ftMinTime);
	stMaxTime.wYear = (WORD)nMaxYear;
	stMaxTime.wMonth = (WORD)nMaxMonth;
	stMaxTime.wDay = (WORD)nMaxDay;
	SystemTimeToFileTime(&stMaxTime, &ftMaxTime);
	SetTime(ftMinTime, ftMaxTime, dwOptionsFlags);
}

void CFinderOpts::SetTime(FILETIME ftMinTime, FILETIME ftMaxTime, DWORD dwOptionsFlags)
{
	ULARGE_INTEGER ulMinTime = {0}, ulMaxTime = {0};
	ulMinTime.LowPart = ftMinTime.dwLowDateTime;
	ulMinTime.HighPart = ftMinTime.dwHighDateTime;
	ulMaxTime.LowPart = ftMaxTime.dwLowDateTime;
	ulMaxTime.HighPart = ftMaxTime.dwHighDateTime;
	if(dwOptionsFlags & FINDER_DATEMODIFIED)
	{
		tMinModified = ulMinTime.QuadPart;
		tMaxModified = ulMaxTime.QuadPart;
	}
	else if(dwOptionsFlags & FINDER_DATECREATED)
	{
		tMinCreated = ulMinTime.QuadPart;
		tMaxCreated = ulMaxTime.QuadPart;
	}
	else if(dwOptionsFlags & FINDER_DATEACCESSED)
	{
		tMinAccessed = ulMinTime.QuadPart;
		tMaxAccessed = ulMaxTime.QuadPart;
	}
}

void CFinderOpts::SetSize(ULARGE_INTEGER ulMinSize, ULARGE_INTEGER ulMaxSize)
{
	ULARGE_INTEGER ulTime = {0};
	ulTime.LowPart = ulMinSize.LowPart;
	ulTime.HighPart = ulMinSize.HighPart;
	nMinSize = ulTime.QuadPart;
	ulTime.LowPart = ulMaxSize.LowPart;
	ulTime.HighPart = ulMaxSize.HighPart;
	nMaxSize = ulTime.QuadPart;
}

//////////////////////////////////////////////////////////////////////////
// CFileFinder
CFileFinder::CFileFinder()
{
	bStopSearch = FALSE;
	bQuitSearch = TRUE;
	lpfnProc = NULL;
	lpData = NULL;
	memset(szSearchFolder, 0, sizeof(szSearchFolder));
}

CFileFinder::~CFileFinder()
{
	Reset();
}

int CFileFinder::Find(LPCWSTR szBaseFolder, LPCWSTR szFileMask, BOOL bSubFolders)
{
	CFinderOpts opts;
	wcscpy(opts.szBaseFolder, szBaseFolder);
	if(szFileMask)
		wcscpy(opts.szFileMask, szFileMask);
	opts.bSubfolders = bSubFolders;

	// Get all files, but no directories
	opts.FindAllFiles();
	Find(opts);

	return GetFileCount();
}

int CFileFinder::Find(CFinderOpts &opts)
{
	return StartSearch(opts);
}

BOOL CFileFinder::FindTextInFile(LPCWSTR lpszFile, LPCWSTR lpszText)
{
	BOOL bRet = FALSE;
	if(!lpszFile || !lpszText)
		return bRet;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	HANDLE hFile = ::CreateFile(lpszFile, GENERIC_READ, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	const UINT	nCMinBufSize = 512;
	char		*lpszBuffer = NULL;
	char		*lpszFindText = NULL;
	UINT		nSearchLen = wcslen(lpszText);
	UINT		nBufSize = 512;
	UINT		nReadSize = 0;
	if ((2 * nSearchLen) >= nCMinBufSize) // 
		nBufSize = (2 * nSearchLen+1);
	lpszFindText = new char[nSearchLen+1];
	if(!lpszFindText)
		return bRet;
	memset(lpszFindText, 0, (nSearchLen+1)*sizeof(char));
	StrUtil::w2a(lpszText, lpszFindText);
	_strupr(lpszFindText);
	lpszBuffer = new char[nBufSize];
	if(!lpszBuffer)
		return bRet;
	memset(lpszBuffer, 0, nBufSize*sizeof(char));
	nReadSize = nBufSize - nSearchLen;
	DWORD dwRead = 0;
	char *lpBufPtr = lpszBuffer;
	BOOL bFirstRead = TRUE;
	BOOL bFound = FALSE;
	do 
	{
		if(bFirstRead)
			lpBufPtr = lpszBuffer;
		bRet = ::ReadFile(hFile, lpBufPtr, nBufSize/2-1, &dwRead, NULL);
		if(!bRet)
			return bRet;
		//char *lpszPtr = (char *)lpBufPtr;
		_strupr(lpszBuffer);
		if(strstr(lpszBuffer, lpszFindText))
		{
			bFound = TRUE;
			break;
		}
		//char *lptemp = strstr(lpszBuffer, lpszFindText);
		if(bFirstRead)
		{
			bFirstRead = FALSE;
			lpBufPtr += dwRead;
		}
		else
		{
			memmove(lpszBuffer, lpBufPtr, dwRead);
			lpBufPtr = lpszBuffer;
			lpBufPtr += dwRead;
			memset(lpBufPtr, 0, nBufSize-dwRead);
		}
	} while (bRet && dwRead && !bFound && !bStopSearch);
	delete lpszFindText;
	delete lpszBuffer;
	::CloseHandle(hFile);

	return bFound;
}

void CFileFinder::Reset()
{
	lpfnProc = NULL;
	bStopSearch = FALSE;
	bQuitSearch = TRUE;
	lpfnProc = NULL;
	lpData = NULL;
	memset(szSearchFolder, 0, sizeof(szSearchFolder));
	stOpts.Reset();
	LPFINDER_INFO lpInfo = NULL;
	list<LPFINDER_INFO>::iterator iter;
	if(!lstFilesFound.size())
		return;
	lpInfo = (LPFINDER_INFO)lstFilesFound.front();
	while(lpInfo)
	{
		lstFilesFound.pop_front();
		delete lpInfo;
		lpInfo = NULL;
		if(!lstFilesFound.size())
			break;
		lpInfo = (LPFINDER_INFO)lstFilesFound.front();
	}
}

int CFileFinder::StartSearch(CFinderOpts &opts)
{
	DWORD dwThread = 0;
	stOpts = opts;
	HANDLE hThread = CreateThread(NULL, 0, FinderTread, (LPVOID)this, 0, &dwThread);
	if(!hThread)
		return -1;
	CloseHandle(hThread);

	return 0;
}

int CFileFinder::InternalSearch(CFinderOpts &opts)
{
	HANDLE		hFileFinder = NULL;
	wchar_t		szFullMask[1024] = {0};
	CFinderOpts	subOpts;
	BOOL		bFound = TRUE, bValidFile = TRUE;

	bStopSearch = FALSE;
	bQuitSearch = FALSE;

	wcscpy(opts.szBaseFolder, opts.szBaseFolder);
	PathAddBackslashW(opts.szBaseFolder);

	// Find directories
	if (opts.bSubfolders)
	{
		swprintf(szFullMask, L"%s%s", opts.szBaseFolder, opts.szFileMask);
		WIN32_FIND_DATAW FindFileData = {0};
		hFileFinder = ::FindFirstFileW(szFullMask, &FindFileData);
		if(INVALID_HANDLE_VALUE == hFileFinder)
			return -1;
		BOOL bFolder = FALSE;
		wchar_t szCurPath[1024];
		ULARGE_INTEGER nFileSize = {0};
		ULARGE_INTEGER ulTime = {0};
		while ((bFound) && (!bStopSearch))
		{
			memset(szCurPath, 0, sizeof(szCurPath));
			wcscpy_s(szCurPath, opts.szBaseFolder);
			PathAppendW(szCurPath, FindFileData.cFileName);
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) // 忽略硬连接文件
			{
				bFolder = TRUE;
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(wcsicmp(FindFileData.cFileName, L".")==0 || wcsicmp(FindFileData.cFileName, L"..")==0)
				{
					if(!FindNextFileW(hFileFinder, &FindFileData))
						break;
					continue;
				}
				subOpts = opts;
				wcscpy(subOpts.szBaseFolder, szCurPath);
				InternalSearch(subOpts);
				bFolder = TRUE;
			}
			if(!bFolder)
			{
				bValidFile = TRUE;
				nFileSize.LowPart = FindFileData.nFileSizeLow;
				nFileSize.HighPart = FindFileData.nFileSizeHigh;
				if(opts.dwOptionsFlags & FINDER_ATTRIBUTES)
					bValidFile = FindFileData.dwFileAttributes & opts.dwOptionsFlags;
				if(bValidFile && (opts.dwOptionsFlags & FINDER_SIZE))
					bValidFile = ((opts.nMinSize <= (__int64)nFileSize.QuadPart) && (opts.nMaxSize >= (__int64)nFileSize.QuadPart));
				ulTime.LowPart = FindFileData.ftLastWriteTime.dwLowDateTime;
				ulTime.HighPart = FindFileData.ftLastWriteTime.dwHighDateTime;
				if(bValidFile && (opts.dwOptionsFlags & FINDER_DATEMODIFIED))
					bValidFile = (((__int64)ulTime.QuadPart >= opts.tMinModified) && ((__int64)ulTime.QuadPart <= opts.tMaxModified));
				ulTime.LowPart = FindFileData.ftCreationTime.dwLowDateTime;
				ulTime.HighPart = FindFileData.ftCreationTime.dwHighDateTime;
				if(bValidFile && (opts.dwOptionsFlags & FINDER_DATECREATED))
					bValidFile = (((__int64)ulTime.QuadPart >= opts.tMinCreated) && ((__int64)ulTime.QuadPart <= opts.tMaxCreated));
				ulTime.LowPart = FindFileData.ftLastAccessTime.dwLowDateTime;
				ulTime.HighPart = FindFileData.ftLastAccessTime.dwHighDateTime;
				if(bValidFile && (opts.dwOptionsFlags & FINDER_DATEACCESSED))
					bValidFile = (((__int64)ulTime.QuadPart >= opts.tMinAccessed) && ((__int64)ulTime.QuadPart <= opts.tMaxAccessed));
				if(bValidFile && (opts.dwOptionsFlags & FINDER_NAME))
					bValidFile = wcsstr(szCurPath, opts.szFindName) ? TRUE : FALSE;
				if(bValidFile && (opts.dwOptionsFlags & FINDER_TEXT))
					bValidFile = FindTextInFile(szCurPath, opts.szFindText);
				if(bValidFile)
				{
					LPFINDER_INFO lpFinderInfo = new FINDER_INFO();
					memset(lpFinderInfo, 0, sizeof(FINDER_INFO));
					wcscpy(lpFinderInfo->szPath, szCurPath);
					lpFinderInfo->llSize = nFileSize.QuadPart;
					lstFilesFound.push_back(lpFinderInfo);
					if(lpfnProc)
						lpfnProc(lpFinderInfo, (WPARAM)&FindFileData, (LPARAM)lpData);
				}
			}
			bFound = FindNextFileW(hFileFinder, &FindFileData);
			bFolder = FALSE;
		}
	}

	return GetFileCount();
}

DWORD WINAPI CFileFinder::FinderTread(LPVOID lpVoid)
{
	CFileFinder *pFinder = (CFileFinder *)lpVoid;
	if(!pFinder)
		return 0;
	return pFinder->InternalSearch(pFinder->stOpts);
}
#pragma warning(pop)

