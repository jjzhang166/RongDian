#pragma once

//////////////////////////////////////////////////////////////////////////
// CIPConfig
RD_DECLARE_BEGIN(IPConfig)
	RD_DECLARE_MEMBER(void, OnClick)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemSelected) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	RD_DECLARE_MEMBER(LRESULT, HandleCustomMessage) (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	RD_DECLARE_MEMBER(LPCWSTR, GetSolutionListName)();
	RD_DECLARE_MEMBER(LPCWSTR, GetAdaptersListName)();
	RD_DECLARE_MEMBER(LPIPCONFIG_INFO, GetCurSolution)();

	RD_DECLARE_MEMBER(BOOL, LoadSolutions)();
	RD_DECLARE_MEMBER(BOOL, InitSolutionsList)(); // 初始化方案列表
	RD_DECLARE_MEMBER(BOOL, LoadAdapters)();
	RD_DECLARE_MEMBER(BOOL, InitAdaptersList)(); // 初始化适配器列表
	RD_DECLARE_MEMBER(void, OnSelectSolution)();
	RD_DECLARE_MEMBER(void, OnSelectAdapter)();

	RD_DECLARE_MEMBER(BOOL, OnNewSolution)();
	RD_DECLARE_MEMBER(BOOL, OnSaveSolution)(HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, OnDelSolution)(HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, OnApplySolution)(HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, ResetSolution)();
	RD_DECLARE_MEMBER(BOOL, CheckFormValid)(HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, ExeCMDComplete)();
	//RD_DECLARE_MEMBER(static unsigned __stdcall, ExeCMDThreadFunc)(void * pParams);
	RD_DECLARE_MEMBER(static DWORD WINAPI, ExeCMDThreadFunc)(void * pParams);

	RD_DECLARE_MEMBER(static LONG CALLBACK, AdapterInfoCallBack)(WPARAM wParam, LPARAM lParam);
	RD_DECLARE_MEMBER(static LONG CALLBACK, AdaptersNameCallBack)(LPVOID lParam, LPCWSTR lpszName, BOOL bConnect, int nIndex);

public:
	CComboUI * m_pSolutionList;
	CComboUI * m_pAdapterList;
	CCheckBoxUI * m_pIpAutoCheckBox;
	CCheckBoxUI * m_pIpManualCheckBox;
	CCheckBoxUI * m_pDnsAutoCheckBox;
	CCheckBoxUI * m_pDnsManualCheckBox;
	CEditUI * m_pNameEdit;
	CEditUI * m_pIPEdit;
	CEditUI * m_pMaskEdit;
	CEditUI * m_pGatewayEdit;
	CEditUI * m_pDns1Edit;
	CEditUI * m_pDns2Edit;
	CEditUI * m_pCurrentAdapterTypeEdit;
	CEditUI * m_pCurrentDescEdit;
	CEditUI * m_pCurrentDhcpEnabledEdit;
	CEditUI * m_pCurrentIPEdit;
	CEditUI * m_pCurrentMaskEdit;
	CEditUI * m_pCurrentGatewayEdit;
	CEditUI * m_pCurrentDns1Edit;
	CEditUI * m_pCurrentDns2Edit;
	CEditUI * m_pMacEdit;

	list<LPIPCONFIG_INFO> m_lstIpConfigInfo;
	list<LPADAPTER_INFO> m_lstAdaptersInfo;
	PCMD_EXE_INFO m_pCmdInfo;
	LPVOID m_lpLoader;
RD_DECLARE_END()
// CIPConfig
//////////////////////////////////////////////////////////////////////////