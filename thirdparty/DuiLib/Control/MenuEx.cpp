#include "stdafx.h"
#include "MenuEx.h"

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

namespace DuiLib {

CMenuUI *CMenuUI::s_pLastMenu;
CStdStringPtrMap CMenuUI::s_mapMenu;

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
		assert (FALSE);
		return;
	}
	wcscpy(pszVirtKey, L"");
	if(m_lpAccel->fVirt & FCONTROL)
		wcscat(pszVirtKey, L"Ctrl+");
	if(m_lpAccel->fVirt & FSHIFT)
		wcscat(pszVirtKey, L"Shift+");
	if(m_lpAccel->fVirt & FALT)
		wcscat(pszVirtKey, L"Atl+");
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
	wchar_t szBuffer[256];
	UINT nScanCode = ::MapVirtualKeyEx (uiVirtKey, 0, ::GetKeyboardLayout(0)) <<16 | 0x1;
	if(uiVirtKey >= VK_PRIOR && uiVirtKey <= VK_HELP)
	{
		nScanCode |= 0x01000000;
	}
	::GetKeyNameText(nScanCode, szBuffer, _countof(szBuffer));
	for(int i=0; i<(int)wcslen(szBuffer); i++)
		szBuffer[i] = (wchar_t)tolower(szBuffer[i]);

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
CMenuItem::CMenuItem()
{
	m_dwMagicNum = MENUEX_MAGIC_NUM;
	m_dwID = 0;
	m_bTitle = FALSE;
	m_bSeparator = FALSE;
	m_bSideBar = FALSE;
	m_bButton = FALSE;
	m_nSize = 16;
	m_fVirt = 0;
	m_wkey = 0;
}

CMenuItem::~CMenuItem()
{
}

LPCTSTR CMenuItem::GetClass() const
{
	return DUI_CTR_MENUITEM;
}

BOOL CMenuItem::IsMyData(void)
{
	return m_dwMagicNum == MENUEX_MAGIC_NUM;
}

void CMenuItem::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("key")) == 0 )
	{
		LPTSTR pstr = NULL;
		m_wkey = _tcstoul(pstrValue, &pstr, 16);
		m_fVirt |= FVIRTKEY;
	}
	else if( _tcscmp(pstrName, _T("mid")) == 0 )
	{
		LPTSTR pstr = NULL;
		m_dwID = _tcstoul(pstrValue, &pstr, 10);
	}
	else if( _tcscmp(pstrName, _T("vctrl")) == 0 )
	{
		if(_tcscmp(pstrValue, _T("true")) == 0)
			m_fVirt |= FCONTROL;
	}
	else if( _tcscmp(pstrName, _T("vatl")) == 0 )
	{
		if(_tcscmp(pstrValue, _T("true")) == 0)
			m_fVirt |= FALT;
	}
	else if( _tcscmp(pstrName, _T("vshift")) == 0 )
	{
		if(_tcscmp(pstrValue, _T("true")) == 0)
			m_fVirt |= FSHIFT;
	}
	CControlUI::SetAttribute(pstrName, pstrValue);
}

//////////////////////////////////////////////////////////////////////////
//
CMenuText::CMenuText()
{

}

CMenuText::CMenuText(DWORD dwID, LPCTSTR pszText, LPCWSTR pszImage /*= NULL*/)
{
	m_dwID = dwID;
	m_sText = pszText;
	m_sBkImage = pszImage;
}

void CMenuText::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CMenuItem::SetAttribute(pstrName, pstrValue);
}

//////////////////////////////////////////////////////////////////////////
//
CMenuTitle::CMenuTitle(int nSize /*= 32*/, LPCTSTR pszText /*= NULL*/, DWORD dwID /*= 0*/)
{
	m_bTitle = TRUE;
	m_sText = pszText;
	m_nSize = nSize;
	m_dwID = dwID;
}

void CMenuTitle::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CMenuItem::SetAttribute(pstrName, pstrValue);
}

//////////////////////////////////////////////////////////////////////////
//
CMenuSeparator::CMenuSeparator()
{
	m_bSeparator = TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
CMenuSideBar::CMenuSideBar(int nSize /*= 32*/, LPCTSTR pszText /*= NULL*/, DWORD dwID /*= 1*/)
{
	m_bSideBar = TRUE;
	m_sText = pszText;
	m_nSize = nSize;
	m_dwID = dwID;
}

void CMenuSideBar::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CMenuItem::SetAttribute(pstrName, pstrValue);
}

//////////////////////////////////////////////////////////////////////////
//
CMenuButton::CMenuButton()
{
	m_bButton = TRUE;
}

CMenuButton::CMenuButton(DWORD dwID, LPCWSTR pszImage)
{
	m_dwID = dwID;
	m_bButton = TRUE;
	m_sBkImage = pszImage;
}

void CMenuButton::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CMenuItem::SetAttribute(pstrName, pstrValue);
}

//////////////////////////////////////////////////////////////////////////
//
CMenuUI::CMenuUI()
	: m_hMenu(NULL)
	, m_hParent(NULL)
	, m_dwNormalColor(0)
	, m_dwNormalColor2(0)
	, m_dwNormalColor3(0)
	, m_dwSelectColor(0)
	, m_dwSelectColor2(0)
	, m_dwSelectColor3(0)
	, m_dwTitleColor(0)
	, m_dwTitleColor2(0)
	, m_dwTitleColor3(0)
	, m_dwSideBarColor(0)
	, m_dwSideBarColor2(0)
	, m_dwSideBarColor3(0)
	, m_dwSeparatorColor(0)
	, m_iTextFont(-1)
	, m_iTitleFont(-1)
	, m_iSideBarFont(-1)
	, m_uTextStyle(DT_SINGLELINE | DT_VCENTER | DT_LEFT)
{
	//initialize colors with system default
	m_dwSelectedTextColor = 0xFF0F396E;
	m_dwNormalTextColor = 0xFF0F396E;
	m_dwDisabledTextColor = 0xFFA0A0A0;

	//initialize Title colors
	m_dwTitleColor = 0xFF0F396E;

	//initialize sidebar colors
	m_dwSideBarColor = 0xFF0F396E;

	m_bPopup = FALSE;
	m_bBreak = FALSE;
	m_bBreakBar = FALSE;
	m_bSubMenu = FALSE;

	m_nWidth = 0;
	m_nHeight = 0;
	
	m_nTitleHeight = 0;
	m_nSideBarWidth = 0;
}

CMenuUI::~CMenuUI()
{
	Clear();
	DestroyMenu();
}

LPCTSTR CMenuUI::GetClass() const
{
	return DUI_CTR_MENU;
}

BOOL CMenuUI::CreateMenu(BOOL bRoot /*= TRUE*/)
{
	m_hMenu = ::CreateMenu();
	if(m_hMenu && bRoot)
	{
		s_pLastMenu = this;
	}
	return (m_hMenu!=NULL);
}

BOOL CMenuUI::CreatePopupMenu(BOOL bRoot /*= TRUE*/)
{
	m_hMenu = ::CreatePopupMenu();
	if(m_hMenu && bRoot)
		s_pLastMenu = this;
	return (m_hMenu!=NULL);
}

BOOL CMenuUI::DestroyMenu()
{
	if(!m_hMenu)
		return FALSE;
	::DestroyMenu(m_hMenu);
	m_hMenu = NULL;
	return TRUE;
}

CMenuUI::operator HMENU() const
{
	assert(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
	return this == NULL ? NULL : m_hMenu;
}

BOOL CMenuUI::DeleteMenu(UINT nPosition, UINT nFlags)
{
	assert(::IsMenu(m_hMenu));
	return ::DeleteMenu(m_hMenu, nPosition, nFlags);
}

BOOL CMenuUI::TrackPopupMenu(UINT nFlags, int x, int y, HWND hWnd, LPCRECT lpRect /*= 0*/)
{
	return ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, hWnd, lpRect);
}

BOOL CMenuUI::TrackPopupMenuEx(UINT fuFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm)
{
	return ::TrackPopupMenuEx(m_hMenu, fuFlags, x, y, hWnd, lptpm);
}

BOOL CMenuUI::operator==(const CMenuUI& menu) const
{
	return ((HMENU) menu) == m_hMenu;
}

BOOL CMenuUI::operator!=(const CMenuUI& menu) const
{
	return ((HMENU) menu) != m_hMenu;
}

BOOL CMenuUI::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
{
	assert(::IsMenu(m_hMenu));
	return ::AppendMenu(m_hMenu, nFlags, nIDNewItem, lpszNewItem);
}

BOOL CMenuUI::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
{
	assert(::IsMenu(m_hMenu)); 
	return ::AppendMenu(m_hMenu, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)hBmp);
}

UINT CMenuUI::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
{
	assert(::IsMenu(m_hMenu));
	return (UINT)::CheckMenuItem(m_hMenu, nIDCheckItem, nCheck);
}

UINT CMenuUI::EnableMenuItem(UINT nIDEnableItem, UINT nEnable)
{
	assert(::IsMenu(m_hMenu));
	return ::EnableMenuItem(m_hMenu, nIDEnableItem, nEnable);
}

UINT CMenuUI::GetMenuItemCount() const
{
	assert(::IsMenu(m_hMenu));
	return ::GetMenuItemCount(m_hMenu);
}

UINT CMenuUI::GetMenuItemID(int nPos) const
{
	assert(::IsMenu(m_hMenu));
	return ::GetMenuItemID(m_hMenu, nPos);
}

UINT CMenuUI::GetMenuState(UINT nID, UINT nFlags) const
{
	assert(::IsMenu(m_hMenu));
	return ::GetMenuState(m_hMenu, nID, nFlags);
}

int CMenuUI::GetMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags) const
{
	assert(::IsMenu(m_hMenu));
	return ::GetMenuString(m_hMenu, nIDItem, lpString, nMaxCount, nFlags);
}

BOOL CMenuUI::GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
{
	assert(::IsMenu(m_hMenu));
	assert(lpMenuItemInfo!=NULL);
	return ::GetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
}

BOOL CMenuUI::SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
{
	assert(::IsMenu(m_hMenu));
	assert(lpMenuItemInfo!=NULL);
	return ::SetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
}

BOOL CMenuUI::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
{
	assert(::IsMenu(m_hMenu));
	return ::InsertMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}

BOOL CMenuUI::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
{
	assert(::IsMenu(m_hMenu));
	return ::InsertMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)hBmp);
}

BOOL CMenuUI::InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
{
	assert(::IsMenu(m_hMenu));
	assert(lpMenuItemInfo!=NULL);
	return ::InsertMenuItem(m_hMenu, uItem, fByPos, lpMenuItemInfo);
}

BOOL CMenuUI::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
{
	assert(::IsMenu(m_hMenu));
	return ::ModifyMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
}

BOOL CMenuUI::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
{
	assert(::IsMenu(m_hMenu));
	return ::ModifyMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)hBmp);
}

BOOL CMenuUI::RemoveMenu(UINT nPosition, UINT nFlags)
{
	assert(::IsMenu(m_hMenu));
	return ::RemoveMenu(m_hMenu, nPosition, nFlags);
}

BOOL CMenuUI::SetMenuItemBitmaps(UINT nPosition, UINT nFlags, const HBITMAP hBmpUnchecked, const HBITMAP hBmpChecked)
{
	assert(::IsMenu(m_hMenu));
	return ::SetMenuItemBitmaps(m_hMenu, nPosition, nFlags, hBmpUnchecked, hBmpChecked);
}

BOOL CMenuUI::CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags)
{
	return ::CheckMenuRadioItem(m_hMenu, nIDFirst, nIDLast, nIDItem, nFlags);
}

BOOL CMenuUI::SetDefaultItem(UINT uItem, BOOL fByPos /*= FALSE*/)
{
	assert(::IsMenu(m_hMenu));
	return ::SetMenuDefaultItem(m_hMenu, uItem, fByPos);
}

UINT CMenuUI::GetDefaultItem(UINT gmdiFlags, BOOL fByPos /*= FALSE*/)
{
	assert(::IsMenu(m_hMenu));
	return ::GetMenuDefaultItem(m_hMenu, fByPos, gmdiFlags);
}

void CMenuUI::DrawItem(LPDRAWITEMSTRUCT pdis)
{
	assert(pdis);
	if(pdis->CtlType != ODT_MENU)
		return; // not handled by me
	CMenuItem *pItem = (CMenuItem *)pdis->itemData;
	if(!pItem)
		return;

	assert(pdis->itemAction != ODA_FOCUS);
	assert(pdis->hDC);
	HDC hDC = pdis->hDC;
	RECT rcClipBox;
	::GetClipBox(hDC, &rcClipBox);
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = NULL, hOldBitmap = NULL;
	hBitmap = CreateCompatibleBitmap(hDC, 
		rcClipBox.right-rcClipBox.left, 
		rcClipBox.bottom-rcClipBox.top);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	if(pItem->m_bTitle)
	{
		//draw the Title
		RECT rcTitle;
		rcTitle.left = rcClipBox.left;
		rcTitle.top = pdis->rcItem.top;
		rcTitle.right = rcClipBox.right;
		rcTitle.bottom = pdis->rcItem.bottom;
		DrawTitle(hMemDC, rcTitle, pItem->GetText());
	}
	else if(pItem->m_bSideBar)
	{
		//draw the side bar
		RECT rcSideBar;
		rcSideBar.left = pdis->rcItem.left;
		rcSideBar.top = rcClipBox.top + m_nTitleHeight;
		rcSideBar.right = pdis->rcItem.right;
		rcSideBar.bottom = rcClipBox.bottom;
		DrawSideBar(hMemDC, rcSideBar, pItem->GetText());
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
		DrawBackGround(hMemDC, rcSeparator, FALSE, FALSE);
		// draw the background
		rcSeparator.top += (rcSeparator.bottom - rcSeparator.top) >> 1; // vertical center
		if(!m_strSeparatorImage.IsEmpty())
		{
			m_rcItem = rcSeparator;
			m_rcPaint = rcSeparator;
			if(!DrawImage(hMemDC, (LPCTSTR)m_strSeparatorImage))
				m_strSeparatorImage.Empty();
		}
		else
		{
			::DrawEdge(hMemDC, &rcSeparator, EDGE_ETCHED, BF_TOP); // draw separator line
		}
	} // end of 'else if(pItem->m_bSeparator)'
	else
	{
		BOOL bGrayed = (pdis->itemState & ODS_GRAYED) || (pdis->itemState & ODS_DISABLED);
		BOOL bSelected = pdis->itemState & ODS_SELECTED;
		BOOL bChecked  = pdis->itemState & ODS_CHECKED;

		//draw the background first
		DrawBackGround(hMemDC, pdis->rcItem, bSelected, bGrayed);

		//draw the icon
		RECT rcButton = { 
			pdis->rcItem.left + 1, 
			pdis->rcItem.top + 1, 
			pdis->rcItem.left + pdis->rcItem.bottom - pdis->rcItem.top - 1,
			pdis->rcItem.top + pdis->rcItem.bottom - pdis->rcItem.top - 1,
		};
		if(pItem->m_bButton)
			rcButton = pdis->rcItem;
		if(wcslen(pItem->GetBkImage()))
		{
			m_rcItem = rcButton;
			m_rcPaint = rcButton;
			if(!DrawImage(hMemDC, pItem->GetBkImage()))
				pItem->SetBkImage(_T(""));

			//if(!bGrayed && (bSelected || bChecked) )
			//	DrawEdge(hDC, &rcButton, bChecked ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);
		}

		if(!pItem->m_bButton)
		{
			RECT rcText =  { 
				pdis->rcItem.left, 
				pdis->rcItem.top, 
				pdis->rcItem.right,
				pdis->rcItem.bottom
			}; // start w/whole item
			rcText.left += rcButton.bottom-rcButton.top + CX_GAP + CX_TEXT_MARGIN; // left margin
			rcText.right -= pItem->m_nSize; // right margin
			DrawText(hMemDC, rcText, pItem->GetText(), bSelected, bGrayed);
		}
	}
	::BitBlt(hDC, 
		pdis->rcItem.left, 
		pdis->rcItem.top, 
		pdis->rcItem.right-pdis->rcItem.left, 
		pdis->rcItem.bottom-pdis->rcItem.top,
		hMemDC,
		pdis->rcItem.left, 
		pdis->rcItem.top,
		SRCCOPY);
	::SelectObject(hMemDC, hOldBitmap);
	::DeleteDC(hMemDC);
	::DeleteObject(hOldBitmap);
}

void CMenuUI::MeasureItem(LPMEASUREITEMSTRUCT pmis)
{
	if(pmis->CtlType != ODT_MENU)
		return;

	CMenuItem	*pItem = (CMenuItem *)pmis->itemData;
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
		pmis->itemHeight = 1;//::GetSystemMetrics(SM_CYMENUCHECK)>>1;
		pmis->itemWidth  = 0;
	}
	else
	{
		if(!m_pManager)
			return;
		//calculate the size needed to draw the text: use DrawText with DT_CALCRECT
		HDC hScreenDC = ::GetWindowDC(NULL);	// screen DC--I won't actually draw on it
		RECT rcText = { 0, 0, 0, 0 };
		HFONT hOldFont = (HFONT)::SelectObject(hScreenDC, m_pManager->GetFont(m_iTextFont));
		//hOldFont = ::SelectObject(hScreenDC, m_hFont);
		::DrawText(hScreenDC, pItem->GetText(), wcslen(pItem->GetText()), &rcText, MENUEX_TEXT_FLAG | DT_CALCRECT);
		::SelectObject(hScreenDC, hOldFont);

		// the height of the item should be the maximum of the text and the button
		pmis->itemHeight = max(rcText.bottom-rcText.top, pItem->m_nSize + (CY_BUTTON_MARGIN<<1));
		if(pItem->m_bButton)
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

void CMenuUI::Init()
{
	CControlUI *pControl = NULL;
	CMenuItem *pItem = NULL;
	CMenuUI *pSubMenu = NULL;
	if(m_bPopup)
		CreatePopupMenu(!m_bSubMenu);
	else
		CreateMenu(!m_bSubMenu);
	ACCEL accel;
	UINT uFlags = 0;
	for(int i=0; i<m_items.GetSize(); i++)
	{
		pControl = static_cast<CControlUI*>(m_items.GetAt(i));
		if(_tcscmp(pControl->GetClass(), DUI_CTR_MENU)==0)
			pSubMenu = static_cast<CMenuUI*>(pControl);
		else
			pItem = static_cast<CMenuItem*>(pControl);
		if(!pControl->IsEnabled())
			uFlags |= MF_DISABLED;
		if(pItem)
		{
			accel.key = pItem->m_wkey;
			accel.fVirt = pItem->m_fVirt;
			if(pItem->m_bSideBar)
				AddSideBar((CMenuSideBar *)pItem);
			else if(pItem->m_bTitle)
				AddTitle((CMenuTitle *)pItem);
			else if(pItem->m_bSeparator)
				AddSeparator((CMenuSeparator *)pItem);
			else
				AddMenu(uFlags, pItem, &accel);
			uFlags = 0;
			accel.key = 0;
			accel.fVirt = 0;
		}
		else if(pSubMenu)
		{
			pSubMenu->Init();
			AddPopup(uFlags, pSubMenu);
		}
		pSubMenu = NULL;
		pItem = NULL;
	}
}

BOOL CMenuUI::AddControl(CControlUI *pItem)
{
	if(!pItem)
		return FALSE;
	m_items.Add(pItem);
	return TRUE;
}

BOOL CMenuUI::AddTitle(CMenuTitle *pItem)
{
	assert(pItem);
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

BOOL CMenuUI::AddSideBar(CMenuSideBar *pItem)
{
	assert(pItem);
	m_bBreak = TRUE;
	m_bBreakBar = FALSE;
	m_nSideBarWidth = pItem->m_nSize;
	return AppendMenu(MF_OWNERDRAW | MF_SEPARATOR, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CMenuUI::AddMenu(UINT nFlags, CMenuItem *pItem, ACCEL *pAccel /*= 0*/)
{
	assert(pItem);
	nFlags |= MF_OWNERDRAW;
	if(m_bBreak) 
		nFlags |= MF_MENUBREAK;
	if(m_bBreakBar)
		nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;
	if(pAccel)
	{
		CMenuKeyHelper keyhelper(pAccel);
		wchar_t szText[256] = {0};
		wchar_t szAccel[256] = {0};
		wcscpy(szText, pItem->GetText());
		keyhelper.Format(szAccel, _countof(szAccel));
		if(wcslen(szAccel))
		{
			if(wcslen(szText))
				wcscat(szText, _T("\t"));
			else
				wcscpy(szText, _T("\t"));
			wcscat(szText, szAccel);
			pItem->SetText(szText);
		}
	}

	return AppendMenu(nFlags, pItem->m_dwID, (LPCTSTR)pItem);
}

BOOL CMenuUI::AddSeparator(CMenuSeparator *pItem /*= NULL*/)
{
	m_bBreak = m_bBreakBar = FALSE;
	if(!pItem)
		pItem = new CMenuSeparator();
	return AppendMenu(MF_OWNERDRAW | MF_SEPARATOR, 0, (LPCTSTR)pItem);
}

BOOL CMenuUI::AddPopup(UINT nFlags, CMenuUI *pPopup)
{
	assert(pPopup);
	nFlags |= MF_OWNERDRAW;
	nFlags |= MF_POPUP;
	if(m_bBreak) 
		nFlags |= MF_MENUBREAK;
	if(m_bBreakBar)
		nFlags |= MF_MENUBARBREAK;
	m_bBreak = m_bBreakBar = FALSE;
	CMenuItem *pItem = new CMenuItem;
	pItem->m_dwID = pPopup->m_dwID;
	pItem->SetText(pPopup->GetText());

	return AppendMenu(nFlags, (UINT)pPopup->m_hMenu, (LPCTSTR)pItem);
}

void CMenuUI::Break(void)
{
	m_bBreak = TRUE;
}

void CMenuUI::BreakBar(void)	
{
	m_bBreakBar = TRUE;
}

int CMenuUI::GetItemCount() const
{
	return m_items.GetSize();
}

DWORD CMenuUI::GetNormalColor() const
{
	return m_dwNormalColor;
}

void CMenuUI::SetNormalColor(DWORD dwColor)
{
	m_dwNormalColor = dwColor;
}

DWORD CMenuUI::GetNormalColor2() const
{
	return m_dwNormalColor2;
}

void CMenuUI::SetNormalColor2(DWORD dwColor)
{
	m_dwSelectColor2 = dwColor;
}

DWORD CMenuUI::GetNormalColor3() const
{
	return m_dwNormalColor3;
}

void CMenuUI::SetNormalColor3(DWORD dwColor)
{
	m_dwSelectColor3 = dwColor;
}

LPCTSTR CMenuUI::GetNormalImage()
{
	return m_strNormalImage.GetData();
}

void CMenuUI::SetNormalImage(LPCTSTR pStrImage)
{
	if( m_strNormalImage == pStrImage )
		return;
	m_strNormalImage = pStrImage;
}

DWORD CMenuUI::GetSelColor() const
{
	return m_dwSelectColor;
}

void CMenuUI::SetSelColor(DWORD dwColor)
{
	m_dwSelectColor = dwColor;
}

DWORD CMenuUI::GetSelColor2() const
{
	return m_dwSelectColor2;
}

void CMenuUI::SetSelColor2(DWORD dwColor)
{
	m_dwSelectColor2 = dwColor;
}

DWORD CMenuUI::GetSelColor3() const
{
	return m_dwSelectColor3;
}

void CMenuUI::SetSelColor3(DWORD dwColor)
{
	m_dwSelectColor3 = dwColor;
}

LPCTSTR CMenuUI::GetSelImage()
{
	return m_strSelectedImage.GetData();
}

void CMenuUI::SetSelectImage(LPCTSTR pStrImage)
{
	if( m_strSelectedImage == pStrImage )
		return;
	m_strSelectedImage = pStrImage;
}

DWORD CMenuUI::GetTitleColor() const
{
	return m_dwTitleColor;
}

void CMenuUI::SetTitleColor(DWORD dwColor)
{
	m_dwTitleColor = dwColor;
}

DWORD CMenuUI::GetTitleColor2() const
{
	return m_dwTitleColor2;
}

void CMenuUI::SetTitleColor2(DWORD dwColor)
{
	m_dwTitleColor2 = dwColor;
}

DWORD CMenuUI::GetTitleColor3() const
{
	return m_dwTitleColor3;
}

void CMenuUI::SetTitleColor3(DWORD dwColor)
{
	m_dwTitleColor3 = dwColor;
}

LPCTSTR CMenuUI::GetTitleImage()
{
	return m_strTitleImage.GetData();
}

void CMenuUI::SetTitleImage(LPCTSTR pStrImage)
{
	if(m_strTitleImage==pStrImage)
		return;
	m_strTitleImage = pStrImage;
}

DWORD CMenuUI::GetSideBarColor() const
{
	return m_dwSideBarColor;
}

void CMenuUI::SetSideBarColor(DWORD dwColor)
{
	m_dwSideBarColor = dwColor;
}

DWORD CMenuUI::GetSideBarColor2() const
{
	return m_dwSideBarColor2;
}

void CMenuUI::SetSideBarColor2(DWORD dwColor)
{
	m_dwSideBarColor2 = dwColor;
}

DWORD CMenuUI::GetSideBarColor3() const
{
	return m_dwSideBarColor3;
}

void CMenuUI::SetSideBarColor3(DWORD dwColor)
{
	m_dwSideBarColor3 = dwColor;
}

LPCTSTR CMenuUI::GetSideBarImage()
{
	return m_strSideBarImage.GetData();
}

void CMenuUI::SetSideBarImage(LPCTSTR pStrImage)
{
	if(m_strSideBarImage==pStrImage)
		return;
	m_strSideBarImage = pStrImage;
}

void CMenuUI::SetNormalTextColor(DWORD dwColor)
{
	m_dwNormalTextColor = dwColor;
}

DWORD CMenuUI::GetNormalTextColor() const
{
	return m_dwNormalTextColor;
}

void CMenuUI::SetSelectedTextColor(DWORD dwColor)
{
	m_dwSelectedTextColor = dwColor;
}

DWORD CMenuUI::GetSelectedTextColor() const
{
	return m_dwSelectedTextColor;
}

void CMenuUI::SetDisabledTextColor(DWORD dwColor)
{
	m_dwDisabledTextColor = dwColor;
}

DWORD CMenuUI::GetDisabledTextColor() const
{
	return m_dwDisabledTextColor;
}

DWORD CMenuUI::GetSeparatorColor() const
{
	return m_dwSeparatorColor;
}

void CMenuUI::SetSeparatorColor(DWORD dwColor)
{
	m_dwSeparatorColor = dwColor;
}

LPCTSTR CMenuUI::GetSeparatorImage()
{
	return m_strSeparatorImage.GetData();
}

void CMenuUI::SetSeparatorImage(LPCTSTR pStrImage)
{
	if(m_strSeparatorImage==pStrImage)
		return;
	m_strSeparatorImage = pStrImage;
}

void CMenuUI::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}

void CMenuUI::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
}

void CMenuUI::SetTextFont(int iFont)
{
	m_iTextFont = iFont;
}

void CMenuUI::SetTitleFont(int iFont)
{
	m_iTitleFont = iFont;
}

int CMenuUI::GetTitleFont()
{
	return m_iTitleFont;
}

void CMenuUI::SetSideBarFont(int iFont)
{
	m_iSideBarFont = iFont;
}

int CMenuUI::GetSideBarFont()
{
	return m_iSideBarFont;
}

void CMenuUI::SetPopup(BOOL bPopup)
{
	m_bPopup = bPopup;
}

BOOL CMenuUI::IsPopup()
{
	return m_bPopup;
}

BOOL CMenuUI::IsSubMenu()
{
	return m_bSubMenu;
}

void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("normalcolor")) == 0 || _tcscmp(pstrName, _T("normalcolor1")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetNormalColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("normalcolor2")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetNormalColor2(clrColor);
	}
	else if( _tcscmp(pstrName, _T("normalcolor3")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetNormalColor3(clrColor);
	}
	else if( _tcscmp(pstrName, _T("selectcolor")) == 0 || _tcscmp(pstrName, _T("selectcolor1")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("selectcolor2")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelColor2(clrColor);
	}
	else if( _tcscmp(pstrName, _T("selectcolor3")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelColor3(clrColor);
	}
	else if( _tcscmp(pstrName, _T("titlecolor")) == 0 || _tcscmp(pstrName, _T("titlecolor1")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTitleColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("titlecolor2")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTitleColor2(clrColor);
	}
	else if( _tcscmp(pstrName, _T("titlecolor3")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTitleColor3(clrColor);
	}
	else if( _tcscmp(pstrName, _T("sidebarcolor")) == 0 || _tcscmp(pstrName, _T("sidebarcolor1")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSideBarColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("sidebarcolor2")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSideBarColor2(clrColor);
	}
	else if( _tcscmp(pstrName, _T("sidebarcolor3")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSideBarColor3(clrColor);
	}
	else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetNormalTextColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("selectedtextcolor")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelectedTextColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("disabledtextcolor")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetDisabledTextColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("normalimage")) == 0 )
	{
		SetNormalImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("selectimage")) == 0 )
	{
		SetSelectImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("titleimage")) == 0 )
	{
		SetTitleImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("sidebarimage")) == 0 )
	{
		SetSideBarImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("separatorimage")) == 0 )
	{
		SetSeparatorImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("width")) == 0 )
	{
		SetWidth(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("height")) == 0 )
	{
		SetHeight(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("textfont")) == 0 )
	{
		SetTextFont(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("titlefont")) == 0 )
	{
		SetTitleFont(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("sidebarfont")) == 0 )
	{
		SetSideBarFont(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("popup")) == 0 )
	{
		m_bPopup = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcscmp(pstrName, _T("submenu")) == 0 )
	{
		m_bSubMenu = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcscmp(pstrName, _T("mid")) == 0 )
	{
		LPTSTR pstr = NULL;
		m_dwID = _tcstoul(pstrValue, &pstr, 10);
	}
	else
	{
		CControlUI::SetAttribute(pstrName, pstrValue);
	}
}

void CMenuUI::DrawBackGround(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled)
{
	m_rcItem = rtPaint;
	m_rcPaint = rtPaint;
	if(bSelected)
	{
		m_dwBackColor = m_dwSelectColor;
		m_dwBackColor2 = m_dwSelectColor2;
		m_dwBackColor3 = m_dwSelectColor3;
		if(bDisabled)
			m_sBkImage = m_strNormalImage;
		else
			m_sBkImage = m_strSelectedImage;
	}
	else
	{
		m_dwBackColor = m_dwNormalColor;
		m_dwBackColor2 = m_dwNormalColor2;
		m_dwBackColor3 = m_dwNormalColor3;
		m_sBkImage = m_strNormalImage;
	}
	PaintBkColor(hDC);
	PaintBkImage(hDC);
}

void CMenuUI::DrawTitle(HDC hDC, RECT rtPaint, LPCWSTR pszText)
{
	if( m_dwTitleColor != 0 )
	{
		if( m_dwTitleColor2 != 0 )
		{
			if( m_dwTitleColor3 != 0 )
			{
				RECT rc = rtPaint;
				rc.bottom = (rc.bottom + rc.top) / 2;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwTitleColor), GetAdjustColor(m_dwTitleColor2), true, 8);
				rc.top = rc.bottom;
				rc.bottom = rtPaint.bottom;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwTitleColor2), GetAdjustColor(m_dwTitleColor3), true, 8);
			}
			else 
			{
				CRenderEngine::DrawGradient(hDC, rtPaint, GetAdjustColor(m_dwTitleColor), GetAdjustColor(m_dwTitleColor2), true, 16);
			}
		}
		else
		{
			CRenderEngine::DrawColor(hDC, rtPaint, GetAdjustColor(m_dwTitleColor));
		}
	}
	if(!m_strTitleImage.IsEmpty())
	{
		m_rcItem = rtPaint;
		m_rcPaint = rtPaint;
		if(!DrawImage(hDC, (LPCTSTR)m_strTitleImage))
			m_strTitleImage.Empty();
	}
	//Draw Title text
	rtPaint.left += 2;
	rtPaint.top += 2;
	CRenderEngine::DrawText(hDC, m_pManager, rtPaint, pszText, m_dwNormalTextColor, m_iTitleFont, MENUEX_TEXT_FLAG);
}

void CMenuUI::DrawSideBar(HDC hDC, RECT rtPaint, LPCWSTR pszText)
{
	rtPaint.right += 3;	//fill the gap produced by the menu break
	if( m_dwSideBarColor != 0 )
	{
		if( m_dwSideBarColor2 != 0 )
		{
			if( m_dwSideBarColor3 != 0 )
			{
				RECT rc = rtPaint;
				rc.bottom = (rc.bottom + rc.top) / 2;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwSideBarColor), GetAdjustColor(m_dwSideBarColor2), true, 8);
				rc.top = rc.bottom;
				rc.bottom = rtPaint.bottom;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwSideBarColor2), GetAdjustColor(m_dwSideBarColor3), true, 8);
			}
			else 
			{
				CRenderEngine::DrawGradient(hDC, rtPaint, GetAdjustColor(m_dwSideBarColor), GetAdjustColor(m_dwSideBarColor2), true, 16);
			}
		}
		else
		{
			CRenderEngine::DrawColor(hDC, rtPaint, GetAdjustColor(m_dwSideBarColor));
		}
	}
	if(!m_strSideBarImage.IsEmpty())
	{
		m_rcItem = rtPaint;
		m_rcPaint = rtPaint;
		if(!DrawImage(hDC, (LPCTSTR)m_strSideBarImage))
			m_strSideBarImage.Empty();
	}
	//Draw Sidebar text
	rtPaint.left += 2;
	rtPaint.bottom -= 4;
	CRenderEngine::DrawText(hDC, m_pManager, rtPaint, pszText, m_dwNormalTextColor, m_iSideBarFont, MENUEX_TEXT_FLAG, false);
}

void CMenuUI::DrawText(HDC hDC, RECT rtPaint, LPCWSTR pszText, BOOL bSelected, BOOL bDisabled)
{
	if(bDisabled)
	{
		DrawItemText(hDC, rtPaint, pszText, m_dwDisabledTextColor);
	}
	else if(bSelected)
	{
		DrawItemText(hDC, rtPaint, pszText, m_dwSelectedTextColor);
	}
	else
	{
		DrawItemText(hDC, rtPaint, pszText, bSelected ? m_dwSelectedTextColor : m_dwNormalTextColor);
	}
}

void CMenuUI::DrawCheckMark(HDC hDC, RECT rtPaint, BOOL bSelected)
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

void CMenuUI::DrawItemText(HDC hDC, RECT rtPaint, LPCWSTR pszText, COLORREF color)
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
	//::SetTextColor(hDC, color);
	if(wcslen(szLText))
	{
		CRenderEngine::DrawText(hDC, m_pManager, rtPaint, szLText, color, m_iTextFont, MENUEX_TEXT_FLAG);
	}
	if(wcslen(szRText))
	{
		CRenderEngine::DrawText(hDC, m_pManager, rtPaint, szRText, color, m_iTextFont, MENUEX_TEXT_FLAG | DT_RIGHT);
	}
}

void CMenuUI::Clear(void)
{
	if(!m_hMenu)
		return ;
	CControlUI *pControl = NULL;
	MENUITEMINFO info;
	for (UINT i=0; i<m_items.GetSize(); i++)
	{
		pControl = (CControlUI *)m_items.GetAt(i);
		if(pControl)
		{
			if(_tcscmp(pControl->GetClass(), DUI_CTR_MENU)==0)
			{
				memset(&info, 0, sizeof(MENUITEMINFO));
				info.cbSize = sizeof(MENUITEMINFO);
				info.fMask = MIIM_DATA | MIIM_TYPE;
				GetMenuItemInfo(i, &info, TRUE);
				CMenuItem *pData = (CMenuItem *)info.dwItemData;
				if((info.fType & MFT_OWNERDRAW) && pData && pData->IsMyData())
					delete pData;
			}
			delete pControl;
		}
	}
}

BOOL CMenuUI::IsMenuHwnd(HWND hWnd)
{
	if(!hWnd)
		return FALSE;
	wchar_t szWndClass[128];
	::GetClassName(hWnd, szWndClass, _countof(szWndClass)-1);
	return (wcsicmp(L"#32768", szWndClass) == 0);
}

CMenuUI* CMenuUI::GetLastMenu()
{
	return s_pLastMenu;
}

} // namespace DuiLib
#pragma warning(pop)
