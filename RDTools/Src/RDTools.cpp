// RDTools.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RDTools.h"

HINSTANCE			g_hInstance;
wchar_t				g_szModule[1024] = {0};
wchar_t				g_szAppName[1024] = {0};
wchar_t				g_szAppVer[1024] = {0};
wchar_t				g_szAppConfig[1024] = {0};
wchar_t				g_szPanelsXml[1024] = {0};
wchar_t				g_szResPath[1024] = {0};
wchar_t				g_szLogPath[1024] = {0};
wchar_t				g_szLangPath[1024] = {0};
wchar_t				g_szSnapShot[1024] = {0};
wchar_t				g_szCoderPath[1024] = {0};
wchar_t				g_szCoderBackupPath[1024] = {0};
Logger				g_Logger;
Options				g_OptOptions;
Parser				g_OptParser;
CLangManager		*g_pLangManager = NULL;
CSkinManager		*g_pSkinManager = NULL;
CSystemTray			*g_pSystemTray = NULL;
CMainFrame			*g_pMainFrame = NULL;
list<LPPANEL_INFO>	g_lstPanelInfo;


BOOL CheckInstance()
{
	HWND hHwnd = ::FindWindow(kRDToolsClass, NULL);
	if(!hHwnd)
	{
		hHwnd = ::FindWindow(_T("Static"), kRDToolsStaticClass);
		if(hHwnd)
			hHwnd = ::GetWindow(hHwnd, GW_CHILD);
	}
	if(hHwnd)
	{
		//SendMessage(hHwnd, WM_ACTIVE_MAIN, 0, 0); // 存在Win7 UI隔离问题，低权限应用无法跟高权限应用发送消息
		COPYDATASTRUCT data;
		data.dwData = WM_ACTIVE_MAIN;
		data.cbData = 0;
		data.lpData = NULL;
		::SendMessage(hHwnd, WM_COPYDATA, (WPARAM)(HWND)hHwnd, (LPARAM)(LPVOID)&data);
		return FALSE;
	}
	return TRUE;
}

BOOL IsImageFile(LPCWSTR lpszFileName)
{
	BOOL bRet = PathFileExistsW(lpszFileName);
	if(!bRet)
		return bRet;
	LPCWSTR lpszExt = PathFindExtensionW(lpszFileName);
	if(!lpszExt)
		return bRet;
	if(wcsicmp(kJPEGExt, lpszExt)==0)
		return TRUE;
	else if(wcsicmp(kJPGExt, lpszExt)==0)
		return TRUE;
	else if(wcsicmp(kPNGExt, lpszExt)==0)
		return TRUE;
	else if(wcsicmp(kBMPExt, lpszExt)==0)
		return TRUE;

	return FALSE;
}

BOOL InitPath()
{
	wchar_t szModule[1024];
	GetModuleFileNameW(g_hInstance, szModule, _countof(szModule));
	Utility::GetVersion(szModule, g_szAppVer);
	LPCWSTR lpszAppName = PathFindFileName(szModule);
	wcscpy(g_szAppName, lpszAppName);
	PathRemoveExtension(g_szAppName);
	PathRemoveFileSpec(szModule);
	swprintf(g_szAppConfig, L"%s\\%s", szModule, kConfigIni);
	wcscpy(g_szModule, szModule);
	PathRemoveFileSpec(g_szModule);
	swprintf(g_szLogPath, L"%s\\log", g_szModule);
	swprintf(g_szLangPath, L"%s\\lang", g_szModule);
	swprintf(g_szResPath, L"%s\\res", g_szModule);
	swprintf(g_szSnapShot, L"%s\\img", g_szModule);
	swprintf(g_szCoderPath, L"%s\\coder", g_szModule);
	swprintf(g_szCoderBackupPath, L"%s\\backup", g_szCoderPath);
	swprintf(g_szPanelsXml, L"%s\\%s", g_szResPath, kPanelsXml);
	if(!PathFileExists(g_szLogPath))
		SHCreateDirectoryEx(NULL, g_szLogPath, NULL);
	
	return TRUE;
}

BOOL InitLog()
{
	wchar_t szLogPath[1024], szLogName[1024];
	time_t _time = time(NULL);
	tm t;
	Utility::TimeToTm(_time, &t);
	swprintf(szLogName, L"%s_%04d_%02d_%02d.log", g_szAppName, t.tm_year+1900, t.tm_mon+1, t.tm_mday);
	swprintf(szLogPath, L"%s\\log\\%s", g_szModule, szLogName);
	SharedAppenderPtr pFileAppender(new RollingFileAppender(szLogPath));
	pFileAppender->setLayout(std::auto_ptr<Layout>(new PatternLayout(L"%d{%Y-%m-%d %H:%M:%S} - %m [%l]%n")));

	g_Logger = Logger::getInstance(L"Logger");
	g_Logger.setLogLevel(ALL_LOG_LEVEL);
	g_Logger.addAppender(pFileAppender);

	LOG4CPLUS_INFO(g_Logger, "RDTools Start.");

	return TRUE;
}

BOOL InitArgs()
{
	return TRUE;
}

BOOL InitSkin()
{
	wchar_t szSkinName[1024];
	if(!PathFileExists(g_szAppConfig))
		return FALSE;
	g_pSkinManager = CSkinManager::Instance();
	if(!g_pSkinManager)
		return 0;
	int nColorBk = -1;
	if(Utility::GetINIInt(g_szAppConfig, LS_SKIN, kIsColor, &nColorBk) && Utility::GetINIStr(g_szAppConfig, LS_SKIN, kColor, szSkinName))
	{
		if(nColorBk)
		{
			wchar_t *pszStopStr = NULL;
			int nBase = 16;
			DWORD dwColor = wcstoul(szSkinName, &pszStopStr, nBase);
			g_pSkinManager->SetBkColor(dwColor);
		}
		else
		{
			wchar_t szBkImage[1024];
			swprintf(szBkImage, L"%s\\%s", g_szResPath, szSkinName);
			if(PathFileExists(szBkImage) && IsImageFile(szBkImage))
				g_pSkinManager->SetBkImage(szBkImage);
		}
	}
	
	return TRUE;
}

BOOL InitLang()
{
	wchar_t szLangName[1024];
	if(!PathFileExists(g_szAppConfig))
		return FALSE;
	Utility::GetINIStr(g_szAppConfig, LS_LANG, L"lang", szLangName);
	wchar_t szLang[1024];
	swprintf(szLang, L"%s\\%s", g_szLangPath, szLangName);
	g_pLangManager = CLangManager::Instance();
	if(!g_pLangManager)
		return 0;
	g_pLangManager->SetLangName(szLang);

	return TRUE;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	// 内存跟踪调试
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(431);
#endif

	g_hInstance = hInstance;
	
	if(!CheckInstance())
		return 0;

	HRESULT hr = ::CoInitialize(NULL);
	if(FAILED(hr)) 
		return 0;
	hr = ::OleInitialize(NULL);
	if(FAILED(hr)) 
		return 0;

	HINSTANCE hInstRich = ::LoadLibraryW(L"Riched20.dll");
	if(!hInstRich) 
		return 0;

	InitPath();

	InitLog();
	
	InitArgs();

	InitSkin();

	if(!InitLang())
	{
		const wchar_t* const ERR_MSG = L"RDTools Can't Find Lang File.";
		const wchar_t* const ERR_CAPTION = L"Error";
		char szMsg[1024];
		StrUtil::w2a(ERR_MSG, szMsg);
		MessageBox(NULL, ERR_MSG, ERR_CAPTION, MB_OK);
		LOG4CPLUS_ERROR(g_Logger, szMsg);
		return 0;
	}

	CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::SetResourcePath(g_szResPath);
	wchar_t szMsgIconPath[1024] = {0};
#if USE_ZIPRES
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	swprintf(szMsgIconPath, L"default\\%s", kMsgIconName);
#else
	CPaintManagerUI::SetResourcePath(g_szResPath);
	swprintf(szMsgIconPath, L"%s\\default\\%s", g_szResPath, kMsgIconName);
#endif
	InitDuiMsg(hInstance, (LPVOID)g_pLangManager, (LPVOID)g_pSkinManager, szMsgIconPath);

	g_pMainFrame = new CMainFrame();
	if(!g_pMainFrame)
		return 0;

	g_pMainFrame->Create(NULL, L"", UI_WNDSTYLE_FRAME | UI_CLASSSTYLE_DIALOG, 0, 0, 0, 0, 0);
	g_pMainFrame->SetIcon(IDI_RDTOOLS);
	g_pMainFrame->CenterWindow();
	::ShowWindow(*g_pMainFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();

	WindowImplBase::ReleaseRes();

	::OleUninitialize();
	::CoUninitialize();

	::FreeLibrary(hInstRich);

	CLangManager::Release();
	CSkinManager::Release();

	LOG4CPLUS_INFO(g_Logger, "RDTools Quit.");

	return 0;
}
