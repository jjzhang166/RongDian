
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <SHHelper.h>

#include "FileFinder.h"

LPFNFileFinderProc lpFinderProc = NULL;

LRESULT FinderProc(LPFINDER_INFO lpInfo, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	wchar_t szText[1024];
	swprintf(szText, L"%s\n", lpInfo->szPath);
	OutputDebugStringW(szText);
	return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	// ÄÚ´æ¸ú×Ùµ÷ÊÔ
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(431);
#endif

	CFileFinder finder;
	CFinderOpts opts;
	opts.SetFolder(L"C:\\Users\\LIng\\Desktop\\NppPluginDemo");
	opts.SetText(L"dialog");
	opts.FindAllFiles();
	opts.RecusiveSearch();
	CFinderOpts opts2(opts);
	finder.SetCallback(FinderProc, NULL);
	finder.Find(opts);

	return 0;
}