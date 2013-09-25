#pragma once

class CMainFrame : public WindowImplBase,
	public ILangUI, public ISkinUI,
	public IPanelXml, public IAbout,
	public IColorPicker, public ICoder,
	public IListCallbackUI, public IFileFinder,
	public IFormatter, public IIPConfig,
	public ITidy
{
public:
	CMainFrame();
	~CMainFrame();

	DUI_DECLARE_MESSAGE_MAP()

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
	void			OnItemSelect(TNotifyUI& msg);
	void			OnMenuSelect(TNotifyUI& msg);
	void			OnTextChanged(TNotifyUI& msg);
	void			OnFinalMessage(HWND hWnd);
	CControlUI*		CreateControl(LPCTSTR pstrClass);

	LRESULT			HandleMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT			OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT			OnCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			OnTrayNotification(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT			OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT			OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LPCWSTR			GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

	BOOL			SelectPanel(LPCWSTR lpszTab);
	BOOL			InitPanels();
	BOOL			AddPanel(LPPANEL_INFO lpPanelInfo);
	BOOL			CreatePanels();
	BOOL			ReleasePanels();
	BOOL			ShowTabs(BOOL bShow);
	BOOL			OnActiveApp();
	BOOL			OnAppQuit();

public:
	CDuiString builder_xml_;
	CDuiString resource_dir_;
	CContainerUI *pFrameContainer;
	CContainerUI *pPanelTabs;
	CContainerUI *pPanelContents;
	CControlUI *pStatusCtrl;
	LPVOID lpLoader;
};