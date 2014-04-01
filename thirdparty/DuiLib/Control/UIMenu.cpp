#include "StdAfx.h"

namespace DuiLib {

const wchar_t* const kMenuClass				= L"DuiMenu_Class";

CMenuUI::CMenuUI(HWND hParent /*= NULL*/)
{
	owner_control_ = NULL;
	popup_alignment = eMenuAlignment_Left | eMenuAlignment_Top;
	parent_hwnd = hParent;
	base_position.x = -1;
	base_position.y = -1;
	base_size.cx = -1;
	base_size.cy = -1;
}

CMenuUI::~CMenuUI()
{

}

LPCWSTR CMenuUI::GetWindowClassName() const
{
	return kMenuClass;
}

void CMenuUI::InitWindow()
{

}

CDuiString CMenuUI::GetSkinFile()
{
	return builder_xml_;
}

CDuiString CMenuUI::GetSkinFolder()
{
	return resource_dir_;
}

void CMenuUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

//
void CMenuUI::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		Close();
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMCLICK)//内部触发的
	{
		if(owner_control_)
			owner_control_->GetManager()->SendNotify(msg.pSender, DUI_MSGTYPE_MENUSELECT, 0, 0, true);//外部的
	}
}

LRESULT CMenuUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMenuUI::HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CMenuUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

LRESULT CMenuUI::OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if( m_hWnd != (HWND) lParam )
		PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMenuUI::OnWindowPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	wchar_t szDebug[1024];
	swprintf(szDebug, L"CMenuUI::OnWindowPosChanged:%d\n", GetTickCount());
	OutputDebugStringW(szDebug);
	return 0;
}

//
LRESULT CMenuUI::OnKillFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( m_hWnd != (HWND) wParam )
		PostMessage(WM_CLOSE);
	return 0;
}

//
LRESULT CMenuUI::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( wParam == VK_ESCAPE ) Close();
	return 0;
}

// 
LRESULT CMenuUI::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
{
	bHandled = FALSE;
	return 0;
}

// 
LRESULT CMenuUI::OnMouseHover(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
{
	bHandled = FALSE;
	return 0;
}

// 
LRESULT CMenuUI::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
{
	bHandled = FALSE;
	return 0;
}

// 
LRESULT CMenuUI::OnActivateApp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &bHandled)
{
	wchar_t szDebug[1024];
	swprintf(szDebug, L"CMenuUI::OnActivateApp:%d\n", GetTickCount());
	OutputDebugStringW(szDebug);
	if(lParam)
		Close();
	bHandled = FALSE;
	return 0;
}

// 
LRESULT CMenuUI::OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
{
	wchar_t szDebug[1024];
	swprintf(szDebug, L"CMenuUI::OnActivate:%d\n", GetTickCount());
	OutputDebugStringW(szDebug);
	bHandled = FALSE;
	return 0;
}

// 
LRESULT CMenuUI::OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CMenuUI::Init(CControlUI* pOwner, LPCTSTR xml, LPCTSTR pSkinType, POINT pt)
{
	SIZE st;
	st.cx = -1;
	st.cy = -1;
	Init(pOwner, xml, pSkinType, pt, st);
};

//
void CMenuUI::Init(CControlUI* pOwner, LPCTSTR xml, LPCTSTR /*pSkinType*/, POINT pt, SIZE st) 
{
	owner_control_ = pOwner;
	base_position.x = pt.x;
	base_position.y = pt.y;
	builder_xml_ = xml;
	if(st.cx!=-1&&st.cx>0)
		base_size.cx = st.cx;
	if(st.cy!=-1&&st.cy>0)
		base_size.cy = st.cy;
	Create(parent_hwnd, L"", WS_POPUP | WS_VISIBLE, WS_EX_TOPMOST, 0, 0, 0, 0);
	AdjustPostion();
}

void CMenuUI::AdjustPostion()
{
	DuiLib::CDuiRect rcWnd;
	GetWindowRect(m_hWnd, &rcWnd);
	int nWidth = rcWnd.GetWidth();
	int nHeight = rcWnd.GetHeight();
	if(base_size.cx>0)
		nWidth = base_size.cx;
	if(base_size.cy>0)
		nHeight = base_size.cy;
	rcWnd.left = base_position.x;
	//if(is_bktrans)
	//	rcWnd.left -= 4; // 菜单有透明外框，直接对齐x坐标会导致实际效果往右偏
	rcWnd.top = base_position.y;
	rcWnd.right = rcWnd.left + nWidth;
	rcWnd.bottom = rcWnd.top + nHeight;
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	DuiLib::CDuiRect rcWork = oMonitor.rcMonitor;
	if(rcWnd.bottom > rcWork.bottom)
	{
		if(nHeight >= rcWork.GetHeight())
		{
			rcWnd.top = 0;
			rcWnd.bottom = nHeight;
		}
		else
		{
			rcWnd.bottom = rcWork.bottom;
			rcWnd.top = rcWnd.bottom - nHeight;
		}
	}
	if(rcWnd.right > rcWork.right)
	{
		if(nWidth >= rcWork.GetWidth())
		{
			rcWnd.left = 0;
			rcWnd.right = nWidth;
		}
		else
		{
			rcWnd.right = rcWork.right;
			rcWnd.left = rcWnd.right - nWidth;
		}
	}
	if (popup_alignment & eMenuAlignment_Right)
	{
		rcWnd.right = base_position.x;
		rcWnd.left = rcWnd.right - nWidth;
	}
	if (popup_alignment & eMenuAlignment_Bottom)
	{
		rcWnd.bottom = base_position.y;
		rcWnd.top = rcWnd.bottom - nHeight;
	}
	::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER/* | SWP_NOMOVE*/ | SWP_NOACTIVATE /*| SWP_NOSIZE*/);
	SetActiveWindow(m_hWnd);
}
} // namespace DuiLib
