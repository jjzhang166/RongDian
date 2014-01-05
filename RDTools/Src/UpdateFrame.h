#pragma once

class CUpdateFrame : public WindowImplBase,
	public ILangUI, public ISkinUI
{
public:
	CUpdateFrame();
	~CUpdateFrame();

public:
	LONG SetSkin();
	LONG SetLang(LPCWSTR lpszLang);

	DUI_DECLARE_MESSAGE_MAP()

public:	
	LPCWSTR			GetWindowClassName() const;
	void			InitWindow();
	CDuiString		GetSkinFile();
	CDuiString		GetSkinFolder();
#if USE_ZIPRES
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR			GetResourceID() const;
#endif
	void			OnClick(TNotifyUI& /*msg*/);
	void			OnFinalMessage(HWND /*hWnd*/);

	LRESULT			HandleMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT			OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT			UpdateProgress(INT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

	void			SetUpdateInfo(const char *pszUrl, const char *pszText);

	static DWORD WINAPI StartDownload(LPVOID /*lpParam*/);

public:
	CDuiString builder_xml_;
	CDuiString resource_dir_;
	CContainerUI *pFrameContainer;
	HANDLE hWorkingThread;
	CTextUI *pUpdateLog;
	CProgressUI *pProgress;
	char szUrl[1024];
	wchar_t *pszLog;
};