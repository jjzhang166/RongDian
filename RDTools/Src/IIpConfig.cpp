#include "stdafx.h"
#include "IIpConfig.h"
#include "../common/TimeUtil.h"

const wchar_t* const kIpConfigSectionPrefix = L"ipconfig_";
//第一列
const wchar_t* const kIPConfigSettingListText = L"setting_list_text";
const wchar_t* const kIPConfigSettingList = L"ipchanger_setting_list";		
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
const wchar_t* const kIPConfigDnsText = L"ipconfig_s_dns_text";
const wchar_t* const kIPConfigDnsEdit = L"ipconfig_s_dns_edit";
const wchar_t* const kIPConfigDns2Text = L"ipconfig_s_dns2_text";
const wchar_t* const kIPConfigDns2Edit = L"ipconfig_s_dns2_edit";
const wchar_t* const kIPConfigAddBtn = L"ipconfig_saveas";
const wchar_t* const kIPConfigModifyBtn = L"ipconfig_save";
const wchar_t* const kIPConfigDelBtn = L"ipconfig_delete";
const wchar_t* const kIPConfigUseBtn = L"ipconfig_active";
//第二列
const wchar_t* const kIPConfigCurrentInfoText = L"ipconfig_current_info";
const wchar_t* const kIPConfigCurrentDescText = L"ipconfig_idesc_text";
const wchar_t* const kIPConfigCurrentDhcpText = L"ipconfig_i_dhcp_text";
const wchar_t* const kIPConfigCurrentIPText = L"ipconfig_i_ip_text";
const wchar_t* const kIPConfigCurrentMaskText = L"ipconfig_i_mask_text";
const wchar_t* const kIPConfigCurrentGatewayText = L"ipconfig_i_netgate_text";
const wchar_t* const kIPConfigCurrentDnsText = L"ipconfig_i_dns_text";
const wchar_t* const kIPConfigCurrentDns2Text = L"current_alternative_dns_text";
const wchar_t* const kIPConfigMacText = L"ipconfig_i_mac_text";

#define SECTION_LENGTH 100
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
	m_pDnsEdit = NULL;
	m_pAddBtn = NULL;
	m_pModifyBtn = NULL;
	m_pDelBtn = NULL;
	m_pUseBtn = NULL;
	m_pCurrentIPLabel = NULL;
	m_pCurrentMaskLabel = NULL;
	m_pCurrentGatewayLabel = NULL;
	m_pCurrentDnsLabel = NULL;
	m_pCurrentDns2Label = NULL;
	m_pMacLabel = NULL;
	m_pIpAutoCheckBox = NULL;
	m_pIpManualCheckBox = NULL;
	m_pDnsAutoCheckBox = NULL;
	m_pDnsManualCheckBox = NULL;
}

IIPConfig::~IIPConfig()
{
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
	FIND_CONTROL_BY_ID(m_pDnsEdit, CEditUI, m_pIPConfigManager, kIPConfigDnsEdit)
	FIND_CONTROL_BY_ID(m_pDns2Edit, CEditUI, m_pIPConfigManager, kIPConfigDns2Edit)
	FIND_CONTROL_BY_ID(m_pAddBtn, CButtonUI, m_pIPConfigManager, kIPConfigAddBtn)
	FIND_CONTROL_BY_ID(m_pModifyBtn, CButtonUI, m_pIPConfigManager, kIPConfigModifyBtn)
	FIND_CONTROL_BY_ID(m_pDelBtn, CButtonUI, m_pIPConfigManager, kIPConfigDelBtn)
	FIND_CONTROL_BY_ID(m_pUseBtn, CButtonUI, m_pIPConfigManager, kIPConfigUseBtn)
	FIND_CONTROL_BY_ID(m_pCurrentIPLabel, CLabelUI, m_pIPConfigManager, kIPConfigCurrentIPText)
	FIND_CONTROL_BY_ID(m_pCurrentMaskLabel, CLabelUI, m_pIPConfigManager, kIPConfigCurrentMaskText)
	FIND_CONTROL_BY_ID(m_pCurrentGatewayLabel, CLabelUI, m_pIPConfigManager, kIPConfigCurrentGatewayText)
	FIND_CONTROL_BY_ID(m_pCurrentDnsLabel, CLabelUI, m_pIPConfigManager, kIPConfigCurrentDnsText)
	FIND_CONTROL_BY_ID(m_pCurrentDns2Label, CLabelUI, m_pIPConfigManager, kIPConfigCurrentDns2Text)
	FIND_CONTROL_BY_ID(m_pMacLabel, CLabelUI, m_pIPConfigManager, kIPConfigMacText)
	FIND_CONTROL_BY_ID(m_pIpAutoCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigAutoSetIPCheckBox)
	FIND_CONTROL_BY_ID(m_pIpManualCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigManualSetIPCheckBox)
	FIND_CONTROL_BY_ID(m_pDnsAutoCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigAutoSetDnsCheckBox)
	FIND_CONTROL_BY_ID(m_pDnsManualCheckBox, CCheckBoxUI, m_pIPConfigManager, kIPConfigManualSetDnsCheckBox)
	return TRUE;
}
BOOL IIPConfig::SetIPConfigLang(LPCWSTR lpszLang)
{
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
	SET_CONTROL_TEXT2(kIPConfigDnsText)
	SET_CONTROL_TEXT2(kIPConfigDns2Text)
	SET_CONTROL_TEXT2(kIPConfigAddBtn)
	SET_CONTROL_TEXT2(kIPConfigModifyBtn)
	SET_CONTROL_TEXT2(kIPConfigDelBtn)
	SET_CONTROL_TEXT2(kIPConfigUseBtn)
	SET_CONTROL_TEXT2(kIPConfigCurrentInfoText)
	SET_CONTROL_TEXT2(kIPConfigCurrentDescText)
	SET_CONTROL_TEXT2(kIPConfigCurrentDhcpText)
	SET_CONTROL_TEXT2(kIPConfigCurrentIPText)
	SET_CONTROL_TEXT2(kIPConfigCurrentMaskText)
	SET_CONTROL_TEXT2(kIPConfigCurrentGatewayText)
	SET_CONTROL_TEXT2(kIPConfigCurrentDnsText)
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
		m_pDnsEdit->SetEnabled(false);
		m_pDns2Edit->SetEnabled(false);
	}
	else if(sCtrlName == kIPConfigManualSetDnsCheckBox)	//手动设置dns
	{
		m_pDnsEdit->SetEnabled(true);
		m_pDns2Edit->SetEnabled(true);
	}
	else if(sCtrlName == kIPConfigAddBtn)	//增加
	{
		//新增配置文件中ip section
		time_t curTime = TimeUtil::GetCurTime();
		wchar_t newSectionName[100]={0};
		wsprintf(newSectionName,L"%s%l",kIpConfigSectionPrefix,curTime);
		SetIPConfigIni(newSectionName);
		bHandled = TRUE;
	}
	else if(sCtrlName == kIPConfigModifyBtn)	//修改
	{
		LPCTSTR selectedText = m_pSetttingList->GetText().GetData();
		LPCTSTR lpszTime = m_pSetttingList->GetUserData().GetData();
		wchar_t sectionName[SECTION_LENGTH];
		wsprintf(sectionName,L"%s%s",kIpConfigSectionPrefix,selectedText);
		bHandled = TRUE;
	}
	else if (sCtrlName == kIPConfigDelBtn)		//删除
	{

		bHandled = TRUE;
	}
	else if (sCtrlName == kIPConfigUseBtn)		//应用方案
	{
		bHandled = TRUE;
	}
}

BOOL IIPConfig::SetIPConfigIni(LPCWSTR lpszSectionName)
{
	LPCTSTR lpszSettingName = m_pNameEdit->GetText().GetData();
	WritePrivateProfileStringW(lpszSectionName,L"name",lpszSettingName,g_szAppConfig);
	if(m_pIpAutoCheckBox->IsSelected())
	{
		WritePrivateProfileStringW(lpszSectionName,L"ip_auto",L"1",g_szAppConfig);
	}
	else
	{
		LPCTSTR ip = m_pIPEdit->GetText().GetData();
		LPCTSTR mask = m_pMaskEdit->GetText().GetData();
		LPCTSTR gateway = m_pGatewayEdit->GetText().GetData();
		WritePrivateProfileStringW(lpszSectionName,L"ip",ip,g_szAppConfig);
		WritePrivateProfileStringW(lpszSectionName,L"mask",mask,g_szAppConfig);
		WritePrivateProfileStringW(lpszSectionName,L"gateway",gateway,g_szAppConfig);
	}
	if (m_pDnsAutoCheckBox->IsSelected())
	{
		WritePrivateProfileStringW(lpszSectionName,L"dns_auto",L"1",g_szAppConfig);
	}
	else
	{
		LPCTSTR dns = m_pDnsEdit->GetText().GetData();
		LPCTSTR dns2 = m_pDns2Edit->GetText().GetData();
		WritePrivateProfileStringW(lpszSectionName,L"dns1",dns,g_szAppConfig);
		WritePrivateProfileStringW(lpszSectionName,L"dns2",dns2,g_szAppConfig);
	}
	return TRUE;
}