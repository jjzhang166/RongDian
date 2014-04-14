#pragma once

class CPanelRegister
{
public:
	CPanelRegister();
	~CPanelRegister();
	
public:
	BOOL InitTools();
	void ReleaseTools();

	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/) ;
	CControlUI* OnCreateControl(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszClass*/, CControlUI* /*pParent*/);
	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	void OnItemClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	void OnItemSelected(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	void OnTextChanged(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	LPCWSTR GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();

	LRESULT HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCopyData(WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT	OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

public:
	list<LPTOOLS_INFO> lstToolsEntries;
};