#pragma once

//////////////////////////////////////////////////////////////////////////
// CFinder
class CFinder : public CBaseTool
{
public:
	CFinder();
	virtual ~CFinder() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/, LPCWSTR /*lpszLangSection*/);

	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	LPCWSTR GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);

	LPCWSTR GetFinderItemText(CControlUI* pControl, int iIndex, int iSubItem);
	BOOL SelectFolder(HWND hWnd);
	BOOL ResetFinder();
	BOOL StartFinder(HWND hWnd);

	static LRESULT FinderCallBack(LPFINDER_INFO lpInfo, WPARAM wParam, LPARAM lParam);

public:
	CListUI * pFinderList;
	CEditUI * pFinderNameEdit;
	CEditUI * pFinderContentEdit;
	CEditUI * pFinderPathEdit;
	CFileFinder finder;
	vector<LPFINDER_INFO> lstFinderInfo;
};
// CFinder
//////////////////////////////////////////////////////////////////////////