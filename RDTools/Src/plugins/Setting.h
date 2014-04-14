#pragma once

//////////////////////////////////////////////////////////////////////////
// CSetting
class CSetting : public CBaseTool
{
public:
	CSetting();
	virtual ~CSetting() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/, LPCWSTR /*lpszLangSection*/);
	
	LRESULT HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	BOOL Apply(HWND hWnd);
	BOOL InitLangs();
	BOOL InitCheckVersion();
	BOOL InitBackground();
public:
	CComboUI * m_pLangsCombo;
	CCheckBoxUI * m_pCheckVersionCheckbox;
	CLabelUI * m_pCheckVersionLabel;
	CEditUI * m_pBackgroundEdit;
	CButtonUI * m_pBackgroundApplyBtn;
	CButtonUI * m_pBackgroundDefaultBtn;
};
// CSetting
//////////////////////////////////////////////////////////////////////////