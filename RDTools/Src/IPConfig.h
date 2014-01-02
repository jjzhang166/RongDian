#pragma once

//////////////////////////////////////////////////////////////////////////
// CIPConfig
RD_DECLARE_BEGIN(IPConfig)
	RD_DECLARE_MEMBER(void, OnClick)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemSelected) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	RD_DECLARE_MEMBER(LRESULT, HandleCustomMessage) (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	RD_DECLARE_MEMBER(LPCWSTR, GetSolutionListName)();
	RD_DECLARE_MEMBER(LPCWSTR, GetAdaptersListName)();

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
	RD_DECLARE_MEMBER(CComboUI *, m_pSolutionList);
	RD_DECLARE_MEMBER(CComboUI *, m_pAdapterList);
	RD_DECLARE_MEMBER(CCheckBoxUI *, m_pIpAutoCheckBox);
	RD_DECLARE_MEMBER(CCheckBoxUI *, m_pIpManualCheckBox);
	RD_DECLARE_MEMBER(CCheckBoxUI *, m_pDnsAutoCheckBox);
	RD_DECLARE_MEMBER(CCheckBoxUI *, m_pDnsManualCheckBox);
	RD_DECLARE_MEMBER(CEditUI *, m_pNameEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pIPEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pMaskEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pGatewayEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pDns1Edit);
	RD_DECLARE_MEMBER(CEditUI *, m_pDns2Edit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentAdapterTypeEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentDescEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentDhcpEnabledEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentIPEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentMaskEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentGatewayEdit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentDns1Edit);
	RD_DECLARE_MEMBER(CEditUI *, m_pCurrentDns2Edit);
	RD_DECLARE_MEMBER(CEditUI *, m_pMacEdit);

	RD_DECLARE_MEMBER(list<LPIPCONFIG_INFO>, lstIpConfigInfo);
	RD_DECLARE_MEMBER(list<LPADAPTER_INFO>, lstAdaptersInfo);
	RD_DECLARE_MEMBER(PCMD_EXE_INFO, m_pCmdInfo);
	RD_DECLARE_MEMBER(LPVOID, m_lpLoader);
RD_DECLARE_END()
// CIPConfig
//////////////////////////////////////////////////////////////////////////