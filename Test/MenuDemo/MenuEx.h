#pragma once

#include <vector>
#include <map>
using namespace std;

#ifndef WM_SHARED_MENU
#define WM_SHARED_MENU	0x1E2
#endif

#define MENUEX_MAGIC_NUM 0x0505a0a0

//Menu style(Default: STYLE_OFFICE)
typedef	enum 
{
	STYLE_OFFICE,		// Draw a float button around the icon
	STYLE_STARTMENU,	// show selected bar below the icon
	STYLE_XP			// use different color for the icon area
} MENUEX_STYLE;

class CMenuKeyHelper
{
public:
	CMenuKeyHelper();
	CMenuKeyHelper(LPACCEL lpAccel);
	virtual ~CMenuKeyHelper();

public:
	void Format(LPWSTR pszVirtKey, int nMax) const;
	void SetAccel(LPACCEL lpAccel) { m_lpAccel = lpAccel; };

protected:
	void AddVirtKey(LPWSTR pszVirtKey, UINT uiVirtKey, BOOL bLast = FALSE) const;

public:
	LPACCEL m_lpAccel;
};

class CMenuExItem
{
public:
	DWORD		m_dwMagicNum;	//A magic number to distingush our data
	DWORD		m_dwID;			//Menu ID
	BOOL		m_bTitle;		//Separator
	BOOL		m_bSeparator;	//Separator
	BOOL		m_bSideBar;		//A gradient sidebar
	BOOL		m_bButtonOnly;	//Button only style item
	wchar_t		m_szText[256];	//Menu item text
	HICON		m_hIcon;		//Menu icon
	HBITMAP		m_hBitmap;
	int			m_nSize;		//Height of the item(Width of the sidebar if m_bSideBar is true)

public:
	CMenuExItem() 
	{
		m_dwMagicNum = MENUEX_MAGIC_NUM;
		m_dwID = 0;
		m_bTitle = FALSE;
		m_bSeparator = FALSE;
		m_bSideBar = FALSE;
		m_bButtonOnly = FALSE;
		m_hIcon = NULL;
		m_hBitmap = NULL;
		m_nSize = 16;
		memset(m_szText, 0, sizeof(m_szText));
	};

	virtual ~CMenuExItem()
	{
		if (m_hIcon)
			::DestroyIcon(m_hIcon);
		if (m_hBitmap)
			::DeleteObject(m_hBitmap);
	}

	BOOL	IsMyData(void) { return m_dwMagicNum == MENUEX_MAGIC_NUM; };
	void	SetHICON(HICON hIcon)
	{
		if(m_hIcon)
		{
			DestroyIcon(m_hIcon);
			m_hIcon = NULL;
		}
		m_hIcon = hIcon;
	};
	void	SetBitmap(HBITMAP hBitmap)
	{
		if(m_hBitmap)
		{
			DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
		m_hBitmap = hBitmap;
	};
};

class CMenuExText : public CMenuExItem	// Normal item with text and an optional icon
{
public:
	CMenuExText(DWORD dwID, LPCTSTR pszText, HICON hIcon = NULL)
	{
		m_dwID = dwID;
		if(pszText)
#pragma warning(suppress:4996)
			wcsncpy(m_szText, pszText, _countof(m_szText));
		m_hIcon = hIcon;
	}
};

class CMenuExTitle : public CMenuExItem	// Normal item with text and an optional icon
{
public:
	CMenuExTitle(int nSize = 32, LPCTSTR pszText = NULL, HICON hIcon = NULL, DWORD dwID = 0)
	{
		m_bTitle = TRUE;
		if(pszText)
#pragma warning(suppress:4996)
			wcsncpy(m_szText, pszText, _countof(m_szText));
		m_hIcon = hIcon;
		m_nSize = nSize;
		m_dwID = dwID;
	}
};

class CMenuExSeparator : public CMenuExItem // A separator item
{
public:
	CMenuExSeparator()
	{
		m_bSeparator = TRUE;
	}
};

class CMenuExSideBar : public CMenuExItem // A gradient sidebar
{
public:
	explicit CMenuExSideBar(int nSize = 32, LPCTSTR pszText = NULL, HICON hIcon = NULL, DWORD dwID = 1)
	{
		m_bSideBar = TRUE;
		if(pszText)
#pragma warning(suppress:4996)
			wcsncpy(m_szText, pszText, _countof(m_szText));
		m_hIcon = hIcon;
		m_nSize = nSize;
		m_dwID = dwID;
	}
};

class CMenuExButton : public CMenuExItem //A button only item
{
public:
	explicit CMenuExButton(DWORD dwID, HICON hIcon)
	{
		m_dwID = dwID;
		m_bButtonOnly = TRUE;
		m_hIcon = hIcon;
	}
};

class CMenuEx
{
public:
	CMenuEx();
	virtual ~CMenuEx();

public:
	BOOL CreateMenu();
	BOOL CreatePopupMenu();
	BOOL LoadMenu(LPCTSTR lpszResourceName);
	BOOL LoadMenu(UINT nIDResource);
	BOOL LoadMenuIndirect(const void* lpMenuTemplate);
	BOOL DestroyMenu();

	HMENU GetSafeHmenu() const;
	operator HMENU() const;

	BOOL DeleteMenu(UINT nPosition, UINT nFlags);
	BOOL TrackPopupMenu(UINT nFlags, int x, int y, HWND hWnd, LPCRECT lpRect = 0);
	BOOL TrackPopupMenuEx(UINT fuFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm);

#if(WINVER >= 0x0500)
	BOOL SetMenuInfo(LPCMENUINFO lpcmi);
	BOOL GetMenuInfo(LPMENUINFO lpcmi) const;
#endif

	BOOL operator==(const CMenuEx& menu) const;
	BOOL operator!=(const CMenuEx& menu) const;

	BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
	BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp);
	UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck);
	UINT EnableMenuItem(UINT nIDEnableItem, UINT nEnable);
	UINT GetMenuItemCount() const;
	UINT GetMenuItemID(int nPos) const;
	UINT GetMenuState(UINT nID, UINT nFlags) const;
	int GetMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags) const;
	BOOL GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE);
	BOOL SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE);
	CMenuEx* GetSubMenu(int nPos) const;
	BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
	BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp);
	BOOL InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE);
	BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
	BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp);
	BOOL RemoveMenu(UINT nPosition, UINT nFlags);
	BOOL SetMenuItemBitmaps(UINT nPosition, UINT nFlags, const HBITMAP hBmpUnchecked, const HBITMAP hBmpChecked);
	BOOL CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags);
	BOOL SetDefaultItem(UINT uItem, BOOL fByPos = FALSE);
	UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE);

	// Context Help Functions
	BOOL SetMenuContextHelpId(DWORD dwContextHelpId);
	DWORD GetMenuContextHelpId() const;

	// Overridables (must override draw and measure for owner-draw menu items)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	// 
	virtual int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnWindowPosChanging(LPWINDOWPOS lpPos);
	virtual void OnSharedMenu(HWND hWnd);
	virtual void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual int OnNcPaint(HWND hWnd, HRGN hRgn);
	virtual int OnPrint(HWND hWnd, HDC hDC, DWORD dwOption);
	virtual int OnPrintClient(HWND hWnd, HDC hDC, DWORD dwOption);
	virtual int OnPaint(HWND hWnd);
	virtual BOOL OnEraseBkgnd(HDC hDC);
	virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnDestroy();
	virtual void OnNcDestroy();
	virtual void OnContextMenu(HWND hWnd, LPPOINT pCursor);
	virtual void OnInitMenuPopup(HMENU hMenu, UINT nIndex, BOOL bSysMenu);
	//
	static BOOL OnMouseMenu(int xPos, int yPos);

	//////////////////////////////////////////////////////////////////////////
	// Below is the functions to build the menu
	BOOL	AddTitle(CMenuExTitle *pItem);
	BOOL	AddSideBar(CMenuExSideBar *pItem);
	BOOL	AddMenu(UINT nFlags, CMenuExItem *pItem, ACCEL *pAccel = 0);
	BOOL	AddSeparator(void);	
	BOOL	AddPopup(UINT nFlags, CMenuEx *pPopup, CMenuExItem *pItem);
	void	Break(void);	// change a column(the next item added will be in a new column)
	void	BreakBar(void);	// change a column with a break line(same as Break, except that a break line is drawn between two columns)

	//////////////////////////////////////////////////////////////////////////
	void	SetBackColor(COLORREF clr) { m_clrBackGround = clr; };
	void	SetSelectedBarColor(COLORREF clr) { m_clrSelectedBar = clr; };
	void	SetTextColor(COLORREF clr) { m_clrText = clr; };
	void	SetSelectedTextColor(COLORREF clr) { m_clrSelectedText = clr; };
	void	SetDisabledTextColor(COLORREF clr) { m_clrDisabledText = clr; };
	void	SetTitleStartColor(COLORREF clr) { m_clrTitleStart = clr; };
	void	SetTitleEndColor(COLORREF clr) { m_clrTitleEnd = clr; };
	void	SetSideBarStartColor(COLORREF clr) { m_clrSideBarStart = clr; };
	void	SetSideBarEndColor(COLORREF clr) { m_clrSideBarEnd = clr; };
	void	SetIconAreaColor(COLORREF clr) { m_clrIconArea = clr; };
	void	SetBackBitmap(HBITMAP hBitmap);
	void	SetSepBitmap(HBITMAP hBitmap);
	void	SetStyle(MENUEX_STYLE style) { m_Style = style; };
	BOOL	SetFont(LOGFONT	lgfont);
	void	SetWidth(int nWidth) { m_nWidth = nWidth; };
	void	SetHeight(int nHeight) { m_nHeight = nHeight; };

protected:
	virtual void DrawBackGround(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled);
	virtual void DrawButton(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
	virtual void DrawIcon(HDC hDC, RECT rtPaint, HICON hIcon, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
	virtual void DrawTitle(HDC hDC, RECT rtPaint, HICON hIcon, LPCWSTR pszText);
	virtual void DrawSideBar(HDC hDC, RECT rtPaint, HICON hIcon, LPCWSTR pszText);
	virtual void DrawText(HDC hDC, RECT rtPaint, LPCWSTR pszText, BOOL bSelected, BOOL bDisabled, BOOL bBold);
	virtual void DrawCheckMark(HDC hDC, RECT rtPaint, BOOL bSelected);
	virtual void DrawItemText(HDC hDC, RECT rtPaint, LPCWSTR pszText, COLORREF color);
	virtual void DrawIconArea(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
	virtual void DrawBorder(HDC hDC, RECT rtPaint);

private:
	HBITMAP CreateGradientBMP(HDC hDC, COLORREF cl1, COLORREF cl2, int nWidth, int nHeight, int nDir, int nNumColors);
	void DrawEmbossed(HDC hDC, HICON hIcon, RECT rtPaint, BOOL bColor = FALSE, BOOL bShadow = FALSE);
	void FillRect(HDC hDC, LPRECT pRect, COLORREF color);
	void Clear(void); // Clean all memory and handles

public:
	static CMenuEx* PASCAL FromHandle(HMENU hMenu);
	static HMENU GetLastMenu();
	static BOOL IsMenuHwnd(HWND hWnd);
	static BOOL InitHook(HWND hWnd);
	static void UninitHook(HWND hWnd);
	static CMenuEx* GetMenuEx(HWND hWnd);
	static BOOL DoSysMenu(HWND hWnd, POINT ptCursor, LPRECT prExclude = NULL, BOOL bCopy = FALSE);
	
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	HMENU m_hMenu;
	HWND m_hWnd;
	WNDPROC m_pOldWndProc;
	static vector<HMENU> s_hLastMenu;

protected:
	HFONT			m_hFont;
	HFONT			m_hVBoldFont;
	HFONT			m_hHBoldFont;
	COLORREF		m_clrBackGround;	// Background color
	COLORREF		m_clrSelectedBar;	// selected bar color
	COLORREF		m_clrText;			// Text color
	COLORREF		m_clrSelectedText;	// selected text color
	COLORREF		m_clrDisabledText;	// disabled text color

	COLORREF		m_clrTitleStart;	// Start color of the gradient title
	COLORREF		m_clrTitleEnd;	// end color of the gradient title

	COLORREF		m_clrSideBarStart;	// Start color of the gradient sidebar
	COLORREF		m_clrSideBarEnd;	// end color of the gradient sidebar
	COLORREF		m_clrIconArea;		// Background color of the button(icon) area

	BOOL			m_bBreak;			// if true, next item inserted into the menu will be added with the sytle MF_MENUBREAK
	BOOL			m_bBreakBar;		// if true, next item inserted into the menu will be added with the sytle MF_MENUBARBREAK

	HBITMAP			m_hBitmap;			// Background bitmap
	HBITMAP			m_hOldBitmap;
	HDC				m_hMemDC;			// Memory dc holding the background bitmap
	HBITMAP			m_hSepBitmap;
	HBITMAP			m_hOldSepBitmap;
	HDC				m_hSepMemDC;

	int				m_nWidth;
	int				m_nHeight;

	int				m_nTitleHeight;
	int				m_nSideBarWidth;

	MENUEX_STYLE	m_Style;			// menu style(currently support office or startmenu style)
};