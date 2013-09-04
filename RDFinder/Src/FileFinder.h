#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x600
#endif

#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <list>

using namespace std;

#include <StrUtil.h>

enum FINDER_OPTIONS
{
	FINDER_NAME			= (1L << 0),
	FINDER_SIZE			= (1L << 1),
	FINDER_DATEMODIFIED	= (1L << 2),
	FINDER_DATECREATED	= (1L << 3),
	FINDER_DATEACCESSED	= (1L << 4),
	FINDER_ATTRIBUTES	= (1L << 5),
	FINDER_TEXT			= (1L << 6)
};

enum FINDER_STATE
{
	FF_FOUND,
	FF_DISCARDED,
	FF_FOLDER,
	FF_FINDTEXT
};

typedef struct _tagFINDER_INFO
{
	wchar_t szPath[1024];
	__int64 llSize;
} FINDER_INFO, *PFINDER_INFO, *LPFINDER_INFO;

typedef LRESULT (*LPFNFileFinderProc) (LPFINDER_INFO lpInfo, WPARAM wParam, LPARAM lParam);

class CFinderOpts
{
public:
	CFinderOpts();
	~CFinderOpts();
	CFinderOpts(const CFinderOpts& );
	CFinderOpts& operator= (const CFinderOpts &);

public:
	void Set(const CFinderOpts *lpOpts);
	void Reset();
	void FindNormalFiles();
	void FindAllFiles();
	void FindDirectories();
	void SetFolder(LPCWSTR lpszFolder);
	void SetName(LPCWSTR lpszName);
	void SetText(LPCWSTR lpszText);
	void RecusiveSearch(BOOL bRecusive = TRUE) { bSubfolders = bRecusive; };
	void SetTime(int nMinYear, int nMinMonth, int nMinDay, int nMaxYear, int nMaxMonth, int nMaxDay, DWORD dwOptionsFlags);
	void SetTime(FILETIME ftMinTime, FILETIME ftMaxTime, DWORD dwOptionsFlags);
	void SetSize(ULARGE_INTEGER ulMinSize, ULARGE_INTEGER ulMaxSize);

public:
	wchar_t		szBaseFolder[1024];		// The starting folder for the search
	wchar_t		szFileMask[1024];		// File mask (e.g.: "*.txt")
	wchar_t		szFindName[1024];		// Name to find in the path
	wchar_t		szFindText[1024];		// Text to find in the files
	BOOL		bSubfolders;			// TRUE for recursive search
	DWORD		dwOptionsFlags;			// Values in FindOptionsEnum
	__int64		nMinSize;				// File minimun size
	__int64		nMaxSize;				// File maximum file
	__int64		tMinCreated;			// File oldest creation date
	__int64		tMaxCreated;			// File newest creation date
	__int64		tMinModified;			// File oldest modified date
	__int64		tMaxModified;			// File newest modified date
	__int64		tMinAccessed;			// File oldest accessed date
	__int64		tMaxAccessed;			// File newest accessed date
	DWORD		dwFileAttributes;		// like in WIN32_FIND_DATA
};

class CFileFinder
{
public:
	CFileFinder();
	virtual ~CFileFinder();

public:
	// Find files matching the mask under the base folder
	int		Find(LPCWSTR lpszBaseFolder, LPCWSTR lpszFileMask, BOOL bSubFolders = FALSE);
	// Find files matching the conditions stablished in the CFindOpts class parameter
	int		Find(CFinderOpts &opts);
	// Return the count of items found up to the moment
	int		GetFileCount() { return (int)lstFilesFound.size(); };
	// Set the find method callback function
	void	SetCallback(LPFNFileFinderProc pFileFinderProc, LPVOID pCustomParam) { lpfnProc = pFileFinderProc; lpData = pCustomParam; };
	// Stop the search process started by a call to Find (or FindFiles)
	void	StopSearch() { bStopSearch = TRUE; };
	// Return the current folder being searched
	LPCWSTR	GetSearchingFolder() { return szSearchFolder; };
	void	Reset();


private:
	int		StartSearch(CFinderOpts &opts);
	int		InternalSearch(CFinderOpts &opts);
	BOOL	FindTextInFile(LPCWSTR lpszFile, LPCWSTR lpszText);
	static DWORD WINAPI FinderTread(LPVOID lpVoid);

private:
	list<LPFINDER_INFO>	lstFilesFound;
	BOOL				bStopSearch;
	BOOL				bQuitSearch;
	LPFNFileFinderProc  lpfnProc;
	LPVOID				lpData;
	wchar_t				szSearchFolder[1024];
	CFinderOpts			stOpts;
};
