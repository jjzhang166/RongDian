#pragma once

class IIPConfig
{
public:
	IIPConfig();
	virtual ~IIPConfig();
public:
	void			SetIPConfigOwner(HWND hWnd, CPaintManagerUI *pManager) { m_hIPConfigOwner = hWnd; m_pIPConfigManager = pManager; };
	void			OnIPConfigClick(TNotifyUI& msg, BOOL& bHandled);
	BOOL			InitIPConfig();
	LPCWSTR			GetIPConfigComboName();
	BOOL			SetIPConfigLang(LPCWSTR lpszLang);
	BOOL			SetIPConfigIni(LPCWSTR lpszSectionName);
	BOOL			RefreshSettingList();
	BOOL			RefreshSettingEdits(const int itemIndex);	//刷新可编辑的ip信息
	BOOL			RefreshSettingInfo();	//刷新当前激活的ip信息

	//事件
	void			OnIPConfigItemSelect(TNotifyUI& msg);
public:
	HWND m_hIPConfigOwner;
	CPaintManagerUI *m_pIPConfigManager;
	CComboUI *m_pSetttingList;
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
	CLabelUI *m_pCurrentIPLabel;
	CLabelUI *m_pCurrentMaskLabel;
	CLabelUI *m_pCurrentGatewayLabel;
	CLabelUI *m_pCurrentDns1Label;
	CLabelUI *m_pCurrentDns2Label;
	CLabelUI *m_pMacLabel;
	wchar_t m_szHandleText[1024];
	wchar_t m_szNoHandleText[1024];
};
