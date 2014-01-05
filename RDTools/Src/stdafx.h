// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Shlwapi.h>
#include <shlobj.h>
#include <Commdlg.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Comdlg32.lib")

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <process.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////
// �����ǵ����������ļ�������
//////////////////////////////////////////////////////////////////////////

#include <OptParse/optparse.h>
#include <md5/md5.h>

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>

#include <UCD/ucdetector.h>
#include <libiconv/Src/include/iconv.h>
#include <SlimXml/Src/SlimXml.h>
#include <StringTokenizer/StringTokenizer.h>
#include <GifLib/lib/gif_lib_private.h>
#include <DuiLib/UIlib.h>
#include <Tidy/include/tidy.h>
#include <Tidy/include/buffio.h>
#include <AsTidyLib/astyle.h>
#include <CssTidyLib/formatter.h>
#include <JsonTidyLib/json.h>
#include <JsonTidyLib/jsontidy.h>
#include <JsTidyLib/jstidy.h>
#include <PhpTidyLib/phptidy.h>
#include <SqlTidyLib/sqltidy.h>

#include <Duilib/Control/UIMenu.h>
#include <DuiMsg.h>
#include <FileFinder.h>
#include <RDNet.h>

using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace slim;
using namespace DuiLib;

// Link Lib
#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/log4cplusd.lib")
#pragma comment(lib, "../RongDian/bin/pcred.lib")
#pragma comment(lib, "../RongDian/bin/pcred.lib")
#pragma comment(lib, "../RongDian/bin/UCDd.lib")
#pragma comment(lib, "../RongDian/bin/libiconvd.lib")
#pragma comment(lib, "../RongDian/bin/SQLite.lib")
#pragma comment(lib, "../RongDian/bin/SlimXmld.lib")
#pragma comment(lib, "../RongDian/bin/Tidy.lib")
#pragma comment(lib, "../RongDian/bin/AsTidyLibd.lib")
#pragma comment(lib, "../RongDian/bin/CssTidyLibd.lib")
#pragma comment(lib, "../RongDian/bin/JsonTidyLibd.lib")
#pragma comment(lib, "../RongDian/bin/JsTidyLibd.lib")
#pragma comment(lib, "../RongDian/bin/PhpTidyLibd.lib")
#pragma comment(lib, "../RongDian/bin/SqlTidyLibd.lib")
#pragma comment(lib, "../RongDian/bin/DuiLibd.lib")
#pragma comment(lib, "../RongDian/bin/DuiMsgd.lib")
#pragma comment(lib, "../RongDian/bin/RDFinderd.lib")
#pragma comment(lib, "../RongDian/bin/RDNetd.lib")
#else
#pragma comment(lib, "../RongDian/bin/log4cplus.lib")
#pragma comment(lib, "../RongDian/bin/pcre.lib")
#pragma comment(lib, "../RongDian/bin/UCD.lib")
#pragma comment(lib, "../RongDian/bin/libiconv.lib")
#pragma comment(lib, "../RongDian/bin/SQLite.lib")
#pragma comment(lib, "../RongDian/bin/SlimXml.lib")
#pragma comment(lib, "../RongDian/bin/Tidy.lib")
#pragma comment(lib, "../RongDian/bin/AsTidyLib.lib")
#pragma comment(lib, "../RongDian/bin/CssTidyLib.lib")
#pragma comment(lib, "../RongDian/bin/JsonTidyLib.lib")
#pragma comment(lib, "../RongDian/bin/JsTidyLib.lib")
#pragma comment(lib, "../RongDian/bin/PhpTidyLib.lib")
#pragma comment(lib, "../RongDian/bin/SqlTidyLib.lib")
#pragma comment(lib, "../RongDian/bin/DuiLib.lib")
#pragma comment(lib, "../RongDian/bin/DuiMsg.lib")
#pragma comment(lib, "../RongDian/bin/RDFinder.lib")
#pragma comment(lib, "../RongDian/bin/RDNet.lib")
#endif 

BOOL				IsImageFile(LPCWSTR lpszFileName);
BOOL				IsCanTidy(LPCWSTR lpszLang);
int					EscapeSQLite(CDuiString strKeyWord);
int					RDMsgBox(HWND hWnd, int nTextId, int nCaptionId, UINT uType);

BOOL				ShowLoading();
void				CancelLoading();

//////////////////////////////////////////////////////////////////////////
// �����ǹ������ļ�������
//////////////////////////////////////////////////////////////////////////
#include <Macros.h>
#include <StrUtil.h>
#include <Util.h>
#include <ColorUtil.h>
#include <SHHelper.h>
#include <ILangUI.h>
#include <ISkinUI.h>
#include <AdapterUtil.h>
#include <SQLiteUtil.h>
#include <PCREUtil.h>
#include <ConfigTable.h>
#include <CharSetTable.h>
#include <AddrTable.h>

//////////////////////////////////////////////////////////////////////////
// �������Զ����ļ�������
//////////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "constant.h"
#include "RongDian.h"
#include "ITools.h"
#include "SystemTraySDK.h"
#include "TidyHelper.h"
#include "MainFrame.h"

extern HANDLE					g_hUserSignature;
extern HANDLE					g_hAdminSignature;
extern BOOL						g_bActiveRunAsAdminThread;
extern HINSTANCE				g_hInstance;
extern wchar_t					g_szModule[1024];
extern wchar_t					g_szAppName[1024];
extern wchar_t					g_szAppVer[1024];
//extern wchar_t					g_szAppConfig[1024];
extern wchar_t					g_szAppConfigDB[1024];
extern wchar_t					g_szPanelsXml[1024];
extern wchar_t					g_szResPath[1024];
extern wchar_t					g_szLangPath[1024];
extern wchar_t					g_szLogPath[1024];
extern wchar_t					g_szSnapShot[1024];
extern wchar_t					g_szCoderPath[1024];
extern wchar_t					g_szCoderBackupPath[1024];
extern wchar_t					g_szHostsPath[1024];
extern wchar_t					g_szBackground[1024];
extern wchar_t					g_szLangFile[1024];
extern wchar_t					g_szAddrSolution[1024];
extern Logger					g_Logger;
extern Options					g_OptOptions;
extern CLangManager				*g_pLangManager;
extern CSkinManager				*g_pSkinManager;
extern CSystemTray				*g_pSystemTray;
extern CMainFrame				*g_pMainFrame;
extern list<LPMAIN_PANEL>		g_lstPanelInfo;
extern CSQLite					g_SQLite;
extern IMG_INFO					g_ImgInfo[MAX_IMG_TYPE];
extern TIDY_INFO				g_TidyInfo[MAX_TIDY_LANG];
extern MSG_INFO					g_LangIDs[MAX_MSG_ID];

