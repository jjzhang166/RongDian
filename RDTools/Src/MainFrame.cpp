#include "stdafx.h"
#include "MainFrame.h"

const DWORD			 kSelBkColor = 0xFF3C5A78;
const DWORD			 kSelBkColor2 = 0xFFC8C8C8;

const wchar_t* const kFrameLogo = L"frame_logo";
const wchar_t* const kMenuBtn = L"menubtn";
const wchar_t* const kMinBtn = L"minbtn";
const wchar_t* const kMaxBtn = L"maxbtn";
const wchar_t* const kFormatBtn = L"formatBtn";
const wchar_t* const kRestoreBtn = L"restorebtn";
const wchar_t* const kCloseBtn = L"closebtn";
const wchar_t* const kFrameContainer = L"frame_container";
const wchar_t* const kFrameTitle = L"frame_title";
const wchar_t* const kFrameStatus = L"frame_status";
const wchar_t* const kTrayTip = L"tray_tip";

// Panel Contents
const wchar_t* const kPanelTabs = L"panel_tabs";
const wchar_t* const kPanelContents = L"panel_contents";

DUI_BEGIN_MESSAGE_MAP(CMainFrame, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) // 这是单击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE, OnItemActive) // 这是List节点双击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick) //list中的item点击
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TEXTCHANGED, OnTextChanged) // 这是文本框内容改变
	DUI_ON_MENU_CTRNAME(kMenuBtn, OnShowSysMenu)
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
	pStatusCtrl = NULL;
	Open(g_szPanelsXml);
}

CMainFrame::~CMainFrame()
{
	if(g_pSystemTray)
	{
		delete g_pSystemTray;
		g_pSystemTray = NULL;
	}
	ReleasePanels();
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
	SET_TITLE_TEXT2(m_hWnd, kFrameTitle)
	SET_CONTROL_TEXT2(kFrameStatus)
SET_CONTROL_END()
	// Status Bar
	FIND_CONTROL_BY_ID(pStatusCtrl, CButtonUI, (&m_PaintManager), kFrameStatus)

	// About Frame
	SetAboutLang(lpszLang);

	// Picker Frame
	SetPickerLang(lpszLang);

	// Coder Frame
	SetCoderLang(lpszLang);

	// Finder Frame
	SetFinderLang(lpszLang);

	SetFormatterLang(lpszLang);

	SetIPConfigLang(lpszLang);
	return 0;
}

LPCWSTR CMainFrame::GetWindowClassName() const 
{ 
	return kRDToolsClass;
}

void CMainFrame::InitWindow()
{
	//::SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);

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

	SetAboutOwner(m_hWnd, &m_PaintManager);
	SetPickerOwner(m_hWnd, &m_PaintManager);
	SetCoderOwner(m_hWnd, &m_PaintManager);
	SetFinderOwner(m_hWnd, &m_PaintManager);
	SetFormatterOwner(m_hWnd,&m_PaintManager);
	SetIPConfigOwner(m_hWnd,&m_PaintManager);

	HICON hIcon = ::LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_RDTOOLS));
	::SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)hIcon);
	HICON hIconSmall = ::LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_RDTOOLS));
	::SendMessage(m_hWnd, WM_SETICON, FALSE, (LPARAM)hIconSmall);

	g_pSystemTray = new CSystemTray();
	if(!g_pSystemTray)
		LOG4CPLUS_ERROR(g_Logger, "InitWindow - new CSystemTray");
	if(g_pSystemTray)
	{
		wchar_t szText[1024] = {0};
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_SYSTRAY, kTrayTip, szText);
		g_pSystemTray->Create(g_hInstance, m_hWnd, WM_ICON_NOTIFY, szText, hIcon, NULL);
	}

	if(!pPanelTabs)
		FIND_CONTROL_BY_ID(pPanelTabs, CContainerUI, (&m_PaintManager), kPanelTabs)
	if(!pPanelContents)
		FIND_CONTROL_BY_ID(pPanelContents, CContainerUI, (&m_PaintManager), kPanelContents)

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
	else if(sCtrlName==kMenuBtn)
	{
		CDuiRect rect = msg.pSender->GetPos();
		DuiLib::CPoint point(rect.left, rect.bottom);
		ClientToScreen(m_hWnd, &point);
		CMenuUI *pMenu = new CMenuUI(m_hWnd);
		pMenu->Init(m_PaintManager.GetRoot(), kSysMenuXml, NULL, point);
	}
	if(!bHandle)
		OnAboutClick(msg, bHandle);
	if(!bHandle)
		OnPickerClick(msg, bHandle);
	if(!bHandle)
		OnCoderClick(msg, bHandle);
	if(!bHandle)
		OnFinderClick(msg, bHandle);
	if(!bHandle)
		OnFormatterClick(msg, bHandle);
	if(!bHandle)
		OnIPConfigClick(msg, bHandle);
	if(!bHandle)
		SelectPanel(sCtrlName);
	WindowImplBase::OnClick(msg);
}

void CMainFrame::OnItemActive(TNotifyUI& msg)
{
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(pParent)
	{
		CListUI *pList = (CListUI *)pParent->GetParent();
		if(!pList)
			return;
		if(pList->GetName()==GetPickerListName())
			OnPickerItemActive(msg);
		else if(pList->GetName()==GetCoderListName())
			OnCoderItemActive(msg);
		else if(pList->GetName()==GetFinderListName())
			OnFinderItemActive(msg);
	}
}

void CMainFrame::OnItemClick(TNotifyUI& msg)
{
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(pParent)
	{
		CListUI *pList = (CListUI *)pParent->GetParent();
		if(!pList)
			return;
		if(pList->GetName()==GetPickerListName())
			OnPickerItemClick(msg);
	}
}

void CMainFrame::OnItemSelect(TNotifyUI& msg)
{
	CDuiString controlName = msg.pSender->GetName();
	if(controlName==GetIPConfigSettingListName()||controlName==GetIPConfigConnectListName())
	{
		OnIPConfigItemSelect(msg);
	}
}

void CMainFrame::OnTextChanged(TNotifyUI& msg)
{
	BOOL bHandle = FALSE;
	OnPickerTextChanged(msg, bHandle);
}

void CMainFrame::OnShowSysMenu(TNotifyUI& /*msg*/)
{

}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
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
		lRes = OnCopyData(uMsg, wParam, lParam);
		break;
	}

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	LRESULT lRes = 0;
	switch(uMsg)
	{
	case WM_ICON_NOTIFY:
		lRes = OnTrayNotification(uMsg, wParam, lParam);
		break;
	}
	return 0;
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

LRESULT CMainFrame::OnCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
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
	else if(lpData->dwData==WM_SNAPSHOT_MSG)
	{
		PSNAPSHOT_INFO lpInfo = (PSNAPSHOT_INFO)lpData->lpData;
		if(lpInfo)
			OnSnapShotUpdate(lpInfo->dwColor, lpInfo->szPath);
		return TRUE;
	}
	else if(lpData->dwData==WM_ZOOMIN_UPDATE_MSG || lpData->dwData==WM_ZOOMIN_COLOR_MSG)
	{
		PZOOMIN_INFO lpInfo = (PZOOMIN_INFO)lpData->lpData;
		if(lpInfo)
		{
			BOOL bRecord = (lpData->dwData==WM_ZOOMIN_COLOR_MSG);
			OnZoomInUpdate(lpInfo->dwColor, lpInfo->ptCursor, bRecord);
		}
		return TRUE;
	}
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
			Utility::SetINIStr(g_szAppConfig, LS_SKIN, kColor, szNewImage);
			Utility::SetINIStr(g_szAppConfig, LS_SKIN, kIsColor, L"0");
			break;
		}
	}
	return 0;
}

LRESULT CMainFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnPickerLButtonDown(uMsg, wParam, lParam);
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnPickerMouseMove(uMsg, wParam, lParam);
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnPickerLButtonUp(uMsg, wParam, lParam);
	bHandled = FALSE;
	return 0;
}

LPCWSTR CMainFrame::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	LPCWSTR lpszData = NULL;
	CListBodyUI *pParent = (CListBodyUI *)pControl->GetParent();
	if(pParent)
	{
		CListUI *pList = (CListUI *)pParent->GetParent();
		if(!pList)
			return L"";
		if(pList->GetName()==GetPickerListName())
			lpszData = GetCoderItemText(pControl, iIndex, iSubItem);
		else if(pList->GetName()==GetFinderListName())
			lpszData = GetFinderItemText(pControl, iIndex, iSubItem);
	}
	return lpszData;
}

BOOL CMainFrame::SelectPanel(LPCWSTR lpszTab)
{
	BOOL bRet = FALSE;
	CDuiString sCtrlName = lpszTab, sCurTab, sCurLayout;
	LPPANEL_INFO lpInfo = NULL;
	CControlUI *pTab = NULL;
	CContainerUI *pLayout = NULL, *pCurTab = NULL, *pCurLayout = NULL;
	sCurTab = pPanelContents->GetUserData();
	FIND_CONTROL_BY_ID(pCurTab, CContainerUI, (&m_PaintManager), sCurTab)
	sCurLayout = pCurTab->GetUserData();
	FIND_CONTROL_BY_ID(pCurLayout, CContainerUI, (&m_PaintManager), sCurLayout)
	if(!pCurTab || !pCurLayout)
		return FALSE;
	if(sCurTab==sCtrlName)
	{
		pCurLayout = NULL;
		pCurTab = NULL;
	}
	list<LPPANEL_INFO>::iterator iter;
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
					pCurTab->SetBkColor(kDefBkColor);
					pCurTab->SetBkColor2(kDefBkColor);
				}
				if(pTab)
				{
					pTab->SetBkColor(kSelBkColor);
					pTab->SetBkColor2(kSelBkColor2);
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
	PANEL_INFO panel = {0};
	wchar_t szName[1024], szDesc[1024];

	memset(&panel, 0, sizeof(PANEL_INFO));
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
	LoadPanels();
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

	InitAbout();
	InitPicker();
	InitCoder((IListCallbackUI*)this);
	InitFinder((IListCallbackUI*)this);
	InitFormatter();
	InitIPConfig();
	return bRet;
}

BOOL CMainFrame::AddPanel(LPPANEL_INFO lpPanelInfo)
{
	BOOL bRet = FALSE;
	LPPANEL_INFO lpNewPanel = new PANEL_INFO();
	if(!lpNewPanel)
		return bRet;
	memset(lpNewPanel, 0, sizeof(PANEL_INFO));
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
	LPPANEL_INFO lpPanelInfo = NULL;
	list<LPPANEL_INFO>::iterator iter;
	CButtonUI *pTab = NULL;
	CChildLayoutUI *pPanel = NULL;
	for(iter=g_lstPanelInfo.begin(); iter!=g_lstPanelInfo.end(); iter++)
	{
		lpPanelInfo = (LPPANEL_INFO)(*iter);
		if(lpPanelInfo)
		{
			FIND_CONTROL_BY_ID(pTab, CButtonUI, (&m_PaintManager), lpPanelInfo->szTab)
			FIND_CONTROL_BY_ID(pPanel, CChildLayoutUI, (&m_PaintManager), lpPanelInfo->szLayout)
			if(pTab && pPanel)
				continue;
			pTab = new CButtonUI();
			if(!pTab)
				return bRet;
			pTab->SetMinWidth(36);
			pTab->SetMaxWidth(100);
			pTab->SetTextStyle(pTab->GetTextStyle() | DT_END_ELLIPSIS);
			pTab->SetTextColor(kTextColor);
			pTab->SetBorderSize(1);
			pTab->SetUserData(lpPanelInfo->szLayout);
			pTab->SetName(lpPanelInfo->szTab);
			pTab->SetText(lpPanelInfo->szName);
			pPanelTabs->AddAt(pTab, pPanelTabs->GetCount()-1);
			pPanel = new CChildLayoutUI();
			if(!pPanel)
				return bRet;
			pPanel->SetName(lpPanelInfo->szLayout);
			pPanel->SetChildLayoutXML(lpPanelInfo->szXml);
			pPanel->SetVisible(false);
			pPanelContents->AddAt(pPanel, pPanelContents->GetCount()-1);
		}
	}

	return bRet;
}

BOOL CMainFrame::ReleasePanels()
{
	LPPANEL_INFO lpPanelInfo = NULL;
	list<LPPANEL_INFO>::iterator iter;
	lpPanelInfo = (LPPANEL_INFO)g_lstPanelInfo.front();
	while(lpPanelInfo)
	{
		g_lstPanelInfo.pop_front();
		delete lpPanelInfo;
		lpPanelInfo = NULL;
		if(g_lstPanelInfo.size())
			lpPanelInfo = (LPPANEL_INFO)g_lstPanelInfo.front();
		else
			break;
	}

	return TRUE;
}
