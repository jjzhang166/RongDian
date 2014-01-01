#include "stdafx.h"
#include "MainFrame.h"
#include "PanelXml.h"

// Panels
#include "ColorPicker.h"
#include "Coder.h"
#include "Finder.h"
#include "Tidy.h"
#include "IPConfig.h"
#include "Hosts.h"
#include "ChildLayoutTest.h"
#include "Setting.h"
#include "About.h"

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
const wchar_t* const kTabButton = L"TabButton";
const wchar_t* const kTabsHide = L"tab_hide";
const wchar_t* const kTabsShow = L"tab_show";
const wchar_t* const kPanelTabs = L"panel_tabs";
const wchar_t* const kPanelContents = L"panel_contents";

RD_BEGIN_TOOLS_MAP(CMainFrame)
	RD_TOOL(CColorPicker)
	RD_TOOL(CCoder)
	RD_TOOL(CFinder)
	RD_TOOL(CTidy)
	RD_TOOL(CIPConfig)
	RD_TOOL(CHosts)
	//RD_TOOL(CChildLayoutTest)
	RD_TOOL(CSetting)
	RD_TOOL(CAbout)
RD_END_TOOLS_MAP()

DUI_BEGIN_MESSAGE_MAP(CMainFrame, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) // 这是单击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE, OnItemActive) // 这是List节点双击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick) //list中的item点击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelected)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MENUSELECT, OnMenuSelect)
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
	pPanelTabs = NULL;
	pFrameContainer = NULL;
	pPanelContents = NULL;
	pLoadindFrame = NULL;
	pStatusCtrl = NULL;
	lpLoader = NULL;
	pszUpdateResponeData = NULL;
	ulUpdateResponeDataSize = 0;
	bRunning = FALSE;
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
	RD_ON_QUIT();
	_ReleaseTools();
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

	RD_ON_LANG_MSG((&m_PaintManager), lpszLang)

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

	_InitTools();

	g_pSystemTray = new CSystemTray();
	if(!g_pSystemTray)
		LOG4CPLUS_ERROR(g_Logger, "InitWindow - new CSystemTray");
	if(g_pSystemTray)
	{
		wchar_t szText[1024] = {0};
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_SYSTRAY, kTrayTip, szText);
		g_pSystemTray->Create(g_hInstance, m_hWnd, WM_ICON_NOTIFY, szText, hIcon, NULL);
	}

	FIND_CONTROL_BY_ID(pPanelTabs, CContainerUI, (&m_PaintManager), kPanelTabs)
	FIND_CONTROL_BY_ID(pPanelContents, CContainerUI, (&m_PaintManager), kPanelContents)
	FIND_CONTROL_BY_ID(pLoadindFrame, CVerticalLayoutUI, (&m_PaintManager), kFrameLoading)

	// Init Panel
	InitPanels();

	g_pSkinManager->AddFrame(this);
	SetSkin();

	g_pLangManager->AddFrame(this);
	SetLang(g_pLangManager->GetLangName());

	if(pPanelContents)
	{
		CDuiString strCurTab = pPanelContents->GetUserData();
		SelectPanel(strCurTab);
	}

	unsigned int nThread = 0;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, UpdateCheckThread, this, 0, &nThread);
	if(hThread)
		CloseHandle(hThread);
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
		BOOL bIsCanQuit = FALSE;
		RD_ISCAN_QUIT(m_hWnd, bIsCanQuit)
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
		ShowLoading();

		//CDuiRect rect = msg.pSender->GetPos();
		//DuiLib::CPoint point(rect.left, rect.bottom);
		//ClientToScreen(m_hWnd, &point);
		//CMenuUI *pMenu = new CMenuUI(m_hWnd);
		//pMenu->Init(m_PaintManager.GetRoot(), kSysMenuXml, NULL, point);
	}
	WindowImplBase::OnClick(msg);

	RD_ON_COMMON_MSG(m_hWnd, (&m_PaintManager), msg, bHandle, OnClick)

	if(!bHandle)
		SelectPanel(sCtrlName);
}

void CMainFrame::OnItemActive(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	RD_ON_COMMON_MSG(m_hWnd, (&m_PaintManager), msg, bHandle, OnItemActive)
}

void CMainFrame::OnItemClick(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	RD_ON_COMMON_MSG(m_hWnd, (&m_PaintManager), msg, bHandle, OnItemClick)
}

void CMainFrame::OnItemSelected(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	RD_ON_COMMON_MSG(m_hWnd, (&m_PaintManager), msg, bHandle, OnItemSelected)
}

void CMainFrame::OnMenuSelect(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kMenuAbout)
	{
		SelectPanel(kTabAbout);
	}
	else if(sCtrlName == kMenuHelp)
	{
		if(!lpLoader)
			DuiShowLoading(m_hWnd, L"Test...", NULL, &lpLoader);
		//DuiMenuMsgBox(m_hWnd, NULL, NULL, MB_OK);
	}
	else if(sCtrlName == kMenuShow)
	{
		if(lpLoader)
		{
			DuiCancelLoading(lpLoader);
			lpLoader = NULL;
		}
		OnActiveApp();
	}
	else if(sCtrlName == kMenuQuit)
	{
		OnAppQuit();
	}
}

void CMainFrame::OnTextChanged(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	RD_ON_COMMON_MSG(m_hWnd, (&m_PaintManager), msg, bHandle, OnTextChanged)
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass, CControlUI *pParent)
{
	CControlUI* pControl = NULL;
	if(wcsicmp(pstrClass, kTabButton)==0)
	{
		pControl = new CButtonUI();
		LPCTSTR pDefaultAttributes = m_PaintManager.GetDefaultAttributeList(kTabButton);
		if(pDefaultAttributes && pControl)
			pControl->ApplyAttributeList(pDefaultAttributes);
		if(pControl)
			return pControl;
	}

	RD_ON_CREATE_CONTROL_MSG(&m_PaintManager, pstrClass, pParent)

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

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	LRESULT lRes = 0;
	switch(uMsg)
	{
	case WM_ICON_NOTIFY:
		lRes = OnTrayNotification(uMsg, wParam, lParam);
		break;
	case WM_UPDATE_RESPONE:
		lRes = OnParseUpdateRespone(uMsg, wParam, lParam);
		break;
	}
	RD_ON_CUSTOM_MSG(uMsg, wParam, lParam);
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
	}
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
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
				::SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | ~WS_EX_TOOLWINDOW);
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
	RD_ON_COPYDATA_MSG(wParam, lParam);
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
			CMenuUI *pMenu = new CMenuUI(m_hWnd);
			pMenu->Init(m_PaintManager.GetRoot(), kSysMenuXml, NULL, point);
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
		strcpy(szVersion, root["version"].asString().c_str());
		strcpy(szUrl, root["url"].asString().c_str());
		wchar_t szPath[1024] = {0};
		wchar_t szRDVersionW[64];
		GetModuleFileName(NULL, szPath, sizeof(szPath));
		Utility::GetVersion(szPath, szRDVersionW);
		StrUtil::w2a(szRDVersionW, szRDVersion);
		if(stricmp(szVersion, szRDVersion))
		{
			OutputDebugStringW(L"Find New Version\n");
#pragma message("CMainFrame::OnParseUpdateRespone需添加更新对话框的弹出代码.")
		}
	}
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
	RD_ON_ITEMTEXT_MSG(m_hWnd, &m_PaintManager, pControl, iIndex, iSubItem);
}

BOOL CMainFrame::SelectPanel(LPCWSTR lpszTab)
{
	BOOL bRet = FALSE;
	CDuiString sCtrlName = lpszTab, sCurTab, sCurLayout;
	LPMAIN_PANEL lpInfo = NULL;
	CControlUI *pTab = NULL, *pCurTab = NULL;
	CContainerUI *pLayout = NULL, *pCurLayout = NULL;
	static LPCWSTR pszTabNormalColor = m_PaintManager.GetDefaultAttributeList(kTabNormalColor);
	static LPCWSTR pszTabHotColor = m_PaintManager.GetDefaultAttributeList(kTabHotColor);
	static LPCWSTR pszTabTextColor = m_PaintManager.GetDefaultAttributeList(kTabTextColor);
	static LPCWSTR pszTabSelTextColor = m_PaintManager.GetDefaultAttributeList(kTabSelTextColor);
	sCurTab = pPanelContents->GetUserData();
	FIND_CONTROL_BY_ID(pCurTab, CControlUI, (&m_PaintManager), sCurTab.GetData())
	if(!pCurTab)
		return FALSE;
	sCurLayout = pCurTab->GetUserData();
	FIND_CONTROL_BY_ID(pCurLayout, CContainerUI, (&m_PaintManager), sCurLayout.GetData())
	if(!pCurTab || !pCurLayout)
		return FALSE;
	if(sCurTab==sCtrlName)
	{
		pCurLayout = NULL;
		pCurTab = NULL;
	}
	list<LPMAIN_PANEL>::iterator iter;
	for(iter=g_lstPanelInfo.begin(); iter!=g_lstPanelInfo.end(); iter++)
	{
		lpInfo = (*iter);
		if(!lpInfo)
			return bRet;
		if(sCtrlName==lpInfo->szTab)
		{
			FIND_CONTROL_BY_ID(pLayout, CContainerUI, (&m_PaintManager), lpInfo->szLayout)
			FIND_CONTROL_BY_ID(pTab, CControlUI, (&m_PaintManager), lpInfo->szTab)
			if(pStatusCtrl && pLayout)
			{
				if(pCurLayout && pCurTab)
				{
					pCurLayout->SetVisible(false);
					//pCurTab->SetBkColor(kDefBkColor);
					//pCurTab->SetBkColor2(kDefBkColor);
					pCurTab->ApplyAttributeList(pszTabNormalColor);
					pCurTab->ApplyAttributeList(pszTabTextColor);
				}
				if(pTab)
				{
					//pTab->SetBkColor(kSelBkColor);
					//pTab->SetBkColor2(kSelBkColor2);
					pTab->ApplyAttributeList(pszTabHotColor);
					pTab->ApplyAttributeList(pszTabSelTextColor);
				}
				pStatusCtrl->SetText(lpInfo->szDesc);
				pLayout->SetVisible();
				pPanelContents->SetUserData(lpInfo->szTab);
				bRet = TRUE;
			}
			break;
		}
	}
	
	return bRet;
}

BOOL CMainFrame::InitPanels()
{
	BOOL bRet = FALSE;
	MAIN_PANEL panel = {0};
	wchar_t szName[1024], szDesc[1024];

	memset(&panel, 0, sizeof(MAIN_PANEL));
	wcscpy(panel.szLayout, kLayoutPicker);
	wcscpy(panel.szXml, kXmlPicker);
	wcscpy(panel.szTab, kTabPicker);
	wcscpy(panel.szPanel, kPanelPicker);
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_PICKERPANEL, kNamePicker, szName);
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_PICKERPANEL, kDescPicker, szDesc);
	wcscpy(panel.szName, szName);
	wcscpy(panel.szDesc, szDesc);
	AddPanel(&panel);

	// Load Plugin's Panel
	CPanelXml panels;
	if(!panels.Open(g_szPanelsXml))
		return bRet;
	if(!panels.LoadPanels(g_szResPath, g_lstPanelInfo))
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
			pTool->nIndex = _toolsEntries.size();
			pTool->lpClass = pToolClass;
			pTool->lpfnDestroy = lpfnDestroyRDTool;
			_toolsEntries.push_back(pTool);
		}
	}

	// Create Plugin's Panel
	CreatePanels();

	wcscpy(panel.szLayout, kLayoutAbout);
	wcscpy(panel.szXml, kXmlAbout);
	wcscpy(panel.szTab, kTabAbout);
	wcscpy(panel.szPanel, kPanelAbout);
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_ABOUTPANEL, kNameAbout, szName);
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_ABOUTPANEL, kDescAbout, szDesc);
	wcscpy(panel.szName, szName);
	wcscpy(panel.szDesc, szDesc);
	AddPanel(&panel);

	RD_ON_INIT_MSG((WPARAM)&m_PaintManager, (LPARAM)(IListCallbackUI*)this);

	return bRet;
}

BOOL CMainFrame::AddPanel(LPMAIN_PANEL lpPanelInfo)
{
	BOOL bRet = FALSE;
	LPMAIN_PANEL lpNewPanel = new MAIN_PANEL();
	if(!lpNewPanel)
		return bRet;
	memset(lpNewPanel, 0, sizeof(MAIN_PANEL));
	wcscpy(lpNewPanel->szLayout, lpPanelInfo->szLayout);
	wcscpy(lpNewPanel->szXml, lpPanelInfo->szXml);
	wcscpy(lpNewPanel->szTab, lpPanelInfo->szTab);
	wcscpy(lpNewPanel->szPanel, lpPanelInfo->szPanel);
	wcscpy(lpNewPanel->szName, lpPanelInfo->szName);
	wcscpy(lpNewPanel->szDesc, lpPanelInfo->szDesc);
	g_lstPanelInfo.push_back(lpNewPanel);

	CControlUI *pTab = NULL;
	FIND_CONTROL_BY_ID(pTab, CControlUI, (&m_PaintManager), lpNewPanel->szTab)
	if(pTab)
		pTab->SetText(lpNewPanel->szName);

	return TRUE;
}

BOOL CMainFrame::CreatePanels()
{
	BOOL bRet = FALSE;
	if(!pPanelTabs || !pPanelContents)
		return bRet;
	LPMAIN_PANEL lpPanelInfo = NULL;
	list<LPMAIN_PANEL>::iterator iter;
	CButtonUI *pTab = NULL;
	CChildLayoutUI *pPanel = NULL;
	LPCTSTR pDefaultAttributes = m_PaintManager.GetDefaultAttributeList(kTabButton);
	for(iter=g_lstPanelInfo.begin(); iter!=g_lstPanelInfo.end(); iter++)
	{
		lpPanelInfo = (LPMAIN_PANEL)(*iter);
		if(lpPanelInfo)
		{
			FIND_CONTROL_BY_ID(pTab, CButtonUI, (&m_PaintManager), lpPanelInfo->szTab)
			FIND_CONTROL_BY_ID(pPanel, CChildLayoutUI, (&m_PaintManager), lpPanelInfo->szLayout)
			if(pTab && pPanel)
				continue;
			pTab = new CButtonUI();
			if(!pTab)
				return bRet;
			//竖版菜单参数
			//pTab->SetAttribute(L"height", L"35");
			//pTab->SetAttribute(L"align", L"center");
			//pTab->SetAttribute(L"endellipsis", L"true");
			if(pDefaultAttributes)
				pTab->ApplyAttributeList(pDefaultAttributes);
			pTab->SetUserData(lpPanelInfo->szLayout);
			pTab->SetName(lpPanelInfo->szTab);
			pTab->SetText(lpPanelInfo->szName);
			pPanelTabs->AddAt(pTab, pPanelTabs->GetCount()-1);
			pPanel = new CChildLayoutUI();
			if(!pPanel)
				return bRet;
			pPanel->SetName(lpPanelInfo->szLayout);
			pPanel->SetChildLayoutXML(lpPanelInfo->szXml, this);
			pPanel->SetVisible(false);
			pPanelContents->AddAt(pPanel, pPanelContents->GetCount()-1);
		}
	}

	return bRet;
}

BOOL CMainFrame::ReleasePanels()
{
	LPMAIN_PANEL lpPanelInfo = NULL;
	list<LPMAIN_PANEL>::iterator iter;
	lpPanelInfo = (LPMAIN_PANEL)g_lstPanelInfo.front();
	while(lpPanelInfo)
	{
		g_lstPanelInfo.pop_front();
		delete lpPanelInfo;
		lpPanelInfo = NULL;
		if(g_lstPanelInfo.size())
			lpPanelInfo = (LPMAIN_PANEL)g_lstPanelInfo.front();
		else
			break;
	}

	return TRUE;
}

BOOL CMainFrame::ShowTabs(BOOL bShow)
{
	CButtonUI *pTabsShow = NULL, *pTabsHide = NULL;
	FIND_CONTROL_BY_ID(pTabsShow, CButtonUI, (&m_PaintManager), kTabsShow)
	FIND_CONTROL_BY_ID(pTabsHide, CButtonUI, (&m_PaintManager), kTabsHide)
	if(bShow)
	{
		if(pPanelTabs)
			pPanelTabs->SetVisible();
		if(pTabsHide)
			pTabsHide->SetVisible();
		if(pTabsShow)
			pTabsShow->SetVisible(false);
	}
	else
	{
		if(pPanelTabs)
			pPanelTabs->SetVisible(false);
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
	if(0==CheckVersion(pThis->m_hWnd, kCheckVersionUrl, szData, &ulSize) && pThis->bRunning)
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