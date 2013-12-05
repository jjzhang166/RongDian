#pragma once

class ICoder
{
public:
	ICoder();
	virtual ~ICoder();

public:
	LPCWSTR			GetCoderListName();
	void			SetCoderOwner(HWND hWnd, CPaintManagerUI *pManager) { hCoderOwner = hWnd; pCoderManager = pManager; };
	BOOL			InitCoder(IListCallbackUI* pIListCallback);
	BOOL			SetCoderLang(LPCWSTR lpszLang);

	LPCWSTR			GetCoderItemText(CControlUI* pControl, int iIndex, int iSubItem);
	void			OnCoderClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnCoderItemActive(TNotifyUI& msg);

	BOOL			StartConvert();
	BOOL			StopConvert();
	BOOL			DelSelFile();
	BOOL			DelAllFiles();
	BOOL			AddNewFile();
	BOOL			AddNewFolder();
	BOOL			AddCoderFolder(LPCWSTR lpszFolder);

	BOOL			GetFileCharSet(LPCWSTR lpszPath, LPSTR lpszSet);
	BOOL			ConvertFile(LPCWSTR lpszPath, LPCSTR lpszFrom, LPCSTR lpszTo);

	static DWORD WINAPI ConvertThread(LPVOID lpVoid);

	BOOL			ResetBtnStatus();
public:
	HWND hCoderOwner;
	CPaintManagerUI *pCoderManager;
	CListUI *m_CoderList;
	CComboBoxUI *pCoderFrom;
	CComboBoxUI *pCoderTo;
	CButtonUI *m_startBtn;
	CButtonUI *m_stopBtn;
	CButtonUI *m_delSelBtn;
	CButtonUI *m_delAllBtn;
	CButtonUI *pCoderBackupCheck;
	CButtonUI *pCoderNoBOM;
	CEditUI *pCoderBackupEdit;
	vector<LPCODER_INFO> lstCoderInfo;
	BOOL bCoderBackup;
	BOOL bWithBom;
	BOOL bWorking;
	BOOL bQuitThread;
	wchar_t szHandleText[1024];
	wchar_t szNoHandleText[1024];
	DWORD dwHightLightColor;
	DWORD dwGrayColor;
};
