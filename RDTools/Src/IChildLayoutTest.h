#pragma once

class IChildLayoutTest
{
public:
	IChildLayoutTest();
	virtual ~IChildLayoutTest();

public:
	void			SetChildLayoutTestOwner(HWND hWnd, CPaintManagerUI *pManager) { hChildLayoutTestOwner = hWnd; pChildLayoutTestManager = pManager; };
	BOOL			InitChildLayoutTest();
	BOOL			SetChildLayoutTestLang(LPCWSTR lpszLang);
	CControlUI* 	OnChildLayoutTestCreateControl(LPCTSTR pstrClass, CControlUI *pParent);

	void			OnChildLayoutTestClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnChildLayoutTestItemActive(TNotifyUI& msg);

	BOOL			CreateTestContainer();

public:
	HWND hChildLayoutTestOwner;
	CPaintManagerUI *pChildLayoutTestManager;
};