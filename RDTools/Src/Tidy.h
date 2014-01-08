#pragma once

//////////////////////////////////////////////////////////////////////////
// CTidy
RD_DECLARE_BEGIN(Tidy)
	RD_DECLARE_MEMBER(CControlUI*, OnCreateControl)(CPaintManagerUI* /*pManager*/, LPCTSTR /*pstrClass*/, CControlUI* /*pParent*/);
	RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemActive) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	RD_DECLARE_MEMBER(BOOL, OnNewTab) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	RD_DECLARE_MEMBER(BOOL, OnSelTab) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	RD_DECLARE_MEMBER(BOOL, OnTidyNormal) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	RD_DECLARE_MEMBER(BOOL, OnTidySel) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	RD_DECLARE_MEMBER(BOOL, OnTidyBatch) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);

public:
	CHorizontalLayoutUI *	pTidyTab;
	CVerticalLayoutUI *		pTidyPanels;
	int						nNewTabIndex;
	CDuiString				strCurTabName;
RD_DECLARE_END()
// CTidy
//////////////////////////////////////////////////////////////////////////