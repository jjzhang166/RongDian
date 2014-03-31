#include "stdafx.h"
#include "MenuEx.h"
#include <math.h>
#include <algorithm>

#pragma warning(push)
#pragma warning(disable:4996)

#pragma comment(lib, "msimg32.lib")

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
#endif

#define ENABLE_VISUAL_KEY	1

const int CX_GAP = 4;			// num pixels between button and text
const int CX_TEXT_MARGIN = 4;	// num pixels after high light to start text
const int CX_BUTTON_MARGIN = 2;	// num pixels wider button is than bitmap
const int CY_BUTTON_MARGIN = 2;	// ditto for height
const int MENUEX_TEXT_FLAG = DT_SINGLELINE | DT_VCENTER | DT_LEFT;


map<HMENU, CMenuEx*> s_mapMenuEx;
map<HWND, CMenuEx*> s_mapMenuExHwnd;
vector<HMENU> CMenuEx::s_hLastMenu;

//////////////////////////////////////////////////////////////////////////
//
CMenuKeyHelper::CMenuKeyHelper()
	: m_lpAccel(NULL)
{

}

CMenuKeyHelper::CMenuKeyHelper(LPACCEL lpAccel)
	: m_lpAccel (lpAccel)
{

}

CMenuKeyHelper::~CMenuKeyHelper()
{

}

void CMenuKeyHelper::Format(LPWSTR pszVirtKey, int nMax) const
{
	if(!pszVirtKey)
		return;
	if(!m_lpAccel)
	{
		ASSERT (FALSE);
		return;
	}
	wcscpy(pszVirtKey, L"");
	if(m_lpAccel->fVirt & FCONTROL)
		wcscat(pszVirtKey, L"Ctrl+");
	if(m_lpAccel->fVirt & FSHIFT)
		wcscat(pszVirtKey, L"Shift+");
	if(m_lpAccel->fVirt & FALT)
		wcscat(pszVirtKey, L"Shift+");
	if(m_lpAccel->fVirt & FVIRTKEY)
	{
		wchar_t szKey[64];
		UINT ukey = MapVirtualKeyW(m_lpAccel->key, 0) << 16;
		GetKeyNameText(ukey, szKey, sizeof(szKey));
		wcscat(pszVirtKey, szKey);
	}
	if(m_lpAccel->key != 27)	// Don't print esc
	{
		//pszVirtKey += (wchar_t)(m_lpAccel->key);
		//wcscat(pszVirtKey, szKey);
	}
}

void CMenuKeyHelper::AddVirtKey(LPWSTR pszVirtKey, UINT uiVirtKey, BOOL bLast /*= FALSE*/) const
{
	TCHAR szBuffer[256];
	UINT nScanCode = ::MapVirtualKeyEx (uiVirtKey, 0, ::GetKeyboardLayout(0)) <<16 | 0x1;
	if(uiVirtKey >= VK_PRIOR && uiVirtKey <= VK_HELP)
	{
		nScanCode |= 0x01000000;
	}
	::GetKeyNameText(nScanCode, szBuffer, _countof(szBuffer));
	for(int i=0; i<(int)wcslen(szBuffer); i++)
		szBuffer[i] = tolower(szBuffer[i]);

	//--------------------------------------
	// The first letter should be uppercase:
	//--------------------------------------
	for (int nCount=0; nCount<(int)wcslen(szBuffer); nCount++)
	{
		wchar_t c = szBuffer[nCount];
		if(IsCharLower (c))
		{
			c = (wchar_t)toupper(c); // Convert single character JY 4-Dec-99
			szBuffer[nCount] = c;
			break;
		}
	}

	if(wcslen(pszVirtKey)==0)
		wcscpy(pszVirtKey, szBuffer);
	else
		wcscat(pszVirtKey, szBuffer);

	if(!bLast)
	{
		if(wcslen(pszVirtKey)==0)
			wcscpy(pszVirtKey, L"+");
		else
			wcscat(pszVirtKey, L"+");
	}
}

//////////////////////////////////////////////////////////////////////////
//
CMenuEx::CMenuEx()
	: m_hMenu(NULL)
	, m_hWnd(NULL)
	, m_pOldWndProc(NULL)
{
	//initialize menu font with the default
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	VERIFY(m_hFont = CreateFontIndirect(&info.lfMenuFont));

	info.lfMenuFont.lfWidth = 0;
	info.lfMenuFont.lfWeight = FW_BOLD;
	VERIFY(m_hHBoldFont != NULL);
	VERIFY(m_hHBoldFont = CreateFontIndirect(&info.lfMenuFont));

	info.lfMenuFont.lfWidth = 0;
	info.lfMenuFont.lfEscapement = 900;
	info.lfMenuFont.lfOrientation = 900;
	info.lfMenuFont.lfWeight = FW_BOLD;
	VERIFY(m_hVBoldFont != NULL);
	VERIFY(m_hVBoldFont = CreateFontIndirect(&info.lfMenuFont));

	//initialize colors with system default
	m_clrBackGround = ::GetSysColor(COLOR_MENU);
	m_clrSelectedBar = ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrSelectedText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrText = ::GetSysColor(COLOR_MENUTEXT);
	m_clrDisabledText = ::GetSysColor(COLOR_GRAYTEXT);
	m_clrIconArea = m_clrBackGround;

	//initialize Title colors
	m_clrTitleStart = RGB(0, 0, 192);
	m_clrTitleEnd = RGB(0, 0, 0);

	//initialize sidebar colors
	m_clrSideBarStart = RGB(0, 0, 192);
	m_clrSideBarEnd = RGB(0, 0, 0);

	//the default style is office style
	m_Style = STYLE_OFFICE;

	m_bBreak = FALSE;
	m_bBreakBar = FALSE;
	m_hBitmap = NULL;
	m_hMemDC = NULL;
	m_hSepBitmap = NULL;
	m_hSepMemDC = NULL;

	m_nWidth = 0;
	m_nHeight = 0;
	
	m_nTitleHeight = 0;
	m_nSideBarWidth = 0;
}

CMenuEx::~CMenuEx()
{
	Clear();
	DestroyMenu();
}

BOOL CMenuEx::CreateMenu()
{
	m_hMenu = ::CreateMenu();
	if(m_hMenu)
	{
		s_mapMenuEx.insert(pair<HMENU, CMenuEx*>(m_hMenu, this));
		s_hLastMenu.push_back(m_hMenu);
	}
	return (m_hMenu!=NULL);
}

BOOL CMenuEx::CreatePopupMenu()
{
	m_hMenu = ::CreatePopupMenu();
	if(m_hMenu)
	{
		s_mapMenuEx.insert(pair<HMENU, CMenuEx*>(m_hMenu, this));
		s_hLastMenu.push_back(m_hMenu);
	}
	return (m_hMenu!=NULL);
}

BOOL CMenuEx::LoadMenu(LPCTSTR lpszResourceName)
{
	m_hMenu = ::LoadMenu(AfxFindResourceHandle(lpszResourceName, RT_MENU), lpszResourceName);
	if(m_hMenu)
	{
		s_mapMenuEx.insert(pair<HMENU, CMenuEx*>(m_hMenu, this));
		s_hLastMenu.push_back(m_hMenu);
	}
	return (m_hMenu!=NULL);
}

BOOL CMenuEx::LoadMenu(UINT nIDResource)
{
	m_hMenu = ::LoadMenu(AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_MENU), MAKEINTRESOURCE(nIDResource));
	if(m_hMenu)
	{
		s_mapMenuEx.insert(pair<HMENU, CMenuEx*>(m_hMenu, this));
		s_hLastMenu.push_back(m_hMenu);
	}
	return (m_hMenu!=NULL);
}

BOOL CMenuEx::LoadMenuIndirect(const void* lpMenuTemplate)
{
	m_hMenu = ::LoadMenuIndirect(lpMenuTemplate);
	if(m_hMenu)
	{
		s_mapMenuEx.insert(pair<HMENU, CMenuEx*>(m_hMenu, this));
		s_hLastMenu.push_back(m_hMenu);
	}
	return (m_hMenu!=NULL);
}

BOOL CMenuEx::DestroyMenu()
{
	if(!m_hMenu)
		return FALSE;
	std::map<HMENU, CMenuEx*>::iterator iter;
	iter = s_mapMenuEx.find(m_hMenu);
	if(iter!=s_mapMenuEx.end())
	{
		s_mapMenuEx.erase(iter);
		s_hLastMenu.erase(s_hLastMenu.begin());
	}
	return ::DestroyMenu(m_hMenu);
}

HMENU CMenuEx::GetSafeHmenu() const
{
	ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
	return this == NULL ? NULL : m_hMenu;
}

CMenuEx::operator HMENU() const
{
	ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
	return this == NULL ? NULL : m_hMenu;
}

BOOL CMenuEx::DeleteMenu(UINT nPosition, UINT nFlags)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::DeleteMenu(m_hMenu, nPosition, nFlags);
}

BOOL CMenuEx::TrackPopupMenu(UINT nFlags, int x, int y, HWND hWnd, LPCRECT lpRect /*= 0*/)
{
	return ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, hWnd, lpRect);
}

BOOL CMenuEx::TrackPopupMenuEx(UINT fuFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm)
{
	return ::TrackPopupMenuEx(m_hMenu, fuFlags, x, y, hWnd, lptpm);
}

#if(WINVER >= 0x0500)
BOOL CMenuEx::SetMenuInfo(LPCMENUINFO lpcmi)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::SetMenuInfo(m_hMenu, lpcmi);
}

BOOL CMenuEx::GetMenuInfo(LPMENUINFO lpcmi) const
{
	ASSERT(::IsMenu(m_hMenu));
	return ::GetMenuInfo(m_hMenu, lpcmi);
}
#endif

BOOL CMenuEx::operator==(const CMenuEx& menu) const
{
	return ((HMENU) menu) == m_hMenu;
}

BOOL CMenuEx::operator!=(const CMenuEx& menu) const
{
	return ((HMENU) menu) != m_hMenu;
}

BOOL CMenuEx::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::AppendMenu(m_hMenu, nFlags, nIDNewItem, lpszNewItem);
}

BOOL CMenuEx::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
{
	ASSERT(::IsMenu(m_hMenu)); 
	return ::AppendMenu(m_hMenu, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)hBmp);
}

UINT CMenuEx::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
{
	ASSERT(::IsMenu(m_hMenu));
	return (UINT)::CheckMenuItem(m_hMenu, nIDCheckItem, nCheck);
}

UINT CMenuEx::EnableMenuItem(UINT nIDEnableItem, UINT nEnable)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::EnableMenuItem(m_hMenu, nIDEnableItem, nEnable);
}

UINT CMenuEx::GetMenuItemCount() const
{
	ASSERT(::IsMenu(m_hMenu));
	return ::GetMenuItemCount(m_hMenu);
}

UINT CMenuEx::GetMenuItemID(int nPos) const
{
	ASSERT(::IsMenu(m_hMenu));
	return ::GetMenuItemID(m_hMenu, nPos);
}

UINT CMenuEx::GetMenuState(UINT nID, UINT nFlags) const
{
	ASSERT(::IsMenu(m_hMenu));
	return ::GetMenuState(m_hMenu, nID, nFlags);
}

int CMenuEx::GetMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags) const
{
	ASSERT(::IsMenu(m_hMenu));
	return ::GetMenuString(m_hMenu, nIDItem, lpString, nMaxCount, nFlags);
}

BOOL CMenuEx::GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
{
	ASSERT(::IsMenu(m_hMenu));
	ASSERT_POINTER(lpMenuItemInfo, MENUITEMINFO);
	return ::GetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
}

BOOL CMenuEx::SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
{
	ASSERT(::IsMenu(m_hMenu));
	ASSERT_POINTER(lpMenuItemInfo, MENUITEMINFO);
	return ::SetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
}

CMenuEx* CMenuEx::GetSubMenu(int nPos) const
{
	ASSERT(::IsMenu(m_hMenu)); 
	return (CMenuEx*)FromHandle(::GetSubMenu(m_hMenu, nPos));
}

BOOL CMenuEx::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::InsertMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}

BOOL CMenuEx::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::InsertMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)hBmp);
}

BOOL CMenuEx::InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
{
	ASSERT(::IsMenu(m_hMenu));
	ASSERT_POINTER(lpMenuItemInfo, MENUITEMINFO);
	return ::InsertMenuItem(m_hMenu, uItem, fByPos, lpMenuItemInfo);
}

BOOL CMenuEx::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::ModifyMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}

BOOL CMenuEx::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::ModifyMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)hBmp);
}

BOOL CMenuEx::RemoveMenu(UINT nPosition, UINT nFlags)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::RemoveMenu(m_hMenu, nPosition, nFlags);
}

BOOL CMenuEx::SetMenuItemBitmaps(UINT nPosition, UINT nFlags, const HBITMAP hBmpUnchecked, const HBITMAP hBmpChecked)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::SetMenuItemBitmaps(m_hMenu, nPosition, nFlags, hBmpUnchecked, hBmpChecked);
}

BOOL CMenuEx::CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags)
{
	return ::CheckMenuRadioItem(m_hMenu, nIDFirst, nIDLast, nIDItem, nFlags);
}

BOOL CMenuEx::SetDefaultItem(UINT uItem, BOOL fByPos /*= FALSE*/)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::SetMenuDefaultItem(m_hMenu, uItem, fByPos);
}

UINT CMenuEx::GetDefaultItem(UINT gmdiFlags, BOOL fByPos /*= FALSE*/)
{
	ASSERT(::IsMenu(m_hMenu));
	return ::GetMenuDefaultItem(m_hMenu, fByPos, gmdiFlags);
}

BOOL CMenuEx::SetMenuContextHelpId(DWORD dwContextHelpId)
{
	return ::SetMenuContextHelpId(m_hMenu, dwContextHelpId);
}

DWORD CMenuEx::GetMenuContextHelpId() const
{
	return ::GetMenuContextHelpId(m_hMenu);
}

void CMenuEx::DrawItem(LPDRAWITEMSTRUCT pdis)
{
	ASSERT(pdis);
	if(pdis->CtlType != ODT_MENU)
		return; // not handled by me
	CMenuExItem *pItem = (CMenuExItem *)pdis->itemData;
	if(!pItem)
		return;

	ASSERT(pdis->itemAction != ODA_FOCUS);
	ASSERT(pdis->hDC);
	HDC hDC = pdis->hDC;
	RECT rcClipBox;
	::GetClipBox(hDC, &rcClipBox);
	if(pItem->m_bTitle)
	{
		//draw the Title
		RECT rcTitle;
		rcTitle.left = rcClipBox.left;
		rcTitle.top = pdis->rcItem.top;
		rcTitle.right = rcClipBox.right;
		rcTitle.bottom = pdis->rcItem.bottom;
		DrawTitle(hDC, rcTitle, pItem->m_hIcon, pItem->m_szText);
	}
	else if(pItem->m_bSideBar)
	{
		//draw the side bar
		RECT rcSideBar;
		rcSideBar.left = pdis->rcItem.left;
		rcSideBar.top = rcClipBox.top + m_nTitleHeight;
		rcSideBar.right = pdis->rcItem.right;
		rcSideBar.bottom = rcClipBox.bottom;
		DrawSideBar(hDC, rcSideBar, pItem->m_hIcon, pItem->m_szText);
	}
	else if(pItem->m_bSeparator) 
	{
		//draw background first
		RECT rcSeparator = {
			pdis->rcItem.left,// + m_nHeight,
			pdis->rcItem.top, 
			pdis->rcItem.right, 
			pdis->rcItem.bottom
		}; // copy rect
		DrawBackGround(hDC, rcSeparator, FALSE, FALSE);
		// draw the background
		rcSeparator.top += (rcSeparator.bottom - rcSeparator.top) >> 1; // vertical center
		if(m_hSepMemDC)
		{
			::BitBlt(hDC, 
				rcSeparator.left + CX_GAP + m_nHeight,
				rcSeparator.top,
				rcSeparator.right - rcSeparator.left - CX_GAP * 2,
				rcSeparator.bottom - rcSeparator.top,
				m_hSepMemDC,
				0,
				0,
				SRCCOPY);
		}
		else
		{
			::DrawEdge(hDC, &rcSeparator, EDGE_ETCHED, BF_TOP); // draw separator line
		}

		// in XP mode, fill the icon area with the icon area color
		if(m_Style == STYLE_XP)
		{
			RECT rcArea = { 
				pdis->rcItem.left, 
				pdis->rcItem.top, 
				pdis->rcItem.left + pItem->m_nSize + (CY_BUTTON_MARGIN<<1), 
				pdis->rcItem.top + pItem->m_nSize + (CY_BUTTON_MARGIN<<1)
			};
			DrawIconArea(hDC, rcArea, FALSE, FALSE, FALSE);
		} // end of 'if(m_Style == STYLE_XP)'
	} // end of 'else if(pItem->m_bSeparator)'
	else
	{
		BOOL bGrayed = (pdis->itemState & ODS_GRAYED) || (pdis->itemState & ODS_DISABLED);
		BOOL bSelected = pdis->itemState & ODS_SELECTED;
		BOOL bChecked  = pdis->itemState & ODS_CHECKED;

		//draw the background first
		DrawBackGround(hDC, pdis->rcItem, bSelected, bGrayed);

		//Draw the icon area for XP style
		if(m_Style == STYLE_XP)
		{
			RECT rcArea = { 
				pdis->rcItem.left, 
				pdis->rcItem.top, 
				pdis->rcItem.left + pdis->rcItem.bottom - pdis->rcItem.top,
				pdis->rcItem.top + pdis->rcItem.bottom - pdis->rcItem.top
			};
			DrawIconArea(hDC, rcArea, bSelected, bGrayed, bChecked);
		}

		//draw the button, not the icon
		RECT rcButton = { 
			pdis->rcItem.left, 
			pdis->rcItem.top, 
			pdis->rcItem.left + pdis->rcItem.bottom - pdis->rcItem.top,
			pdis->rcItem.top + pdis->rcItem.bottom - pdis->rcItem.top,
		};
		if(pItem->m_bButtonOnly)
		{
			rcButton.left = pdis->rcItem.left;
			rcButton.top = pdis->rcItem.top;
			rcButton.right = pdis->rcItem.right;
			rcButton.bottom = pdis->rcItem.bottom;
		}
		if(pItem->m_hIcon || bChecked)
		{
			DrawButton(hDC, rcButton, bSelected, bGrayed, bChecked);
		}
		//draw the icon actually
		if(pItem->m_hIcon)
		{
			RECT rcIcon =  { 
				rcButton.left, 
				rcButton.top, 
				rcButton.right,
				rcButton.bottom
			};
			::InflateRect(&rcIcon, -2, -2);
			DrawIcon(hDC, rcIcon, pItem->m_hIcon, bSelected, bGrayed, bChecked);
		}
		else if(bChecked)	
		{
			//draw the check mark
			RECT rcCheck =  { 
				rcButton.left, 
				rcButton.top, 
				rcButton.right,
				rcButton.bottom
			};
			::InflateRect(&rcCheck, -2, -2);
			DrawCheckMark(hDC, rcCheck, bSelected);
		}

		//draw text finally
		if(!pItem->m_bButtonOnly)
		{
			RECT rcText =  { 
				pdis->rcItem.left, 
				pdis->rcItem.top, 
				pdis->rcItem.right,
				pdis->rcItem.bottom
			}; // start w/whole item
			rcText.left += rcButton.right-rcButton.left + CX_GAP + CX_TEXT_MARGIN; // left margin
			rcText.right -= pItem->m_nSize;				 // right margin
			DrawText(hDC, rcText, pItem->m_szText, bSelected, bGrayed, pdis->itemState&ODS_DEFAULT ? 1 : 0);
		}
	}
}

void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT pmis)
{
	if(pmis->CtlType != ODT_MENU)
		return;

	CMenuExItem	*pItem = (CMenuExItem *)pmis->itemData;
	if(!pItem || !pItem->IsMyData())
		return;
	if(pItem->m_bSideBar)
	{
		pmis->itemWidth = pItem->m_nSize;
		pmis->itemHeight = 0;
	}
	else if(pItem->m_bSeparator)
	{
		// separator: use half system height and zero width
		pmis->itemHeight = ::GetSystemMetrics(SM_CYMENUCHECK)>>1;
		pmis->itemWidth  = 0;
	}
	else
	{
		//calculate the size needed to draw the text: use DrawText with DT_CALCRECT
		HDC hScreenDC = ::GetWindowDC(NULL);	// screen DC--I won't actually draw on it
		RECT rcText = { 0, 0, 0, 0 };
		HFONT hOldFont = NULL;
		// Calculate the size with bold font, for default item to be correct
		HFONT hBoldFont = NULL;
		LOGFONT logFont;
		::GetObject(m_hFont, sizeof(LOGFONT), &logFont);
		hBoldFont = ::CreateFontIndirect(&logFont);
		hOldFont = (HFONT)::SelectObject(hScreenDC, hBoldFont);
		//hOldFont = ::SelectObject(hScreenDC, m_hFont);
		::DrawText(hScreenDC, pItem->m_szText, wcslen(pItem->m_szText), &rcText, MENUEX_TEXT_FLAG | DT_CALCRECT);
		::SelectObject(hScreenDC, hOldFont);

		// the height of the item should be the maximum of the text and the button
		pmis->itemHeight = max(rcText.bottom-rcText.top, pItem->m_nSize + (CY_BUTTON_MARGIN<<1));
		if(pItem->m_bButtonOnly)
		{	
			// for button only style, we set the item's width to be the same as its height
			pmis->itemWidth = pmis->itemHeight;
		}
		else
		{
			// width is width of text plus a bunch of stuff
			int cx = rcText.right - rcText.left; // text width 
			cx += CX_TEXT_MARGIN << 1; // L/R margin for readability
			cx += CX_GAP; // space between button and menu text
			cx += (pItem->m_nSize + CY_BUTTON_MARGIN * 2) << 1; // button width (L=button; R=empty margin)

			pmis->itemWidth = cx; // done deal
		}
		if(m_nHeight!=0)
			pmis->itemHeight = m_nHeight;
		if(m_nWidth!=0)
			pmis->itemWidth = m_nWidth;
		if(pItem->m_bTitle)
			pmis->itemHeight = pItem->m_nSize;
	}

	// whatever value I return in pmis->itemWidth, Windows will add the
	// width of a menu check mark, so I must subtract to defeat Windows. Argh.
	//
	pmis->itemWidth -= ::GetSystemMetrics(SM_CXMENUCHECK)-1;
}

int CMenuEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return 0;
}

void CMenuEx::OnWindowPosChanging(LPWINDOWPOS lpPos)
{

}

void CMenuEx::OnSharedMenu(HWND hWnd)
{

}

void CMenuEx::OnShowWindow(BOOL bShow, UINT nStatus)
{

}

int CMenuEx::OnNcPaint(HWND hWnd, HRGN hRgn)
{
	HDC hdc;
	hdc = GetDCEx(hWnd, hRgn, DCX_WINDOW|DCX_INTERSECTRGN);
	// Paint into this DC
	ReleaseDC(hWnd, hdc);

	return 0; // return 0 if not handled, return 1 if handled
}

int CMenuEx::OnPrint(HWND hWnd, HDC hDC, DWORD dwOption)
{
	return 0;
}

int CMenuEx::OnPrintClient(HWND hWnd, HDC hDC, DWORD dwOption)
{
	return 0;
}

int CMenuEx::OnPaint(HWND hWnd)
{
	return 0;
}

BOOL CMenuEx::OnEraseBkgnd(HDC hDC)
{
	return TRUE;
}

void CMenuEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if(m_nSideBarWidth)
	{
		LPRECT pRect = bCalcValidRects ? &(lpncsp)->rgrc[0] : (LPRECT)lpncsp;
		pRect->left += m_nSideBarWidth;
	}
}

void CMenuEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CMenuEx::OnDestroy()
{

}

void CMenuEx::OnNcDestroy()
{

}

void CMenuEx::OnContextMenu(HWND hWnd, LPPOINT pCursor)
{

}

void CMenuEx::OnInitMenuPopup(HMENU hMenu, UINT nIndex, BOOL bSysMenu)
{

}

BOOL CMenuEx::OnMouseMenu(int xPos, int yPos)
{
	BOOL bRet = FALSE;
	POINT pt = { xPos, yPos };
	HWND hWnd = ::WindowFromPoint(pt);
	if(hWnd && (::GetWindowLong(hWnd, GWL_STYLE) & WS_SYSMENU))
	{
		// convert to window coords
		RECT rWindow;
		::GetWindowRect(hWnd, &rWindow);
		POINT ptScreen = { xPos, yPos };
		xPos -= rWindow.left;
		yPos -= rWindow.top;
		if(xPos > 0 && xPos < 16 && yPos > 0 && yPos < 16)
		{
			CMenuEx::DoSysMenu(hWnd, ptScreen, NULL, TRUE);
			return TRUE; // handled
		}
	}
	return bRet;
}

BOOL CMenuEx::AddTitle(CMenuExTitle *pItem)
{
	ASSERT(pItem);
	m_nTitleHeight = pItem->m_nSize;
	UINT nFlags = 0;
	if(m_bBreak) 
		nFlags |= MF_MENUBREAK;
	if(m_bBreakBar)
		nFlags |= MF_MENUBARBREAK;
	nFlags |= MF_BYPOSITION | MF_OWNERDRAW | MF_SEPARATOR;
	m_bBreak = m_bBreakBar = FALSE;
	return AppendMenu(nFlags, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CMenuEx::AddSideBar(CMenuExSideBar *pItem)
{
	ASSERT(pItem);
	m_bBreak = TRUE;
	m_bBreakBar = FALSE;
	m_nSideBarWidth = pItem->m_nSize;
	return AppendMenu(MF_OWNERDRAW | MF_SEPARATOR, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CMenuEx::AddMenu(UINT nFlags, CMenuExItem *pItem, ACCEL *pAccel /*= 0*/)
{
	ASSERT(pItem);
	nFlags |= MF_OWNERDRAW;
	if(m_bBreak) 
		nFlags |= MF_MENUBREAK;
	if(m_bBreakBar)
		nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;
	if(pAccel)
	{
		CMenuKeyHelper keyhelper(pAccel);
		wchar_t szAccel[256] = {0};
		keyhelper.Format(szAccel, _countof(szAccel));
		if(wcslen(szAccel))
		{
			if(wcslen(pItem->m_szText))
				wcscat(pItem->m_szText, _T("\t"));
			else
				wcscpy(pItem->m_szText, _T("\t"));
			wcscat(pItem->m_szText, szAccel);
		}
	}

	return AppendMenu(nFlags, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CMenuEx::AddSeparator(void)
{
	m_bBreak = m_bBreakBar = FALSE;
	CMenuExSeparator *pItem = new CMenuExSeparator();
	return AppendMenu(MF_OWNERDRAW | MF_SEPARATOR, 0, (LPCTSTR)pItem);
}

BOOL CMenuEx::AddPopup(UINT nFlags, CMenuEx *pPopup, CMenuExItem *pItem)
{
	ASSERT(pPopup);
	ASSERT(pItem);

	nFlags |= MF_OWNERDRAW;
	nFlags |= MF_POPUP;
	if(m_bBreak) 
		nFlags |= MF_MENUBREAK;
	if(m_bBreakBar)
		nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;

	return AppendMenu(nFlags, (UINT)pPopup->m_hMenu, (LPCTSTR)pItem);
}

void CMenuEx::Break(void)
{
	m_bBreak = TRUE;
}

void CMenuEx::BreakBar(void)	
{
	m_bBreakBar = TRUE;
}

void CMenuEx::SetBackBitmap(HBITMAP hBitmap)
{
	if(hBitmap == NULL && m_hBitmap)
	{
		::SelectObject(m_hMemDC, m_hOldBitmap);
		::DeleteDC(m_hMemDC);
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
		return;
	}
	m_hBitmap = hBitmap;
	if(!m_hMemDC)
	{
		HDC hDC = GetWindowDC(NULL);
		m_hMemDC = ::CreateCompatibleDC(hDC);
		::ReleaseDC(NULL, hDC);
	}
	ASSERT(m_hMemDC);
	m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemDC, m_hBitmap);
}

void CMenuEx::SetSepBitmap(HBITMAP hBitmap)
{
	if(hBitmap == NULL && m_hSepBitmap)
	{
		::SelectObject(m_hSepMemDC, m_hOldSepBitmap);
		::DeleteDC(m_hSepMemDC);
		::DeleteObject(m_hSepBitmap);
		m_hSepBitmap = NULL;
		return;
	}
	m_hSepBitmap = hBitmap;
	if(!m_hSepMemDC)
	{
		HDC hDC = GetWindowDC(NULL);
		m_hSepMemDC = ::CreateCompatibleDC(hDC);
		::ReleaseDC(NULL, hDC);
	}
	ASSERT(m_hSepMemDC);
	m_hOldSepBitmap = (HBITMAP)::SelectObject(m_hSepMemDC, m_hSepBitmap);
}

BOOL CMenuEx::SetFont(LOGFONT lgfont)
{
	if(m_hFont)
		DeleteObject(m_hFont);
	if(m_hVBoldFont)
		DeleteObject(m_hVBoldFont);
	if(m_hHBoldFont)
		DeleteObject(m_hHBoldFont);
	m_hFont = ::CreateFontIndirect(&lgfont);

	lgfont.lfWidth = 0;
	lgfont.lfWeight = FW_BOLD;
	m_hHBoldFont = ::CreateFontIndirect(&lgfont);

	lgfont.lfWidth = 0;
	lgfont.lfEscapement = 900;
	lgfont.lfOrientation = 900;
	lgfont.lfWeight = FW_BOLD;
	m_hVBoldFont = ::CreateFontIndirect(&lgfont);
	return (m_hFont!=NULL);
}

void CMenuEx::DrawBackGround(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled)
{
	if(m_hBitmap && (!bSelected || bDisabled))
	{
		BOOL bRet = ::BitBlt(hDC, rtPaint.left, rtPaint.top, rtPaint.right - rtPaint.left, rtPaint.bottom - rtPaint.top, m_hMemDC, 0, rtPaint.top, SRCCOPY);
		ASSERT(bRet!=FALSE);
	}
	else if(bSelected)
	{
		FillRect(hDC, &rtPaint, bDisabled? ((m_Style==STYLE_XP)?m_clrBackGround:m_clrSelectedBar) : m_clrSelectedBar);
	}
	else
	{
		FillRect(hDC, &rtPaint, m_clrBackGround);
	}

	//in XP mode, draw a line rectangle around
	if(m_Style == STYLE_XP && bSelected && !bDisabled)
	{
		return; // No Line Frame around 
		HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
		HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
		::Rectangle(hDC, rtPaint.left, rtPaint.top, rtPaint.right, rtPaint.bottom);
		::SelectObject(hDC, hOldBrush);
		::SelectObject(hDC, hOldPen);
	}
}

void CMenuEx::DrawButton(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	if(m_Style == STYLE_OFFICE)
	{
		// normal: fill BG depending on state
		if(bChecked && !bSelected)
			FillRect(hDC, &rtPaint, GetSysColor(COLOR_3DHILIGHT));
		//else
		//	FillRect(hDC, &rtPaint, m_clrBackGround);

		// draw pushed-in or popped-out edge
		if(!bDisabled && (bSelected || bChecked) )
			DrawEdge(hDC, &rtPaint, bChecked ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);
	}
	else if(m_Style == STYLE_XP && !bSelected)
	{
		if(bChecked && !bDisabled)
			DrawBackGround(hDC, rtPaint, TRUE, FALSE);
	}
}

void CMenuEx::DrawIcon(HDC hDC, RECT rtPaint, HICON hIcon, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	if(bDisabled)
	{
		DrawEmbossed(hDC, hIcon, rtPaint);
	}
	else
	{
		if(m_Style==STYLE_XP && bSelected && !bChecked)
		{
			DrawEmbossed(hDC, hIcon, rtPaint, FALSE, TRUE);
			OffsetRect(&rtPaint, -2, -2);
		}
		::DrawIconEx(hDC, rtPaint.left, rtPaint.top, hIcon, rtPaint.right-rtPaint.left, rtPaint.bottom-rtPaint.top, 0, NULL, DI_NORMAL);
	}
}

void CMenuEx::DrawTitle(HDC hDC, RECT rtPaint, HICON hIcon, LPCWSTR pszText)
{
	HBITMAP	bmpTitle = CreateGradientBMP(hDC, m_clrTitleStart, m_clrTitleEnd,
		rtPaint.right-rtPaint.left, rtPaint.bottom-rtPaint.top, 0, 256);
	if(bmpTitle)
	{
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, bmpTitle);
		::BitBlt(hDC, rtPaint.left, rtPaint.top, rtPaint.right-rtPaint.left, rtPaint.bottom-rtPaint.top, hMemDC, 0, 0, SRCCOPY);
		::SelectObject(hMemDC, hOldBmp);
		::DeleteObject(bmpTitle);
	}
	//Draw Title text
	HFONT hOldFont = (HFONT)SelectObject(hDC, m_hHBoldFont);
	COLORREF oldColor = GetTextColor(hDC);
	::SetTextColor(hDC, RGB(255, 255, 255));
	::SetBkMode(hDC, TRANSPARENT);
	::TextOut(hDC, rtPaint.left+2, rtPaint.top+2, pszText, wcslen(pszText));
	::SetTextColor(hDC, oldColor);	
	::SelectObject(hDC, hOldFont);
}

void CMenuEx::DrawSideBar(HDC hDC, RECT rtPaint, HICON hIcon, LPCWSTR pszText)
{
	rtPaint.right += 3;	//fill the gap produced by the menubreak
	HBITMAP	bmpBar = CreateGradientBMP(hDC, m_clrSideBarStart, m_clrSideBarEnd,
		rtPaint.right-rtPaint.left, rtPaint.bottom-rtPaint.top, 0, 256);
	if(bmpBar)
	{
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, bmpBar);
		::BitBlt(hDC, rtPaint.left, rtPaint.top, rtPaint.right-rtPaint.left, rtPaint.bottom-rtPaint.top, hMemDC, 0, 0, SRCCOPY);
		::SelectObject(hMemDC, hOldBmp);
		::DeleteObject(bmpBar);
	}
	//Draw Sidebar text
	HFONT hOldFont = (HFONT)SelectObject(hDC, m_hVBoldFont);
	COLORREF oldColor = GetTextColor(hDC);
	::SetTextColor(hDC, RGB(255, 255, 255));
	::SetBkMode(hDC, TRANSPARENT);
	::TextOut(hDC, rtPaint.left+2, rtPaint.bottom-4, pszText, wcslen(pszText));
	::SetTextColor(hDC, oldColor);	
	::SelectObject(hDC, hOldFont);
}

void CMenuEx::DrawText(HDC hDC, RECT rtPaint, LPCWSTR pszText, BOOL bSelected, BOOL bDisabled, BOOL bBold)
{
	HFONT hOldFont = NULL;
	HFONT hBoldFont = NULL;
	if(bBold)
	{
		LOGFONT	logFont;
		::GetObject(m_hFont, sizeof(LOGFONT), (LPVOID)&logFont);
		logFont.lfWeight = FW_BOLD;
		hBoldFont = ::CreateFontIndirect(&logFont);
		hOldFont = (HFONT)::SelectObject(hDC, hBoldFont);
	}
	else
	{
		hOldFont = (HFONT)::SelectObject(hDC, m_hFont);
	}

	::SetBkMode(hDC, TRANSPARENT);
	if(bDisabled && (!bSelected || m_Style == STYLE_XP))
	{
		::OffsetRect(&rtPaint, 1, 1);
		DrawItemText(hDC, rtPaint, pszText, m_clrSelectedText);
	}
	if(bDisabled)
	{
		DrawItemText(hDC, rtPaint, pszText, m_clrDisabledText);
	}
	else
	{
		DrawItemText(hDC, rtPaint, pszText, bSelected ? m_clrSelectedText : m_clrText);
	}
	::SelectObject(hDC, hOldFont);

	if(bBold)
		DeleteObject(hBoldFont);
}

void CMenuEx::DrawCheckMark(HDC hDC, RECT rtPaint, BOOL bSelected)
{
	const int nCheckDots = 8;
	POINT pt1, pt2, pt3; // 3 point of the checkmark
	pt1.x = 0;	// 5/18 of the rect width
	pt1.y = 3;	
	pt2.x = 2;
	pt2.y = 5;
	pt3.x = 7;
	pt3.y = 0;

	int xOff = (rtPaint.right-rtPaint.left-nCheckDots)/2 + rtPaint.left ;
	int yOff = (rtPaint.bottom-rtPaint.top-nCheckDots)/2 + rtPaint.top;
	pt1.x += xOff; pt1.y += yOff;
	pt2.x += xOff; pt2.y += yOff;
	pt3.x += xOff; pt3.y += yOff;

	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::MoveToEx(hDC, pt1.x, pt1.y, NULL);
	::LineTo(hDC, pt2.x, pt2.y);
	::LineTo(hDC, pt3.x, pt3.y);
	pt1.x += 0; pt1.y += 1;
	pt2.x += 0; pt2.y += 1;
	pt3.x += 0; pt3.y += 1;
	::MoveToEx(hDC, pt1.x, pt1.y, NULL);
	::LineTo(hDC, pt2.x, pt2.y);
	::LineTo(hDC, pt3.x, pt3.y);
	pt1.x += 0; pt1.y += 1;
	pt2.x += 0; pt2.y += 1;
	pt3.x += 0; pt3.y += 1;
	::MoveToEx(hDC, pt1.x, pt1.y, NULL);
	::LineTo(hDC, pt2.x, pt2.y);
	::LineTo(hDC, pt3.x, pt3.y);
	::SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void CMenuEx::DrawItemText(HDC hDC, RECT rtPaint, LPCWSTR pszText, COLORREF color)
{
	wchar_t szText[1024] = {0};
	wcscpy(szText, pszText);
	wchar_t szLText[256] = {0}, szRText[256] = {0};
	wchar_t *pszDest = wcschr(szText, L'\t');
	int iTabPos = -1;
	if(pszDest)
		iTabPos = pszDest - szText + 1;
	else
		wcscpy(szLText, szText);
	if(iTabPos>=0)
	{
		wcsncpy(szRText, &szText[iTabPos], wcslen(szText) - iTabPos);
		wcsncpy(szLText, szText, iTabPos);
	}
	::SetTextColor(hDC, color);
	if(wcslen(szLText))
		::DrawText(hDC, szLText, wcslen(szLText), &rtPaint, MENUEX_TEXT_FLAG);
	if(wcslen(szRText))
	{
		::DrawText(hDC, szRText, wcslen(szRText), &rtPaint, MENUEX_TEXT_FLAG | DT_RIGHT);
	}
}

void CMenuEx::DrawIconArea(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled, BOOL bChecked)
{
	return; // No Icon's Area Background
	if(m_Style != STYLE_XP)
		return;
	// normal: fill BG depending on state
	if(!bSelected || bDisabled)
		FillRect(hDC, &rtPaint, m_clrIconArea);
}

void CMenuEx::DrawBorder(HDC hDC, RECT rtPaint)
{

}

HBITMAP CMenuEx::CreateGradientBMP(HDC hDC, COLORREF cl1, COLORREF cl2, int nWidth, int nHeight, int nDir, int nNumColors)
{
	if(nNumColors > 256)
		nNumColors = 256;

	int nIndex;
	COLORREF PalVal[256];
	memset(PalVal, 0, sizeof(COLORREF)*256);
	BYTE peRed=0, peGreen=0, peBlue=0;

	int r1 = GetRValue(cl1);
	int r2 = GetRValue(cl2);
	int g1 = GetGValue(cl1);
	int g2 = GetGValue(cl2);
	int b1 = GetBValue(cl1);
	int b2 = GetBValue(cl2);
	for (nIndex = 0; nIndex < nNumColors; nIndex++)
	{
		peRed = (BYTE)(r1 + MulDiv((r2 - r1), nIndex, nNumColors-1));
		peGreen = (BYTE)(g1 + MulDiv((g2 - g1), nIndex, nNumColors-1));
		peBlue = (BYTE)(b1 + MulDiv((b2 - b1), nIndex, nNumColors-1));
		PalVal[nIndex] = (peRed << 16) | (peGreen << 8) | (peBlue);
	}

	LPDWORD pGradBits;
	BITMAPINFO GradBitInfo;
	pGradBits = (DWORD*)malloc(nWidth*nHeight*sizeof(DWORD));
	memset(&GradBitInfo, 0, sizeof(BITMAPINFO));
	GradBitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GradBitInfo.bmiHeader.biWidth = nWidth;
	GradBitInfo.bmiHeader.biHeight = nHeight;
	GradBitInfo.bmiHeader.biPlanes = 1;
	GradBitInfo.bmiHeader.biBitCount = 32;
	GradBitInfo.bmiHeader.biCompression = BI_RGB;

	if(nDir==0) 
	{
		for(int y=0; y<nHeight; y++) 
		{
			for(int x=0; x<nWidth; x++)
			{
				*(pGradBits + (y*nWidth) + x) = PalVal[MulDiv(nNumColors, y, nHeight)];
			}
		}
	}
	else if(nDir==1) 
	{
		for(int y=0; y<nHeight; y++)
		{
			int l, r;
			l = MulDiv((nNumColors/2), y, nHeight);
			r = l + (nNumColors/2) - 1;
			for(int x=0; x<nWidth; x++)
			{
				*(pGradBits + (y*nWidth) + x) = PalVal[l + MulDiv((r-l), x, nWidth)];
			}
		}
	}
	else if(nDir==2)
	{
		for(int x=0; x<nWidth; x++)
		{
			for(int y=0; y<nHeight; y++)
			{
				*(pGradBits + (y*nWidth) +x) = PalVal[MulDiv(nNumColors, x, nWidth)];
			}
		}
	}
	else if(nDir==3)
	{
		for(int y=0; y<nHeight; y++)
		{
			int l, r;
			r = MulDiv((nNumColors/2), y, nHeight);
			l = r + (nNumColors/2)-1;
			for(int x=0; x<nWidth; x++)
			{
				*(pGradBits + (y*nWidth) + x) = PalVal[l + MulDiv((r-l), x, nWidth)];
			}
		}
	}
	HBITMAP hBitmap = CreateDIBitmap(hDC, &GradBitInfo.bmiHeader, CBM_INIT, pGradBits, &GradBitInfo, DIB_RGB_COLORS);
	free(pGradBits);
	return hBitmap;
}

void CMenuEx::DrawEmbossed(HDC hDC, HICON hIcon, RECT rtPaint, BOOL bColor /*= FALSE*/, BOOL bShadow /*= FALSE*/)
{
	HDC	hMemDC = CreateCompatibleDC(hDC);
	int nWidth = rtPaint.right - rtPaint.left;
	int nHeight = rtPaint.bottom - rtPaint.top;

	// create mono or color bitmap
	HBITMAP hBitmap = NULL;
	if(bColor)
		hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	else
		hBitmap = ::CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// draw image into memory DC--fill BG white first
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
	//FillRect(&hMemDC, CRect(0, 0, cx, cy), m_clrBackGround);
	PatBlt(hMemDC, 0, 0, nWidth, nHeight, WHITENESS);
	::DrawIconEx(hMemDC, 0, 0, hIcon, nWidth, nHeight, 1, NULL, DI_NORMAL);

	// This seems to be required. Why, I don't know. ???
	COLORREF colorOldBG = ::SetBkColor(hDC, RGB(255,255,255));

	// Draw using high light offset by (1,1), then shadow
	HBRUSH hbrShadow = ::CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
	HBRUSH hbrHilite = ::CreateSolidBrush(GetSysColor(COLOR_3DHIGHLIGHT));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, bShadow ? hbrShadow : hbrHilite);
	::BitBlt(hDC, rtPaint.left+1, rtPaint.top+1, nWidth, nHeight, hMemDC, 0, 0, 0xb8074a);
	::SelectObject(hDC, hbrShadow);
	::BitBlt(hDC, rtPaint.left, rtPaint.top, nWidth, nHeight, hMemDC, 0, 0, 0xb8074a);
	::SelectObject(hDC, hOldBrush);
	::SetBkColor(hDC, colorOldBG); // restore
	::SelectObject(hMemDC, hOldBitmap); // ...
	::DeleteObject(hbrShadow);
	::DeleteObject(hbrHilite);
	::DeleteObject(hBitmap);
	::DeleteDC(hMemDC);
}

void CMenuEx::FillRect(HDC hDC, LPRECT pRect, COLORREF color)
{
	HBRUSH brush = ::CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, brush);
	::PatBlt(hDC, pRect->left, pRect->top, pRect->right-pRect->left, pRect->bottom-pRect->top, PATCOPY);
	SelectObject(hDC, hOldBrush);
	DeleteObject(brush);
}

void CMenuEx::Clear(void)
{
	if(m_hMemDC)
	{
		::SelectObject(m_hMemDC, m_hOldBitmap);
		::DeleteDC(m_hMemDC);
	}
	if(m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	if(m_hSepMemDC)
	{
		::SelectObject(m_hSepMemDC, m_hOldSepBitmap);
		::DeleteDC(m_hSepMemDC);
	}
	if(m_hSepBitmap)
	{
		::DeleteObject(m_hSepBitmap);
		m_hSepBitmap = NULL;
	}
	if(m_hFont)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if(m_hHBoldFont)
	{
		::DeleteObject(m_hHBoldFont);
		m_hHBoldFont = NULL;
	}
	if(m_hVBoldFont)
	{
		::DeleteObject(m_hVBoldFont);
		m_hVBoldFont = NULL;
	}
	if(m_hMenu)
	{
		UINT nCount = GetMenuItemCount();
		for (UINT i=0; i<nCount; i++)
		{
			MENUITEMINFO	info;
			memset(&info, 0, sizeof(MENUITEMINFO));
			info.cbSize = sizeof(MENUITEMINFO);
			info.fMask = MIIM_DATA | MIIM_TYPE;
			GetMenuItemInfo(i, &info, TRUE);

			CMenuExItem *pData = (CMenuExItem *)info.dwItemData;
			if((info.fType & MFT_OWNERDRAW) && pData && pData->IsMyData())
				delete pData;

			CMenuEx *pSubMenu = GetSubMenu(i);
			if(pSubMenu)
				delete pSubMenu;
		}
	}
}

CMenuEx* PASCAL CMenuEx::FromHandle(HMENU hMenu)
{
	CMenuEx *pMenu = NULL;
	std::map<HMENU, CMenuEx*>::iterator iter;
	iter = s_mapMenuEx.find(hMenu);
	if(iter!=s_mapMenuEx.end())
		pMenu = iter->second;
	return pMenu;
}

HMENU CMenuEx::GetLastMenu()
{
	if(s_hLastMenu.size()==0)
		return NULL;
	return s_hLastMenu[0];
}

BOOL CMenuEx::IsMenuHwnd(HWND hWnd)
{
	if(!hWnd)
		return FALSE;
	wchar_t szWndClass[128];
	::GetClassName(hWnd, szWndClass, _countof(szWndClass)-1);
	return (wcsicmp(L"#32768", szWndClass) == 0);
}

BOOL CMenuEx::InitHook(HWND hWnd)
{
	BOOL bRet = FALSE;
	CMenuEx *pMenu = GetMenuEx(hWnd);
	if(pMenu)
		return TRUE; // already done
	pMenu = new CMenuEx();
	pMenu->m_hWnd = hWnd;
	s_mapMenuExHwnd.insert(pair<HWND, CMenuEx*>(hWnd, pMenu));
	pMenu->m_pOldWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)CMenuEx::HookWndProc);
	return TRUE;
}

void CMenuEx::UninitHook(HWND hWnd)
{
	std::map<HWND, CMenuEx*>::iterator iter;
	iter = s_mapMenuExHwnd.find(hWnd);
	if(iter!=s_mapMenuExHwnd.end())
	{
		CMenuEx *pMenu = (*iter).second;
		s_mapMenuExHwnd.erase(iter);
		if(pMenu->m_pOldWndProc)
			SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)pMenu->m_pOldWndProc);
		delete pMenu;
	}
}

CMenuEx* CMenuEx::GetMenuEx(HWND hWnd)
{
	CMenuEx *pMenu = NULL;
	std::map<HWND, CMenuEx*>::iterator iter;
	iter = s_mapMenuExHwnd.find(hWnd);
	if(iter!=s_mapMenuExHwnd.end())
		pMenu = iter->second;
	return pMenu;
}

BOOL CMenuEx::DoSysMenu(HWND hWnd, POINT ptCursor, LPRECT prExclude /*= NULL*/, BOOL bCopy /*= FALSE*/)
{
	BOOL bRet = FALSE;
	HMENU hMenu = ::GetSystemMenu(hWnd, FALSE);
	if(!hMenu)
		return bRet;
	TPMPARAMS tpmp;
	tpmp.cbSize = sizeof(tpmp);
	if(prExclude)
		tpmp.rcExclude = *prExclude;
	else
		SetRectEmpty(&tpmp.rcExclude);
	UINT uAlignFlags = TPM_LEFTALIGN | TPM_TOPALIGN | TPM_VERTICAL | TPM_RIGHTBUTTON | TPM_RETURNCMD;
	UINT uID = 0;
	return bRet;
}

LRESULT CALLBACK CMenuEx::HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMenuEx *pMenu = GetMenuEx(hWnd);
	if(!pMenu)
		return 0;
	WNDPROC oldWndProc = pMenu->m_pOldWndProc;
	if(!oldWndProc)
		return 0;
	switch(uMsg)
	{
	case WM_NCPAINT:
		{
			if(pMenu->OnNcPaint(hWnd, (HRGN)wParam))
				return 0;
		}
		break;
	case WM_PRINT:
		{
			pMenu->OnPrint(hWnd, (HDC)wParam, lParam);
		}
		break;
	case WM_PRINTCLIENT:
		{
			pMenu->OnPrintClient(hWnd, (HDC)wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			if(pMenu->OnPaint(hWnd))
				return 0;
		}
		break;
	case WM_KEYDOWN:
		{
			pMenu->OnKeyDown((UINT)wParam, (UINT)GET_X_LPARAM(lParam), (UINT)GET_Y_LPARAM(lParam));
		}
		break;
	case 0x1e5:
		{

		}
		break;
	case WM_NCCALCSIZE:
		{
			LRESULT lRes = CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
			pMenu->OnNcCalcSize((BOOL)wParam, (LPNCCALCSIZE_PARAMS)lParam);
			return lRes;
		}
		break;
	case WM_WINDOWPOSCHANGING:
		{
			pMenu->OnWindowPosChanging((WINDOWPOS*)lParam);
		}
		break;
	case WM_ERASEBKGND:
		{
			return pMenu->OnEraseBkgnd((HDC)wParam);
		}
		break;
	case WM_NCDESTROY:
		{
			pMenu->OnNcDestroy();
		}
		break;
	}
	//return 0;
	return CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
}
#pragma warning(pop)
