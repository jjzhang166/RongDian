#pragma once

//////////////////////////////////////////////////////////////////////////
// CFinder
RD_DECLARE_BEGIN(Finder)
	RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	RD_DECLARE_MEMBER(void, OnItemActive) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	RD_DECLARE_MEMBER(LPCWSTR, GetItemText)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* pControl, int iIndex, int iSubItem);

	RD_DECLARE_MEMBER(LPCWSTR, GetFinderItemText) (CControlUI* pControl, int iIndex, int iSubItem);
	RD_DECLARE_MEMBER(BOOL, SelectFolder)(HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, ResetFinder)();
	RD_DECLARE_MEMBER(BOOL, StartFinder)(HWND hWnd);

	RD_DECLARE_MEMBER(static LRESULT, FinderCallBack)(LPFINDER_INFO lpInfo, WPARAM wParam, LPARAM lParam);

public:
	RD_DECLARE_MEMBER(CListUI *, pFinderList);
	RD_DECLARE_MEMBER(CEditUI *, pFinderNameEdit);
	RD_DECLARE_MEMBER(CEditUI *, pFinderContentEdit);
	RD_DECLARE_MEMBER(CEditUI *, pFinderPathEdit);
	RD_DECLARE_MEMBER(CFileFinder, finder);
	RD_DECLARE_MEMBER(vector<LPFINDER_INFO>, lstFinderInfo);
RD_DECLARE_END(Finder)
// CFinder
//////////////////////////////////////////////////////////////////////////