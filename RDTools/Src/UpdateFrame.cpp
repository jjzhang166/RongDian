#include "stdafx.h"
#include "UpdateFrame.h"

const wchar_t* const kFrameLogo = L"frame_logo";
const wchar_t* const kCloseBtn = L"closebtn";
const wchar_t* const kFrameContainer = L"frame_container";
const wchar_t* const kFrameTitle = L"frame_title";
const wchar_t* const kUpdateOk = L"update_ok";
const wchar_t* const kUpdateOkTip = L"update_ok_tip";
const wchar_t* const kUpdateCancel = L"update_cancel";
const wchar_t* const kUpdateCancelTip = L"update_cancel_tip";
const wchar_t* const kUpdateLog = L"update_log";
const wchar_t* const kUpdateProgressText = L"update_progress_text";
const wchar_t* const kUpdateProgress = L"update_progress";

DUI_BEGIN_MESSAGE_MAP(CUpdateFrame, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) // 这是单击
DUI_END_MESSAGE_MAP()

CUpdateFrame::CUpdateFrame()
{
	builder_xml_ = kUpdateFrameXml;

#if USE_ZIPRES
	resource_dir_ = L"default\\";
#else
	resource_dir_ = CPaintManagerUI::GetResourcePath();
#endif
	pFrameContainer = NULL;
	hWorkingThread = NULL;
	pUpdateLog = NULL;
	pProgress = NULL;
	memset(m_szUrl, 0, sizeof(m_szUrl));
	m_pszLog = NULL;
}

CUpdateFrame::~CUpdateFrame()
{
	if(m_pszLog)
		delete m_pszLog;
}

LONG CUpdateFrame::SetSkin()
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

LONG CUpdateFrame::SetLang(LPCWSTR lpszLang)
{
SET_CONTROL_BEGIN((&m_PaintManager), lpszLang, LS_UPDATEFRAME)
	SET_CONTROL_TIP2(kFrameLogo)
	SET_CONTROL_TIP2(kCloseBtn)
	SET_CONTROL_TEXT_AND_TIP(kUpdateOk, kUpdateOkTip)
	SET_CONTROL_TEXT_AND_TIP(kUpdateCancel, kUpdateCancelTip)
	SET_TITLE_TEXT2(m_hWnd, kFrameTitle)
	SET_CONTROL_TEXT2(kUpdateProgressText)
SET_CONTROL_END()
	return 0;
}

LPCWSTR CUpdateFrame::GetWindowClassName() const 
{ 
	return kRDUpdateClass;
}

void CUpdateFrame::InitWindow()
{
	FIND_CONTROL_BY_ID(pUpdateLog, CTextUI, (&m_PaintManager), kUpdateLog)
	FIND_CONTROL_BY_ID(pProgress, CProgressUI, (&m_PaintManager), kUpdateProgress)

	g_pSkinManager->AddFrame(this);
	SetSkin();

	g_pLangManager->AddFrame(this);
	SetLang(g_pLangManager->GetLangName());

	if(m_pszLog)
		pUpdateLog->SetText(m_pszLog);

#ifdef _DEBUG
	assert(pUpdateLog!=NULL);
	wchar_t szLog[1024] = L"1、调整主面板Tab背景色为动态设置\n2、简化Tidy面板功能\n3、添加版本检测API调用 －更新对话框为实现";
	pUpdateLog->SetText(szLog);
	pProgress->SetValue(35);
#endif
}

CDuiString CUpdateFrame::GetSkinFile()
{
	return builder_xml_;
}

CDuiString CUpdateFrame::GetSkinFolder()
{
	return resource_dir_;
}

#if USE_ZIPRES
UILIB_RESOURCETYPE CUpdateFrame::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CUpdateFrame::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_RESOURCE);
}
#endif

void CUpdateFrame::OnClick(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kUpdateOk)
	{
		DWORD dwThread = 0;
		hWorkingThread = CreateThread(NULL, 0, StartDownload, (LPVOID)this, 0, &dwThread);
		ShowWindow(SW_HIDE);
	}
	else if(sCtrlName==kUpdateCancel)
	{
		ShowWindow(SW_HIDE);
	}
	WindowImplBase::OnClick(msg);
}

void CUpdateFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CUpdateFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	lRes = WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CUpdateFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	LRESULT lRes = 0;
	switch(uMsg)
	{
	case WM_UPDATE_PROGRESS:
		return UpdateProgress(uMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CUpdateFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//return __super::OnClose(uMsg, wParam, lParam, bHandled);
	ShowWindow(SW_HIDE);
	return TRUE;
}

LRESULT CUpdateFrame::UpdateProgress(INT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
{
	if(::IsWindowVisible(m_hWnd))
	{
		int nProgress = (int)wParam;
		wchar_t szProgress[64] = {0};
		swprintf(szProgress, L"%d%%", nProgress);
		pProgress->SetValue(nProgress);
		pProgress->SetText(szProgress);
	}
	return 0;
}


void CUpdateFrame::SetUpdateInfo(const char *pszUrl, const char *pszText)
{
	if(pszUrl)
	{
		memset(m_szUrl, 0, sizeof(m_szUrl));
		strcpy(m_szUrl, pszUrl);
	}
	if(pszText)
	{
		m_pszLog = StrUtil::a2w(pszText);
		assert(m_pszLog!=NULL);
	}
}

DWORD WINAPI CUpdateFrame::StartDownload(LPVOID lpParam)
{
	CUpdateFrame* pFrame = (CUpdateFrame *)lpParam;
	if(!pFrame)
		return 0;

	char szUrl[1024] = {0};
	char szPath[1024] = {0};
#pragma message("CUpdateFrame::StartDownload - RDDownLoad待实现 - 进度更新采用PostMessage方式更新，进度值为0~100，通过wParam值传递")
	RDDownLoad(pFrame->GetHWND(), szUrl, szPath, WM_UPDATE_PROGRESS);
	
	return 0;
}