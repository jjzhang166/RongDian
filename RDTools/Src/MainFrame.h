#pragma once

class CMainFrame : public WindowImplBase,
	public ILangUI, public ISkinUI,
	public IListCallbackUI
{
public:
	CMainFrame();
	~CMainFrame();

	DUI_DECLARE_MESSAGE_MAP()

	RD_DECLARE_TOOLS()

public:
	LONG SetSkin();
	LONG SetLang(LPCWSTR lpszLang);

public:	
	LPCWSTR			GetWindowClassName() const;
	void			InitWindow();
	CDuiString		GetSkinFile();
	CDuiString		GetSkinFolder();
#if USE_ZIPRES
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR			GetResourceID() const;
#endif
	void			OnClick(TNotifyUI& msg);
	void			OnItemActive(TNotifyUI& msg);
	void			OnItemClick(TNotifyUI& msg);
	void			OnItemSelected(TNotifyUI& msg);
	void			OnMenuSelect(TNotifyUI& msg);
	void			OnTextChanged(TNotifyUI& msg);
	void			OnFinalMessage(HWND hWnd);
	CControlUI*		CreateControl(LPCTSTR pstrClass, CControlUI *pParent);

	LRESULT			HandleMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT			OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT			OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT			OnCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			OnTrayNotification(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LPCWSTR			GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

	BOOL			SelectPanel(LPCWSTR lpszTab);
	BOOL			InitPanels();
	BOOL			AddPanel(LPMAIN_PANEL lpPanelInfo);
	BOOL			CreatePanels();
	BOOL			ReleasePanels();
	BOOL			ShowTabs(BOOL bShow);
	BOOL			OnActiveApp();
	BOOL			OnAppQuit();

	BOOL			ShowLoading();
	void			CancelLoading();

public:
	CDuiString builder_xml_;
	CDuiString resource_dir_;
	CContainerUI *pFrameContainer;
	CContainerUI *pPanelTabs;
	CContainerUI *pPanelContents;
	CVerticalLayoutUI *pLoadindFrame;
	CControlUI *pStatusCtrl;
	LPVOID lpLoader;
};