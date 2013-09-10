#include "stdafx.h"
#include "IIpConfig.h"
#include <vector>
#include <time.h>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <process.h>
#pragma comment(lib,"Iphlpapi.lib")

using namespace std;
const wchar_t* const kIpConfigSectionPrefix = L"ipconfig_";
//第一列
const wchar_t* const kIPConfigSettingListText = L"setting_list_text";
const wchar_t* const kIPConfigSettingList = L"ipconfig_solution";		
const wchar_t* const kIPConfigNameText = L"ipconfig_s_name_text";
const wchar_t* const kIPConfigNameEdit = L"ipconfig_s_name_edit";
const wchar_t* const kIPConfigAutoSetIPCheckBox = L"ipconfig_ip_auto";
const wchar_t* const kIPConfigManualSetIPCheckBox = L"ipconfig_ip_manual";
const wchar_t* const kIPConfigIPText = L"ipconfig_s_ip_text";
const wchar_t* const kIPConfigIPEdit = L"ipconfig_s_ip_edit";
const wchar_t* const kIPConfigMaskText = L"ipconfig_s_mask_text";
const wchar_t* const kIPConfigMaskEdit = L"ipconfig_s_mask_edit";
const wchar_t* const kIPConfigGatewayText = L"ipconfig_s_gateway_text";
const wchar_t* const kIPConfigGatewayEdit = L"ipconfig_s_gateway_edit";
const wchar_t* const kIPConfigAutoSetDnsCheckBox = L"ipconfig_dns_auto";
const wchar_t* const kIPConfigManualSetDnsCheckBox = L"ipconfig_dns_manual";
const wchar_t* const kIPConfigDns1Text = L"ipconfig_s_dns_text";
const wchar_t* const kIPConfigDns1Edit = L"ipconfig_s_dns_edit";
const wchar_t* const kIPConfigDns2Text = L"ipconfig_s_dns2_text";
const wchar_t* const kIPConfigDns2Edit = L"ipconfig_s_dns2_edit";
const wchar_t* const kIPConfigAddBtn = L"ipconfig_saveas";
const wchar_t* const kIPConfigModifyBtn = L"ipconfig_save";
const wchar_t* const kIPConfigDelBtn = L"ipconfig_delete";
const wchar_t* const kIPConfigUseBtn = L"ipconfig_active";
//第二列
const wchar_t* const kIPConfigCurrentInfoText = L"ipconfig_current_info";
const wchar_t* const kIPConfigCurrentAdapterListText = L"ipconfig_network_adapters_text";
const wchar_t* const kIPConfigCurrentAdapterList = L"ipconfig_network_adapters";
const wchar_t* const kIPConfigCurrentAdapterTypeText = L"ipconfig_iftype_text";
const wchar_t* const kIPConfigCurrentDescText = L"ipconfig_idesc_text";
const wchar_t* const kIPConfigCurrentDhcpText = L"ipconfig_i_dhcp_text";
const wchar_t* const kIPConfigCurrentIPText = L"ipconfig_i_ip_text";
const wchar_t* const kIPConfigCurrentMaskText = L"ipconfig_i_mask_text";
const wchar_t* const kIPConfigCurrentGatewayText = L"ipconfig_i_netgate_text";
const wchar_t* const kIPConfigCurrentDns1Text = L"ipconfig_i_dns_text";
const wchar_t* const kIPConfigCurrentDns2Text = L"ipconfig_i_dns2_text";
const wchar_t* const kIPConfigMacText = L"ipconfig_i_mac_text";
const wchar_t* const kIPConfigCurrentAdapterTypeEdit = L"ipconfig_iftype_edit";
const wchar_t* const kIPConfigCurrentDescEdit = L"ipconfig_idesc_edit";
const wchar_t* const kIPConfigCurrentDhcpEdit = L"ipconfig_i_dhcp_edit";
const wchar_t* const kIPConfigCurrentIPEdit = L"ipconfig_i_ip_edit";
const wchar_t* const kIPConfigCurrentMaskEdit = L"ipconfig_i_mask_edit";
const wchar_t* const kIPConfigCurrentGatewayEdit = L"ipconfig_i_netgate_edit";
const wchar_t* const kIPConfigCurrentDns1Edit = L"ipconfig_i_dns_edit";
const wchar_t* const kIPConfigCurrentDns2Edit = L"ipconfig_i_dns2_edit";
const wchar_t* const kIPConfigMacEdit = L"ipconfig_i_mac_edit";
//msg
const wchar_t* const kIPConfigMsgCanntDel = L"ipconfig_cannt_del";
const wchar_t* const kIPConfigMsgNotNull = L"ipconfig_not_null";
const wchar_t* const kIPConfigMsgDelSuc = L"ipconfig_del_success";

#define ROW_HEIGHT 24
#define IP_LENGTH 129
#define IP_MAX 5
#define SECTION_LENGTH 100
#define INI_KEY_NAME L"name"
#define INI_KEY_IP_AUTO L"ip_auto"
#define INI_KEY_IP L"ip"
#define INI_KEY_MASK L"mask"
#define INI_KEY_GATEWAY L"gateway"
#define INI_KEY_DNS_AUTO L"dns_auto"
#define INI_KEY_DNS1 L"dns1"
#define INI_KEY_DNS2 L"dns2"

IIPConfig::IIPConfig()
{
	m_hIPConfigOwner = NULL;
	m_pIPConfigManager = NULL;
	m_pSetttingList = NULL;
	m_pNameEdit = NULL;
	m_pIPEdit = NULL;
	m_pMaskEdit = NULL;
	m_pGatewayEdit = NULL;
	m_pDns2Edit = NULL;
	m_pDns1Edit = NULL;
	m_pAddBtn = NULL;
	m_pModifyBtn = NULL;
	m_pDelBtn = NULL;
	m_pUseBtn = NULL;
	m_pAdapterList = NULL;
	m_pCurrentAdapterTypeEdit = NULL;
	m_pCurrentDescEdit = NULL;
	m_pCurrentDhcpEnabledEdit = NULL;
	m_pCurrentIPEdit = NULL;
	m_pCurrentMaskEdit = NULL;
	m_pCurrentGatewayEdit = NULL;
	m_pCurrentDns1Edit = NULL;
	m_pCurrentDns2Edit = NULL;
	m_pIpAutoCheckBox = NULL;
	m_pIpManualCheckBox = NULL;
	m_pDnsAutoCheckBox = NULL;
	m_pDnsManualCheckBox = NULL;
	m_lpszLang = NULL;
	m_pNetworkAdapterUtil = new CNetWorkAdapterUtil();
}

IIPConfig::~IIPConfig()
{
	if (m_pNetworkAdapterUtil)
	{
		delete m_pNetworkAdapterUtil;
	}
}
BOOL IIPConfig::InitIPConfig()
{
	if(!m_pIPConfigManager)
		return FALSE;

	FIND_CONTROL_BY_ID(m_pSetttingList, CComboUI, m_pIPConfigManager, kIPConfigSettingList)
	FIND_CONTROL_BY_ID(m_pNameEdit, CEditUI, m_pIPConfigManager, kIPConfigNameEdit)
	FIND_CONTROL_BY_ID(m_pIPEdit, CEditUI, m_pIPConfigManager, kIPConfigIPEdit)
	FIND_CONTROL_BY_ID(m_pMaskEdit, CEditUI, m_pIPConfigManager, kIPConfigMaskEdit)
	FIND_CONTROL_BY_ID(m_pGatewayEdit, CEditUI, m_pIPConfigManager, kIPConfigGatewayEdit)
	FIND_CONTROL_BY_ID(m_pDns1Edit, CEditUI, m_pIPConfigManager, kIPConfigDns1Edit)
	FIND_CONTROL_BY_ID(m_pDns2Edit, CEditUI, m_pIPConfigManager, kIPConfigDns2Edit)
	FIND_CONTROL_BY_ID(m_pAddBtn, CButtonUI, m_pIPConfigManager, kIPConfigAddBtn)
	FIND_CONTROL_BY_ID(m_pModifyBtn, CButtonUI, m_pIPConfigManager, kIPConfigModifyBtn)
	FIND_CONTROL_BY_ID(m_pDelBtn, CButtonUI, m_pIPConfigManager, kIPConfigDelBtn)
	FIND_CONTROL_BY_ID(m_pUseBtn, CButtonUI, m_pIPConfigManager, kIPConfigUseBtn)
	FIND_CONTROL_BY_ID(m_pAdapterList, CComboUI, m_pIPConfigManager, kIPConfigCurrentAdapterList)
	FIND_CONTROL_BY_ID(m_pCurrentAdapterTypeEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentAdapterTypeEdit)
	FIND_CONTROL_BY_ID(m_pCurrentDescEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentDescEdit)
	FIND_CONTROL_BY_ID(m_pCurrentDhcpEnabledEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentDhcpEdit)
	FIND_CONTROL_BY_ID(m_pCurrentIPEdit, CRichEditUI, m_pIPConfigManager, kIPConfigCurrentIPEdit)
	FIND_CONTROL_BY_ID(m_pCurrentMaskEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentMaskEdit)
	FIND_CONTROL_BY_ID(m_pCurrentGatewayEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentGatewayEdit)
	FIND_CONTROL_BY_ID(m_pCurrentDns1Edit, CEditUI, m_pIPConfigManager, kIPConfigCurrentDns1Edit)
	FIND_CONTROL_BY_ID(m_pCurrentDns2Edit, CEditUI, m_pIPConfigManager, kIPConfigCurrentDns2Edit)
	FIND_CONTROL_BY_ID(m_pMacEdit, CEditUI, m_pIPConfigManager, kIPConfigMacEdit)
	FIND_CONTROL_BY_ID(m_pIpAutoCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigAutoSetIPCheckBox)
	FIND_CONTROL_BY_ID(m_pIpManualCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigManualSetIPCheckBox)
	FIND_CONTROL_BY_ID(m_pDnsAutoCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigAutoSetDnsCheckBox)
	FIND_CONTROL_BY_ID(m_pDnsManualCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigManualSetDnsCheckBox)

	//初始值
	RefreshConnectNames();
	SetSettingInfo(0);
	m_pAdapterList->SelectItem(0);
	return TRUE;
}
BOOL IIPConfig::SetIPConfigLang(LPCWSTR lpszLang)
{
	m_lpszLang = lpszLang;
	if(!m_pIPConfigManager)
		return FALSE;
	memset(m_szHandleText, 0, sizeof(m_szHandleText));
	memset(m_szNoHandleText, 0, sizeof(m_szNoHandleText));
	Utility::GetINIStr(lpszLang, LS_MSG, kHandleText, m_szHandleText);
	Utility::GetINIStr(lpszLang, LS_MSG, kNoHandleText, m_szNoHandleText);

SET_CONTROL_BEGIN(m_pIPConfigManager, lpszLang, LS_IPCHANGERPANEL)
	SET_CONTROL_TEXT2(kIPConfigSettingListText)
	SET_CONTROL_TEXT2(kIPConfigNameText)
	SET_CONTROL_TEXT2(kIPConfigAutoSetIPCheckBox)
	SET_CONTROL_TEXT2(kIPConfigManualSetIPCheckBox)
	SET_CONTROL_TEXT2(kIPConfigIPText)
	SET_CONTROL_TEXT2(kIPConfigMaskText)
	SET_CONTROL_TEXT2(kIPConfigGatewayText)
	SET_CONTROL_TEXT2(kIPConfigAutoSetDnsCheckBox)
	SET_CONTROL_TEXT2(kIPConfigManualSetDnsCheckBox)
	SET_CONTROL_TEXT2(kIPConfigDns1Text)
	SET_CONTROL_TEXT2(kIPConfigDns2Text)
	SET_CONTROL_TEXT2(kIPConfigAddBtn)
	SET_CONTROL_TEXT2(kIPConfigModifyBtn)
	SET_CONTROL_TEXT2(kIPConfigDelBtn)
	SET_CONTROL_TEXT2(kIPConfigUseBtn)
	SET_CONTROL_TEXT2(kIPConfigCurrentAdapterListText)
	SET_CONTROL_TEXT2(kIPConfigCurrentAdapterTypeText)
	SET_CONTROL_TEXT2(kIPConfigCurrentInfoText)
	SET_CONTROL_TEXT2(kIPConfigCurrentDescText)
	SET_CONTROL_TEXT2(kIPConfigCurrentDhcpText)
	SET_CONTROL_TEXT2(kIPConfigCurrentIPText)
	SET_CONTROL_TEXT2(kIPConfigCurrentMaskText)
	SET_CONTROL_TEXT2(kIPConfigCurrentGatewayText)
	SET_CONTROL_TEXT2(kIPConfigCurrentDns1Text)
	SET_CONTROL_TEXT2(kIPConfigCurrentDns2Text)
	SET_CONTROL_TEXT2(kIPConfigMacText)
SET_CONTROL_END()

	return TRUE;
}

void IIPConfig::OnIPConfigClick(TNotifyUI& msg, BOOL& bHandled)
{
	if(!m_pIPConfigManager)
		return;
	CDuiString sCtrlName = msg.pSender->GetName();
	if (sCtrlName == kIPConfigAutoSetIPCheckBox)	//自动获取ip
	{
		m_pIPEdit->SetEnabled(false);
		m_pGatewayEdit->SetEnabled(false);
		m_pMaskEdit->SetEnabled(false);
		m_pDnsAutoCheckBox->SetEnabled(true);
	}
	else if(sCtrlName == kIPConfigManualSetIPCheckBox)	//手动设置ip,则dns也只能手动设置
	{
		m_pIPEdit->SetEnabled(true);
		m_pGatewayEdit->SetEnabled(true);
		m_pMaskEdit->SetEnabled(true);
		m_pDnsAutoCheckBox->SetEnabled(false);
		m_pDnsManualCheckBox->SetCheck(true);
	}
	else if (sCtrlName == kIPConfigAutoSetDnsCheckBox)	//自动获取dns
	{
		m_pDns1Edit->SetEnabled(false);
		m_pDns2Edit->SetEnabled(false);
	}
	else if(sCtrlName == kIPConfigManualSetDnsCheckBox)	//手动设置dns
	{
		m_pDns1Edit->SetEnabled(true);
		m_pDns2Edit->SetEnabled(true);
	}
	else if(sCtrlName == kIPConfigAddBtn)	//增加
	{
		if (CheckFormValid()==TRUE)
		{
			//新增配置文件中ip section
			time_t curTime = time(NULL);
			wchar_t newSectionName[100]={0};
			wsprintf(newSectionName,L"%s%ld",kIpConfigSectionPrefix,curTime);
			SetIPConfigIni(newSectionName);
			RefreshSettingList();
			//选择当前的item
			const int count = m_pSetttingList->GetCount();
			m_pSetttingList->SelectItem(count-1);
		}
		bHandled = TRUE;
	}
	else if(sCtrlName == kIPConfigModifyBtn)	//修改
	{
		const int curSel = m_pSetttingList->GetCurSel();
		CListLabelElementUI *item = (CListLabelElementUI*)m_pSetttingList->GetItemAt(curSel);
		LPCTSTR lpszItemText = item->GetText().GetData();
		SetIPConfigIni(lpszItemText);
		bHandled = TRUE;
	}
	else if (sCtrlName == kIPConfigDelBtn)		//删除
	{
		const int count = m_pSetttingList->GetCount();
		if(count<=1)		//至少有一个方案存在
		{
			wchar_t msgTitle[100];
			Utility::GetINIStr(g_szLangPath,LS_IPCHANGERPANEL,kIPConfigMsgCanntDel,msgTitle);
			wchar_t msgCaption[100];
			Utility::GetINIStr(g_szLangPath,LS_IPCHANGERPANEL,kIPConfigMsgNotNull,msgCaption);
			MessageBoxW(NULL,msgTitle,msgCaption,MB_OK);
		}
		else
		{
			const int curSel = m_pSetttingList->GetCurSel();
			CListLabelElementUI *item = (CListLabelElementUI*)m_pSetttingList->GetItemAt(curSel);
			LPCTSTR lpszSectionName = item->GetUserData().GetData();
			WritePrivateProfileSectionW(lpszSectionName,NULL,g_szAppConfig);
			m_pSetttingList->RemoveAt(curSel);
			
			//刷新显示
			const int curSel2 = m_pSetttingList->GetCurSel();
			RefreshSettingEdits(curSel2);
		}
		bHandled = TRUE;
	}
	else if (sCtrlName == kIPConfigUseBtn)		//应用方案
	{
		if (CheckFormValid()==FALSE)
		{
			return;
		}
		BOOL optFlag = FALSE;
		int curSel = m_pAdapterList->GetCurSel();
		CListLabelElementUI *pItem = (CListLabelElementUI*)m_pAdapterList->GetItemAt(curSel);
		LPCTSTR lpszConnectName = pItem->GetText().GetData();
		if(m_pIpAutoCheckBox->IsSelected())
		{
			//m_pNetworkAdapterUtil->SetIPAddressDHCP(lpszConnectName);
		}
		else
		{
			/*LPCTSTR ip = m_pIPEdit->GetText().GetData();
			LPCTSTR mask = m_pMaskEdit->GetText().GetData();
			LPCTSTR gateway = m_pGatewayEdit->GetText().GetData();
			m_pNetworkAdapterUtil->SetIPAddressStatic(lpszConnectName,ip,mask,gateway);*/
		}
		//dns
		if (m_pDnsAutoCheckBox->IsSelected())
		{
			//m_pNetworkAdapterUtil->SetDNSDHCP(lpszConnectName);
		}
		else
		{
			LPCTSTR dns1 = m_pDns1Edit->GetText().GetData();
			LPCTSTR dns2 = m_pDns2Edit->GetText().GetData();

			
			PShellParams shellParams = (PShellParams)malloc(sizeof(ShellParams)) ;
			shellParams->hwnd = m_hIPConfigOwner;
			shellParams->lpszConnectName = lpszConnectName;
			shellParams->lpszParam = dns1;

			UINT threadID;
			HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,CNetWorkAdapterUtil::SetDNSStatic,shellParams,0,&threadID);

			//m_pNetworkAdapterUtil->SetDNSStatic(lpszConnectName,dns1);
			//m_pNetworkAdapterUtil->AddDNSStatic(lpszConnectName,dns2);
		}
		bHandled = TRUE;
	}
}

BOOL IIPConfig::CheckFormValid()
{
	wchar_t msgTitle[100];
	Utility::GetINIStr(m_lpszLang,LS_MSG,L"msg_warn",msgTitle);

	if(!m_pIpAutoCheckBox->IsSelected())
	{
		//判断ip
		LPCTSTR ip = m_pIPEdit->GetText().GetData();
		BOOL isIp = ValidateUtil::IsIPv4(ip);
		if(isIp==FALSE)
		{
			wchar_t text[100];
			Utility::GetINIStr(m_lpszLang,LS_MSG,L"invalid_ipv4_err",text);
			MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			return FALSE;
		}
		//判断子网掩码
		LPCTSTR mask = m_pMaskEdit->GetText().GetData();
		BOOL isMask = ValidateUtil::IsMask(mask);
		if(isMask==FALSE)
		{
			wchar_t text[100];
			Utility::GetINIStr(m_lpszLang,LS_MSG,L"invalid_subnet_mask_err",text);
			MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			return FALSE;
		}
		//判断网关
		LPCTSTR gateway = m_pGatewayEdit->GetText().GetData();
		BOOL isGateway = ValidateUtil::IsIPv4(gateway);
		if(isGateway==FALSE)
		{
			wchar_t text[100];
			Utility::GetINIStr(m_lpszLang,LS_MSG,L"invalid_gateway_err",text);
			MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			return FALSE;
		}
	}
	//dns
	if (!m_pDnsAutoCheckBox->IsSelected())
	{
		LPCTSTR dns1 = m_pDns1Edit->GetText().GetData();
		BOOL isDns1 = ValidateUtil::IsIPv4(dns1);
		if(isDns1==FALSE)
		{
			wchar_t text[100];
			Utility::GetINIStr(m_lpszLang,LS_MSG,L"invalid_dns_err",text);
			MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			return FALSE;
		}
		LPCTSTR dns2 = m_pDns2Edit->GetText().GetData();
		BOOL isDns2 = ValidateUtil::IsIPv4(dns2);
		if(isDns2==FALSE)
		{
			wchar_t text[100];
			Utility::GetINIStr(m_lpszLang,LS_MSG,L"invalid_dns_err",text);
			MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL IIPConfig::SetIPConfigIni(LPCWSTR lpszSectionName)
{
	if (CheckFormValid()==FALSE)
	{
		return FALSE;
	}
	LPCTSTR lpszSettingName = m_pNameEdit->GetText().GetData();
	WritePrivateProfileStringW(lpszSectionName,INI_KEY_NAME,lpszSettingName,g_szAppConfig);
	//ip
	if(m_pIpAutoCheckBox->IsSelected())
	{
		WritePrivateProfileStringW(lpszSectionName,INI_KEY_IP_AUTO,L"1",g_szAppConfig);
	}
	else
	{
		LPCTSTR ip = m_pIPEdit->GetText().GetData();
		LPCTSTR mask = m_pMaskEdit->GetText().GetData();
		LPCTSTR gateway = m_pGatewayEdit->GetText().GetData();
		WritePrivateProfileStringW(lpszSectionName,INI_KEY_IP,ip,g_szAppConfig);
		WritePrivateProfileStringW(lpszSectionName,INI_KEY_MASK,mask,g_szAppConfig);
		WritePrivateProfileStringW(lpszSectionName,INI_KEY_GATEWAY,gateway,g_szAppConfig);
	}
	//dns
	if (m_pDnsAutoCheckBox->IsSelected())
	{
		WritePrivateProfileStringW(lpszSectionName,INI_KEY_DNS_AUTO,L"1",g_szAppConfig);
	}
	else
	{
		LPCTSTR dns = m_pDns1Edit->GetText().GetData();
		LPCTSTR dns2 = m_pDns2Edit->GetText().GetData();
		WritePrivateProfileStringW(lpszSectionName,INI_KEY_DNS1,dns,g_szAppConfig);
		WritePrivateProfileStringW(lpszSectionName,INI_KEY_DNS2,dns2,g_szAppConfig);
	}
	return TRUE;
}

BOOL IIPConfig::RefreshSettingList()
{
	m_pSetttingList->RemoveAll();

	LPWSTR lpszReturnBuffer = (LPWSTR)malloc(sizeof(wchar_t)*1024);
	GetPrivateProfileSectionNamesW(lpszReturnBuffer,1024,g_szAppConfig);
	LPWSTR pNextSection = lpszReturnBuffer;
	while (*pNextSection != 0x00)     
	{         
		pNextSection = pNextSection + wcslen(pNextSection) + 1;
		LPWSTR lpszIndex = StrStrW(pNextSection,kIpConfigSectionPrefix);
		if(*pNextSection != 0x00 && lpszIndex != NULL)    //包含ipconfig_开头的section     
		{
			wchar_t lpszSettingName[100] = {0};
			GetPrivateProfileStringW(pNextSection,INI_KEY_NAME,L"",lpszSettingName,100*sizeof(wchar_t),g_szAppConfig);
			CListLabelElementUI *newItem = new CListLabelElementUI;
			newItem->SetText(lpszSettingName);
			newItem->SetUserData(pNextSection);
			m_pSetttingList->Add(newItem);
		}     
	}
	free(lpszReturnBuffer);
	return TRUE;
}

BOOL IIPConfig::RefreshSettingEdits(const int itemIndex)
{
	CListLabelElementUI *item  = (CListLabelElementUI*)m_pSetttingList->GetItemAt(itemIndex);
	LPCTSTR lpszSectionName = item->GetUserData().GetData();

	wchar_t name[100] = {0};
	GetPrivateProfileStringW(lpszSectionName,INI_KEY_NAME,L"",name,100,g_szAppConfig);
	m_pNameEdit->SetText(name);

	int ipAuto = GetPrivateProfileInt(lpszSectionName,INI_KEY_IP_AUTO,-1,g_szAppConfig);
	int dnsAuto = GetPrivateProfileInt(lpszSectionName,INI_KEY_DNS_AUTO,-1,g_szAppConfig);
	//ip
	if(ipAuto == -1)
	{
		m_pIpManualCheckBox->SetCheck(true);

		wchar_t ip[100] = {0};
		wchar_t mask[100] = {0};
		wchar_t gateway[100] = {0};
		GetPrivateProfileStringW(lpszSectionName,INI_KEY_IP,L"",ip,100,g_szAppConfig);
		GetPrivateProfileStringW(lpszSectionName,INI_KEY_MASK,L"",mask,100,g_szAppConfig);
		GetPrivateProfileStringW(lpszSectionName,INI_KEY_GATEWAY,L"",gateway,100,g_szAppConfig);

		m_pIPEdit->SetText(ip);
		m_pMaskEdit->SetText(mask);
		m_pGatewayEdit->SetText(gateway);
	}
	else
	{
		m_pIpAutoCheckBox->SetCheck(true);
		m_pIPEdit->SetText(L"");
		m_pMaskEdit->SetText(L"");
		m_pGatewayEdit->SetText(L"");
	}

	//dns
	if (dnsAuto == -1)
	{
		m_pDnsManualCheckBox->SetCheck(true);
		wchar_t dns1[100]={0};
		wchar_t dns2[100]={0};
		GetPrivateProfileStringW(lpszSectionName,INI_KEY_DNS1,L"",dns1,100,g_szAppConfig);
		GetPrivateProfileStringW(lpszSectionName,INI_KEY_DNS2,L"",dns2,100,g_szAppConfig);

		m_pDns1Edit->SetText(dns1);
		m_pDns2Edit->SetText(dns2);
	}
	else
	{
		m_pDnsAutoCheckBox->SetCheck(true);
		m_pDns1Edit->SetText(L"");
		m_pDns2Edit->SetText(L"");
	}
	return TRUE;
}

BOOL IIPConfig::RefreshConnectNames()
{
	m_pAdapterList->RemoveAll();
	vector<LPWSTR> connectNames =  m_pNetworkAdapterUtil->m_connectNames;
	for (vector<LPWSTR>::size_type i=0;i<connectNames.size();i++)
	{
		LPWSTR conectName = connectNames.at(i);
		CListLabelElementUI *item = new CListLabelElementUI;
		item->SetText(conectName);
		m_pAdapterList->Add(item);
	}
	return TRUE;
}

BOOL IIPConfig::SetSettingInfo(const int itemIndex)
{
	PIP_ADAPTER_INFO pAdapterInfo = m_pNetworkAdapterUtil->m_allAdapters.at(itemIndex);
	//desc
	wchar_t desc[100] = {0};
	m_pNetworkAdapterUtil->GetDesc(pAdapterInfo,desc,_countof(desc));
	m_pCurrentDescEdit->SetText(desc);
	m_pCurrentDescEdit->SetToolTip(desc);
	//类型
	wchar_t type[100] = {0};
	m_pNetworkAdapterUtil->GetAdapterType(pAdapterInfo,type,_countof(type));
	m_pCurrentAdapterTypeEdit->SetText(type);
	m_pCurrentAdapterTypeEdit->SetToolTip(type);
	//dhcp是否开启
	LPCWSTR langName = g_pLangManager->GetLangName();
	wchar_t langCofig[MAX_PATH] = {0};
	wsprintf(langCofig,L"%s%s",g_szLangPath,langName);
	wchar_t yes[4]={0};
	Utility::GetINIStr(langName,L"DuiMsg",L"msg_yes",yes);
	wchar_t no[3]={0};
	Utility::GetINIStr(langName,L"DuiMsg",L"msg_no",no);
	UINT dhcpEnabled = m_pNetworkAdapterUtil->IsDHCPEnabled(pAdapterInfo);
	m_pCurrentDhcpEnabledEdit->SetText(dhcpEnabled==1?yes:no);
	//ip
	LPWSTR ips[IP_MAX];
	int ipCount = m_pNetworkAdapterUtil->GetIPs(pAdapterInfo,ips);
	m_pCurrentIPEdit->SetText(ips[0]);
	if (ipCount>1)
	{
		m_pCurrentIPEdit->GetParent()->SetMaxHeight(ROW_HEIGHT*ipCount);
	}
	else
	{
		m_pCurrentIPEdit->GetParent()->SetMaxHeight(ROW_HEIGHT);
	}
	//子网掩码
	wchar_t mask[100] = {0};
	m_pNetworkAdapterUtil->GetMask(pAdapterInfo,mask,_countof(mask));
	m_pCurrentMaskEdit->SetText(mask);
	//网关
	wchar_t gateway[100] = {0};
	m_pNetworkAdapterUtil->GetGateway(pAdapterInfo,gateway,_countof(gateway));
	m_pCurrentGatewayEdit->SetText(gateway);
	//dns
	LPWSTR dnsList[2] = {L"",L""};
	m_pNetworkAdapterUtil->GetDns(pAdapterInfo,dnsList);
	m_pCurrentDns1Edit->SetText(dnsList[0]);
	m_pCurrentDns2Edit->SetText(dnsList[1]);
	//mac
	wchar_t lpszMAC[100] = {0};
	m_pNetworkAdapterUtil->GetPhysicalAddress(pAdapterInfo,lpszMAC,_countof(lpszMAC));
	m_pMacEdit->SetText(lpszMAC);
	return TRUE;
}

LPCWSTR IIPConfig::GetIPConfigSettingListName()
{
	return kIPConfigSettingList;
}

LPCWSTR IIPConfig::GetIPConfigConnectListName()
{
	return kIPConfigCurrentAdapterList;
}

void IIPConfig::OnIPConfigItemSelect(TNotifyUI& msg)
{
	CComboUI* combo = (CComboUI*)msg.pSender;
	LPCTSTR lpszComboName = combo->GetName().GetData();
	if(wcscmp(lpszComboName,kIPConfigSettingList)==0)
	{
		const int curSel = combo->GetCurSel();
		CListLabelElementUI* selectItem = (CListLabelElementUI*)combo->GetItemAt(curSel);
		assert(selectItem!=NULL);
		LPCTSTR lpszSectionName = selectItem->GetUserData().GetData();
		if (lpszSectionName[0]!=NULL)
		{		
			RefreshSettingEdits(curSel);
		}
	}
	else if (wcscmp(lpszComboName,kIPConfigCurrentAdapterList)==0)
	{
		const int curSel = combo->GetCurSel();
		SetSettingInfo(curSel);
	}
}
void IIPConfig::ExecuteShellResult()
{
	MessageBoxW(NULL,L"fdf",L"fdfdf",MB_OK);
}
