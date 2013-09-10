#pragma once
#include <NetWorkAdapterUtil.h>

class IIPConfig
{
public:
	IIPConfig();
	virtual ~IIPConfig();
public:
	void			SetIPConfigOwner(HWND hWnd, CPaintManagerUI *pManager) { m_hIPConfigOwner = hWnd; m_pIPConfigManager = pManager; };
	void			OnIPConfigClick(TNotifyUI& msg, BOOL& bHandled);
	BOOL			InitIPConfig();
	LPCWSTR			GetIPConfigSettingListName();
	LPCWSTR			GetIPConfigConnectListName();
	BOOL			SetIPConfigLang(LPCWSTR lpszLang);
	BOOL			SetIPConfigIni(LPCWSTR lpszSectionName);
	BOOL			RefreshConnectNames();						//刷新链接列表
	BOOL			RefreshSettingList();						//刷新方案列表
	BOOL			RefreshSettingEdits(const int itemIndex);	//刷新可编辑的ip信息
	BOOL			SetSettingInfo(const int adapterIndex);	//刷新当前选择网卡的ip信息
	
	void			ExecuteShellResult();						//shell执行结束
	//事件
	BOOL			CheckFormValid();
	void			OnIPConfigItemSelect(TNotifyUI& msg);
public:
	HWND m_hIPConfigOwner;
	CPaintManagerUI *m_pIPConfigManager;
	CComboUI *m_pSetttingList;
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
	CButtonUI *m_pAddBtn;
	CButtonUI *m_pModifyBtn;
	CButtonUI *m_pDelBtn;
	CButtonUI *m_pUseBtn;
	CEditUI *m_pCurrentAdapterTypeEdit;
	CEditUI *m_pCurrentDescEdit;
	CEditUI *m_pCurrentDhcpEnabledEdit;
	CRichEditUI *m_pCurrentIPEdit;
	CEditUI *m_pCurrentMaskEdit;
	CEditUI *m_pCurrentGatewayEdit;
	CEditUI *m_pCurrentDns1Edit;
	CEditUI *m_pCurrentDns2Edit;
	CEditUI *m_pMacEdit;
	wchar_t m_szHandleText[1024];
	wchar_t m_szNoHandleText[1024];

	LPCWSTR m_lpszLang;
	CNetWorkAdapterUtil *m_pNetworkAdapterUtil;
};
