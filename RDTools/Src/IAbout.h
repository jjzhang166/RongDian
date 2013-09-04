#pragma once

class IAbout
{
public:
	IAbout();
	virtual ~IAbout();

public:
	void			SetAboutOwner(HWND hWnd, CPaintManagerUI *pManager) { hAboutOwner = hWnd; pAboutManager = pManager; };
	BOOL			InitAbout();
	BOOL			SetAboutLang(LPCWSTR lpszLang);

	void			OnAboutClick(TNotifyUI& msg, BOOL& bHandled);

public:
	HWND hAboutOwner;
	CPaintManagerUI *pAboutManager;
};
