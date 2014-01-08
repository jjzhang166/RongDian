#pragma once

//////////////////////////////////////////////////////////////////////////
// CCoder
RD_DECLARE_BEGIN(Coder)
	RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemActive) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(LPCWSTR, GetItemText) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);

	RD_DECLARE_MEMBER(LPCWSTR, GetCoderItemText) (CControlUI* pControl, int iIndex, int iSubItem);
	RD_DECLARE_MEMBER(BOOL, InitCharSet) ();
	RD_DECLARE_MEMBER(BOOL, LoadCharSet) ();
	RD_DECLARE_MEMBER(BOOL, InitCharSetCombo) ();
	RD_DECLARE_MEMBER(BOOL, StartConvert) (HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, StopConvert) ();
	RD_DECLARE_MEMBER(BOOL, DelSelFile) ();
	RD_DECLARE_MEMBER(BOOL, DelAllFiles) ();
	RD_DECLARE_MEMBER(BOOL, AddNewFile) (HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, AddNewFolder) (HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, AddCoderFolder) (LPCWSTR lpszFolder);
	RD_DECLARE_MEMBER(BOOL, SelFromCharSet) (LPCWSTR lpszCharSet);

	RD_DECLARE_MEMBER(BOOL, GetFileCharSet) (LPCWSTR lpszPath, LPSTR lpszSet);
	RD_DECLARE_MEMBER(BOOL, ConvertFile) (HWND hWnd, LPCWSTR lpszPath, LPCSTR lpszFrom, LPCSTR lpszTo);

	RD_DECLARE_MEMBER(static DWORD WINAPI, ConvertThread) (LPVOID lpVoid);

public:
	CListUI * pCoderList;
	CComboUI * pCoderComboFrom;
	CComboUI * pCoderComboTo;
	CButtonUI * pCoderBackupCheck;
	CButtonUI * pCoderNoBOM;
	CEditUI * pCoderBackupEdit;
	vector<LPCODER_INFO> lstCoderInfo;
	BOOL bCoderBackup;
	BOOL bWithBom;
	BOOL bWorking;
	BOOL bQuitThread;
	wchar_t szHandleText[1024];
	wchar_t szHandleErrText[1024];
	wchar_t szNoHandleText[1024];
	DWORD dwHightLightColor;
	DWORD dwGrayColor;
	list<LPCHARSET_INFO> lstCharSetsInfo;
	HANDLE hConvertThread;
RD_DECLARE_END()
// CCoder
//////////////////////////////////////////////////////////////////////////