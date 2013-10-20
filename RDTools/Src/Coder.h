#pragma once

//////////////////////////////////////////////////////////////////////////
// CCoder
RD_DECLARE_BEGIN(Coder)
	RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	RD_DECLARE_MEMBER(void, OnItemActive) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	RD_DECLARE_MEMBER(LPCWSTR, GetItemText) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);

	RD_DECLARE_MEMBER(LPCWSTR, GetCoderItemText) (CControlUI* pControl, int iIndex, int iSubItem);
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
	RD_DECLARE_MEMBER(CListUI *, pCoderList);
	RD_DECLARE_MEMBER(CComboUI *, pCoderComboFrom);
	RD_DECLARE_MEMBER(CComboUI *, pCoderComboTo);
	RD_DECLARE_MEMBER(CButtonUI *, pCoderBackupCheck);
	RD_DECLARE_MEMBER(CButtonUI *, pCoderNoBOM);
	RD_DECLARE_MEMBER(CEditUI *, pCoderBackupEdit);
	RD_DECLARE_MEMBER(vector<LPCODER_INFO>, lstCoderInfo);
	RD_DECLARE_MEMBER(BOOL, bCoderBackup);
	RD_DECLARE_MEMBER(BOOL, bWithBom);
	RD_DECLARE_MEMBER(BOOL, bWorking);
	RD_DECLARE_MEMBER(BOOL, bQuitThread);
	RD_DECLARE_MEMBER(wchar_t, szHandleText[1024]);
	RD_DECLARE_MEMBER(wchar_t, szHandleErrText[1024]);
	RD_DECLARE_MEMBER(wchar_t, szNoHandleText[1024]);
	RD_DECLARE_MEMBER(DWORD, dwHightLightColor);
	RD_DECLARE_MEMBER(DWORD, dwGrayColor);
	RD_DECLARE_MEMBER(list<LPCHARSET_INFO>, lstCharSetsInfo);
	RD_DECLARE_MEMBER(HANDLE, hConvertThread);
RD_DECLARE_END(Coder)
// CCoder
//////////////////////////////////////////////////////////////////////////