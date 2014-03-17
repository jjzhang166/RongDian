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
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/, LPCWSTR /*lpszLangSection*/);

	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	BOOL InitLangs();	// init programming lanuages combo
	BOOL OnTidyFormat();

public:
	CComboUI*								m_pLangsCombo;
	CRichEditUI* 						m_pEditor;
};
// CTidy
//////////////////////////////////////////////////////////////////////////