#include "DuiMsg.h"

#include "resource.h"
#include "RongDian.h"

#ifndef IDR_RESOURCE
#define IDR_RESOURCE					202
#endif

#include <DuiLib/UIlib.h>
using namespace DuiLib;

#pragma warning(push)
#pragma warning(disable:4996)

//#define USE_ZIPRES		1

#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/DuiLibd.lib")
#else
#pragma comment(lib, "../RongDian/bin/DuiLib.lib")
#endif

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <Macros.h>
#include <ILangUI.h>
#include <ISkinUI.h>
#include <Util.h>

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000L
#endif

typedef BOOL (WINAPI *PFNSetLayeredWindowAttributes)(HWND hWnd, COLORREF crColor, BYTE bAlpha, DWORD dwFlags);

const wchar_t* const kFrameContainer = L"frame_container";
const wchar_t* const kFrameTitleBar = L"frame_title_contaner";
const wchar_t* const kFrameIcon = L"frame_logo";
const wchar_t* const kFrameTitle = L"frame_title";
const wchar_t* const kFrameTextContainer = L"frame_text_container";
const wchar_t* const kFrameGifContainer = L"frame_gif_container";
const wchar_t* const kFrameLoading = L"frame_loading";
const wchar_t* const kFrameLoadingText = L"frame_loading_text";
const wchar_t* const kFrameText = L"frame_text";
const wchar_t* const kFrameFooter = L"frame_footer";
const wchar_t* const kFirstBtn = L"first_btn";
const wchar_t* const kSecondBtn = L"second_btn";
const wchar_t* const kSecondBlank = L"second_blank";
const wchar_t* const kThirdBtn = L"third_btn";
const wchar_t* const kThirdBlank = L"third_blank";

const wchar_t* const LS_MSG = L"Msg";
const wchar_t* const LS_DUIMSG = L"DuiMsg";
const wchar_t* const kMsgErr = L"msg_err";
const wchar_t* const kMsgYes = L"msg_yes";
const wchar_t* const kMsgNo = L"msg_no";
const wchar_t* const kMsgOK = L"msg_ok";
const wchar_t* const kMsgRetry = L"msg_retry";
const wchar_t* const kMsgCancel = L"msg_cancel";
const wchar_t* const kMsgLoading = L"msg_loading";

const int MIN_WIDTH				= 240;
const int MIN_HEIGHT			= 160;
const int POPUP_WIDTH			= 360;
const int POPUP_HEIGHT			= 240;

const int MAX_TRANSPRANT		= 250;
const int SOLID_TIMER			= 5000;
const int TRANSPRANT_TIMER		= 5001;
const int POPUP_TIMEOUT			= 5002;
const int LOADING_QUIT			= 5003;

static HINSTANCE g_hMsgInst = NULL;
static BOOL g_bInitResPath = FALSE;
static wchar_t g_szIconName[1024] = {0};
static CLangManager *g_pLangMan = NULL;
static CSkinManager *g_pSkinMan = NULL;
static PFNSetLayeredWindowAttributes g_pfnSetLayeredWindowAttributes = NULL;
static HMODULE g_hUser32 = NULL;
static BYTE g_uAlpha = 0;
static HANDLE g_hLoadEvent = NULL;

class CDuiMsg : public WindowImplBase,
	public ILangUI, public ISkinUI
{
public:
	CDuiMsg()
	{
		builder_xml_ = L"MsgFrame.xml";

		resource_dir_ = CPaintManagerUI::GetResourcePath();

		pFrameContainer = NULL;
		lpszContent = NULL;
		memset(szIcon, 0, sizeof(szIcon));
		memset(szCaption, 0, sizeof(szCaption));

		nType = MB_OK;
		nRet = IDOK;
		pFirstBtn = NULL;
		pSecondBtn = NULL;
		pSecondBlank = NULL;

		bPopup = FALSE;
		hParent = NULL;
		bTransparent = FALSE;
	};
	virtual ~CDuiMsg()
	{
		if(lpszContent)
		{
			delete lpszContent;
			lpszContent = NULL;
		}
	};

public:
	LONG SetSkin()
	{
		if(!pFrameContainer)
			//pFrameContainer = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFrameContainer));
			FIND_CONTROL_BY_ID(pFrameContainer, CContainerUI, (&m_PaintManager), kFrameContainer);
		if(!pFrameContainer)
			return -1;
		if(g_pSkinMan->IsBkColor())
		{
			pFrameContainer->SetBkColor(g_pSkinMan->GetBkColor());
			pFrameContainer->SetBkColor2(g_pSkinMan->GetBkColor2());
			pFrameContainer->SetBkImage(L"");
		}
		else
		{
			pFrameContainer->SetBkColor(kDefBkColor);
			pFrameContainer->SetBkColor2(kDefBkColor);
			pFrameContainer->SetBkImage(g_pSkinMan->GetBkImage());
		}
		return 0;
	}

	LONG SetLang(LPCWSTR lpszLang)
	{
		CControlUI *pTitle = NULL;
		//pTitle = static_cast<CControlUI*>(m_PaintManager.FindControl(kFrameTitle));
		FIND_CONTROL_BY_ID(pTitle, CControlUI, (&m_PaintManager), kFrameTitle);
		if(pTitle)
		{
			pTitle->SetText(szCaption);
			SetWindowText(m_hWnd, szCaption);
		}
		CControlUI *pText = NULL;
		//pText = static_cast<CControlUI*>(m_PaintManager.FindControl(kFrameText));
		if(nType!=MB_LOADING)
		{
			FIND_CONTROL_BY_ID(pText, CControlUI, (&m_PaintManager), kFrameText);
		}
		else
		{
			FIND_CONTROL_BY_ID(pText, CControlUI, (&m_PaintManager), kFrameLoadingText);
		}
		if(pText && lpszContent)
			pText->SetText(lpszContent);

SET_CONTROL_BEGIN((&m_PaintManager), lpszLang, LS_DUIMSG)
		switch(nType)
		{
		case MB_OK:
			{
				SET_CONTROL_TEXT4(kFirstBtn, kMsgOK, IDS_MSG_OK);
			}
			break;
		case MB_YESNO:
			{
				SET_CONTROL_TEXT4(kFirstBtn, kMsgYes, IDS_MSG_YES);
				SET_CONTROL_TEXT4(kSecondBtn, kMsgNo, IDS_MSG_NO);
			}
			break;
		case MB_OKCANCEL:
			{
				SET_CONTROL_TEXT4(kFirstBtn, kMsgOK, IDS_MSG_OK);
				SET_CONTROL_TEXT4(kSecondBtn, kMsgCancel, IDS_MSG_CANCEL);
			}
			break;
		case MB_RETRYCANCEL:
			{
				SET_CONTROL_TEXT4(kFirstBtn, kMsgRetry, IDS_MSG_RETRY);
				SET_CONTROL_TEXT4(kSecondBtn, kMsgCancel, IDS_MSG_CANCEL);
			}
			break;
		}
SET_CONTROL_END()
	
		return 0;
	}

	LPCTSTR GetWindowClassName() const 
	{
		if(bPopup)
			return kDuiMsgPopupClass;
		else
			return kDuiMsgModalClass;
	}

	UINT GetClassStyle() const
	{ 
		return CS_DBLCLKS; 
	}

	CDuiString GetSkinFile()
	{
		return builder_xml_;
	}

	CDuiString GetSkinFolder()
	{
		return resource_dir_;
	}

#if USE_ZIPRES
	UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_ZIPRESOURCE;
	}

	LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_RESOURCE);
	}
#endif
	void OnFinalMessage(HWND hWnd)
	{
		WindowImplBase::OnFinalMessage(hWnd);
		delete this;
	}
	void InitWindow()
	{
		if(!bPopup)
		{
			HDC hDC = GetDC(NULL);
			RECT rcContent;
			rcContent.left = 0;
			rcContent.top = 0;
			rcContent.right = MIN_WIDTH;
			rcContent.bottom = MIN_HEIGHT;
			CLabelUI *pText = NULL;
			//pText = static_cast<CLabelUI*>(m_PaintManager.FindControl(kFrameText));
			FIND_CONTROL_BY_ID(pText, CLabelUI, (&m_PaintManager), kFrameText);

			int nTitle = 0, nFooter = 0;
			CControlUI *pTitle = NULL;
			CControlUI *pFooter = NULL;
			//pTitle = static_cast<CControlUI*>(m_PaintManager.FindControl(kFrameTitleBar));
			//pFooter = static_cast<CControlUI*>(m_PaintManager.FindControl(kFrameFooter));
			FIND_CONTROL_BY_ID(pTitle, CControlUI, (&m_PaintManager), kFrameTitleBar);
			FIND_CONTROL_BY_ID(pFooter, CControlUI, (&m_PaintManager), kFrameFooter);
			if(pTitle)
				nTitle = pTitle->GetFixedHeight();
			if(pFooter)
				nFooter = pFooter->GetFixedHeight();
			if(nType==MB_LOADING)
			{
				if(pTitle)
					pTitle->SetVisible(false);
				if(pFooter)
					pFooter->SetVisible(false);
				nTitle = 0;
				nFooter = 0;
				CHorizontalLayoutUI *pTextContainer = NULL;
				CHorizontalLayoutUI *pGifContainer = NULL;
				CGifUI *pLoading = NULL;
				FIND_CONTROL_BY_ID(pTextContainer, CHorizontalLayoutUI, (&m_PaintManager), kFrameTextContainer);
				FIND_CONTROL_BY_ID(pGifContainer, CHorizontalLayoutUI, (&m_PaintManager), kFrameGifContainer);
				FIND_CONTROL_BY_ID(pLoading, CGifUI, (&m_PaintManager), kFrameLoading);
				if(pTextContainer)
					pTextContainer->SetVisible(false);
				if(pGifContainer)
					pGifContainer->SetVisible();
				if(pLoading && wcslen(szIcon))
					pLoading->SetGifImage(szIcon);
				RECT rcCaption = { 0, 0, 0, 0};
				m_PaintManager.SetCaptionRect(rcCaption);
			}
			else
			{
				if(hDC && lpszContent && pText)
				{
					int iFont = 0;
					HFONT hOldFont = NULL;
					iFont = pText->GetFont();
					hOldFont = (HFONT)::SelectObject(hDC, m_PaintManager.GetFont(iFont));
					::DrawText(hDC, lpszContent, wcslen(lpszContent), &rcContent, DT_CALCRECT | DT_LEFT | DT_TOP); // 计算文本矩形大小，动态调整消息框大小
					if(hOldFont)
						::SelectObject(hDC, hOldFont);
				}
			}
			int nWidth = MIN_WIDTH, nHeight = MIN_HEIGHT;
			if(nWidth<(rcContent.right - rcContent.left))
				nWidth = rcContent.right - rcContent.left + 10;
			if(nHeight<(rcContent.bottom - rcContent.top + nTitle + nFooter))
				nHeight = rcContent.bottom - rcContent.top + nTitle + nFooter;
			ResizeClient(nWidth, nHeight);
		}
		else
		{
			g_hUser32 = LoadLibrary(_T("user32.dll"));
			if(g_hUser32)
			{
				g_pfnSetLayeredWindowAttributes = (PFNSetLayeredWindowAttributes)GetProcAddress(g_hUser32, "SetLayeredWindowAttributes"); 
				if(g_pfnSetLayeredWindowAttributes)
				{
					::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
					g_pfnSetLayeredWindowAttributes(m_hWnd, 0, g_uAlpha, LWA_ALPHA);
					SetTimer(m_hWnd, SOLID_TIMER, 200, 0);
				}
			}
		}
		CButtonUI *pIcon = NULL;
		//pIcon = static_cast<CButtonUI*>(m_PaintManager.FindControl(kFrameIcon));
		FIND_CONTROL_BY_ID(pIcon, CButtonUI, (&m_PaintManager), kFrameIcon);
		if(pIcon)
			pIcon->SetNormalImage(szIcon);
		InitFooter();

		if(g_pSkinMan)
		{
			g_pSkinMan->AddFrame(this);
			SetSkin();
		}

		if(g_pLangMan)
		{
			g_pLangMan->AddFrame(this);
			SetLang(g_pLangMan->GetLangName());
		}
		else
		{
			SetLang(NULL);
		}
	}
	void InitFooter()
	{
		if(bPopup)
		{
			CControlUI *pFrameFooter = NULL;
			//static_cast<CControlUI*>(m_PaintManager.FindControl(kFrameFooter));
			FIND_CONTROL_BY_ID(pFrameFooter, CControlUI, (&m_PaintManager), kFrameFooter);
			if(pFrameFooter)
				pFrameFooter->SetVisible(false);
		}
		else
		{
			//pFirstBtn = static_cast<CControlUI*>(m_PaintManager.FindControl(kFirstBtn));
			FIND_CONTROL_BY_ID(pFirstBtn, CControlUI, (&m_PaintManager), kFirstBtn);
			if(nType==MB_YESNO || nType==MB_OKCANCEL || nType==MB_RETRYCANCEL)
			{
				//pSecondBtn = static_cast<CControlUI*>(m_PaintManager.FindControl(kSecondBtn));
				//pSecondBlank = static_cast<CControlUI*>(m_PaintManager.FindControl(kSecondBlank));
				FIND_CONTROL_BY_ID(pSecondBtn, CControlUI, (&m_PaintManager), kSecondBtn);
				FIND_CONTROL_BY_ID(pSecondBlank, CControlUI, (&m_PaintManager), kSecondBlank);
				if(pSecondBtn)
					pSecondBtn->SetVisible();
				if(pSecondBlank)
					pSecondBlank->SetVisible();
			}
			else if(nType==MB_LOADING)
			{
				if(pFirstBtn)
					pFirstBtn->SetVisible(false);
			}
		}
	}
	void OnClick(TNotifyUI& msg)
	{
		BOOL bHandle = FALSE;
		CDuiString sCtrlName = msg.pSender->GetName();
		CheckClick(sCtrlName, bHandle);
		if(!bHandle)
			WindowImplBase::OnClick(msg);
	}
	void CheckClick(LPCWSTR lpszName, BOOL &bHandle)
	{
		switch(nType)
		{
		case MB_OK:
		case MB_OKCANCEL:
			{
				if(wcsicmp(lpszName, kFirstBtn)==0)
					nRet = IDOK;
				else
					nRet = IDCANCEL;
				Close();
				bHandle = TRUE;
			}
			break;
		case MB_YESNO:
			{
				if(wcsicmp(lpszName, kFirstBtn)==0)
					nRet = IDYES;
				else if(wcsicmp(lpszName, kSecondBtn)==0)
					nRet = IDNO;
				else
					nRet = IDCANCEL;
				Close();
				bHandle = TRUE;
			}
			break;
		case MB_RETRYCANCEL:
			{
				if(wcsicmp(lpszName, kFirstBtn)==0)
					nRet = IDRETRY;
				else
					nRet = IDCANCEL;
				Close();
				bHandle = TRUE;
			}
			break;
		}
	}
	int PopupWindow()
	{
		RECT rect;
		int BLANK_WIDTH = 10;
		::SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rect, 0);
		int nLeft = rect.right - POPUP_WIDTH - BLANK_WIDTH;
		int nTop = rect.bottom - POPUP_HEIGHT - BLANK_WIDTH;
		SetWindowPos(m_hWnd, HWND_TOPMOST, nLeft, nTop, POPUP_WIDTH, POPUP_HEIGHT, SWP_SHOWWINDOW);

		UINT nRet = 0;
		MSG msg = { 0 };
		while( ::IsWindow(m_hWnd) && ::GetMessage(&msg, NULL, 0, 0) )
		{
			if( msg.message == WM_CLOSE && msg.hwnd == m_hWnd )
				nRet = msg.wParam;
			if( !CPaintManagerUI::TranslateMessage(&msg) ) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			if( msg.message == WM_QUIT )
				break;
		}
		return nRet;
	}
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		if(bPopup)
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, lParam);
		bTransparent = FALSE;
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnMouseHover(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bTransparent = FALSE;
		if(g_uAlpha<MAX_TRANSPRANT)
		{
			KillTimer(m_hWnd, TRANSPRANT_TIMER);
			KillTimer(m_hWnd, POPUP_TIMEOUT);
			SetTimer(m_hWnd, SOLID_TIMER, 200, 0);
		}
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bTransparent = TRUE;
		KillTimer(m_hWnd, SOLID_TIMER);
		SetTimer(m_hWnd, TRANSPRANT_TIMER, 200, 0);
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		UINT uIDEvent = (UINT)wParam;
		switch(uIDEvent)
		{
		case SOLID_TIMER:
			{
				if(g_pfnSetLayeredWindowAttributes && !bTransparent)
				{
					g_pfnSetLayeredWindowAttributes(m_hWnd, 0, g_uAlpha, LWA_ALPHA);
					g_uAlpha = (g_uAlpha + 20)>MAX_TRANSPRANT?MAX_TRANSPRANT:(g_uAlpha + 20);
					if(g_uAlpha==MAX_TRANSPRANT)
					{
						SetTimer(m_hWnd, POPUP_TIMEOUT, 30*1000, NULL);
						KillTimer(m_hWnd, SOLID_TIMER);
					}
				}
			}
			break;
		case TRANSPRANT_TIMER:
			{
				if(g_pfnSetLayeredWindowAttributes && bTransparent)
				{
					g_pfnSetLayeredWindowAttributes(m_hWnd, 0, g_uAlpha, LWA_ALPHA);
					g_uAlpha = (g_uAlpha<10) ? 0 : (g_uAlpha - 10);
					if(g_uAlpha==0)
						PostMessage(WM_CLOSE, 0L, 0L);
				}
			}
			break;
		case POPUP_TIMEOUT:
			{
				SetTimer(m_hWnd, TRANSPRANT_TIMER, 200, 0);
			}
			break;
		case LOADING_QUIT:
			{
				KillTimer(m_hWnd, LOADING_QUIT);
				::PostMessage(m_hWnd, WM_CLOSE, 0L, 0L);
				HWND hWndParent = GetWindowOwner(m_hWnd);
				if(hWndParent)
				{
					::EnableWindow(hWndParent, TRUE);
					::SetFocus(hWndParent);
				}
			}
			break;
		}
		bHandled = FALSE;
		return 0;
	}
	void SetIcon(LPCWSTR lpszIcon)
	{
		if(lpszIcon)
			wcscpy(szIcon, lpszIcon);
	}
	void SetLoading(LPCWSTR lpszLoadingImg)
	{
		if(lpszLoadingImg)
			wcscpy(szIcon, lpszLoadingImg);
	}
	void SetCaption(LPCWSTR lpszCaption)
	{
		if(!lpszCaption)
			return;
		if(lpszCaption)
			wcscpy(szCaption, lpszCaption);
	}
	BOOL SetText(LPCWSTR lpszText)
	{
		if(!lpszText)
			return TRUE;
		int nLen = 1024;
		int nStrLen = 0;
		if(lpszText && (nStrLen=wcslen(lpszText)+1)>nLen)
			nLen = (((nStrLen+nLen)/nLen)+1)*nLen;
		else
			nLen = 1024;
		lpszContent = new wchar_t[nLen + 1];
		if(!lpszContent)
			return FALSE;
		memset(lpszContent, 0, sizeof(wchar_t)*nLen +1);
		if(lpszText)
			wcscpy(lpszContent, lpszText);
		return TRUE;
	}
	void GetLoader(LPVOID *ppParam)
	{
		*ppParam = (LPVOID)this;
	}

public:
	CDuiString builder_xml_;
	CDuiString resource_dir_;
	CContainerUI *pFrameContainer;
	wchar_t *lpszContent;
	wchar_t szIcon[1024];
	wchar_t szCaption[1024];
	int nType;
	int nRet;
	CControlUI *pFirstBtn;
	CControlUI *pSecondBtn;
	CControlUI *pSecondBlank;
	BOOL bPopup;
	HWND hParent;
	BOOL bTransparent;
};

DWORD WINAPI StartMenuMsgBox(LPVOID lpParam)
{
	CDuiMsg *pMsg = (CDuiMsg *)lpParam;
	if(pMsg)
	{
		pMsg->Create(pMsg->hParent, L"", UI_WNDSTYLE_FRAME | UI_CLASSSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0);
		pMsg->CenterWindow();
		pMsg->ShowModal();
	}
	return 0;
}

DWORD WINAPI StartLoading(LPVOID lpParam)
{
	CDuiMsg *pMsg = (CDuiMsg *)lpParam;
	if(pMsg)
	{
		pMsg->Create(pMsg->hParent, L"", UI_WNDSTYLE_FRAME | UI_CLASSSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0);
		pMsg->CenterWindow();
		pMsg->ShowModal();
	}
	return 0;
}

int __stdcall InitDuiMsg(HINSTANCE hInst, LPVOID pLangMan, LPVOID pSkinMan, LPCWSTR lpszIconName)
{
	if(!g_bInitResPath)
	{
		g_hMsgInst = hInst;
		g_pLangMan = (CLangManager *)pLangMan;
		g_pSkinMan = (CSkinManager *)pSkinMan;
		CPaintManagerUI::SetInstance(g_hMsgInst);
		if(lpszIconName)
			wcscpy(g_szIconName, lpszIconName);
		//wchar_t szModule[1024];
		//GetModuleFileNameW(NULL, szModule, _countof(szModule));
		//PathRemoveFileSpec(szModule);
		//PathRemoveFileSpec(szModule);
		//wchar_t szRes[1024];
		//swprintf(szRes, L"%s\\res", szModule);
		//CPaintManagerUI::SetResourcePath(szRes);
		g_bInitResPath = TRUE;
	}
	return 0;
}

int __stdcall DuiMsgBox(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszCaption, UINT uType)
{
	if(!g_hMsgInst)
		return -1;
	if(uType!=MB_POPUP)
	{
		HWND hModalWnd = FindWindow(kDuiMsgModalClass, NULL);
		if(hModalWnd)
		{
			SetForegroundWindow(hModalWnd);
			return -1;
		}
	}
	CDuiMsg *pMsg = new CDuiMsg();
	if(!pMsg)
		return -1;
	pMsg->hParent = hWnd;
	pMsg->SetIcon(g_szIconName);
	wchar_t szText[1024];
	if(lpszCaption)
		wcscpy(szText, lpszCaption);
	else if(g_pLangMan)
		Utility::GetINIStr(g_pLangMan->GetLangName(), LS_MSG, kMsgErr, szText);
	else
		LoadString(g_hMsgInst, IDS_MSG_ERR, szText, _countof(szText));
	pMsg->SetCaption(szText);
	if(!pMsg->SetText(lpszText))
		return -1;
	
	int nTemp = 0;
	if(uType!=MB_POPUP)
	{
		nTemp = uType & 0x0000000FL;
		if(nTemp==MB_RETRYCANCEL)
			pMsg->nType = MB_RETRYCANCEL;
		else if(nTemp==MB_YESNO)
			pMsg->nType = MB_YESNO;
		else if(nTemp==MB_OKCANCEL)
			pMsg->nType = MB_OKCANCEL;
		else
			pMsg->nType = MB_OK;
	}
	else
	{
		pMsg->nType = uType;
		pMsg->bPopup = TRUE;
	}
	if(uType & MB_MENU)
	{
		DWORD dwThread = 0;
		HANDLE hThread = CreateThread(NULL, 0, StartMenuMsgBox, (LPVOID)pMsg, 0, &dwThread);
		CloseHandle(hThread);
		return 0;
	}
	if(!pMsg->bPopup)
	{
		pMsg->Create(hWnd, L"", UI_WNDSTYLE_FRAME | UI_CLASSSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0);
		pMsg->CenterWindow();
		pMsg->ShowModal();
	}
	else
	{
		pMsg->Create(NULL, L"", UI_WNDSTYLE_FRAME | UI_CLASSSTYLE_DIALOG, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, 0, 0, 0, 0);
		pMsg->PopupWindow();
	}
	int nRet = pMsg->nRet;

	return nRet;
}

int __stdcall DuiMenuMsgBox(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszCaption, UINT uType)
{
	return DuiMsgBox(hWnd, lpszText, lpszCaption, uType | MB_MENU);
}

int __stdcall DuiPopupMsg(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszCaption)
{
	return DuiMsgBox(hWnd, lpszText, lpszCaption, MB_POPUP);
}

int __stdcall DuiShowLoading(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszLoadImg, LPVOID *ppParam)
{
	if(!g_hMsgInst)
		return -1;
	CDuiMsg *pMsg = new CDuiMsg();
	if(!pMsg)
		return -1;
	pMsg->hParent = hWnd;
	pMsg->SetIcon(L"");
	pMsg->SetCaption(L"");
	pMsg->GetLoader(ppParam);
	wchar_t szText[1024];
	if(lpszText)
		wcscpy(szText, lpszText);
	else if(g_pLangMan)
		Utility::GetINIStr(g_pLangMan->GetLangName(), LS_MSG, kMsgLoading, szText);
	else
		LoadString(g_hMsgInst, IDS_MSG_LOADING, szText, _countof(szText));
	pMsg->SetText(szText);
	if(lpszLoadImg && wcslen(lpszLoadImg))
		pMsg->SetLoading(lpszLoadImg);
	pMsg->nType = MB_LOADING;
	DWORD dwThread = 0;
	HANDLE hThread = CreateThread(NULL, 0, StartLoading, (LPVOID)pMsg, 0, &dwThread);
	CloseHandle(hThread);
	return 0;
}

int __stdcall DuiCancelLoading(LPVOID lpParam)
{
	CDuiMsg *pMsg = (CDuiMsg *)lpParam;
	if(pMsg)
	{
		SetTimer(pMsg->GetHWND(), LOADING_QUIT, 500, NULL);
	}
	return 0;
}
#pragma warning(pop)
