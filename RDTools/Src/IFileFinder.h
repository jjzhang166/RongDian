#pragma once

class IFileFinder
{
public:
	IFileFinder();
	virtual ~IFileFinder();

public:
	LPCWSTR			GetFinderListName();
	void			SetFinderOwner(HWND hWnd, CPaintManagerUI *pManager) { hFinderOwner = hWnd; pFinderManager = pManager; };
	BOOL			InitFinder(IListCallbackUI* pIListCallback);
	BOOL			SetFinderLang(LPCWSTR lpszLang);

	LPCWSTR			GetFinderItemText(CControlUI* pControl, int iIndex, int iSubItem);
	void			OnFinderClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnFinderItemActive(TNotifyUI& msg);

	BOOL			SelectFolder();
	BOOL			ResetFinder();
	BOOL			StartFinder();

	static LRESULT	FinderCallBack(LPFINDER_INFO lpInfo, WPARAM wParam, LPARAM lParam);

public:
	HWND hFinderOwner;
	CPaintManagerUI *pFinderManager;
	CListUI *pFinderList;
	CEditUI *pFinderNameEdit;
	CEditUI *pFinderContentEdit;
	CEditUI *pFinderPathEdit;
	CFileFinder finder;
	vector<LPFINDER_INFO> lstFinderInfo;
};