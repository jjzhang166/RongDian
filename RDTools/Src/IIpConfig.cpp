#include "stdafx.h"
#include "IIpConfig.h"
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <process.h>
#pragma comment(lib,"Iphlpapi.lib")

const wchar_t* const kIpConfigSectionPrefix = L"ipconfig_";
// 第一列
const wchar_t* const kIPConfigSettingListText = L"setting_list_text";
const wchar_t* const kIPConfigSolutionList = L"ipconfig_solution";		
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
const wchar_t* const kIPConfigNewBtn = L"ipconfig_new";
const wchar_t* const kIPConfigSaveBtn = L"ipconfig_save";
const wchar_t* const kIPConfigDelBtn = L"ipconfig_delete";
const wchar_t* const kIPConfigApplyBtn = L"ipconfig_apply";
// 第二列
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
	m_pSolutionList = NULL;
	m_pNameEdit = NULL;
	m_pIPEdit = NULL;
	m_pMaskEdit = NULL;
	m_pGatewayEdit = NULL;
	m_pDns2Edit = NULL;
	m_pDns1Edit = NULL;
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
}

IIPConfig::~IIPConfig()
{
}

BOOL IIPConfig::InitIPConfig()
{
	if(!m_pIPConfigManager)
		return FALSE;

	FIND_CONTROL_BY_ID(m_pSolutionList, CComboUI, m_pIPConfigManager, kIPConfigSolutionList)
	FIND_CONTROL_BY_ID(m_pNameEdit, CEditUI, m_pIPConfigManager, kIPConfigNameEdit)
	FIND_CONTROL_BY_ID(m_pIPEdit, CEditUI, m_pIPConfigManager, kIPConfigIPEdit)
	FIND_CONTROL_BY_ID(m_pMaskEdit, CEditUI, m_pIPConfigManager, kIPConfigMaskEdit)
	FIND_CONTROL_BY_ID(m_pGatewayEdit, CEditUI, m_pIPConfigManager, kIPConfigGatewayEdit)
	FIND_CONTROL_BY_ID(m_pDns1Edit, CEditUI, m_pIPConfigManager, kIPConfigDns1Edit)
	FIND_CONTROL_BY_ID(m_pDns2Edit, CEditUI, m_pIPConfigManager, kIPConfigDns2Edit)
	FIND_CONTROL_BY_ID(m_pAdapterList, CComboUI, m_pIPConfigManager, kIPConfigCurrentAdapterList)
	FIND_CONTROL_BY_ID(m_pCurrentAdapterTypeEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentAdapterTypeEdit)
	FIND_CONTROL_BY_ID(m_pCurrentDescEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentDescEdit)
	FIND_CONTROL_BY_ID(m_pCurrentDhcpEnabledEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentDhcpEdit)
	FIND_CONTROL_BY_ID(m_pCurrentIPEdit, CEditUI, m_pIPConfigManager, kIPConfigCurrentIPEdit)
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
	LoadSolutions();
	InitSolutionsList();
	LoadAdapters();
	InitAdaptersList();

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
	SET_CONTROL_TEXT2(kIPConfigNewBtn)
	SET_CONTROL_TEXT2(kIPConfigSaveBtn)
	SET_CONTROL_TEXT2(kIPConfigDelBtn)
	SET_CONTROL_TEXT2(kIPConfigApplyBtn)
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
	else if(sCtrlName == kIPConfigManualSetDnsCheckBox)	// 手动设置dns
	{
		m_pDns1Edit->SetEnabled(true);
		m_pDns2Edit->SetEnabled(true);
	}
	else if(sCtrlName == kIPConfigNewBtn) // 增加
	{
		OnNewSolution();
		bHandled = TRUE;
	}
	else if(sCtrlName == kIPConfigSaveBtn) // 保存
	{
		OnSaveSolution();
		bHandled = TRUE;
	}
	else if (sCtrlName == kIPConfigDelBtn) // 删除
	{
		OnDelSolution();
		bHandled = TRUE;
	}
	else if (sCtrlName == kIPConfigApplyBtn) // 应用方案
	{
		OnApplySolution();
		bHandled = TRUE;
	}
}

void IIPConfig::OnIPConfigItemSelect(TNotifyUI& msg)
{
	CComboUI* pCombo = (CComboUI*)msg.pSender;
	CDuiString sItemName = pCombo->GetName();
	if(sItemName == kIPConfigSolutionList)
		OnSelectSolution();
	else if (sItemName == kIPConfigCurrentAdapterList)
		OnSelectAdapter();
}

LPCWSTR IIPConfig::GetIPConfigSolutionListName()
{
	return kIPConfigSolutionList;
}

LPCWSTR IIPConfig::GetIPConfigAdaptersListName()
{
	return kIPConfigCurrentAdapterList;
}

BOOL IIPConfig::LoadSolutions()
{
	BOOL bRet = FALSE;
	wchar_t szSection[MAX_PATH] = {0};
	IPCONFIG_INFO stIPConfigInfo = {0};
	LPIPCONFIG_INFO lpIPConfigInfo = NULL;
	int nIndex = 0;
	while(TRUE)
	{
		swprintf(szSection, L"Solution%d", nIndex);
		Utility::GetINIInt(g_szAppConfig, szSection, kValid, &stIPConfigInfo.nValid);
		if(stIPConfigInfo.nValid==-1)
			break;
		Utility::GetINIInt(g_szAppConfig, szSection, kAddrType, &stIPConfigInfo.nAddrType);
		Utility::GetINIStr(g_szAppConfig, szSection, kSolution, stIPConfigInfo.szSolution);
		Utility::GetINIStr(g_szAppConfig, szSection, kAddr, stIPConfigInfo.szAddr);
		Utility::GetINIStr(g_szAppConfig, szSection, kMask, stIPConfigInfo.szMask);
		Utility::GetINIStr(g_szAppConfig, szSection, kGateway, stIPConfigInfo.szGateway);
		Utility::GetINIInt(g_szAppConfig, szSection, kDnsType, &stIPConfigInfo.nDnsType);
		Utility::GetINIStr(g_szAppConfig, szSection, kDns1, stIPConfigInfo.szDns1);
		Utility::GetINIStr(g_szAppConfig, szSection, kDns2, stIPConfigInfo.szDns2);

		lpIPConfigInfo = new IPCONFIG_INFO();
		if(!lpIPConfigInfo)
			return bRet;
		memcpy(lpIPConfigInfo, &stIPConfigInfo, sizeof(stIPConfigInfo));
		lstIpConfigInfo.push_back(lpIPConfigInfo);

		memset(&stIPConfigInfo, 0, sizeof(stIPConfigInfo));
		nIndex++;
	}
	bRet = TRUE;

	return bRet;
}

BOOL IIPConfig::InitSolutionsList()
{
	int nSelected = 0, nIndex = 0;
	Utility::GetINIInt(g_szAppConfig, LS_IPSETS, kSelected, &nSelected);
	wchar_t szName[MAX_PATH] = {0};
	wchar_t szKey[MAX_PATH] = {0};
	CListLabelElementUI *pItem = NULL;
	m_pSolutionList->RemoveAll();
	while(TRUE)
	{
		swprintf(szKey, L"Name%d", nIndex);
		Utility::GetINIStr(g_szAppConfig, LS_IPSETS, szKey, szName);
		if(wcslen(szName)==0)
			break;
		pItem = new CListLabelElementUI();
		pItem->SetText(szName);
		m_pSolutionList->AddAt(pItem, m_pSolutionList->GetCount());
		if(nSelected==nIndex)
			pItem->Select();
		nIndex ++;
		memset(szKey, 0, sizeof(szKey));
		memset(szName, 0, sizeof(szName));
	}
	m_pSolutionList->SelectItem(nSelected);

	return TRUE;
}

BOOL IIPConfig::LoadAdapters()
{
	BOOL bRet = FALSE;
	ULONG uAdapterSize = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	LPADAPTER_INFO pAdapter = NULL;
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(uAdapterSize);
	if(!pAdapterInfo)
		return bRet;
	if(GetAdaptersInfo(pAdapterInfo, &uAdapterSize)==ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (PIP_ADAPTER_INFO)malloc(uAdapterSize);
		if(!pAdapterInfo)
			return bRet;
	}
	if(GetAdaptersInfo(pAdapterInfo, &uAdapterSize)!=ERROR_SUCCESS)
	{
		free(pAdapterInfo);
		pAdapterInfo = NULL;
		return bRet;
	}
	char szType[STRING_LENGTH] = {0};
	char szDesc[STRING_LENGTH] = {0};
	char szAddr[4][STRING_LENGTH] = {{0}, {0}, {0}, {0}};
	char szMask[STRING_LENGTH] = {0};
	char szGateway[STRING_LENGTH] = {0};
	char szDns[2][STRING_LENGTH] = {{0}, {0}};
	char szMac[STRING_LENGTH] = {0};
	while(pAdapterInfo)
	{
		pAdapter = new ADAPTER_INFO();
		if(!pAdapter)
			break;
		pAdapter->bEnableDHCP = (BOOL)pAdapterInfo->DhcpEnabled;
		pAdapter->nIndex = pAdapterInfo->Index;
		AdapterUtil::GetType(pAdapterInfo, szType);
		StrUtil::a2w(szType, pAdapter->szType);
		AdapterUtil::GetDesc(pAdapterInfo, szDesc);
		StrUtil::a2w(szDesc, pAdapter->szDesc);
		AdapterUtil::GetAddr(pAdapterInfo, szAddr[0], STRING_LENGTH, _countof(szAddr));
		for(int i=0; i<_countof(szAddr); i++)
			StrUtil::a2w(szAddr[i], pAdapter->szAddr[i]);
		AdapterUtil::GetMask(pAdapterInfo, szMask);
		StrUtil::a2w(szMask, pAdapter->szMask);
		AdapterUtil::GetGateway(pAdapterInfo, szGateway);
		StrUtil::a2w(szGateway, pAdapter->szGateway);
		AdapterUtil::GetDns(pAdapterInfo, szDns[0], STRING_LENGTH, _countof(szDns));
		for(int i=0; i<_countof(szDns); i++)
			StrUtil::a2w(szDns[i], pAdapter->szDns[i]);
		AdapterUtil::GetMac(pAdapterInfo, szMac);
		StrUtil::a2w(szMac, pAdapter->szMac);
		lstAdaptersInfo.push_back(pAdapter);
		pAdapterInfo = pAdapterInfo->Next;
		if(pAdapterInfo)
			bRet = TRUE;
	}
	AdapterUtil::GetName(AdaptersNameCallBack, (LPVOID)&lstAdaptersInfo);
	free(pAdapterInfo);
	return bRet;
}

BOOL IIPConfig::InitAdaptersList()
{
	CListLabelElementUI *pItem = NULL;
	LPCWSTR lpszAddr = NULL;
	if(m_pIPEdit)
		lpszAddr = m_pIPEdit->GetText();
	m_pAdapterList->RemoveAll();
	int nSelected = 0, nIndex = 0;
	list<LPADAPTER_INFO>::iterator iter;
	LPADAPTER_INFO lpAdapter = NULL;
	for(iter=lstAdaptersInfo.begin(); iter!=lstAdaptersInfo.end(); iter++)
	{
		lpAdapter = (*iter);
		pItem = new CListLabelElementUI;
		if(!pItem)
			break;
		pItem->SetText(lpAdapter->szName);
		m_pAdapterList->AddAt(pItem, m_pAdapterList->GetCount());
		for(int i=0; i<_countof(lpAdapter->szAddr) && wcslen(lpAdapter->szAddr[i])>0; i++)
		{
			if(wcsicmp(lpAdapter->szAddr[0], lpszAddr)==0)
			{
				pItem->Select();
				nSelected = nIndex;
			}
		}
		nIndex++;
	}
	m_pAdapterList->SelectItem(nSelected);

	return TRUE;
}

void IIPConfig::OnSelectSolution()
{
	if(!m_pSolutionList)
		return;
	int nSelected = m_pSolutionList->GetCurSel();
	list<LPIPCONFIG_INFO>::iterator iter;
	LPIPCONFIG_INFO lpIPConfigInfo = NULL;
	int nIndex = 0;
	for(iter=lstIpConfigInfo.begin(); iter!=lstIpConfigInfo.end(); iter++)
	{
		lpIPConfigInfo = (*iter);
		if(nIndex==nSelected)
			break;
		else
			lpIPConfigInfo = NULL;
		nIndex++;
	}
	if((!lpIPConfigInfo && nSelected!=-1) || (lpIPConfigInfo && nSelected==-1))
		return;
	if(m_pNameEdit)
		m_pNameEdit->SetText(lpIPConfigInfo->szSolution);
	if(lpIPConfigInfo->nAddrType==ADDR_AUTO_SET)
	{
		if(m_pIpAutoCheckBox)
			m_pIpAutoCheckBox->SetCheck(true);
		if(m_pIPEdit)
		{
			m_pIPEdit->SetText(L"");
			m_pIPEdit->SetEnabled(false);
		}
		if(m_pMaskEdit)
		{
			m_pMaskEdit->SetText(L"");
			m_pMaskEdit->SetEnabled(false);
		}
		if(m_pGatewayEdit)
		{
			m_pGatewayEdit->SetText(L"");
			m_pGatewayEdit->SetEnabled(false);
		}
		if(m_pDnsAutoCheckBox)
			m_pDnsAutoCheckBox->SetEnabled(true);
		if(lpIPConfigInfo->nDnsType==DNS_AUTO_SET)
		{
			if(m_pDnsAutoCheckBox)
				m_pDnsAutoCheckBox->SetCheck(true);
		}
		else
		{
			if(m_pDnsManualCheckBox)
				m_pDnsManualCheckBox->SetCheck(true);
		}
	} // end of 'if(lpIPConfigInfo->nAddrType==ADDR_AUTO_SET)'
	else
	{
		if(m_pIpManualCheckBox)
			m_pIpManualCheckBox->SetCheck(true);
		if(m_pIPEdit)
		{
			m_pIPEdit->SetText(lpIPConfigInfo->szAddr);
			m_pIPEdit->SetEnabled(true);
		}
		if(m_pMaskEdit)
		{
			m_pMaskEdit->SetText(lpIPConfigInfo->szMask);
			m_pMaskEdit->SetEnabled(true);
		}
		if(m_pGatewayEdit)
		{
			m_pGatewayEdit->SetText(lpIPConfigInfo->szGateway);
			m_pGatewayEdit->SetEnabled(true);
		}
		if(m_pDnsAutoCheckBox)
		{
			m_pDnsAutoCheckBox->SetCheck(false);
			m_pDnsAutoCheckBox->SetEnabled(false);
		}
		if(m_pDnsManualCheckBox)
			m_pDnsManualCheckBox->SetCheck(true);
	}
	if(m_pDns1Edit)
	{
		m_pDns1Edit->SetEnabled(true);
		m_pDns1Edit->SetText(lpIPConfigInfo->szDns1);
	}
	if(m_pDns2Edit)
	{
		m_pDns2Edit->SetEnabled(true);
		m_pDns2Edit->SetText(lpIPConfigInfo->szDns2);
	}
}

void IIPConfig::OnSelectAdapter()
{
	if(!m_pAdapterList)
		return;
	int nSelected = m_pAdapterList->GetCurSel();
	list<LPADAPTER_INFO>::iterator iter;
	LPADAPTER_INFO lpAdapter = NULL;
	int nIndex = 0;
	for(iter=lstAdaptersInfo.begin(); iter!=lstAdaptersInfo.end(); iter++)
	{
		lpAdapter = (*iter);
		if(nIndex==nSelected)
			break;
		else
			lpAdapter = NULL;
		nIndex++;
	}
	if(!lpAdapter)
		return;
	if(m_pCurrentAdapterTypeEdit)
		m_pCurrentAdapterTypeEdit->SetText(lpAdapter->szType);
	if(m_pCurrentDescEdit)
		m_pCurrentDescEdit->SetText(lpAdapter->szDesc);
	wchar_t szDHCP[32] = {0};
	if(lpAdapter->bEnableDHCP)
		wcscpy(szDHCP, L"Enable");
	else
		wcscpy(szDHCP, L"Disable");
	if(m_pCurrentDhcpEnabledEdit)
		m_pCurrentDhcpEnabledEdit->SetText(szDHCP);
	if(m_pCurrentIPEdit)
	{
		wchar_t szAddr[1024] = {0};
		wcscpy(szAddr, lpAdapter->szAddr[0]);
		for(int i=1; i<_countof(lpAdapter->szAddr); i++)
		{
			if(wcslen(lpAdapter->szAddr[i])==0)
				continue;
			wcscat(szAddr, L",");
			wcscat(szAddr, lpAdapter->szAddr[i]);
		}
		m_pCurrentIPEdit->SetText(szAddr);
	}
	if(m_pCurrentMaskEdit)
		m_pCurrentMaskEdit->SetText(lpAdapter->szMask);
	if(m_pCurrentGatewayEdit)
		m_pCurrentGatewayEdit->SetText(lpAdapter->szGateway);
	if(m_pCurrentDns1Edit)
		m_pCurrentDns1Edit->SetText(lpAdapter->szDns[0]);
	if(m_pCurrentDns2Edit)
		m_pCurrentDns2Edit->SetText(lpAdapter->szDns[1]);
	if(m_pMacEdit)
		m_pMacEdit->SetText(lpAdapter->szMac);
}

BOOL IIPConfig::OnNewSolution()
{
	m_pSolutionList->SelectItem(-1);
	return TRUE;
}

BOOL IIPConfig::OnSaveSolution()
{
	return TRUE;
}

BOOL IIPConfig::OnDelSolution()
{
	return TRUE;
}

BOOL IIPConfig::OnApplySolution()
{
	return TRUE;
}

LONG IIPConfig::AdapterInfoCallBack(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//list<LPADAPTER_INFO> *plstAdapters = (list<LPADAPTER_INFO> *)lParam;
	//if(!plstAdapters)
	//	return 0;
	return 0;
}

LONG IIPConfig::AdaptersNameCallBack(LPVOID lParam, LPCWSTR lpszName, int nIndex)
{
	list<LPADAPTER_INFO> *plstAdapters = (list<LPADAPTER_INFO> *)lParam;
	if(!plstAdapters)
		return 0;
	list<LPADAPTER_INFO>::iterator lstHeader = (*plstAdapters).begin();
	BOOL bFound = FALSE;
	for(; lstHeader!=(*plstAdapters).end(); )
	{
		if((*lstHeader)->nIndex==nIndex)
		{
			bFound = TRUE;
			break;
		}
		lstHeader++;
	}
	if(bFound)
		wcscpy((*lstHeader)->szName, lpszName);
	return 0;
}
