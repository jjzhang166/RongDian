#pragma once

//////////////////////////////////////////////////////////////////////////
// CAbout
class CAbout : public CBaseTool
{
public:
	CAbout();
	virtual ~CAbout() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/);

	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
};
// CAbout
//////////////////////////////////////////////////////////////////////////