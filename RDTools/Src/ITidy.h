#pragma once

class ITidy
{
public:
	ITidy();
	virtual ~ITidy();

public:
	LPCWSTR			GetTidyListName();
	void			SetTidyOwner(HWND hWnd, CPaintManagerUI *pManager) { hTidyOwner = hWnd; pTidyManager = pManager; };
	BOOL			InitTidy(IListCallbackUI* pIListCallback);
	BOOL			SetTidyLang(LPCWSTR lpszLang);
	CControlUI* 	OnTidyCreateControl(LPCTSTR pstrClass, CControlUI *pParent);

	void			OnTidyClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnTidyItemActive(TNotifyUI& msg);

public:
	HWND hTidyOwner;
	CPaintManagerUI *pTidyManager;
	CListUI *pTidyList;
};