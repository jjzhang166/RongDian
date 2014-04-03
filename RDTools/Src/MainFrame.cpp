#include "stdafx.h"
#include "PanelXml.h"
#include "MainFrame.h"

const wchar_t* const kFrameLogo = L"frame_logo";
const wchar_t* const kMenuBtn = L"menubtn";
const wchar_t* const kMinBtn = L"minbtn";
const wchar_t* const kMaxBtn = L"maxbtn";
const wchar_t* const kFormatBtn = L"formatBtn";
const wchar_t* const kRestoreBtn = L"restorebtn";
const wchar_t* const kCloseBtn = L"closebtn";
const wchar_t* const kFrameContainer = L"frame_container";
const wchar_t* const kFrameTitleContainer = L"frame_title_container";
const wchar_t* const kFrameFooterContainer = L"frame_footer_container";
const wchar_t* const kFrameTitle = L"frame_title";
const wchar_t* const kFrameStatus = L"frame_status";
const wchar_t* const kFrameAdminTip = L"frame_admin_tip";
const wchar_t* const kFrameLoading = L"frame_loading";

// System Menu
const wchar_t* const kMenuAbout = L"menu_about";
const wchar_t* const kMenuHelp = L"menu_help";
const wchar_t* const kMenuShow = L"menu_show";
const wchar_t* const kMenuQuit = L"menu_quit";

// Panel Contents
const wchar_t* const kTabNormalColor = L"TabNormalColor";
const wchar_t* const kTabHotColor = L"TabHotColor";
const wchar_t* const kTabTextColor = L"TabTextColor";
const wchar_t* const kTabSelTextColor = L"TabSelTextColor";
const wchar_t* const kTabNodeAttr = L"TabNode";
const wchar_t* const kTabNodeFloaderAttr = L"TabNodeFolder";
const wchar_t* const kTabsHide = L"tab_hide";
const wchar_t* const kTabsShow = L"tab_show";
const wchar_t* const kPanelTabs = L"panel_tabs";
const wchar_t* const kPanelContents = L"panel_contents";

DUI_BEGIN_MESSAGE_MAP(CMainFrame, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) // 这是单击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE, OnItemActive) // 这是List节点双击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick) //list中的item点击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelected)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TEXTCHANGED, OnTextChanged) // 这是文本框内容改变
DUI_END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
	builder_xml_ = kMainFrameXml;

#if USE_ZIPRES
	resource_dir_ = L"default\\";
#else
	resource_dir_ = CPaintManagerUI::GetResourcePath();
#endif
	m_pPanelTabs = NULL;
	pFrameContainer = NULL;
	m_pPanelContents = NULL;
	pLoadindFrame = NULL;
	pStatusCtrl = NULL;
	lpLoader = NULL;
	pszUpdateResponeData = NULL;
	ulUpdateResponeDataSize = 0;
	bRunning = FALSE;
	m_bFirstCheckVersion = TRUE;
	pUpdateFrame = NULL;
	memset(m_szSelectedNode,0,sizeof(m_szSelectedNode));
	memset(m_szVisiblePanel,0,sizeof(m_szVisiblePanel));
	m_pPanelRegister = new CPanelRegister;
	m_pPanelXml = new CPanelXml;
}

CMainFrame::~CMainFrame()
{
	if(g_pSystemTray)
	{
		delete g_pSystemTray;
		g_pSystemTray = NULL;
	}
	if(pszUpdateResponeData)
		delete pszUpdateResponeData;
	ReleasePanels();
	if(m_pPanelRegister)
	{
		m_pPanelRegister->OnQuit();
		m_pPanelRegister->ReleaseTools();
		delete m_pPanelRegister;
	}
	if(m_pPanelXml)
		delete m_pPanelXml;
	PostQuitMessage(0);
}

LONG CMainFrame::SetSkin()
{
	if(!pFrameContainer)
		FIND_CONTROL_BY_ID(pFrameContainer, CContainerUI, (&m_PaintManager), kFrameContainer)
	if(!pFrameContainer)
		return -1;
	if(g_pSkinManager->IsBkColor())
	{
		pFrameContainer->SetBkColor(g_pSkinManager->GetBkColor());
		pFrameContainer->SetBkColor2(g_pSkinManager->GetBkColor2());
		pFrameContainer->SetBkImage(L"");
	}
	else
	{
		pFrameContainer->SetBkColor(kDefBkColor);
		pFrameContainer->SetBkColor2(kDefBkColor);
		pFrameContainer->SetBkImage(g_pSkinManager->GetBkImage());
	}
	return 0;
}

LONG CMainFrame::SetLang(LPCWSTR lpszLang)
{
SET_CONTROL_BEGIN((&m_PaintManager), lpszLang, LS_MAINFRAME)
	SET_CONTROL_TIP2(kFrameLogo)
	SET_CONTROL_TIP2(kMenuBtn)
	SET_CONTROL_TIP2(kMinBtn)
	SET_CONTROL_TIP2(kCloseBtn)
	if(Utility::IsAdminPrivilege())
	{
		wchar_t szAdminTip[1024] = {0};
		Utility::GetINIStr(lpszLang, LS_MAINFRAME, kFrameAdminTip, szAdminTip);
		SET_TITLE_TEXT3(m_hWnd, kFrameTitle, szAdminTip)
	}
	else
	{
		SET_TITLE_TEXT2(m_hWnd, kFrameTitle)
	}
	SET_CONTROL_TEXT2(kFrameStatus)
SET_CONTROL_END()
	// Status Bar
	FIND_CONTROL_BY_ID(pStatusCtrl, CButtonUI, (&m_PaintManager), kFrameStatus)

	m_pPanelRegister->SetLang(&m_PaintManager, lpszLang);

	return 0;
}

LPCWSTR CMainFrame::GetWindowClassName() const 
{ 
	return kRDToolsClass;
}

void CMainFrame::InitWindow()
{
	bRunning = TRUE;

	// Vista/Win7，由于UAC的限制，低权限进程不能向高权限进程发送消息
	// 如果我们想容许一个消息可以发送给较高特权等级的进程
	// 我们可以在较高特权等级的进程中调用ChangeWindowMessageFilter(Vista以上的API)函数，以MSGFLT_ADD作为参数将消息添加进消息过滤器的白名单。
	PFNChangeWindowMessageFilter pfnChangeWindowMessageFilter = NULL;
	HMODULE hModule = LoadLibrary(_T("user32.dll"));
	if(hModule)
	{
		pfnChangeWindowMessageFilter = (PFNChangeWindowMessageFilter)GetProcAddress(hModule, "ChangeWindowMessageFilter");
		if(pfnChangeWindowMessageFilter)
		{
#ifndef MSGFLT_ADD
#define MSGFLT_ADD		1
#define MSGFLT_REMOVE	2
#endif
#define WM_COPYGLOBALDATA 0x0049
			pfnChangeWindowMessageFilter(WM_COPYGLOBALDATA, MSGFLT_ADD);
			pfnChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
			pfnChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
#undef WM_COPYGLOBALDATA
		}
		FreeLibrary(hModule);
	}

	DragAcceptFiles(m_hWnd, TRUE); // 添加拖拽支持

	HICON hIcon = ::LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_RDTOOLS));
	::SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)hIcon);
	HICON hIconSmall = ::LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_RDTOOLS));
	::SendMessage(m_hWnd, WM_SETICON, FALSE, (LPARAM)hIconSmall);

	m_pPanelRegister->InitTools();

	g_pSystemTray = new CSystemTray();
	if(!g_pSystemTray)
		LOG4CPLUS_ERROR(g_Logger, "InitWindow - new CSystemTray");
	if(g_pSystemTray)
	{
		wchar_t szText[1024] = {0};
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_SYSTRAY, kTrayTip, szText);
		g_pSystemTray->Create(g_hInstance, m_hWnd, WM_ICON_NOTIFY, szText, hIcon, NULL);
	}

	FIND_CONTROL_BY_ID(m_pPanelTabs, CTreeViewUI, (&m_PaintManager), kPanelTabs)
	FIND_CONTROL_BY_ID(m_pPanelContents, CContainerUI, (&m_PaintManager), kPanelContents)
	FIND_CONTROL_BY_ID(pLoadindFrame, CVerticalLayoutUI, (&m_PaintManager), kFrameLoading)

	// Init Panel
	InitPanels();

	g_pSkinManager->AddFrame(this);
	SetSkin();

	g_pLangManager->AddFrame(this);
	SetLang(g_pLangManager->GetLangName());

	if(m_pPanelContents)
	{
		CONFIG_TABLE stTable;
		wcscpy(stTable.szName, L"LastVisitTab");
		CConfigTableDB table(&g_SQLite);
		table.Query(&stTable);
		if(wcslen(table.GetResults()->szValue)==0)
		{
			CDuiString strCurTab = m_pPanelContents->GetUserData();
			SelectPanel(strCurTab);
		}
		else
		{
			SelectPanel(table.GetResults()->szValue);
		}
	}

	//check version
	CONFIG_TABLE stTable;
	wcscpy(stTable.szName, tConfigCheckVersion);
	CConfigTableDB table(&g_SQLite);
	table.Query(&stTable);
	if(wcslen(table.GetResults()->szValue)>0&&wcsicmp(table.GetResults()->szValue,L"1")==0)
	{
		unsigned int nThread = 0;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, UpdateCheckThread, this, 0, &nThread);
		if(hThread)
			CloseHandle(hThread);
	}
}

CDuiString CMainFrame::GetSkinFile()
{
	return builder_xml_;
}

CDuiString CMainFrame::GetSkinFolder()
{
	return resource_dir_;
}

#if USE_ZIPRES
UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CMainFrame::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_RESOURCE);
}
#endif

void CMainFrame::OnClick(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kMinBtn && g_pSystemTray)
	{
		bHandle = TRUE;
		g_pSystemTray->MinimiseToTray(m_hWnd);
		::SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		return;
	}
	else if(sCtrlName == kMaxBtn)
	{
		bHandle = TRUE;
		CControlUI *pRestore = NULL;
		FIND_CONTROL_BY_ID(pRestore, CControlUI, (&m_PaintManager), kRestoreBtn)
		if(pRestore && msg.pSender)
		{
			msg.pSender->SetVisible(false);
			pRestore->SetVisible();
		}
	}
	else if(sCtrlName == kRestoreBtn)
	{
		bHandle = TRUE;
		CControlUI *pMax = NULL;
		FIND_CONTROL_BY_ID(pMax, CControlUI, (&m_PaintManager), kMaxBtn)
		if(pMax && msg.pSender)
		{
			msg.pSender->SetVisible(false);
			pMax->SetVisible();
		}
	}
	else if(sCtrlName == kCloseBtn)
	{
		bHandle = TRUE;
		BOOL bIsCanQuit = FALSE;
		//RD_ISCAN_QUIT(m_hWnd, (&m_PaintManager), bIsCanQuit)
		bIsCanQuit = m_pPanelRegister->IsCanQuit(m_hWnd, &m_PaintManager);
		if(!bIsCanQuit)
		{
			return;
		}
	}
	else if(sCtrlName == kTabsHide)
	{
		bHandle = TRUE;
		ShowTabs(FALSE);
	}
	else if(sCtrlName == kTabsShow)
	{
		bHandle = TRUE;
		ShowTabs(TRUE);
	}
	else if(sCtrlName==kMenuBtn)
	{
		bHandle = TRUE;
		//ShowLoading();

		CDuiRect rect = msg.pSender->GetPos();
		DuiLib::CPoint point(rect.left, rect.bottom);
		ClientToScreen(m_hWnd, &point);
		CMenuUI *pMenu = static_cast<CMenuUI*>(m_PaintManager.GetMenu(L"system_menu"));
		if(pMenu)
		{
			pMenu->SetManager(&m_PaintManager, NULL, false);
			pMenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, m_hWnd);
		}
	}
	WindowImplBase::OnClick(msg);

	if(!bHandle)
		m_pPanelRegister->OnClick(m_hWnd, &m_PaintManager, msg, bHandle);
}

void CMainFrame::OnItemActive(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	m_pPanelRegister->OnItemActive(m_hWnd, &m_PaintManager, msg, bHandle);
}

void CMainFrame::OnItemClick(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	if (!SelectPanel(msg.pSender->GetName()))
		m_pPanelRegister->OnItemClick(m_hWnd, &m_PaintManager, msg, bHandle);
}

void CMainFrame::OnItemSelected(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	m_pPanelRegister->OnItemSelected(m_hWnd, &m_PaintManager, msg, bHandle);
}

void CMainFrame::OnTextChanged(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	m_pPanelRegister->OnTextChanged(m_hWnd, &m_PaintManager, msg, bHandle);
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass, CControlUI *pParent)
{
	CControlUI* pControl = NULL;
	if(wcsicmp(pstrClass, kTabNodeAttr)==0)
	{
		pControl = new CButtonUI();
		LPCTSTR pDefaultAttributes = m_PaintManager.GetDefaultAttributeList(kTabNodeAttr);
		if(pDefaultAttributes && pControl)
			pControl->ApplyAttributeList(pDefaultAttributes);
		if(pControl)
			return pControl;
	}

	pControl = m_pPanelRegister->OnCreateControl(&m_PaintManager, pstrClass, pParent);
	return pControl;
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	switch(uMsg)
	{
	case WM_DROPFILES:
		OnDropFiles(uMsg, wParam, lParam);
		break;
	case WM_COPYDATA:
		OnCopyData(uMsg, wParam, lParam);
		break;
	}

	lRes = WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch(uMsg)
	{
	case WM_ICON_NOTIFY:
		return OnTrayNotification(uMsg, wParam, lParam);
		break;
	case WM_UPDATE_RESPONE:
		return OnParseUpdateRespone(uMsg, wParam, lParam);
		break;
	}
	m_pPanelRegister->HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = __super::OnSize(uMsg, wParam, lParam, bHandled);
	if(pLoadindFrame && pLoadindFrame->IsVisible())
		ShowLoading();
	return lRes;
}

#if 0
LRESULT CMainFrame::OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	CDuiRect rcWork;
	GetWindowRect(m_hWnd, &rcWork);
	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.GetWidth();
	lpMMI->ptMaxSize.y		= rcWork.GetHeight();
	bHandled = TRUE;
	return 0;
}
#endif

LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hModalWnd = FindWindow(kDuiMsgModalClass, NULL);
	if(hModalWnd)
	{
		SetForegroundWindow(hModalWnd);
		return 0;
	}
	bRunning = FALSE;
	HWND hZoomIn = FindWindow(kZoomInClass, NULL);
	if(hZoomIn)
	{
		COPYDATASTRUCT data;
		data.dwData = WM_ZOOMIN_QUIT_MSG;
		data.cbData = 0;
		data.lpData = NULL;
		::SendMessage(hZoomIn, WM_COPYDATA, (WPARAM)(HWND)m_hWnd, (LPARAM)(LPVOID)&data);
		return TRUE;
	}
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(HIWORD(wParam)==0)
	{
		switch(wParam)
		{
		case SYS_MENU_ABOUT:
			SelectPanel(kTabAbout);
			break;
		case SYS_MENU_CHECKVERSION:
			{
				unsigned int nThread = 0;
				HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, UpdateCheckThread, this, 0, &nThread);
				if(hThread)
					CloseHandle(hThread);
			}
			break;
		case SYS_MENU_SHOW:
			if(lpLoader)
			{
				DuiCancelLoading(lpLoader);
				lpLoader = NULL;
			}
			OnActiveApp();
			break;
		case SYS_MENU_QUIT:
			OnAppQuit();
			break;
		}
	}
	return __super::OnCommand(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::OnCopyData(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam)
{
	PCOPYDATASTRUCT lpData = (PCOPYDATASTRUCT)lParam;
	if(lpData->dwData==WM_ACTIVE_MAIN)
	{
		if(!::IsWindowVisible(m_hWnd))
		{
			if(g_pSystemTray)
			{
				::SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW);
				g_pSystemTray->MaximiseFromTray(m_hWnd);
				return 0;
			}
			else
			{
				::SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
				ShowWindow((bool)SW_SHOW);
			}
		}
		::SetActiveWindow(m_hWnd);
		::SetForegroundWindow(m_hWnd);
		return TRUE;
	}
	m_pPanelRegister->OnCopyData(wParam, lParam);
	return 0;
}

LRESULT CMainFrame::OnTrayNotification(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam)
{
	if(g_pSystemTray)
	{
		if(LOWORD(lParam) == WM_LBUTTONDBLCLK && !IsWindowVisible(m_hWnd))
		{
			::SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW);
			g_pSystemTray->MaximiseFromTray(m_hWnd);
		}
		else if(LOWORD(lParam) == WM_RBUTTONUP)
		{
			POINT point;
			GetCursorPos(&point);
			CMenuUI *pMenu = static_cast<CMenuUI*>(m_PaintManager.GetMenu(L"system_menu"));
			if(pMenu)
			{
				pMenu->SetManager(&m_PaintManager, NULL, false);
				pMenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, m_hWnd);
			}
		}
		else
		{
			return g_pSystemTray->OnTrayNotification(wParam, lParam);
		}
	}
	return 0;
}

LRESULT CMainFrame::OnParseUpdateRespone(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam)
{	
	JsonTidyLib::Reader reader;
	JsonTidyLib::Value root;
	char szVersion[64] = {0};
	char szUrl[1024] = {0};
	char szRDVersion[64] = {0};
	if (pszUpdateResponeData && reader.parse(pszUpdateResponeData, root)&&root["version"].type()!=JsonTidyLib::nullValue)
	{
		BOOL showPopup = TRUE;
		strcpy(szVersion, root["version"].asString().c_str());
		strcpy(szUrl, root["url"].asString().c_str());
		wchar_t szPath[1024] = {0};
		wchar_t szRDVersionW[64];
		char szLog[1024];
		GetModuleFileName(NULL, szPath, sizeof(szPath));
		Utility::GetVersion(szPath, szRDVersionW);
		StrUtil::w2a(szRDVersionW, szRDVersion);
		if(stricmp(szVersion, szRDVersion))
		{
			OutputDebugStringW(L"Find New Version\n");
//#pragma message("CMainFrame::OnParseUpdateRespone - 版本更新待完善，需添加更新对话框的弹出代码.")
			//RDPopupBox(m_hWnd, MSG_HOST_SAVE_MSG, MSG_WARNING);
				
			char aTmpLog[1024];
			wchar_t wTmpLog[1024];
			char fTmpLog[1024];
			Utility::GetINIStr(g_pLangManager->GetLangName(), LS_UPDATEFRAME, L"update_new_version", wTmpLog);
			StrUtil::u2u8(wTmpLog,aTmpLog);
			
			sprintf(fTmpLog,aTmpLog,szVersion);
			strcpy(szLog,fTmpLog);
			strcat(szLog,"\r\n");
			strcat(szLog,root["log"].asString().c_str());
			
		}
		else if(!m_bFirstCheckVersion)
		{
			OutputDebugStringW(L"This version is latest\n");
			char aTmpLog[1024];
			wchar_t wTmpLog[1024];
			Utility::GetINIStr(g_pLangManager->GetLangName(), LS_UPDATEFRAME, L"update_latest_version", wTmpLog);
			StrUtil::u2u8(wTmpLog,aTmpLog);
			strcpy(szLog,aTmpLog);
		}
		else 
		{
			showPopup = FALSE;
		}
		if(showPopup)
		{
			PopupUpdateFrame(root["url"].asString().c_str(), szLog);
		}
	}
	m_bFirstCheckVersion = FALSE;
	return 0;
}

LRESULT CMainFrame::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
{
	HDROP hDrop = (HDROP)wParam;
	int nCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
	int nStrLen = 0;
	wchar_t szFileName[1024], szDstImage[1024];
	swprintf(szDstImage, L"%s\\custom", g_szResPath);
	if(!PathFileExistsW(szDstImage))
		SHCreateDirectoryExW(m_hWnd, szDstImage, NULL);
	LPCWSTR lpszFileName = NULL, lpszExt = NULL;
	char szFileName_a[1024];
	wchar_t szCoder_w[1024] = {0}, szNewImage[1024];
	for(int i=0; i<nCount; i++)
	{
		nStrLen = DragQueryFileW(hDrop, i, NULL, 100);
		DragQueryFileW(hDrop, i, szFileName, nStrLen+1);
		lpszFileName = PathFindFileNameW(szFileName);
		lpszExt = PathFindExtensionW(szFileName);
		StrUtil::w2a(szFileName, szFileName_a);
		std::string strFileName = szFileName_a;
		std::string strCoder = MD5Str(strFileName);
		StrUtil::a2w(strCoder.c_str(), szCoder_w);
		PathAddExtension(szCoder_w, lpszExt);
		PathAppend(szDstImage, szCoder_w);
		if(IsImageFile(szFileName) && g_pSkinManager)
		{
			if(!CopyFileW(szFileName, szDstImage, FALSE))
				continue;
			g_pSkinManager->UpdateBkImage(szDstImage);
			swprintf(szNewImage, L"custom\\%s", szCoder_w);
			//Utility::SetINIStr(g_szAppConfig, LS_SKIN, kBackGround, szNewImage);
			CConfigTableDB table(&g_SQLite);
			CONFIG_TABLE config;
			wcscpy(config.szName, kBackGround);
			wcscpy(config.szValue, szNewImage);
			table.Update(&config);
			break;
		}
	}
	return 0;
}

LPCWSTR CMainFrame::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	return m_pPanelRegister->GetItemText(m_hWnd, &m_PaintManager, pControl, iIndex, iSubItem);
}

BOOL CMainFrame::SelectPanel(LPCWSTR lpszTab)
{
	BOOL bRet = FALSE;
	CDuiString sCtrlName = lpszTab, sCurLayout;
	LPMAIN_PANEL lpInfo = NULL;
	CTreeNodeUI *pNode = NULL, *pSelectedNode = NULL;
	CContainerUI *pLayout = NULL, *pSelectedLayout = NULL;
	static LPCWSTR pszTabNormalColor = m_PaintManager.GetDefaultAttributeList(kTabNormalColor);
	static LPCWSTR pszTabHotColor = m_PaintManager.GetDefaultAttributeList(kTabHotColor);
	static LPCWSTR pszTabTextColor = m_PaintManager.GetDefaultAttributeList(kTabTextColor);
	static LPCWSTR pszTabSelTextColor = m_PaintManager.GetDefaultAttributeList(kTabSelTextColor);
	m_pPanelXml->GetPanel(sCtrlName.GetData(),lpInfo);
	
	if(lpInfo)
	{
		FIND_CONTROL_BY_ID(pSelectedNode, CTreeNodeUI, (&m_PaintManager), m_szSelectedNode)
		FIND_CONTROL_BY_ID(pSelectedLayout, CContainerUI, (&m_PaintManager), m_szVisiblePanel)
		FIND_CONTROL_BY_ID(pLayout, CContainerUI, (&m_PaintManager), lpInfo->szLayout)
		FIND_CONTROL_BY_ID(pNode, CTreeNodeUI, (&m_PaintManager), lpInfo->szTab)

		wcscpy(m_szSelectedNode,lpInfo->szTab);
		if(pSelectedNode)
		{
			pSelectedNode->ApplyAttributeList(pszTabNormalColor);
			pSelectedNode->ApplyAttributeList(pszTabTextColor);
		}
		if(pSelectedLayout)
			pSelectedLayout->SetVisible(false);
		if(pNode)
		{
			pNode->Select(true);
			pNode->ApplyAttributeList(pszTabHotColor);
			pNode->ApplyAttributeList(pszTabSelTextColor);
		}
		if(pLayout)	//tab with panel
		{
			pLayout->SetVisible();
			wcscpy(m_szVisiblePanel,lpInfo->szLayout);
		}
		else if(pNode->GetCountChild()>0)
		{
			wchar_t childLayout[256]={0};
			CContainerUI* pChildLayout;
			CTreeNodeUI* childNode = pNode->GetChildNode(0);
			LPCTSTR childName = childNode->GetName().GetData();
			wsprintf(childLayout,L"%s_layout",childName);
			FIND_CONTROL_BY_ID(pChildLayout, CContainerUI, (&m_PaintManager), childLayout)
			if(pChildLayout)
			{
				pChildLayout->SetVisible();
				wcscpy(m_szVisiblePanel,childLayout);
			}
		}
		pStatusCtrl->SetText(lpInfo->szDesc);
		bRet = TRUE;
	}
	
	//after selected tab ,update config table
	if (bRet)
	{
		CONFIG_TABLE lpTable;
		wcscpy(lpTable.szName,L"LastVisitTab");
		wcscpy(lpTable.szValue,lpszTab);
		CConfigTableDB table(&g_SQLite);
		table.Update(&lpTable);
	}
	return bRet;
}

BOOL CMainFrame::InitPanels()
{
	BOOL bRet = FALSE;
	wchar_t szName[1024], szDesc[1024];

	if(!m_pPanelXml->Open(g_szPanelsXml))
		return bRet;
	if(!m_pPanelXml->LoadPanels(g_szResPath))
		return bRet;
	// Load Panel Instance from DLL
	LPFNCreateRDTool lpfnCreateTool = NULL;
	LPFNDestroyRDTool lpfnDestroyRDTool = NULL;
	wchar_t szTool[1024] = {0};
	CBaseTool *pToolClass = NULL;
	LPMAIN_PANEL lpPanelInfo = NULL;
	list<LPMAIN_PANEL>::iterator iter;
	for(iter=g_lstPanelInfo.begin(); iter!=g_lstPanelInfo.end(); iter++)
	{
		lpPanelInfo = (*iter);
		if(wcslen(lpPanelInfo->szDll)==0)
			continue;
		HMODULE hModule = LoadLibraryW(lpPanelInfo->szDll);
		if(!hModule)
			continue;
		lpfnCreateTool = (LPFNCreateRDTool)GetProcAddress(hModule, "CreateRDTool");
		lpfnDestroyRDTool = (LPFNDestroyRDTool)GetProcAddress(hModule, "DestroyRDTool");
		if(!lpfnDestroyRDTool || lpfnCreateTool)
		{
			FreeLibrary(hModule);
			continue;
		}
		if(lpfnCreateTool(szTool, (LPVOID *)&pToolClass)==0)
		{
			LPTOOLS_INFO pTool = new TOOLS_INFO();
			if(!pTool)
			{
				lpfnDestroyRDTool(pToolClass);
				continue;
			}
			pTool->nIndex = m_pPanelRegister->lstToolsEntries.size();
			pTool->lpClass = pToolClass;
			pTool->lpfnDestroy = lpfnDestroyRDTool;
			m_pPanelRegister->lstToolsEntries.push_back(pTool);
		}
	}

	// Create Plugin's Panel
	CreatePanels();

	LPMAIN_PANEL aboutPanel = new MAIN_PANEL();
	wcscpy(aboutPanel->szLayout, kLayoutAbout);
	wcscpy(aboutPanel->szXml, kXmlAbout);
	wcscpy(aboutPanel->szTab, kTabAbout);
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_ABOUTPANEL, kNameAbout, szName);
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_ABOUTPANEL, kDescAbout, szDesc);
	wcscpy(aboutPanel->szName, szName);
	wcscpy(aboutPanel->szDesc, szDesc);
	//AddPanel(&aboutPanel);
	g_lstPanelInfo.push_back(aboutPanel);
	CreatePanel(aboutPanel,NULL);

	m_pPanelRegister->OnInit((WPARAM)&m_PaintManager, (LPARAM)(IListCallbackUI*)this);

	return bRet;
}

BOOL CMainFrame::CreatePanels()
{
	BOOL bRet = FALSE;
	if(!m_pPanelTabs || !m_pPanelContents)
		return bRet;
	LPMAIN_PANEL lpPanelInfo = NULL;
	list<LPMAIN_PANEL>::iterator iter;
	for(iter=g_lstPanelInfo.begin(); iter!=g_lstPanelInfo.end(); iter++)
	{
		lpPanelInfo = (LPMAIN_PANEL)(*iter);
		if(lpPanelInfo)
		{
			CreatePanel(lpPanelInfo,NULL);
		}
	}
	return bRet;
}

BOOL CMainFrame::CreatePanel(LPMAIN_PANEL lpPanelInfo,CTreeNodeUI* pParentNode)
{
	static LPCTSTR pTabAttributes = m_PaintManager.GetDefaultAttributeList(kTabNodeAttr);
	static LPCTSTR pNodeFolderAttributes = m_PaintManager.GetDefaultAttributeList(kTabNodeFloaderAttr);
	CTreeNodeUI *pTab;
	CChildLayoutUI *pPanel = NULL;
	FIND_CONTROL_BY_ID(pTab, CTreeNodeUI, (&m_PaintManager), lpPanelInfo->szTab)
	FIND_CONTROL_BY_ID(pPanel, CChildLayoutUI, (&m_PaintManager), lpPanelInfo->szLayout)
	if(pTab && pPanel)
		return FALSE;
	
	pTab = new CTreeNodeUI();

	pTab->ApplyAttributeList(pTabAttributes);
	//pTab->SetUserData(szPath);
	pTab->SetName(lpPanelInfo->szTab);
	pTab->SetItemText(lpPanelInfo->szName);
	if(pParentNode)
		pParentNode->AddChildNode(pTab);
	m_pPanelTabs->AddAt(pTab, m_pPanelTabs->GetCount()-1);

	if(wcslen(lpPanelInfo->szXml)>0)
	{
		pPanel = new CChildLayoutUI();
		pPanel->SetName(lpPanelInfo->szLayout);
		pPanel->SetChildLayoutXML(lpPanelInfo->szXml, this);
		pPanel->SetVisible(false);
		m_pPanelContents->AddAt(pPanel, m_pPanelContents->GetCount()-1);
	}
		
	
	// sub panel
	if(!lpPanelInfo->childs.empty())
	{
		pTab->GetFolderButton()->ApplyAttributeList(pNodeFolderAttributes);
		list<LPMAIN_PANEL>::iterator it;
		for(it = lpPanelInfo->childs.begin();it!=lpPanelInfo->childs.end();it++)
		{
			CreatePanel(*it,pTab);
		}
	}
	else
	{
		pTab->GetFolderButton()->SetEnabled(false);
		pTab->GetFolderButton()->SetVisible(false);
	}
		
	return TRUE;
}

BOOL CMainFrame::ReleasePanels()
{
	LPMAIN_PANEL lpPanelInfo = NULL;
	lpPanelInfo = (LPMAIN_PANEL)g_lstPanelInfo.front();
	g_lstPanelInfo.pop_front();
	while(lpPanelInfo)
	{
		ReleasePanel(lpPanelInfo);
		if(!g_lstPanelInfo.empty())
		{
			lpPanelInfo = (LPMAIN_PANEL)g_lstPanelInfo.front();
			g_lstPanelInfo.pop_front();
		}
		
	}
	return TRUE;
}

BOOL CMainFrame::ReleasePanel(LPMAIN_PANEL &lpPanelInfo)
{
	if(!lpPanelInfo->childs.empty())
	{
		LPMAIN_PANEL subPanel = lpPanelInfo->childs.front();
		lpPanelInfo->childs.pop_front();
		while (subPanel)
		{
			ReleasePanel(subPanel);
			if (!lpPanelInfo->childs.empty())
			{
				subPanel = lpPanelInfo->childs.front();
				lpPanelInfo->childs.pop_front();
			}
		}
	}
	delete lpPanelInfo;
	lpPanelInfo = NULL;
	return TRUE;
}

BOOL CMainFrame::ShowTabs(BOOL bShow)
{
	CButtonUI *pTabsShow = NULL, *pTabsHide = NULL;
	FIND_CONTROL_BY_ID(pTabsShow, CButtonUI, (&m_PaintManager), kTabsShow)
	FIND_CONTROL_BY_ID(pTabsHide, CButtonUI, (&m_PaintManager), kTabsHide)
	if(bShow)
	{
		if(m_pPanelTabs)
			m_pPanelTabs->SetVisible();
		if(pTabsHide)
			pTabsHide->SetVisible();
		if(pTabsShow)
			pTabsShow->SetVisible(false);
	}
	else
	{
		if(m_pPanelTabs)
			m_pPanelTabs->SetVisible(false);
		if(pTabsHide)
			pTabsHide->SetVisible(false);
		if(pTabsShow)
			pTabsShow->SetVisible();
	}
	return TRUE;
}

BOOL CMainFrame::OnActiveApp()
{
	if(!IsWindowVisible(m_hWnd))
	{
		::SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW);
		g_pSystemTray->MaximiseFromTray(m_hWnd);
	}
	else
	{
		::SetForegroundWindow(m_hWnd); 
	}
	return TRUE;
}

BOOL CMainFrame::OnAppQuit()
{
	PostMessage(WM_CLOSE, 0, 0);
	return TRUE;
}

BOOL CMainFrame::PopupUpdateFrame(const char *pszUrl, const char *pszLog)
{
	if(!pUpdateFrame)
	{
		pUpdateFrame = new CUpdateFrame();
		pUpdateFrame->Create(m_hWnd, L"", UI_WNDSTYLE_FRAME | UI_CLASSSTYLE_DIALOG, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
		pUpdateFrame->SetIcon(IDI_RDTOOLS);
	}
	pUpdateFrame->SetUpdateInfo(pszUrl, pszLog);
	pUpdateFrame->CenterWindow();
	::ShowWindow(pUpdateFrame->GetHWND(), SW_SHOW);
	SetForegroundWindow(pUpdateFrame->GetHWND());
	return TRUE;
}

BOOL CMainFrame::ShowLoading()
{
	if(!pLoadindFrame || !pFrameContainer)
		return FALSE;
	int nTitleBar = 0, nFooter = 0;
	CHorizontalLayoutUI *pTitleBar = NULL, *pFooter = NULL;
	FIND_CONTROL_BY_ID(pTitleBar, CHorizontalLayoutUI, (&m_PaintManager), kFrameTitleContainer)
	FIND_CONTROL_BY_ID(pFooter, CHorizontalLayoutUI, (&m_PaintManager), kFrameFooterContainer)
	if(pTitleBar)
		nTitleBar = pTitleBar->GetHeight();
	if(pFooter)
		nFooter = pFooter->GetHeight();
	CControlUI *pRoot = m_PaintManager.GetRoot();
	RECT rtClient;
	GetClientRect(m_hWnd, &rtClient);
	CDuiRect rtPos;
	rtPos.left = pRoot->GetBorderSize();
	rtPos.right = rtClient.right - pRoot->GetBorderSize();
	rtPos.top = nTitleBar;
	rtPos.bottom = rtClient.bottom - nFooter;
	pLoadindFrame->SetPos(rtPos);
	if(pLoadindFrame->IsVisible()==false)
		pLoadindFrame->SetVisible();

	return TRUE;
}

void CMainFrame::CancelLoading()
{
	if(!pLoadindFrame)
		return;
	pLoadindFrame->SetVisible(false);
}

unsigned int CMainFrame::UpdateCheckThread(LPVOID lpData)
{
	const int MAX_RESPONE_SIZE = 4096;
	CMainFrame *pThis = (CMainFrame *)lpData;
	if(!pThis)
		return 0;
	char szData[MAX_RESPONE_SIZE] = {0};
	unsigned long ulSize = 0;
	if(0==RDCheckVersion(pThis->m_hWnd, kCheckVersionUrl, szData, &ulSize) && pThis->bRunning)
	{
		pThis->pszUpdateResponeData = new char[MAX_RESPONE_SIZE];
		assert(NULL!=pThis->pszUpdateResponeData);
		memset(pThis->pszUpdateResponeData, 0, sizeof(char)*MAX_RESPONE_SIZE);
		memcpy(pThis->pszUpdateResponeData, szData, MAX_RESPONE_SIZE);
		pThis->ulUpdateResponeDataSize = ulSize;
		::PostMessage(pThis->m_hWnd, WM_UPDATE_RESPONE, 0, 0);
	}
	return 0;
}

LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_ESCAPE:                     // ESC 
			{
				if (m_pPanelTabs->IsVisible())
				{
					ShowTabs(FALSE);
				}
				else
				{
					ShowTabs(TRUE);
				}
				bHandled = true;
			}
			break;
		}
	}
	return FALSE;
}