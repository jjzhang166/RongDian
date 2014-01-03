#include "StdAfx.h"
#include "Hosts.h"

// 头部
const wchar_t* const kHostPathText = L"host_path_text";
const wchar_t* const kHostPathEdit = L"host_path_edit";		
const wchar_t* const kHostOpenBtn = L"host_open";
const wchar_t* const kHostOpenTip = L"host_open_tip";
const wchar_t* const kHostBackupBtn = L"host_backup";
const wchar_t* const kHostBackupTip = L"host_backup_tip";
const wchar_t* const kHostLoadBtn = L"host_load";
const wchar_t* const kHostLoadTip = L"host_load_tip";
const wchar_t* const kHostSaveBtn = L"host_save";
const wchar_t* const kHostSaveTip = L"host_save_tip";
const wchar_t* const kHostNewGroupBtn = L"host_new_group";
const wchar_t* const kHostNewGroupTip = L"host_new_group_tip";
const wchar_t* const kHostGroupContainer = L"host_group_container";
//xml路径
const wchar_t* const kHostGroupXmlPath = L"HostGroup.xml";
const wchar_t* const kHostGroupItemXmlPath = L"HostGroupItem.xml";
//动态控件名
const wchar_t* const kHostGroup = L"host_group";
const wchar_t* const kHostGroupVisibleContentBtn = L"host_group_visible_content";
const wchar_t* const kHostGroupTitleEdit = L"host_group_title_edit";
const wchar_t* const kHostGroupNewRowBtn = L"host_group_new_row";
const wchar_t* const kHostGroupSaveBtn = L"host_group_save";
const wchar_t* const kHostGroupDeleteBtn = L"host_group_delete";
const wchar_t* const kHostGroupRowsVLayout = L"host_group_rows";

const wchar_t* const kHostRowVLayout = L"host_row";
const wchar_t* const kHostRowActiveCB = L"host_row_active_cb";
const wchar_t* const kHostRowIPEdit = L"host_row_ip_edit";
const wchar_t* const kHostRowDomainEdit = L"host_row_domain_edit";
const wchar_t* const kHostRowDescEdit = L"host_row_desc_edit";
const wchar_t* const kHostRowSaveBtn = L"host_row_save";


const wchar_t* const kHostDownArrowImage = L"default\\arrow_down.png";
const wchar_t* const kHostUpArrowImage = L"default\\arrow_up.png";

const wchar_t* const kHostGroupLayoutUI = L"HostGroupLayout";
const wchar_t* const kHostGroupDescTopicUI = L"HostGroupDescTopic";
const wchar_t* const kHostGroupDescStateUI = L"HostGroupDescState";
const wchar_t* const kHostGroupDescEditUI = L"HostGroupDescEdit";
const wchar_t* const kHostGroupStateUI = L"HostGroupState";
const wchar_t* const kHostGroupNameUI = L"HostGroupName";
const wchar_t* const kHostGroupNewUI = L"HostGroupNew";
const wchar_t* const kHostGroupDelUI = L"HostGroupDel";
const wchar_t* const kHostGroupSaveUI = L"HostGroupSave";
const wchar_t* const kHostGroupItemsLayoutUI = L"HostGroupItemsLayout";

const wchar_t* const kHostItemLayoutUI = L"HostItemLayout";
const wchar_t* const kHostItemStateUI = L"HostItemState";
const wchar_t* const kHostItemAddrUI = L"HostItemAddr";
const wchar_t* const kHostItemDomainUI = L"HostItemDomain";
const wchar_t* const kHostItemDescUI = L"HostItemDesc";
const wchar_t* const kHostItemSaveUI = L"HostItemSave";
const wchar_t* const kHostItemDelUI = L"HostItemDel";

//
class CHostGroupLayoutUI : public CVerticalLayoutUI
{
public:
	CHostGroupLayoutUI()
	{
		pDescTopicCtrl = NULL;
		pDescStateCtrl = NULL;
		pDescEditCtrl = NULL;
		pStateCtrl = NULL;
		pNameCtrl = NULL;
		pNewCtrl = NULL;
		pDelCtrl = NULL;
		pSaveCtrl = NULL;
		pItemsLayout = NULL;
	};
	~CHostGroupLayoutUI() {};

	LPCTSTR GetClass() const { return kHostGroupLayoutUI; };

	void DescTopicCtrl(CControlUI *pCtrl) { pDescTopicCtrl = pCtrl; };
	void DescStateCtrl(CControlUI *pCtrl) { pDescStateCtrl = pCtrl; };
	void DescEditCtrl(CControlUI *pCtrl) { pDescEditCtrl = pCtrl; };
	void StateCtrl(CControlUI *pCtrl) { pStateCtrl = pCtrl; };
	void NameCtrl(CControlUI *pCtrl) { pNameCtrl = pCtrl; };
	void NewCtrl(CControlUI *pCtrl) { pNewCtrl = pCtrl; };
	void DelCtrl(CControlUI *pCtrl) { pDelCtrl = pCtrl; };
	void SaveCtrl(CControlUI *pCtrl) { pSaveCtrl = pCtrl; };
	void ItemsLayout(CControlUI *pCtrl) { pItemsLayout = pCtrl; };
	CControlUI * DescTopicCtrl() const { return pDescTopicCtrl; };
	CControlUI * DescStateCtrl() const { return pDescStateCtrl; };
	CControlUI * DescEditCtrl() const { return pDescEditCtrl; };
	CControlUI * StateCtrl() const { return pStateCtrl; };
	CControlUI * NameCtrl() const { return pNameCtrl; }
	CControlUI * NewCtrl() const { return pNewCtrl; }
	CControlUI * DelCtrl() const { return pDelCtrl; }
	CControlUI * SaveCtrl() const { return pSaveCtrl; }
	CControlUI * ItemsLayout() const { return pItemsLayout; }

private:
	CControlUI *pDescTopicCtrl;
	CControlUI *pDescStateCtrl;
	CControlUI *pDescEditCtrl;
	CControlUI *pStateCtrl;
	CControlUI *pNameCtrl;
	CControlUI *pNewCtrl;
	CControlUI *pDelCtrl;
	CControlUI *pSaveCtrl;
	CControlUI *pItemsLayout;
};

//
class CHostGroupItemLayoutUI : public CHorizontalLayoutUI
{
public:
	CHostGroupItemLayoutUI()
	{
		pStateCtrl = NULL;
		pAddrCtrl = NULL;
		pDomainCtrl = NULL;
		pDescCtrl = NULL;
		pSaveCtrl = NULL;
		pDelCtrl = NULL;
	};
	~CHostGroupItemLayoutUI() {};

	LPCTSTR GetClass() const { return kHostItemLayoutUI; };

	void StateCtrl(CControlUI *pCtrl) { pStateCtrl = pCtrl; };
	void AddrCtrl(CControlUI *pCtrl) { pAddrCtrl = pCtrl; };
	void DomainCtrl(CControlUI *pCtrl) { pDomainCtrl = pCtrl; };
	void DescCtrl(CControlUI *pCtrl) { pDescCtrl = pCtrl; };
	void SaveCtrl(CControlUI *pCtrl) { pSaveCtrl = pCtrl; };
	void DelCtrl(CControlUI *pCtrl) { pDelCtrl = pCtrl; };
	CControlUI * StateCtrl() const { return pStateCtrl; };
	CControlUI * AddrCtrl() const { return pAddrCtrl; }
	CControlUI * DomainCtrl() const { return pDomainCtrl; }
	CControlUI * DescCtrl() const { return pDescCtrl; }
	CControlUI * SaveCtrl() const { return pSaveCtrl; }
	CControlUI * DelCtrl() const { return pDelCtrl; }

private:
	CControlUI *pStateCtrl;
	CControlUI *pAddrCtrl;
	CControlUI *pDomainCtrl;
	CControlUI *pDescCtrl;
	CControlUI *pSaveCtrl;
	CControlUI *pDelCtrl;
};

//
CHosts::CHosts()
{
	m_groupIndex = 0;
	m_rowIndex = 0;
	m_pHostGroupContainerLayout = NULL;
}

BOOL CHosts::IsCanQuit(HWND /*hWnd*/)
{
	return TRUE;
}

void CHosts::OnQuit()
{

}

BOOL CHosts::OnInit(WPARAM wParam, LPARAM /*lParam*/)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	if(!pManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_pHostPathEdit, CEditUI, pManager, kHostPathEdit)
	FIND_CONTROL_BY_ID(m_pHostGroupContainerLayout, CVerticalLayoutUI, pManager, kHostGroupContainer)

	wchar_t szPath[1024] = {0};
	char szPathA[1024] = {0};
	GetSystemDirectory(szPath, _countof(szPath));
	PathAppend(szPath, L"drivers\\etc\\hosts");
	if(m_pHostPathEdit)
		m_pHostPathEdit->SetText(szPath);
	StrUtil::w2a(szPath, szPathA);
	if(!HostsHelper.Load(szPathA))
	{
		LOG4CPLUS_ERROR(g_Logger, L"Hosts File Load Fialed.");
	}
	else
	{
		InitHosts(pManager);
	}
	return TRUE;
}

BOOL CHosts::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang)
{
	if(!pManager)
		return FALSE;
SET_CONTROL_BEGIN(pManager, lpszLang, LS_HOSTADMINPANEL)
	SET_CONTROL_TEXT2(kHostPathText)
	SET_CONTROL_TEXT_AND_TIP(kHostOpenBtn, kHostOpenTip)
	SET_CONTROL_TEXT_AND_TIP(kHostBackupBtn, kHostBackupTip)
	SET_CONTROL_TEXT_AND_TIP(kHostNewGroupBtn, kHostNewGroupTip)
	SET_CONTROL_TEXT_AND_TIP(kHostLoadBtn, kHostLoadTip)
	SET_CONTROL_TEXT_AND_TIP(kHostSaveBtn, kHostSaveTip)
SET_CONTROL_END()

	return TRUE;
}

CControlUI* CHosts::OnCreateControl(CPaintManagerUI* pManager, LPCTSTR pstrClass, CControlUI* pParent)
{
	enum HOSTS_CTRL // 自定义标识
	{
		HOSTS_NONE = 0,			// Undefine
		// Group Control
		HOSTS_GROUP_DESC_TOPIC,	// kHostGroupDescTopicUI - Label
		HOSTS_GROUP_DESC_STATE,	// kHostGroupDescStateUI - Button
		HOSTS_GROUP_DESC_EDIT,	// kHostGroupDescEditUI - RichEdit
		HOSTS_GROUP_STATE,		// kHostGroupStateUI - CheckBox
		HOSTS_GROUP_NAME,		// kHostGroupNameUI - Edit
		HOSTS_GROUP_NEW,		// kHostGroupNewUI - Button
		HOSTS_GROUP_DEL,		// kHostGroupDelUI - Button
		HOSTS_GROUP_SAVE,		// kHostGroupSaveUI - Button
		HOSTS_GROUP_ITEMS,		// kHostGroupItemsLayoutUI - Layout
		// Item Control
		HOSTS_ITEM_STATE = 21,	// kHostItemStateUI - CheckBox
		HOSTS_ITEM_ADDR,		// kHostItemAddrUI - Edit
		HOSTS_ITEM_DOMAIN,		// kHostItemDomainUI - Edit
		HOSTS_ITEM_DESC,		// kHostItemDescUI - Edit
		HOSTS_ITEM_SAVE,		// kHostItemSaveUI - Button
		HOSTS_ITEM_DEL,			// kHostItemDelUI - Button
	};
	int nSubType = HOSTS_NONE;
	CControlUI* pControl = NULL;
	if(!pManager)
		return pControl;
#pragma region Group Control
	if (wcsicmp(pstrClass, kHostGroupLayoutUI)==0)
	{
		pControl = new CHostGroupLayoutUI();
	}
	else if (wcsicmp(pstrClass, kHostGroupDescTopicUI)==0)
	{
		pControl = new CLabelUI();
		nSubType = HOSTS_GROUP_DESC_TOPIC;
	}
	else if (wcsicmp(pstrClass, kHostGroupDescStateUI)==0)
	{
		pControl = new CButtonUI();
		nSubType = HOSTS_GROUP_DESC_STATE;
	}
	else if (wcsicmp(pstrClass, kHostGroupDescEditUI)==0)
	{
		pControl = new CRichEditUI();
		nSubType = HOSTS_GROUP_DESC_EDIT;
	}
	else if (wcsicmp(pstrClass, kHostGroupStateUI)==0)
	{
		pControl = new CCheckBoxUI();
		nSubType = HOSTS_GROUP_STATE;
	}
	else if (wcsicmp(pstrClass, kHostGroupNameUI)==0)
	{
		pControl = new CEditUI();
		nSubType = HOSTS_GROUP_NAME;
	}
	else if (wcsicmp(pstrClass, kHostGroupNewUI)==0)
	{
		pControl = new CButtonUI();
		nSubType = HOSTS_GROUP_NEW;
	}
	else if (wcsicmp(pstrClass, kHostGroupDelUI)==0)
	{
		pControl = new CButtonUI();
		nSubType = HOSTS_GROUP_DEL;
	}
	else if (wcsicmp(pstrClass, kHostGroupSaveUI)==0)
	{
		pControl = new CButtonUI();
		nSubType = HOSTS_GROUP_SAVE;
	}
	else if (wcsicmp(pstrClass, kHostGroupItemsLayoutUI)==0)
	{
		pControl = new CVerticalLayoutUI();
		nSubType = HOSTS_GROUP_ITEMS;
	}
#pragma endregion Group Control

#pragma region Item Control
	else if (wcsicmp(pstrClass, kHostItemLayoutUI)==0)
	{
		pControl = new CHostGroupItemLayoutUI();
	}
	else if (wcsicmp(pstrClass, kHostItemStateUI)==0)
	{
		pControl = new CCheckBoxUI();
		nSubType = HOSTS_ITEM_STATE;
	}
	else if (wcsicmp(pstrClass, kHostItemAddrUI)==0)
	{
		pControl = new CEditUI();
		nSubType = HOSTS_ITEM_ADDR;
	}
	else if (wcsicmp(pstrClass, kHostItemDomainUI)==0)
	{
		pControl = new CEditUI();
		nSubType = HOSTS_ITEM_DOMAIN;
	}
	else if (wcsicmp(pstrClass, kHostItemDescUI)==0)
	{
		pControl = new CEditUI();
		nSubType = HOSTS_ITEM_DESC;
	}
	else if (wcsicmp(pstrClass, kHostItemSaveUI)==0)
	{
		pControl = new CButtonUI();
		nSubType = HOSTS_ITEM_SAVE;
	}
	else if (wcsicmp(pstrClass, kHostItemDelUI)==0)
	{
		pControl = new CButtonUI();
		nSubType = HOSTS_ITEM_DEL;
	}
#pragma endregion Item Control

	if(pControl && nSubType!=HOSTS_NONE)
	{
		CControlUI *pContainer = pParent;
		while(pContainer && pContainer->GetParent())
			pContainer = pContainer->GetParent();
		if(!pContainer)
		{
			delete pControl;
			pControl = NULL;
			return pControl;
		}
#pragma region Group Control
		if(wcsicmp(pContainer->GetClass(), kHostGroupLayoutUI)==0)
		{
			CHostGroupLayoutUI *pGroupLayout = NULL;
			pGroupLayout = static_cast<CHostGroupLayoutUI *>(pContainer);
			switch(nSubType)
			{
			case HOSTS_GROUP_DESC_TOPIC:
				pGroupLayout->DescTopicCtrl(pControl);
				break;
			case HOSTS_GROUP_DESC_STATE:
				pGroupLayout->DescStateCtrl(pControl);
				break;
			case HOSTS_GROUP_DESC_EDIT:
				pGroupLayout->DescEditCtrl(pControl);
				break;
			case HOSTS_GROUP_STATE:
				pGroupLayout->StateCtrl(pControl);
				break;
			case HOSTS_GROUP_NAME:
				pGroupLayout->NameCtrl(pControl);
				break;
			case HOSTS_GROUP_NEW:
				pGroupLayout->NewCtrl(pControl);
				break;
			case HOSTS_GROUP_DEL:
				pGroupLayout->DelCtrl(pControl);
				break;
			case HOSTS_GROUP_SAVE:
				pGroupLayout->SaveCtrl(pControl);
				break;
			case HOSTS_GROUP_ITEMS:
				pGroupLayout->ItemsLayout(pControl);
				break;
			}
		}
#pragma endregion Group Control

#pragma region Item Control
		else if(wcsicmp(pContainer->GetClass(), kHostItemLayoutUI)==0)
		{
			CHostGroupItemLayoutUI *pItemLayout = NULL;
			pItemLayout = static_cast<CHostGroupItemLayoutUI *>(pContainer);
			switch(nSubType)
			{
			case HOSTS_ITEM_STATE:
				pItemLayout->StateCtrl(pControl);
				break;
			case HOSTS_ITEM_ADDR:
				pItemLayout->AddrCtrl(pControl);
				break;
			case HOSTS_ITEM_DOMAIN:
				pItemLayout->DomainCtrl(pControl);
				break;
			case HOSTS_ITEM_DESC:
				pItemLayout->DescCtrl(pControl);
				break;
			case HOSTS_ITEM_SAVE:
				pItemLayout->SaveCtrl(pControl);
				break;
			case HOSTS_ITEM_DEL:
				pItemLayout->DelCtrl(pControl);
				break;
			}
		}
#pragma endregion Item Control
	}
	return pControl;
}

void CHosts::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kHostOpenBtn)
	{
		OpenHostsFile(hWnd);
		bHandled = TRUE;
	}
	else if(sCtrlName==kHostBackupBtn)
	{
		BackupHostsFile(hWnd);
		bHandled = TRUE;
	}
	else if(sCtrlName==kHostNewGroupBtn)
	{
		CreateGroup(NULL, NULL, NULL);
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), L"host_group_state")!=NULL)
	{
		LPCWSTR lpszUserData = msg.pSender->GetUserData();
		if(lpszUserData)
		{
			CControlUI *pControl = NULL;
			FIND_CONTROL_BY_ID(pControl, CControlUI, pManager, lpszUserData);
			if(pControl)
			{
				if(pControl->IsVisible())
					pControl->SetVisible(false);
				else
					pControl->SetVisible();
			}
		}
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), L"host_group_desc_state")!=NULL)
	{
		LPCWSTR lpszUserData = msg.pSender->GetUserData();
		if(lpszUserData)
		{
			CControlUI *pControl = NULL;
			FIND_CONTROL_BY_ID(pControl, CControlUI, pManager, lpszUserData);
			if(pControl)
			{
				if(pControl->IsVisible())
					pControl->SetVisible(false);
				else
					pControl->SetVisible();
			}
		}
		bHandled = TRUE;
	}
}

void CHosts::OnItemSelect(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg)
{
	CComboUI* pCombo = (CComboUI*)msg.pSender;
	CDuiString sItemName = pCombo->GetName();
}

BOOL CHosts::InitHosts(CPaintManagerUI* pManager)
{
	BOOL bRet = FALSE;
	wchar_t szGroupId[256] = {0};
	wchar_t szSection[1024] = {0}, szGroupDesc[1024] = {0};
	wchar_t szAddr[64] = {0}, szDomain[256] = {0}, szItemDesc[1024] = {0};
	LPHOSTS_INFO pGroup = HostsHelper.pHosts;
	PHOSTS_ITEM pItem = NULL;
	if(!m_pHostGroupContainerLayout)
	{
		LOG4CPLUS_ERROR(g_Logger, L"CHosts::InitHosts - Can't Find Group Container Layout.");
		return bRet;
	}
	for(int i=0; i<HostsHelper.nCount; i++)
	{
		assert(pGroup!=NULL);
		swprintf(szGroupId, L"%d", i);
		StrUtil::a2w(pGroup->szSection, szSection);
		StrUtil::a2w(pGroup->szDesc, szGroupDesc);
		if(!CreateGroup(pManager, szGroupId, szSection, szGroupDesc))
		{
			LOG4CPLUS_ERROR_FMT(g_Logger, L"CHosts::InitHosts Create Group %s Failed.", szSection);
			break;
		}
		LOG4CPLUS_INFO_FMT(g_Logger, L"CHosts::InitHosts Create Group %s.", szSection);
		pItem = pGroup->pItem;
		for(int j=0; j<pGroup->nCount; j++)
		{
			assert(pItem!=NULL);
			StrUtil::a2w(pItem->szAddr, szAddr);
			StrUtil::a2w(pItem->szDomain, szDomain);
			StrUtil::a2w(pItem->szDesc, szItemDesc);
			if(!CreateItem(pManager, szGroupId, szAddr, szDomain, szItemDesc))
			{
				LOG4CPLUS_ERROR_FMT(g_Logger, L"CHosts::InitHosts Create Item %s Failed.", szAddr);
			}
			LOG4CPLUS_INFO_FMT(g_Logger, L"CHosts::InitHosts Create Item %s.", szAddr);
			memset(szAddr, 0, sizeof(szAddr));
			memset(szDomain, 0, sizeof(szDomain));
			memset(szItemDesc, 0, sizeof(szItemDesc));
			pItem = pItem->pNext;
		}
		memset(szSection, 0, sizeof(szSection));
		memset(szGroupDesc, 0, sizeof(szGroupDesc));
		pGroup = pGroup->pNext;
	}
	//m_pHostGroupContainerLayout->EstimateSize()
	return bRet;
}

BOOL CHosts::OpenHostsFile(HWND hWnd)
{
	BOOL bRet = FALSE;
	if(!m_pHostPathEdit)
		return bRet;
	LPCWSTR lpszHostsPath = m_pHostPathEdit->GetText();
	if(wcslen(lpszHostsPath))
		SHHelper::OpenFileEx(hWnd, L"notepad.exe", lpszHostsPath);
	return bRet;
}

BOOL CHosts::BackupHostsFile(HWND hWnd)
{
	BOOL bRet = FALSE;
	if(!m_pHostPathEdit)
		return bRet;
	LPCWSTR lpszHostsPath = m_pHostPathEdit->GetText();
	if(wcslen(lpszHostsPath))
	{
		time_t t;
		time(&t);
		struct tm *now;
		now = localtime(&t);
		wchar_t szBackupPath[1024] = {0}, szHosts[64] = {0};
		wsprintf(szHosts, L"hosts_%d%02d%02d%02d%02d%02d", now->tm_year+1900,now->tm_mon,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		wcscpy(szBackupPath, g_szHostsPath);
		PathAppend(szBackupPath, szHosts);
		SHHelper::CopyFile(hWnd, lpszHostsPath, szBackupPath);
		RDMsgBox(hWnd, MSG_DONE, MSG_SUCCESS, MB_OK);
	}
	return bRet;
}

BOOL CHosts::CreateNewHosts(HWND hWnd)
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CHosts::CreateGroup(CPaintManagerUI* pManager, const wchar_t* pszGroupId, const wchar_t* pszGroupName, const wchar_t* pszDesc /*= NULL*/)
{
	BOOL bRet = FALSE;
	if(!m_pHostGroupContainerLayout)
	{
		LOG4CPLUS_ERROR(g_Logger, L"CHosts::CreateGroup Can't Find Host Group Container.");
		return bRet;
	}
	CChildLayoutUI *pChildLayout = new CChildLayoutUI();
	if(!pChildLayout)
		return bRet;
	wchar_t szChildLayout[64] = {0};
	swprintf(szChildLayout, L"child_host_group_%s", pszGroupId);
	pChildLayout->SetName(szChildLayout);
	pChildLayout->SetChildLayoutXML(kHostGroupXmlPath, (DuiLib::IDialogBuilderCallback *)g_pMainFrame);
	m_pHostGroupContainerLayout->AddAt(pChildLayout, m_pHostGroupContainerLayout->GetCount() - 1);

	// Reset SubControl Name
	CHostGroupLayoutUI *pGroupLayout = (CHostGroupLayoutUI *)pChildLayout->GetItemAt(0);
	assert(pGroupLayout!=NULL);
	wchar_t szHostGroup[64] = {0}, szDescTopic[64] = {0}, szDescState[64] = {0}, szDescEdit[64] = {0}, szState[64] = {0}, 
		szName[64] = {0}, szNew[64] = {0}, szSave[64] = {0}, szDel[64] = {0}, szItems[64] = {0};
	swprintf(szHostGroup, L"host_group_%s", pszGroupId);
	swprintf(szDescTopic, L"host_group_desc_topic_%s", pszGroupId);
	swprintf(szDescState, L"host_group_desc_state_%s", pszGroupId);
	swprintf(szDescEdit, L"host_group_desc_edit_%s", pszGroupId);
	swprintf(szState, L"host_group_state_%s", pszGroupId);
	swprintf(szName, L"host_group_name_%s", pszGroupId);
	swprintf(szNew, L"host_group_new_%s", pszGroupId);
	swprintf(szSave, L"host_group_save_%s", pszGroupId);
	swprintf(szDel, L"host_group_del_%s", pszGroupId);
	swprintf(szItems, L"host_group_items_%s", pszGroupId);
	pGroupLayout->SetName(szHostGroup);
	CControlUI *pSubControl = NULL;
	pSubControl = pGroupLayout->DescTopicCtrl();
	if(pSubControl)
		pSubControl->SetName(szDescTopic);
	pSubControl = pGroupLayout->DescStateCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDescState);
		pSubControl->SetUserData(szDescEdit);
	}
	pSubControl = pGroupLayout->DescEditCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDescEdit);
		if(pszGroupName)
			pSubControl->SetText(pszDesc);
	}
	pSubControl = pGroupLayout->StateCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szState);
		pSubControl->SetUserData(szItems);
	}
	pSubControl = pGroupLayout->NameCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szName);
		if(pszGroupName)
			pSubControl->SetText(pszGroupName);
	}
	pSubControl = pGroupLayout->NewCtrl();
	if(pSubControl)
		pSubControl->SetName(szNew);
	pSubControl = pGroupLayout->SaveCtrl();
	if(pSubControl)
		pSubControl->SetName(szSave);
	pSubControl = pGroupLayout->DelCtrl();
	if(pSubControl)
		pSubControl->SetName(szDel);
	pSubControl = pGroupLayout->ItemsLayout();
	if(pSubControl)
	{
		pSubControl->SetName(szItems);
		pSubControl->SetUserData(L"0"); // for Hosts Item's Index
	}
	pManager->UpdateControls(pGroupLayout);
#ifdef _DEBUG
	pGroupLayout = NULL;
	FIND_CONTROL_BY_ID(pGroupLayout, CHostGroupLayoutUI, pManager, szHostGroup);
	assert(pGroupLayout!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szState);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szName);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szNew);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szSave);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szDel);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szItems);
	assert(pSubControl!=NULL);
#endif
	bRet = TRUE;
	return bRet;
}

BOOL CHosts::CreateItem(CPaintManagerUI* pManager, const wchar_t* pszGroupId, const wchar_t* pszAddr, const wchar_t* pszDomain, const wchar_t* pszDesc /*= NULL*/)
{
	BOOL bRet = FALSE;
	wchar_t szItems[64] = {0};
	swprintf(szItems, L"host_group_items_%s", pszGroupId);
	CVerticalLayoutUI *pGroupItems = NULL;
	FIND_CONTROL_BY_ID(pGroupItems, CVerticalLayoutUI, pManager, szItems);
	assert(pGroupItems!=NULL);
	int nItemIndex = _wtoi(pGroupItems->GetUserData());
	CChildLayoutUI *pChildLayout = new CChildLayoutUI();
	if(!pChildLayout)
		return bRet;
	wchar_t szChildLayout[64] = {0};
	swprintf(szChildLayout, L"child_host_items_%s_%d", pszGroupId, nItemIndex);
	pChildLayout->SetName(szChildLayout);
	pChildLayout->SetChildLayoutXML(kHostGroupItemXmlPath, (DuiLib::IDialogBuilderCallback *)g_pMainFrame);
	pGroupItems->AddAt(pChildLayout, pGroupItems->GetCount());
	wchar_t szIndex[32] = {0};
	swprintf(szIndex, L"%d", nItemIndex+1);
	pGroupItems->SetUserData(szIndex); // Update Host Item's Index

	// Reset SubControl Name
	CHostGroupItemLayoutUI *pItemLayout = (CHostGroupItemLayoutUI *)pChildLayout->GetItemAt(0);
	assert(pItemLayout!=NULL);
	pChildLayout->SetFixedHeight(pItemLayout->GetFixedHeight());

	wchar_t szHostItem[64] = {0}, szState[64] = {0}, szAddr[64] = {0}, szDomain[64] = {0}, 
		szDesc[64] = {0}, szSave[64] = {0}, szDel[64] = {0};
	swprintf(szHostItem, L"host_item_%s_%d", pszGroupId, nItemIndex);
	swprintf(szState, L"host_item_state_%s_%d", pszGroupId, nItemIndex);
	swprintf(szAddr, L"host_item_addr_%s", pszGroupId, nItemIndex);
	swprintf(szDomain, L"host_item_domain_%s", pszGroupId, nItemIndex);
	swprintf(szDesc, L"host_item_desc_%s", pszGroupId, nItemIndex);
	swprintf(szSave, L"host_item_save_%s", pszGroupId, nItemIndex);
	swprintf(szDel, L"host_item_del_%s", pszGroupId, nItemIndex);
	pItemLayout->SetName(szHostItem);
	CControlUI *pSubControl = NULL;
	pSubControl = pItemLayout->StateCtrl();
	if(pSubControl)
		pSubControl->SetName(szState);
	pSubControl = pItemLayout->AddrCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szAddr);
		if(pszAddr)
			pSubControl->SetText(pszAddr);
	}
	pSubControl = pItemLayout->DomainCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDomain);
		if(pszDomain)
			pSubControl->SetText(pszDomain);
	}
	pSubControl = pItemLayout->DescCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDesc);
		if(pszDesc)
			pSubControl->SetText(pszDesc);
	}
	pSubControl = pItemLayout->SaveCtrl();
	if(pSubControl)
		pSubControl->SetName(szSave);
	pSubControl = pItemLayout->DelCtrl();
	if(pSubControl)
		pSubControl->SetName(szDel);
	pManager->UpdateControls(pItemLayout);
#ifdef _DEBUG
	pItemLayout = NULL;
	FIND_CONTROL_BY_ID(pItemLayout, CHostGroupItemLayoutUI, pManager, szHostItem);
	assert(pItemLayout!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szState);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szAddr);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szDomain);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szDesc);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szSave);
	assert(pSubControl!=NULL);
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szDel);
	assert(pSubControl!=NULL);
#endif
	bRet = TRUE;

	return bRet;
}