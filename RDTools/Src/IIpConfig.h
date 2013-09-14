#pragma once
#include <AdapterUtil.h>

class IIPConfig
{
public:
	IIPConfig();
	virtual ~IIPConfig();
public:
	void			SetIPConfigOwner(HWND hWnd, CPaintManagerUI *pManager) { m_hIPConfigOwner = hWnd; m_pIPConfigManager = pManager; };
	BOOL			InitIPConfig();
	BOOL			SetIPConfigLang(LPCWSTR lpszLang);

	void			OnIPConfigClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnIPConfigItemSelect(TNotifyUI& msg);

	LPCWSTR			GetIPConfigSolutionListName();
	LPCWSTR			GetIPConfigAdaptersListName();

	BOOL			LoadSolutions();
	BOOL			InitSolutionsList(); // 初始化方案列表
	BOOL			LoadAdapters();
	BOOL			InitAdaptersList(); // 初始化适配器列表
	void			OnSelectSolution();
	void			OnSelectAdapter();

	BOOL			OnNewSolution();
	BOOL			OnSaveSolution();
	BOOL			OnDelSolution();
	BOOL			OnApplySolution();

	static LONG		AdapterInfoCallBack(WPARAM wParam, LPARAM lParam);
	static LONG		AdaptersNameCallBack(LPVOID lParam, LPCWSTR lpszName, int nIndex);

public:
	HWND m_hIPConfigOwner;
	CPaintManagerUI *m_pIPConfigManager;
	CComboUI *m_pSolutionList;
	CComboUI *m_pAdapterList;
	CCheckBoxUI *m_pIpAutoCheckBox;
	CCheckBoxUI *m_pIpManualCheckBox;
	CCheckBoxUI *m_pDnsAutoCheckBox;
	CCheckBoxUI *m_pDnsManualCheckBox;
	CEditUI *m_pNameEdit;
	CEditUI *m_pIPEdit;
	CEditUI *m_pMaskEdit;
	CEditUI *m_pGatewayEdit;
	CEditUI *m_pDns1Edit;
	CEditUI *m_pDns2Edit;
	CEditUI *m_pCurrentAdapterTypeEdit;
	CEditUI *m_pCurrentDescEdit;
	CEditUI *m_pCurrentDhcpEnabledEdit;
	CEditUI *m_pCurrentIPEdit;
	CEditUI *m_pCurrentMaskEdit;
	CEditUI *m_pCurrentGatewayEdit;
	CEditUI *m_pCurrentDns1Edit;
	CEditUI *m_pCurrentDns2Edit;
	CEditUI *m_pMacEdit;
	wchar_t m_szHandleText[1024];
	wchar_t m_szNoHandleText[1024];

	LPCWSTR m_lpszLang;
	list<LPIPCONFIG_INFO> lstIpConfigInfo;
	list<LPADAPTER_INFO> lstAdaptersInfo;
};
