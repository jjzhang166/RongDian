#pragma once

//////////////////////////////////////////////////////////////////////////
// CTidy
class CTidy : public CBaseTool
{
public:
	CTidy();
	virtual ~CTidy() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/);

	CControlUI* OnCreateControl(CPaintManagerUI* /*pManager*/, LPCTSTR /*pstrClass*/, CControlUI* /*pParent*/);
	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	BOOL OnNewTab(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	BOOL OnSelTab(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	BOOL OnTidyNormal(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	BOOL OnTidySel(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);
	BOOL OnTidyBatch(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);

public:
	CHorizontalLayoutUI *	pTidyTab;
	CVerticalLayoutUI *		pTidyPanels;
	int						nNewTabIndex;
	CDuiString				strCurTabName;
};
// CTidy
//////////////////////////////////////////////////////////////////////////