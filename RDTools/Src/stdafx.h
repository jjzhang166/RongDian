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

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////
// 以下是第三方库类文件的引用
//////////////////////////////////////////////////////////////////////////
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

// Link Lib
#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/log4cplusd.lib")
#else
#pragma comment(lib, "../RongDian/bin/log4cplus.lib")
#endif 

#define PCRE_STATIC // 静态库编译选项

#include <pcre/pcre.h>
#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/pcred.lib")
#else
#pragma comment(lib, "../RongDian/bin/pcre.lib")
#endif

#include <DuiLib/UIlib.h>
using namespace DuiLib;

#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/DuiLibd.lib")
#else
#pragma comment(lib, "../RongDian/bin/DuiLib.lib")
#endif
#include <Duilib\Control\UIMenu.h>

#include <OptParse/optparse.h>
#include <md5/md5.h>

#include <SlimXml/Src/SlimXml.h>

using namespace slim;

#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/SlimXmld.lib")
#else
#pragma comment(lib, "../RongDian/bin/SlimXml.lib")
#endif

#include <UCD/ucdetector.h>

#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/UCDd.lib")
#else
#pragma comment(lib, "../RongDian/bin/UCD.lib")
#endif

#include <libiconv/Src/include/iconv.h>
#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/libiconvd.lib")
#else
#pragma comment(lib, "../RongDian/bin/libiconv.lib")
#endif

#include <DuiMsg.h>
#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/DuiMsgd.lib")
#else
#pragma comment(lib, "../RongDian/bin/DuiMsg.lib")
#endif

#include <FileFinder.h>
#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/RDFinderd.lib")
#else
#pragma comment(lib, "../RongDian/bin/RDFinder.lib")
#endif

BOOL IsImageFile(LPCWSTR lpszFileName);

//////////////////////////////////////////////////////////////////////////
// 以下是工具类文件的引用
//////////////////////////////////////////////////////////////////////////
#include <Macros.h>
#include <StrUtil.h>
#include <Util.h>
#include <ColorUtil.h>
#include <SHHelper.h>
#include <ILangUI.h>
#include <ISkinUI.h>

//////////////////////////////////////////////////////////////////////////
// 以下是自定义文件的引用
//////////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "constant.h"
#include "RongDian.h"
#include "SystemTraySDK.h"
#include "IPanelXml.h"
#include "IAbout.h"
#include "IColorPicker.h"
#include "ICoder.h"
#include "IFileFinder.h"
#include "IFormatter.h"
#include "IIpConfig.h"
#include "MainFrame.h"

extern HINSTANCE				g_hInstance;
extern wchar_t					g_szModule[1024];
extern wchar_t					g_szAppName[1024];
extern wchar_t					g_szAppVer[1024];
extern wchar_t					g_szAppConfig[1024];
extern wchar_t					g_szPanelsXml[1024];
extern wchar_t					g_szResPath[1024];
extern wchar_t					g_szLangPath[1024];
extern wchar_t					g_szLogPath[1024];
extern wchar_t					g_szSnapShot[1024];
extern wchar_t					g_szCoderPath[1024];
extern wchar_t					g_szCoderBackupPath[1024];
extern Logger					g_Logger;
extern Options					g_OptOptions;
extern CLangManager				*g_pLangManager;
extern CSkinManager				*g_pSkinManager;
extern CSystemTray				*g_pSystemTray;
extern CMainFrame				*g_pMainFrame;
extern list<LPPANEL_INFO>		g_lstPanelInfo;

