#include "stdafx.h"
#include "IIpConfig.h"
#include <process.h>

// 第一列
const wchar_t* const kIPConfigSettingListText = L"ipconfig_setting_list_text";
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
const wchar_t* const kIPConfigCurrentAdapterTypeEdit = L"ipconfig_i_type_edit";
const wchar_t* const kIPConfigCurrentDescEdit = L"ipconfig_i_desc_edit";
const wchar_t* const kIPConfigCurrentDhcpEdit = L"ipconfig_i_dhcp_edit";
const wchar_t* const kIPConfigCurrentIPEdit = L"ipconfig_i_ip_edit";
const wchar_t* const kIPConfigCurrentMaskEdit = L"ipconfig_i_mask_edit";
const wchar_t* const kIPConfigCurrentGatewayEdit = L"ipconfig_i_netgate_edit";
const wchar_t* const kIPConfigCurrentDns1Edit = L"ipconfig_i_dns_edit";
const wchar_t* const kIPConfigCurrentDns2Edit = L"ipconfig_i_dns2_edit";
const wchar_t* const kIPConfigMacEdit = L"ipconfig_i_mac_edit";
const wchar_t* const kIPConfigNewSolution = L"ipconfig_new_solution";

DWORD WINAPI RunAsAdminThread(LPVOID lpData)
{
	if(g_bActiveRunAsAdminThread)
		return 0;
	g_bActiveRunAsAdminThread = TRUE;
	Sleep(1000);
	wchar_t szFileName[2048] = {0};
	GetModuleFileName(NULL, szFileName, _countof(szFileName));
	if(Utility::RunAsAdmin(szFileName, L"-a"))
	{
		if(g_pMainFrame)
			SendMessage(g_pMainFrame->GetHWND(), WM_CLOSE, 0, 0);
	}
	return 0;
}

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
	m_pCmdInfo = NULL;
}

IIPConfig::~IIPConfig()
{
	list<LPIPCONFIG_INFO>::iterator addr_iter;
	LPIPCONFIG_INFO lpAddrInfo = NULL;
	for(addr_iter=lstIpConfigInfo.begin(); addr_iter!=lstIpConfigInfo.end(); )
	{
		lpAddrInfo = (*addr_iter);
		addr_iter++;
		delete lpAddrInfo;
	}
	lstIpConfigInfo.clear();

	list<LPADAPTER_INFO>::iterator adapter_iter;
	LPADAPTER_INFO lpAdapterInfo = NULL;
	for(adapter_iter=lstAdaptersInfo.begin(); adapter_iter!=lstAdaptersInfo.end(); )
	{
		lpAdapterInfo = (*adapter_iter);
		adapter_iter++;
		delete lpAdapterInfo;
	}
	lstAdaptersInfo.clear();
	if (m_pCmdInfo)
	{
		free(m_pCmdInfo);
	}
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
		m_pDns1Edit->SetEnabled(true);
		m_pDns2Edit->SetEnabled(true);
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
		if(!Utility::IsAdminPrivilege())
		{
			DWORD dwThreadId = 0;
			HANDLE hThread = CreateThread(NULL, 0, RunAsAdminThread, 0, 0, &dwThreadId);
			CloseHandle(hThread);
		}
		else
		{
			OnApplySolution();
		}
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
	CAddrTableDB addr(&g_SQLite);
	if(!addr.Init())
		return FALSE;
	addr.Query(NULL);
	LPADDR_TABLE lpAddrTable = addr.GetResults();
	LPIPCONFIG_INFO lpIPConfigInfo = NULL;
	for(int i=0; i<addr.GetRows(); i++)
	{
		lpIPConfigInfo = new IPCONFIG_INFO();
		if(!lpIPConfigInfo)
			return bRet;
		lpIPConfigInfo->nAddrType = lpAddrTable[i].nAddrType;
		wcscpy(lpIPConfigInfo->szSolution, lpAddrTable[i].szSolution);
		wcscpy(lpIPConfigInfo->szAddr, lpAddrTable[i].szAddr);
		wcscpy(lpIPConfigInfo->szMask, lpAddrTable[i].szMask);
		wcscpy(lpIPConfigInfo->szGateway, lpAddrTable[i].szGateway);
		lpIPConfigInfo->nDnsType = lpAddrTable[i].nDnsType;
		wcscpy(lpIPConfigInfo->szDns1, lpAddrTable[i].szDns1);
		wcscpy(lpIPConfigInfo->szDns2, lpAddrTable[i].szDns2);
		lstIpConfigInfo.push_back(lpIPConfigInfo);
	}

	bRet = TRUE;
	return bRet;
}

BOOL IIPConfig::InitSolutionsList()
{
	int nSelected = 0;
	CConfigTableDB table(&g_SQLite);
	CONFIG_TABLE config;
	wcscpy(config.szName, kSolution);
	if(table.Query(&config) && table.GetRows())
	{
		LPCONFIG_TABLE lpSolution = table.GetResults();
		if(lpSolution)
			nSelected = _wtoi(lpSolution->szValue);
	}
	CListLabelElementUI *pItem = NULL;
	m_pSolutionList->RemoveAll();
	list<LPIPCONFIG_INFO>::iterator iter;
	LPIPCONFIG_INFO lpIPConfigInfo = NULL;
	for(iter=lstIpConfigInfo.begin(); iter!=lstIpConfigInfo.end(); iter++)
	{
		lpIPConfigInfo = (*iter);
		pItem = new CListLabelElementUI();
		pItem->SetText(lpIPConfigInfo->szSolution);
		m_pSolutionList->AddAt(pItem, m_pSolutionList->GetCount());
	}
	m_pSolutionList->SelectItem(nSelected);

	return TRUE;
}

BOOL IIPConfig::LoadAdapters()
{
	BOOL bRet = FALSE;
	ULONG uAdapterSize = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterHeader = NULL;
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	LPADAPTER_INFO pAdapter = NULL;
	if(GetAdaptersInfo(pAdapterInfo, &uAdapterSize)==ERROR_BUFFER_OVERFLOW)
	{
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
	pAdapterHeader = pAdapterInfo;
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
	free(pAdapterHeader);
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
	//m_pSolutionList->SetText(L"");
	ResetSolution();
	return TRUE;
}

BOOL IIPConfig::OnSaveSolution()
{
	CAddrTableDB table(&g_SQLite);
	ADDR_TABLE addr;
	addr.nAddrType = ADDR_AUTO_SET;
	if(m_pIpManualCheckBox && m_pIpManualCheckBox->IsSelected())
		addr.nAddrType = ADDR_MANUAL_SET;
	addr.nDnsType = DNS_AUTO_SET;
	if(m_pDnsAutoCheckBox && m_pDnsAutoCheckBox->IsSelected())
		addr.nDnsType = DNS_MANUAL_SET;
	if(m_pNameEdit)
		wcscpy(addr.szSolution, m_pNameEdit->GetText());
	if(m_pIPEdit)
		wcscpy(addr.szAddr, m_pIPEdit->GetText());
	if(m_pMaskEdit)
		wcscpy(addr.szMask, m_pMaskEdit->GetText());
	if(m_pGatewayEdit)
		wcscpy(addr.szGateway, m_pGatewayEdit->GetText());
	if(m_pDns1Edit)
		wcscpy(addr.szDns1, m_pDns1Edit->GetText());
	if(m_pDns2Edit)
		wcscpy(addr.szDns2, m_pDns2Edit->GetText());
	if(!table.Insert(&addr))
	{
		RDMsgBox(m_hIPConfigOwner, MSG_INVALID_NAME, MSG_ERR, MB_OK);
		return FALSE;
	}
	LPIPCONFIG_INFO lpIPConfigInfo = NULL;
	lpIPConfigInfo = new IPCONFIG_INFO();
	if(!lpIPConfigInfo)
		return FALSE;
	lpIPConfigInfo->nAddrType = addr.nAddrType;
	wcscpy(lpIPConfigInfo->szSolution, addr.szSolution);
	wcscpy(lpIPConfigInfo->szAddr, addr.szAddr);
	wcscpy(lpIPConfigInfo->szMask, addr.szMask);
	wcscpy(lpIPConfigInfo->szGateway, addr.szGateway);
	lpIPConfigInfo->nDnsType = addr.nDnsType;
	wcscpy(lpIPConfigInfo->szDns1, addr.szDns1);
	wcscpy(lpIPConfigInfo->szDns2, addr.szDns2);
	lstIpConfigInfo.push_back(lpIPConfigInfo);

	CListLabelElementUI *pItem = NULL;
	pItem = new CListLabelElementUI();
	pItem->SetText(lpIPConfigInfo->szSolution);
	m_pSolutionList->AddAt(pItem, m_pSolutionList->GetCount());

	m_pSolutionList->SelectItem(m_pSolutionList->GetCount());

	return TRUE;
}

BOOL IIPConfig::OnDelSolution()
{
	if(!m_pSolutionList)
		return FALSE;

	if(RDMsgBox(m_hIPConfigOwner, MSG_DELETE_SOLUTION, MSG_WARNING, MB_YESNO)!=IDYES)
		return FALSE;
	LPCWSTR lpszSolution = m_pSolutionList->GetText();
	int nSelected = m_pSolutionList->GetCurSel();
	ADDR_TABLE table;
	wcscpy(table.szSolution, lpszSolution);
	CAddrTableDB addr(&g_SQLite);
	if(addr.Delete(&table))
	{
		list<LPIPCONFIG_INFO>::iterator iter;
		LPIPCONFIG_INFO lpIPConfigInfo = NULL;
		int nIndex = 0;
		for(iter=lstIpConfigInfo.begin(); iter!=lstIpConfigInfo.end(); iter++)
		{
			lpIPConfigInfo = (*iter);
			if(nIndex==nSelected)
			{
				lstIpConfigInfo.erase(iter);
				delete lpIPConfigInfo;
				break;
			}
			lpIPConfigInfo = NULL;
			nIndex++;
		}
		m_pSolutionList->RemoveAt(nSelected);
	}
	if(m_pSolutionList->GetCount()==0)
		ResetSolution();
	return TRUE;
}

BOOL IIPConfig::OnApplySolution()
{
	if (CheckFormValid()==FALSE)
	{
		return FALSE;
	}
	//连接名字
	int curSel = m_pAdapterList->GetCurSel();
	CListLabelElementUI *pItem = (CListLabelElementUI*)m_pAdapterList->GetItemAt(curSel);
	LPCTSTR lpszConnectName = pItem->GetText().GetData();
	
	wchar_t *lpszCommand = NULL;
	wchar_t szIpCommand[MAX_PATH] = {0};
	wchar_t szDnsCommand[MAX_PATH] = {0};
	lpszCommand = (wchar_t *)malloc((MAX_PATH*4)*sizeof(wchar_t));
	if(!lpszCommand)
		return FALSE;
	memset(lpszCommand, 0, (MAX_PATH*4)*sizeof(wchar_t));
	if(m_pIpAutoCheckBox->IsSelected())
	{
		wsprintf(szIpCommand, L"/c netsh interface ip set address %s dhcp", lpszConnectName);
		wcscpy_s(lpszCommand, 1024, szIpCommand);
	}
	else
	{		
		LPCTSTR ip = m_pIPEdit->GetText().GetData();
		LPCTSTR mask = m_pMaskEdit->GetText().GetData();
		LPCTSTR gateway = m_pGatewayEdit->GetText().GetData();
		wsprintf(szIpCommand, L"/c netsh interface ip set address %s static %s %s %s", lpszConnectName,ip,mask,gateway);
		wcscpy_s(lpszCommand, 1024, szIpCommand);	
	}
	//dns
	if (m_pDnsAutoCheckBox->IsSelected())
	{
		wsprintf(szDnsCommand, L"&netsh interface ip set dnsservers %s dhcp", lpszConnectName);
		wcscat_s(lpszCommand, 1024, szDnsCommand);
	}
	else
	{
		LPCTSTR dns1 = m_pDns1Edit->GetText().GetData();
		wsprintf(szDnsCommand, L"&netsh interface ip set dnsservers %s static %s", lpszConnectName, dns1);
		wcscat_s(lpszCommand, 1024, szDnsCommand);
		if (!m_pDns2Edit->GetText().IsEmpty())
		{
			wmemset(szDnsCommand, 0, 200);
			LPCTSTR dns2 = m_pDns2Edit->GetText().GetData();
			wsprintf(szDnsCommand, L"&netsh interface ip add dnsservers %s %s", lpszConnectName, dns2);
			wcscat_s(lpszCommand, 1024, szDnsCommand);
		}
	}
	m_pCmdInfo = (PCMD_EXE_INFO)malloc(sizeof(CMD_EXE_INFO));
	memset(m_pCmdInfo, 0, sizeof(CMD_EXE_INFO));
	m_pCmdInfo->hWnd = m_hIPConfigOwner;
	m_pCmdInfo->lpszCommand = lpszCommand;
	//UINT uThreadId;
	//HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,ExeCMDThreadFunc, (void*)m_pCmdInfo, 0, &uThreadId);
	DWORD dwThreadId = 0;
	HANDLE hThread = (HANDLE)CreateThread(NULL, 0, ExeCMDThreadFunc, (LPVOID)m_pCmdInfo, 0, &dwThreadId);
	CloseHandle(hThread);
	return TRUE;
}

BOOL IIPConfig::ResetSolution()
{
	wchar_t szNewSolution[1024] = {0};
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_IPCHANGERPANEL, kIPConfigNewSolution, szNewSolution);
	if(m_pNameEdit)
		m_pNameEdit->SetText(szNewSolution);
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
	{
		m_pDnsAutoCheckBox->SetEnabled(true);
		m_pDnsAutoCheckBox->SetCheck(true);
	}
	if(m_pDns1Edit)
	{
		m_pDns1Edit->SetEnabled(false);
		m_pDns1Edit->SetText(L"");
	}
	if(m_pDns2Edit)
	{
		m_pDns2Edit->SetEnabled(false);
		m_pDns2Edit->SetText(L"");
	}
	return TRUE;
}

LONG CALLBACK IIPConfig::AdapterInfoCallBack(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//list<LPADAPTER_INFO> *plstAdapters = (list<LPADAPTER_INFO> *)lParam;
	//if(!plstAdapters)
	//	return 0;
	return 0;
}

LONG CALLBACK IIPConfig::AdaptersNameCallBack(LPVOID lParam, LPCWSTR lpszName, int nIndex)
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

BOOL IIPConfig::CheckFormValid()
{
	//wchar_t msgTitle[100];
	//Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, L"msg_warn", msgTitle);

	if(!m_pIpAutoCheckBox->IsSelected())
	{
		//判断ip
		LPCTSTR ip = m_pIPEdit->GetText().GetData();
		BOOL isIp = ValidateUtil::IsIPv4(ip);
		if(isIp==FALSE)
		{
			//wchar_t text[100];
			//Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, L"invalid_ipv4_err", text);
			//MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			RDMsgBox(m_hIPConfigOwner, MSG_IPV4_ERR, MSG_WARNING, MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
		//判断子网掩码
		LPCTSTR mask = m_pMaskEdit->GetText().GetData();
		BOOL isMask = ValidateUtil::IsMask(mask);
		if(isMask==FALSE)
		{
			//wchar_t text[100];
			//Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, L"invalid_subnet_mask_err", text);
			//MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			RDMsgBox(m_hIPConfigOwner, MSG_MASK_ERR, MSG_WARNING, MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
		//判断网关
		LPCTSTR gateway = m_pGatewayEdit->GetText().GetData();
		BOOL isGateway = ValidateUtil::IsIPv4(gateway);
		if(isGateway==FALSE)
		{
			//wchar_t text[100];
			//Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, L"invalid_gateway_err", text);
			//MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			RDMsgBox(m_hIPConfigOwner, MSG_GATEWAY_ERR, MSG_WARNING, MB_OK | MB_ICONINFORMATION);
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
			//wchar_t text[100];
			//Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, L"invalid_dns_err", text);
			//MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
			RDMsgBox(m_hIPConfigOwner, MSG_DNS_ERR, MSG_WARNING, MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
		if(!m_pDns2Edit->GetText().IsEmpty())
		{
			LPCTSTR dns2 = m_pDns2Edit->GetText().GetData();
			BOOL isDns2 = ValidateUtil::IsIPv4(dns2);
			if(isDns2==FALSE)
			{
				//wchar_t text[100];
				//Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, L"invalid_dns_err", text);
				//MessageBoxW(NULL,text,msgTitle,MB_OK|MB_ICONWARNING);
				RDMsgBox(m_hIPConfigOwner, MSG_DNS_ERR, MSG_WARNING, MB_OK | MB_ICONINFORMATION);
				return FALSE;
			}
			
		}
	}
	return TRUE;
}
//unsigned IIPConfig::ExeCMDThreadFunc(void * pParams)
DWORD IIPConfig::ExeCMDThreadFunc(void * pParams)
{	
	PCMD_EXE_INFO info = (PCMD_EXE_INFO)pParams;
	HWND hWnd = info->hWnd;

	DuiShowLoading(hWnd, L"Loading...", L"", &info->lpLoader);
	//Utility::ExcuteCommand(info->szCommand);
	Sleep(3000);
	SendMessage(hWnd, WM_CMD_COMPLETE, NULL, NULL);

	return 0;
}

BOOL IIPConfig::ExeCMDComplete()
{
	if (m_pCmdInfo)
	{
		DuiCancelLoading(m_pCmdInfo->lpLoader);
		m_pCmdInfo->lpLoader = NULL;
		if(m_pCmdInfo->lpszCommand)
			free(m_pCmdInfo->lpszCommand);
		free(m_pCmdInfo);
		m_pCmdInfo = NULL;
	}
	return TRUE;
}
