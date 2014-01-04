// RDTools.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RDGlobal.h"
#include "ITools.h"

const wchar_t*	const	ARGS_ADMIN			= L"-a";
const wchar_t*	const	ARGS_LONG_ADMIN		= L"--admin";
const wchar_t*	const	ARGS_ADMIN_DESC		= L"RDTools Run as Administrator Mode";


DWORD WINAPI UserSignatureThread(LPVOID /*lpData*/)
{
	if(!g_hUserSignature)
		return 0;
	WaitForSingleObject(g_hUserSignature, INFINITE);
	if(g_pMainFrame)
		SendMessage(g_pMainFrame->GetHWND(), WM_CLOSE, 0, 0);
	return 0;
}

BOOL ActiveInstance()
{
	HWND hHwnd = ::FindWindow(kRDToolsClass, NULL);
	if(!hHwnd)
	{
		hHwnd = ::FindWindow( L"Static", kRDToolsStaticClass);
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
		return TRUE;
	}
	return FALSE;
}

BOOL CheckInstance()
{
	BOOL bAdminMode = FALSE;
	BOOL bIsAdmin = Utility::IsAdminPrivilege();
	if(g_OptOptions.isSet(ARGS_ADMIN))
		bAdminMode = TRUE;
	else if(g_OptOptions.isSet(ARGS_LONG_ADMIN))
		bAdminMode = TRUE;
	if(!bIsAdmin && bAdminMode) // 管理员模式，但当前进程无管理员权限
	{
		MessageBox(NULL, L"Admin Mode Need Admin Privilege.", L"Error", MB_OK);
		return FALSE;
	}
	if(bIsAdmin)
		bAdminMode = TRUE;
	if(!bAdminMode) // 普通用户模式
	{
		g_hUserSignature = CreateEvent(NULL, TRUE, 0, kUserSignature);
		int nErr = GetLastError();
		if(nErr==0) // 创建用户模式标识成功
		{
			g_hAdminSignature = CreateEvent(NULL, TRUE, 0, kAdminSignature);
			nErr = GetLastError();
			if(nErr==0) // 创建管理员模式标识成功，说明系统不存在多个RDTools
			{
				// 关闭管理员模式标识，进入启动RDTools流程
				CloseHandle(g_hAdminSignature);
				g_hAdminSignature = NULL;
				DWORD dwThreadID = 0;
				HANDLE hThread = CreateThread(NULL, 0, UserSignatureThread, NULL, 0, &dwThreadID);
				CloseHandle(hThread);
				return TRUE;
			}
			else if(nErr==ERROR_ALREADY_EXISTS) // 已经存在管理员模式的RDTools
			{
				// 关闭普通用户模式、管理员模式标识，激活管理员模式RDTools，并退出自己
				CloseHandle(g_hAdminSignature);
				CloseHandle(g_hUserSignature);
				g_hAdminSignature = NULL;
				g_hUserSignature = NULL;
				ActiveInstance();
				return FALSE;
			}
			CloseHandle(g_hUserSignature);
			g_hUserSignature = NULL;
			MessageBox(NULL, L"Create User Signature Error!", L"Error", MB_OK);
			return FALSE; // 未知错误则退出自己
		}
		else if(nErr==ERROR_ALREADY_EXISTS)
		{
			CloseHandle(g_hUserSignature);
			g_hUserSignature = NULL;
			ActiveInstance();
			return FALSE;
		}
		MessageBox(NULL, L"Create User Signature Error!", L"Error", MB_OK);
		return FALSE; // 未知错误则退出自己
	}
	else // 管理员模式
	{
		g_hUserSignature = CreateEvent(NULL, TRUE, 0, kUserSignature);
		int nErr = GetLastError();
		if(nErr==0) // 创建用户模式标识成功，即不存在用户模式RDTools
		{
			CloseHandle(g_hUserSignature);
			g_hUserSignature = NULL;
			g_hAdminSignature = CreateEvent(NULL, TRUE, 0, kAdminSignature);
			nErr = GetLastError();
			if(nErr==0)
				return TRUE; // 成功创建管理员模式标识，进入启动RDTools流程
			if(nErr==ERROR_ALREADY_EXISTS) // 已经存在管理员模式RDTools，，激活管理员模式RDTools，并退出自己
			{
				CloseHandle(g_hAdminSignature);
				g_hAdminSignature = NULL;
				ActiveInstance();
				return FALSE;
			}
			CloseHandle(g_hAdminSignature);
			g_hAdminSignature = NULL;
			MessageBox(NULL, L"Create Admin Signature Error!", L"Error", MB_OK);
			return FALSE; // 未知错误则退出自己
		}
		else if(nErr==ERROR_ALREADY_EXISTS)
		{
			SetEvent(g_hUserSignature);
			Sleep(1000);
			CloseHandle(g_hUserSignature);
			g_hUserSignature = NULL;
			g_hAdminSignature = CreateEvent(NULL, TRUE, 0, kAdminSignature);
			nErr = GetLastError();
			if(nErr==0)
				return TRUE; // 成功创建管理员模式标识，进入启动RDTools流程
			MessageBox(NULL, L"Create Admin Signature Error!", L"Error", MB_OK);
			return FALSE;
		}
	}
	
	return FALSE;
}

BOOL IsImageFile(LPCWSTR lpszFileName)
{
	BOOL bRet = PathFileExistsW(lpszFileName);
	if(!bRet)
		return bRet;
	LPCWSTR lpszExt = PathFindExtensionW(lpszFileName);
	if(!lpszExt)
		return bRet;
	for(int i=0; i<IMG_UNDEFINE; i++)
	{
		if(wcsicmp(g_ImgInfo[i].szName, lpszExt)==0)
		{
			if(i!=IMG_UNDEFINE && g_ImgInfo[i].bSupport)
			{
				bRet = TRUE;
				break;
			}
		}
	}
	return bRet;
}

BOOL IsCanTidy(LPCWSTR lpszLang)
{
	BOOL bRet = FALSE;

	for(int i=0; i<TIDY_END; i++)
	{
		if(wcsicmp(g_TidyInfo[i].szName, lpszLang)==0)
		{
			if(i!=TIDY_END)
			{
				bRet = TRUE;
				break;
			}
		}
	}
	return bRet;
}

/*
 /   ->    //
 '   ->    ''
 [   ->    /[
 ]   ->    /]
 %   ->    /%
 &   ->    /&
 _   ->    /_
 (   ->    /(
 )   ->    /)
 */
/*************************************************************************
 * Method:    		EscapeSQLite
 * Description:		sqlite转义符处理 
 * ParameterList:	LPSTR lpszValue
 * Parameter:       lpszValue为sqlite语句
 * Return Value:	int为返回字符串长度
 * Date:        	13:08:29 12:37:04
 * Author:			
 * CopyRight:		
 ************************************************************************/
int EscapeSQLite(CDuiString strKeyWord)
{
	strKeyWord.Replace(L"/", L"//");
	strKeyWord.Replace(L"'", L"''");
	strKeyWord.Replace(L"[", L"/[");
	strKeyWord.Replace(L"]", L"/]");
	strKeyWord.Replace(L"%", L"/%");
	strKeyWord.Replace(L"&",L"/&");
	strKeyWord.Replace(L"_", L"/_");
	strKeyWord.Replace(L"(", L"/(");
	strKeyWord.Replace(L")", L"/)");
	return 0;
}

int RDMsgBox(HWND hWnd, int nTextId, int nCaptionId, UINT uType)
{
#pragma message("RDMsgBox消息框注意项：调用\"RDMsgBox\"请确保已经调用\"AssocLangIDs\" && \"InitDuiMsg\"初始化.")
	int nRet = MB_OK;
	wchar_t szErr[1024], szTitle[1024];
	Utility::GetINIStr(g_pLangManager->GetLangName(), GET_ASSOC_SECTION(g_LangIDs, nCaptionId), GET_ASSOC_ID(g_LangIDs, nCaptionId), szTitle);
	Utility::GetINIStr(g_pLangManager->GetLangName(), GET_ASSOC_SECTION(g_LangIDs, nTextId), GET_ASSOC_ID(g_LangIDs, nTextId), szErr);
	nRet = DuiMsgBox(hWnd, szErr, szTitle, uType);
	return nRet;
}

BOOL ShowLoading()
{
	if(g_pMainFrame)
		return g_pMainFrame->ShowLoading();
	return FALSE;
}

void CancelLoading()
{
	if(g_pMainFrame)
		return g_pMainFrame->CancelLoading();
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
	//swprintf(g_szAppConfig, L"%s\\%s", szModule, kConfigIni);
	swprintf(g_szAppConfigDB, L"%s\\%s", szModule, kConfigDB);
	wcscpy(g_szModule, szModule);
	PathRemoveFileSpec(g_szModule);
	swprintf(g_szLogPath, L"%s\\log", g_szModule);
	swprintf(g_szLangPath, L"%s\\lang", g_szModule);
	swprintf(g_szResPath, L"%s\\res", g_szModule);
	swprintf(g_szSnapShot, L"%s\\img", g_szModule);
	swprintf(g_szCoderPath, L"%s\\coder", g_szModule);
	swprintf(g_szCoderBackupPath, L"%s\\backup", g_szCoderPath);
	swprintf(g_szHostsPath, L"%s\\hosts", g_szModule);
	swprintf(g_szPanelsXml, L"%s\\%s", g_szResPath, kPanelsXml);
	if(!PathFileExists(g_szLogPath))
		SHCreateDirectoryEx(NULL, g_szLogPath, NULL);
	if(!PathFileExists(g_szHostsPath))
		SHCreateDirectoryEx(NULL, g_szHostsPath, NULL);
	
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

	LOG4CPLUS_INFO(g_Logger, L"RDTools Start.");

	return TRUE;
}

BOOL InitArgs()
{
	int nRetCode = E_OK;
	int nCount = 0;
	LPWSTR* lpszArgs = CommandLineToArgvW(GetCommandLine(), &nCount);
	nRetCode = g_OptOptions.addOption(ARGS_ADMIN, ARGS_LONG_ADMIN, ARGS_ADMIN_DESC, OPT_NONE, NULL);
	if(nRetCode==E_OK)
	{
		g_OptParser.setFlags(OPT_PARSE_QUOTED_ARGS | OPT_PARSE_UNQUOTE_ARGS | OPT_PARSE_AUTO_HELP | OPT_PARSE_INCLUDE_ARGV0);
		nRetCode = g_OptParser.parse(nCount, lpszArgs, g_OptOptions);
	}
	LocalFree(lpszArgs);

	return (nRetCode==E_OK || nRetCode==E_OPT_UNKNOWN);
}

BOOL InitSkin()
{
	g_pSkinManager = CSkinManager::Instance();
	if(!g_pSkinManager)
		return 0;
	wchar_t szBkImage[1024];
	swprintf(szBkImage, L"%s\\%s", g_szResPath, g_szBackground);
	if(PathFileExists(szBkImage) && IsImageFile(szBkImage))
	{
		g_pSkinManager->SetBkImage(szBkImage);
		return TRUE;
	}
	wchar_t *pszStopStr = NULL;
	int nBase = 16;
	DWORD dwColor = wcstoul(g_szBackground, &pszStopStr, nBase);
	g_pSkinManager->SetBkColor(dwColor);
	
	return TRUE;
}

BOOL InitLang()
{
	wchar_t szLang[1024];
	swprintf(szLang, L"%s\\%s", g_szLangPath, g_szLangFile);
	if(!PathFileExists(szLang))
		return FALSE;
	g_pLangManager = CLangManager::Instance();
	if(!g_pLangManager)
		return 0;
	g_pLangManager->SetLangName(szLang);

	return TRUE;
}

BOOL AssocLangIDs()
{
ASSOC_BEGIN()
	// MSG
	ASSOC_LANG(MSG_ERR, kMsgErr, LS_MSG)
	ASSOC_LANG(MSG_SUCCESS, kMsgSuccess, LS_MSG)
	ASSOC_LANG(MSG_WARNING, kMsgWarning, LS_MSG)
	ASSOC_LANG(MSG_LOADING, kLoading, LS_MSG)
	ASSOC_LANG(MSG_BUSY, kBusy, LS_MSG)
	ASSOC_LANG(MSG_PLUGIN_ERR, kPluginErr, LS_MSG)
	ASSOC_LANG(MSG_NOFOUND_ERR, kNoFoundErr, LS_MSG)
	ASSOC_LANG(MSG_FOLDER_TITLE, kFolderTitle, LS_MSG)
	ASSOC_LANG(MSG_STILL_WORKING, kStillWorkingErr, LS_MSG)
	ASSOC_LANG(MSG_HANDLE_TEXT, kHandleText, LS_MSG)
	ASSOC_LANG(MSG_HANDLE_ERR_TEXT, kHandleErrText, LS_MSG)
	ASSOC_LANG(MSG_NOHANDLE_TEXT, kNoHandleText, LS_MSG)
	ASSOC_LANG(MSG_CHARSET_ERR, kCharSetErr, LS_MSG)
	ASSOC_LANG(MSG_INVALID_PATH, kInvalidPathErr, LS_MSG)
	ASSOC_LANG(MSG_FINDER_ERR, kInvalidFinderErr, LS_MSG)
	ASSOC_LANG(MSG_FORMATTER_ERR, kInvalidFormatterErr, LS_MSG)
	ASSOC_LANG(MSG_IPV4_ERR, kInvalidIPErr, LS_MSG)
	ASSOC_LANG(MSG_MASK_ERR, kInvalidMaskErr, LS_MSG)
	ASSOC_LANG(MSG_GATEWAY_ERR, kInvalidGatewayErr, LS_MSG)
	ASSOC_LANG(MSG_DNS_ERR, kInvalidDnsErr, LS_MSG)
	ASSOC_LANG(MSG_DELETE_SOLUTION, kDeleteSolution, LS_MSG)
	ASSOC_LANG(MSG_INVALID_NAME, kInvalidName, LS_MSG)
	ASSOC_LANG(MSG_DEBUG_TEXT, kDebugText, LS_MSG)
	ASSOC_LANG(MSG_DONE, kDone, LS_MSG)
ASSOC_END()

	return TRUE;
}

BOOL InitSQLite()
{
	char szConDB[1024] = {0};
	StrUtil::u2u8(g_szAppConfigDB, szConDB);
	if(!g_SQLite.Open(szConDB))
		return FALSE;
	CConfigTableDB config(&g_SQLite);
	if(!config.Init())
		return FALSE;
	if(!config.Query(NULL))
		return FALSE;
	LPCONFIG_TABLE lpTable = config.GetResults();
	for(int i=0; i<config.GetRows(); i++)
	{
		if(wcsicmp(lpTable[i].szName, kBackGround)==0)
			wcscpy(g_szBackground, lpTable[i].szValue);
		else if(wcsicmp(lpTable[i].szName, kLang)==0)
			wcscpy(g_szLangFile, lpTable[i].szValue);
		else if(wcsicmp(lpTable[i].szName, kSolution)==0)
			wcscpy(g_szAddrSolution, lpTable[i].szValue);
	}

	return TRUE;
}

BOOL WMITest()
{
	//if(!Utility::IsAdminPrivilege())
	//{
	//	MessageBox(NULL, L"此操作需要管理原权限", NULL, MB_ICONINFORMATION);
	//	return FALSE;
	//}
	CAdapterUtil util;
	HRESULT hRes = util.Init();
	if(FAILED(hRes))
		return FALSE;
	hRes = util.ConnectLocal(L"root\\cimv2");
	if(FAILED(hRes))
		return FALSE;
	hRes = util.EnumExecQuery(L"SELECT * from Win32_NetworkAdapterConfiguration where IPEnabled = -1");
	if(FAILED(hRes))
		return FALSE;
	while(TRUE)
	{
		CAdapterCfgObj pObj(util.GetServices());
		hRes = util.NexClassObject(pObj);
		if(FAILED(hRes))
			break;
		LONG lRet = 0;
		VARIANT vtDesc = {0}, vtCaption = {0};
		hRes = pObj.Get(L"Description", &vtDesc);
		hRes = pObj.Get(L"Caption", &vtCaption);
		wchar_t szDns[2][MAX_PATH] = { L"192.168.1.2", L"192.168.1.3" };
		hRes = pObj.SetDns((LPBYTE)szDns, _countof(szDns), &lRet);
		hRes = pObj.SetGateway(L"192.168.1.2", &lRet);
		hRes = pObj.SetAddr(L"192.168.1.102", L"255.255.255.0", &lRet);
		hRes = pObj.EnableDHCP(&lRet);
		break;
	}
	util.ClearEnum();
	util.Disconnect();
	return TRUE;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	// 内存跟踪调试
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(7218);
#endif

	g_hInstance = hInstance;

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
	
	if(!InitArgs())
	{
		const wchar_t* const ERR_MSG = L"RDTools Can't Init Startup Paramaters.";
		const wchar_t* const ERR_CAPTION = L"Error";
		char szMsg[1024];
		StrUtil::w2a(ERR_MSG, szMsg);
		MessageBox(NULL, ERR_MSG, ERR_CAPTION, MB_OK);
		LOG4CPLUS_ERROR(g_Logger, szMsg);
		return 0;
	}

	if(!CheckInstance())
		return 0;

	InitSQLite();

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

	AssocLangIDs();

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

	//RDMsgBox(NULL, MSG_INVALID_NAME, MSG_WARNING, MB_OK);

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

	if(g_SQLite.IsValid())
		g_SQLite.Close();

	CLangManager::Release();
	CSkinManager::Release();

	if(g_hUserSignature)
	{
		CloseHandle(g_hUserSignature);
		g_hUserSignature = NULL;
	}
	if(g_hAdminSignature)
	{
		CloseHandle(g_hAdminSignature);
		g_hAdminSignature = NULL;
	}

	LOG4CPLUS_INFO(g_Logger, L"RDTools Quit.");

	return 0;
}
