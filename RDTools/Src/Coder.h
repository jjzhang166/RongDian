#pragma once

//////////////////////////////////////////////////////////////////////////
// CCoder
class CCoder : public CBaseTool
{
public:
	CCoder();
	virtual ~CCoder() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/);

	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	LPCWSTR GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);

	LPCWSTR GetCoderItemText(CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);
	BOOL InitCharSet();
	BOOL LoadCharSet();
	BOOL InitCharSetCombo();
	BOOL StartConvert(HWND /*hWnd*/);
	BOOL StopConvert();
	BOOL DelSelFile();
	BOOL DelAllFiles();
	BOOL AddNewFile(HWND /*hWnd*/);
	BOOL AddNewFolder(HWND /*hWnd*/);
	BOOL AddCoderFolder(LPCWSTR /*lpszFolder*/);
	BOOL SelFromCharSet(LPCWSTR /*lpszCharSet*/);

	BOOL GetFileCharSet(LPCWSTR /*lpszPath*/, LPSTR /*lpszSet*/);
	BOOL ConvertFile(HWND /*hWnd*/, LPCWSTR /*lpszPath*/, LPCSTR /*lpszFrom*/, LPCSTR /*lpszTo*/);

	static DWORD WINAPI ConvertThread(LPVOID /*lpVoid*/);

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
};
// CCoder
//////////////////////////////////////////////////////////////////////////