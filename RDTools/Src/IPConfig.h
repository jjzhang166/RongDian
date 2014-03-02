#pragma once

//////////////////////////////////////////////////////////////////////////
// CIPConfig
class CIPConfig : public CBaseTool
{
public:
	CIPConfig();
	virtual ~CIPConfig() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/);

	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemSelected(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	LRESULT HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LPCWSTR GetSolutionListName();
	LPCWSTR GetAdaptersListName();
	LPIPCONFIG_INFO GetCurSolution();

	BOOL LoadSolutions();
	BOOL InitSolutionsList(); // 初始化方案列表
	BOOL LoadAdapters();
	BOOL InitAdaptersList(); // 初始化适配器列表
	void OnSelectSolution();
	void OnSelectAdapter();

	BOOL OnNewSolution();
	BOOL OnSaveSolution(HWND hWnd);
	BOOL OnDelSolution(HWND hWnd);
	BOOL OnApplySolution(HWND hWnd);
	BOOL ResetSolution();
	BOOL CheckFormValid(HWND hWnd);
	BOOL ExeCMDComplete();
	//static unsigned __stdcall ExeCMDThreadFunc(void * pParams);
	static DWORD WINAPI ExeCMDThreadFunc(void * pParams);

	static LONG CALLBACK AdapterInfoCallBack(WPARAM wParam, LPARAM lParam);
	static LONG CALLBACK AdaptersNameCallBack(LPVOID lParam, LPCWSTR lpszName, BOOL bConnect, int nIndex);

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