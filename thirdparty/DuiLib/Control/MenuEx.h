#pragma once


namespace DuiLib { 

#ifndef WM_SHARED_MENU
#define WM_SHARED_MENU	0x1E2
#endif

#define MENUEX_MAGIC_NUM 0x0505a0a0

class UILIB_API CMenuKeyHelper
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

class UILIB_API CMenuItem : public CControlUI
{
public:
	CMenuItem();
	virtual ~CMenuItem();

	LPCTSTR GetClass() const;
	BOOL	IsMyData(void);
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

public:
	DWORD		m_dwMagicNum;	//A magic number to distinguish our data
	DWORD		m_dwID;			//Menu ID
	BYTE		m_fVirt;        /* Also called the flags field */
	WORD		m_wkey;			//
	BOOL		m_bTitle;		//Separator
	BOOL		m_bSeparator;	//Separator
	BOOL		m_bSideBar;		//A gradient sidebar
	BOOL		m_bButton;		//Button only style item
	int			m_nSize;		//Height of the item(Width of the sidebar if m_bSideBar is true)
};

class UILIB_API CMenuText : public CMenuItem	// Normal item with text and an optional icon
{
public:
	CMenuText();
	CMenuText(DWORD dwID, LPCTSTR pszText, LPCWSTR pszImage = NULL);
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

class UILIB_API CMenuTitle : public CMenuItem	// Normal item with text and an optional icon
{
public:
	CMenuTitle(int nSize = 32, LPCTSTR pszText = NULL, DWORD dwID = 0);
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

class UILIB_API CMenuSeparator : public CMenuItem // A separator item
{
public:
	CMenuSeparator();
};

class UILIB_API CMenuSideBar : public CMenuItem // A gradient sidebar
{
public:
	CMenuSideBar(int nSize = 32, LPCTSTR pszText = NULL, DWORD dwID = 1);
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

class UILIB_API CMenuButton : public CMenuItem //A button only item
{
public:
	CMenuButton();
	CMenuButton(DWORD dwID, LPCWSTR pszImage);
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

class UILIB_API CMenuUI : public CControlUI
{
public:
	CMenuUI();
	virtual ~CMenuUI();

public:
	LPCTSTR GetClass() const;
	BOOL CreateMenu(BOOL bRoot = TRUE);
	BOOL CreatePopupMenu(BOOL bRoot = TRUE);
	BOOL DestroyMenu();

	operator HMENU() const;

	BOOL DeleteMenu(UINT nPosition, UINT nFlags);
	BOOL TrackPopupMenu(UINT nFlags, int x, int y, HWND hWnd, LPCRECT lpRect = 0);
	BOOL TrackPopupMenuEx(UINT fuFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm);

	BOOL operator==(const CMenuUI& menu) const;
	BOOL operator!=(const CMenuUI& menu) const;

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

	// Overridables (must override draw and measure for owner-draw menu items)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	virtual void Init();

	//////////////////////////////////////////////////////////////////////////
	// Below is the functions to build the menu
	BOOL	AddControl(CControlUI *pItem);
	BOOL	AddTitle(CMenuTitle *pItem);
	BOOL	AddSideBar(CMenuSideBar *pItem);
	BOOL	AddMenu(UINT nFlags, CMenuItem *pItem, ACCEL *pAccel = 0);
	BOOL	AddSeparator(CMenuSeparator *pItem = NULL);	
	BOOL	AddPopup(UINT nFlags, CMenuUI *pPopup);
	void	Break(void);	// change a column(the next item added will be in a new column)
	void	BreakBar(void);	// change a column with a break line(same as Break, except that a break line is drawn between two columns)

	//////////////////////////////////////////////////////////////////////////
	int		GetItemCount() const;
	DWORD	GetNormalColor() const;
	void	SetNormalColor(DWORD dwColor);
	DWORD	GetNormalColor2() const;
	void	SetNormalColor2(DWORD dwColor);
	DWORD	GetNormalColor3() const;
	void	SetNormalColor3(DWORD dwColor);
	LPCTSTR	GetNormalImage();
	void	SetNormalImage(LPCTSTR pStrImage);
	DWORD	GetSelColor() const;
	void	SetSelColor(DWORD dwColor);
	DWORD	GetSelColor2() const;
	void	SetSelColor2(DWORD dwColor);
	DWORD	GetSelColor3() const;
	void	SetSelColor3(DWORD dwColor);
	LPCTSTR	GetSelImage();
	void	SetSelImage(LPCTSTR pStrImage);
	DWORD	GetTitleColor() const;
	void	SetTitleColor(DWORD dwColor);
	DWORD	GetTitleColor2() const;
	void	SetTitleColor2(DWORD dwColor);
	DWORD	GetTitleColor3() const;
	void	SetTitleColor3(DWORD dwColor);
	LPCTSTR	GetTitleImage();
	void	SetTitleImage(LPCTSTR pStrImage);
	DWORD	GetSideBarColor() const;
	void	SetSideBarColor(DWORD dwColor);
	DWORD	GetSideBarColor2() const;
	void	SetSideBarColor2(DWORD dwColor);
	DWORD	GetSideBarColor3() const;
	void	SetSideBarColor3(DWORD dwColor);
	LPCTSTR	GetSideBarImage();
	void	SetSideBarImage(LPCTSTR pStrImage);
	void	SetNormalTextColor(DWORD dwColor);
	DWORD	GetNormalTextColor() const;
	void	SetSelectedTextColor(DWORD dwColor);
	DWORD	GetSelectedTextColor() const;
	void	SetDisabledTextColor(DWORD dwColor);
	DWORD	GetDisabledTextColor() const;
	DWORD	GetSeparatorColor() const;
	void	SetSeparatorColor(DWORD dwColor);
	LPCTSTR	GetSeparatorImage();
	void	SetSeparatorImage(LPCTSTR pStrImage);
	void	SetWidth(int nWidth);
	void	SetHeight(int nHeight);
	void	SetTextFont(int iFont);
	int		GetTextFont();
	void	SetTitleFont(int iFont);
	int		GetTitleFont();
	void	SetSideBarFont(int iFont);
	int		GetSideBarFont();
	void	SetPopup(BOOL bPopup);
	BOOL	IsPopup();
	BOOL	IsSubMenu();
	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	virtual void DrawBackGround(HDC hDC, RECT rtPaint, BOOL bSelected, BOOL bDisabled);
	virtual void DrawTitle(HDC hDC, RECT rtPaint, LPCWSTR pszText);
	virtual void DrawSideBar(HDC hDC, RECT rtPaint, LPCWSTR pszText);
	virtual void DrawText(HDC hDC, RECT rtPaint, LPCWSTR pszText, BOOL bSelected, BOOL bDisabled);
	virtual void DrawCheckMark(HDC hDC, RECT rtPaint, BOOL bSelected);
	virtual void DrawItemText(HDC hDC, RECT rtPaint, LPCWSTR pszText, COLORREF color);

private:
	void Clear(void); // Clean all memory and handles

public:
	static BOOL IsMenuHwnd(HWND hWnd);
	static CMenuUI* GetLastMenu();

public:
	HWND m_hParent;
	HMENU m_hMenu;
	CStdPtrArray m_items;
	static CMenuUI *s_pLastMenu;
	static CStdStringPtrMap s_mapMenu;

protected:
	DWORD			m_dwNormalColor;
	DWORD			m_dwNormalColor2;
	DWORD			m_dwNormalColor3;
	DWORD			m_dwSelectColor;
	DWORD			m_dwSelectColor2;
	DWORD			m_dwSelectColor3;
	DWORD			m_dwTitleColor;
	DWORD			m_dwTitleColor2;
	DWORD			m_dwTitleColor3;
	DWORD			m_dwSideBarColor;
	DWORD			m_dwSideBarColor2;
	DWORD			m_dwSideBarColor3;
	DWORD			m_dwNormalTextColor;	// Text color
	DWORD			m_dwSelectedTextColor;	// selected text color
	DWORD			m_dwDisabledTextColor;	// disabled text color
	DWORD			m_dwSeparatorColor;
	DWORD			m_dwID;					//Menu ID

	UINT			m_uTextStyle;
	int				m_iTextFont;
	int				m_iTitleFont;
	int				m_iSideBarFont;

	int				m_nWidth;
	int				m_nHeight;

	int				m_nTitleHeight;
	int				m_nSideBarWidth;

	BOOL			m_bPopup;
	BOOL			m_bSubMenu;
	BOOL			m_bBreak;			// if true, next item inserted into the menu will be added with the sytle MF_MENUBREAK
	BOOL			m_bBreakBar;		// if true, next item inserted into the menu will be added with the sytle MF_MENUBARBREAK

	CDuiString		m_strNormalImage;
	CDuiString		m_strSelectedImage;
	CDuiString		m_strTitleImage;
	CDuiString		m_strSideBarImage;
	CDuiString		m_strSeparatorImage;
};
} // namespace DuiLib
