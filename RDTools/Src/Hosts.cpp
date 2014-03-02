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
const wchar_t* const kHostTipText = L"host_tip_text";
const wchar_t* const kHostNewItemTip = L"host_new_item_tip";
const wchar_t* const kHostItemDeleteTip = L"host_item_delete_tip";
const wchar_t* const kHostGroupDeleteTip = L"host_group_delete_tip";
const wchar_t* const kHostGroupDescShowTip = L"host_group_desc_show_tip";
const wchar_t* const kHostGroupDescHideTip = L"host_group_desc_hide_tip";
const wchar_t* const kHostItemsShowTip = L"host_items_show_tip";
const wchar_t* const kHostItemsHideTip = L"host_items_hide_tip";

//xml路径
const wchar_t* const kHostGroupXmlPath = L"HostGroup.xml";
const wchar_t* const kHostGroupItemXmlPath = L"HostGroupItem.xml";

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

const wchar_t* const kChildHostGroupPrefix = L"child_host_group_";
const wchar_t* const kHostGroupPrefix = L"host_group_";
const wchar_t* const kHostGroupDescTopicPrefix = L"host_group_desc_topic_";
const wchar_t* const kHostGroupDescStatePrefix = L"host_group_desc_state_";
const wchar_t* const kHostGroupDescEditPrefix = L"host_group_desc_edit_";
const wchar_t* const kHostGroupStatePrefix = L"host_group_state_";
const wchar_t* const kHostGroupNamePrefix = L"host_group_name_";
const wchar_t* const kHostGroupNewPrefix = L"host_group_new_";
const wchar_t* const kHostGroupSavePrefix = L"host_group_save_";
const wchar_t* const kHostGroupDelPrefix = L"host_group_del_";
const wchar_t* const kHostGroupItemsPrefix = L"host_group_items_";

const wchar_t* const kChildHostItemPrefix = L"child_host_item_";
const wchar_t* const kHostItemPrefix = L"host_item_";
const wchar_t* const kHostItemStatePrefix = L"host_item_state_";
const wchar_t* const kHostItemAddrPrefix = L"host_item_addr_";
const wchar_t* const kHostItemDomainPrefix = L"host_item_domain_";
const wchar_t* const kHostItemDescPrefix = L"host_item_desc_";
const wchar_t* const kHostItemSavePrefix = L"host_item_save_";
const wchar_t* const kHostItemDelPrefix = L"host_item_del_";

const wchar_t* const kHostSectionPrefix = L"Hosts Group ";

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
	m_pHostPathEdit = NULL;
	m_pHostGroupContainerLayout = NULL;
	m_pHostTipText = NULL;
	m_bModified = FALSE;
}

BOOL CHosts::IsCanQuit(HWND hWnd, CPaintManagerUI* pManager)
{
//#pragma message("CHosts::IsCanQuit － 检测数据是否有更新，保存数据到hosts文件")
	if(m_bModified)
	{
//#pragma message("CHosts::IsCanQuit － 数据有更新，但为保存，提示用户是否保存当前修改")
		if(RDMsgBox(hWnd, MSG_HOST_SAVE_MSG, MSG_WARNING, MB_YESNO)==IDYES)
			SaveHostsFile(hWnd, pManager);
	}
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
	FIND_CONTROL_BY_ID(m_pHostTipText, CLabelUI, pManager, kHostTipText)

	wchar_t szPath[1024] = {0};
	char szPathA[1024] = {0};
	GetSystemDirectory(szPath, _countof(szPath));
	PathAppend(szPath, L"drivers\\etc\\hosts");
	if(m_pHostPathEdit)
		m_pHostPathEdit->SetText(szPath);
	StrUtil::w2a(szPath, szPathA);
	if(!m_HostsHelper.Load(szPathA))
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
		pControl = new CCheckBoxUI();
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
		OpenHostsFile(hWnd, pManager);
		bHandled = TRUE;
	}
	else if(sCtrlName==kHostBackupBtn)
	{
		BackupHostsFile(hWnd, pManager);
		bHandled = TRUE;
	}
	else if(sCtrlName==kHostNewGroupBtn)
	{
		NewHostsGroup(hWnd, pManager);
		bHandled = TRUE;
	}
	else if(sCtrlName==kHostLoadBtn)
	{
		LoadHostsFile(hWnd, pManager);
		bHandled = TRUE;
	}
	else if(sCtrlName==kHostSaveBtn)
	{
		SaveHostsFile(hWnd, pManager);
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostGroupStatePrefix)!=NULL)
	{
		ShowHostItems(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostGroupDescStatePrefix)!=NULL)
	{
		ShowHostDesc(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostGroupNewPrefix)!=NULL)
	{
		NewHostItem(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostGroupDelPrefix)!=NULL)
	{
		DelHostGroup(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostItemDelPrefix)!=NULL)
	{
		DelHostItem(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostItemStatePrefix)!=NULL)
	{
		ToggleHostItem(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
	}
}

void CHosts::OnTextChanged(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(wcsstr(sCtrlName.GetData(), kHostGroupNamePrefix)!=NULL)
	{
		UpdateHostGroupName(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
		m_bModified = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostGroupDescEditPrefix)!=NULL)
	{
#pragma message("CHosts::OnTextChanged - Duilib richedit 无响应，待解决")
		UpdateHostGroupDesc(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
		m_bModified = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostItemAddrPrefix)!=NULL)
	{
		UpdateHostItemAddr(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
		m_bModified = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostItemDomainPrefix)!=NULL)
	{
		UpdateHostItemDomain(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
		m_bModified = TRUE;
	}
	else if(wcsstr(sCtrlName.GetData(), kHostItemDescPrefix)!=NULL)
	{
		UpdateHostItemDesc(hWnd, pManager, msg.pSender);
		bHandled = TRUE;
		m_bModified = TRUE;
	}
}

BOOL CHosts::InitHosts(CPaintManagerUI* pManager)
{
	BOOL bRet = FALSE;
//	int utf8Type = 0;
	wchar_t szGroupId[256] = {0};
	wchar_t szSection[1024] = {0}, szGroupDesc[1024] = {0};
	wchar_t szAddr[64] = {0}, szDomain[256] = {0}, szItemDesc[1024] = {0};
	LPHOSTS_INFO pGroup = m_HostsHelper.m_pHosts;
	PHOSTS_ITEM pItem = NULL;
	if(!m_pHostGroupContainerLayout)
	{
		LOG4CPLUS_ERROR(g_Logger, L"CHosts::InitHosts - Can't Find Group Container Layout.");
		return bRet;
	}
	m_pHostGroupContainerLayout->SetUserData(L"0"); // 分组id起始值，用于HostsGroup分组id命名
	for(int i=0; i<m_HostsHelper.m_nCount; i++)
	{
		assert(pGroup!=NULL);
		StrUtil::a2w(pGroup->szSection, szSection);
		StrUtil::a2w(pGroup->szId, szGroupId);
//		utf8Type = StrUtil::is_utf8(pGroup->szDesc);
//		if(utf8Type==UTF8_BOM||utf8Type==UTF8_NOBOM)
//		{
//			StrUtil::u82u(pGroup->szDesc,szGroupDesc);
//		}
//		else
//		{
			StrUtil::a2w(pGroup->szDesc, szGroupDesc);
//		}
		
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
//			utf8Type = StrUtil::is_utf8(pItem->szDesc);
//			if(utf8Type==UTF8_BOM||utf8Type==UTF8_NOBOM)
//			{
//				StrUtil::u82u(pItem->szDesc, szItemDesc);
//			}
//			else
//			{
				StrUtil::a2w(pItem->szDesc, szItemDesc);
//			}
			if(!CreateItem(pManager, pItem->bActive,szGroupId, szAddr, szDomain, szItemDesc))
			{
				LOG4CPLUS_ERROR_FMT(g_Logger, L"CHosts::InitHosts Create Item %s Failed.", szAddr);
			}
			LOG4CPLUS_INFO_FMT(g_Logger, L"CHosts::InitHosts Create Item %s.", szAddr);
			memset(szAddr, 0, sizeof(szAddr));
			memset(szDomain, 0, sizeof(szDomain));
			memset(szItemDesc, 0, sizeof(szItemDesc));
			pItem = pItem->pNext;
		}
		AdjustGroupHeight(pManager, szGroupId, TRUE);
		memset(szSection, 0, sizeof(szSection));
		memset(szGroupDesc, 0, sizeof(szGroupDesc));
		pGroup = pGroup->pNext;
	}
	bRet = TRUE;
	return bRet;
}

BOOL CHosts::OpenHostsFile(HWND hWnd, CPaintManagerUI* /*pManager*/)
{
	BOOL bRet = FALSE;
	if(!m_pHostPathEdit)
		return bRet;
	LPCWSTR lpszHostsPath = m_pHostPathEdit->GetText();
	if(wcslen(lpszHostsPath))
		SHHelper::OpenFileEx(hWnd, L"notepad.exe", lpszHostsPath);
	return bRet;
}

BOOL CHosts::BackupHostsFile(HWND hWnd, CPaintManagerUI* /*pManager*/)
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
		wsprintf(szHosts, L"hosts_%d%02d%02d%02d%02d%02d", now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		wcscpy(szBackupPath, g_szHostsPath);
		PathAppend(szBackupPath, szHosts);
		SHHelper::CopyFile(hWnd, lpszHostsPath, szBackupPath);
		RDMsgBox(hWnd, MSG_BACKEDUP_TO,szBackupPath, MSG_SUCCESS, MB_OK);
	}
	return bRet;
}

BOOL CHosts::NewHostsGroup(HWND hWnd, CPaintManagerUI* pManager)
{
	BOOL bRet = FALSE;
	CDuiString sNewGroupIndex = m_pHostGroupContainerLayout->GetUserData();
	wchar_t szGroupName[64] = {0};
	char szGroupNameA[64] = {0};
	swprintf(szGroupName, L"%s%s", kHostSectionPrefix, sNewGroupIndex.GetData());
	StrUtil::w2a(szGroupName, szGroupNameA);
	if(!m_HostsHelper.AddSection(szGroupNameA))
	{
//#pragma message("CHosts::NewHostsGroup - 添加分组数据失败，需添加提示信息")
		RDMsgBox(hWnd, MSG_CREATE_HOST_GROUP_ERR, MSG_ERR, MB_OK);
		return bRet;
	}
	bRet = CreateGroup(pManager, sNewGroupIndex.GetData(), NULL);
	if(!bRet)
	{
//#pragma message("CHosts::NewHostsGroup - 添加分组到UI失败，需添加提示信息")
		RDMsgBox(hWnd, MSG_CREATE_HOST_GROUP_ERR, MSG_ERR, MB_OK);
		return bRet;
	}
	AdjustGroupHeight(pManager, sNewGroupIndex);
	m_bModified = TRUE;
	return bRet;
}

BOOL CHosts::LoadHostsFile(HWND hWnd, CPaintManagerUI* pManager)
{
	BOOL bRet = FALSE;
	if(m_bModified)
	{
//#pragma message("CHosts::LoadHostsFile － 当前hosts被更新但为保存，提示用户是否保存当前修改")
		if(RDMsgBox(hWnd, MSG_HOST_SAVE_MSG, MSG_WARNING, MB_YESNO)==IDYES)
			SaveHostsFile(hWnd, pManager);
	}
//#pragma message("Hosts加载：CHosts::LoadHostsFile － 加载hosts文件数据到RDTools")
	wchar_t szPath[1024] = {0};
	char szPathA[1024] ={0};
	bRet = SHHelper::SelectFile(hWnd, szPath, L"All(*.*)\0*.*\0\0");
	if(!bRet)
		return bRet;
	StrUtil::w2a(szPath, szPathA);
	assert(m_pHostGroupContainerLayout!=NULL);
	if(!m_HostsHelper.Load(szPathA))
	{
//#pragma message("CHosts::LoadHostsFile - 添加加载hosts文件错误提示")
		RDMsgBox(hWnd, MSG_LOAD_HOST_ERR, MSG_ERR, MB_OK);
		return FALSE;
	}
	m_pHostGroupContainerLayout->RemoveAll();
	CControlUI *pBlank = new CControlUI();
	assert(pBlank!=NULL);
	pBlank->SetFixedHeight(1);
	m_pHostGroupContainerLayout->Add(pBlank);
	bRet = InitHosts(pManager);
	if(!bRet)
	{
//#pragma message("CHosts::LoadHostsFile - 添加初始化hosts文件错误提示")
		RDMsgBox(hWnd, MSG_LOAD_HOST_ERR, MSG_ERR, MB_OK);
		return bRet;
	}
	if(m_pHostPathEdit)
		m_pHostPathEdit->SetText(szPath);
	bRet = TRUE;
	m_bModified = FALSE; // 重新加载hosts文件，重置修改标识
	return bRet;
}

BOOL CHosts::SaveHostsFile(HWND hWnd, CPaintManagerUI* /*pManager*/)
{
	BOOL bRet = FALSE;
//#pragma message("CHosts::SaveHostsFile － 保存hosts数据到文件")
	assert(m_pHostPathEdit!=NULL);
	CDuiString strPath = m_pHostPathEdit->GetText();
	char szPathA[1024];
	StrUtil::w2a(strPath.GetData(), szPathA);
	if(!m_HostsHelper.SaveAs(szPathA))
	{
//#pragma message("CHosts::SaveHostsFile - 保存数据到hosts文件失败，需添加提示信息")
		RDMsgBox(hWnd, MSG_DONE, MSG_FAILED, MB_OK);
	}
	else
	{
//#pragma message("CHosts::SaveHostsFile - 保存数据到hosts文件完成，需添加提示信息")
		RDMsgBox(hWnd, MSG_DONE, MSG_SUCCESS, MB_OK);
		bRet = TRUE;
		m_bModified = FALSE;
	}
	return bRet;
}

BOOL CHosts::ShowHostItems(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	CDuiString sCtrlName = pSender->GetName();
	LPCWSTR lpszUserData = pSender->GetUserData();
	if(lpszUserData)
	{
		CControlUI *pControl = NULL;
		LPCWSTR pszIndex = wcsrchr(sCtrlName.GetData(), L'_');
		wchar_t szIndex[64] = {0};
		assert(wcslen(pszIndex)>=2);
		wcscpy(szIndex, &pszIndex[1]);
		FIND_CONTROL_BY_ID(pControl, CControlUI, pManager, lpszUserData);
		if(pControl)
		{
			if(pControl->IsVisible())
			{
				pControl->SetVisible(false);
				SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSender,kHostItemsShowTip)
			}
			else
			{
				pControl->SetVisible();
				SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSender,kHostItemsHideTip)
			}
			AdjustGroupHeight(pManager, szIndex);
		}
	}
	return TRUE;
}

BOOL CHosts::ShowHostDesc(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	CDuiString sCtrlName = pSender->GetName();
	LPCWSTR lpszUserData = pSender->GetUserData();
	if(lpszUserData)
	{
		CControlUI *pControl = NULL;
		FIND_CONTROL_BY_ID(pControl, CControlUI, pManager, lpszUserData);
		if(pControl)
		{
			LPCWSTR pszIndex = wcsrchr(sCtrlName.GetData(), L'_');
			wchar_t szIndex[64] = {0};
			assert(wcslen(pszIndex)>=2);
			wcscpy(szIndex, &pszIndex[1]);
			if(pControl->IsVisible())
			{
				pControl->SetVisible(false);
				SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSender,kHostGroupDescShowTip)
			}
			else
			{
				pControl->SetVisible();
				SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSender,kHostGroupDescHideTip);
			}
			AdjustGroupHeight(pManager, szIndex);
		}
	}
	return TRUE;
}

BOOL CHosts::NewHostItem(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	LPCWSTR lpszUserData = pSender->GetUserData();
	CreateItem(pManager,false,lpszUserData, L"", L"");
//#pragma message("CHosts::NewHostItem - 节点初始IP、Domain均为空，且需同步hosts内存数据")
	char szGroupId[64] = {0};
	StrUtil::w2a(lpszUserData, szGroupId);
	m_HostsHelper.AddItemById(szGroupId, "", "");
	AdjustGroupHeight(pManager, lpszUserData);
	m_bModified = TRUE;
	return TRUE;
}

BOOL CHosts::DelHostGroup(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	LPCWSTR lpszUserData = pSender->GetUserData();
	CChildLayoutUI *pChildLayout = NULL;
	wchar_t szChildLayout[64] = {0};
	swprintf(szChildLayout, L"%s%s", kChildHostGroupPrefix, lpszUserData);
	FIND_CONTROL_BY_ID(pChildLayout, CChildLayoutUI, pManager, szChildLayout);
	assert(m_pHostGroupContainerLayout!=NULL);
	assert(pChildLayout!=NULL);

//#pragma message("Hosts删除分组：CHosts::DelHostGroup - 需添加同步hosts文件数据代码")
	wchar_t szGroupNameId[64] = {0}, szGroupName[1024] = {0};
	char szSection[1024] = {0};
	swprintf(szGroupNameId, L"%s%s", kHostGroupNamePrefix, lpszUserData);
	CControlUI *pGroupName = NULL;
	FIND_CONTROL_BY_ID(pGroupName, CControlUI, pManager, szGroupNameId);
	assert(pGroupName!=NULL);
	wcscpy(szGroupName, pGroupName->GetText().GetData());
	StrUtil::w2a(szGroupName, szSection);
	m_HostsHelper.DelSection(szSection);

	m_pHostGroupContainerLayout->Remove(pChildLayout);
	m_bModified = TRUE;
	return TRUE;
}

BOOL CHosts::DelHostItem(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	CDuiString sCtrlName = pSender->GetName();
	CDuiString sUserData = pSender->GetUserData();
	CChildLayoutUI *pChildLayout = NULL;
	FIND_CONTROL_BY_ID(pChildLayout, CChildLayoutUI, pManager, sUserData.GetData());
	assert(pChildLayout!=NULL);
	LPCWSTR pszGroupId = pChildLayout->GetUserData();
//#pragma message("Hosts删除节点：CHosts::DelHostItem - 需添加同步hosts文件数据代码")
	wchar_t szItems[64] = {0};
	swprintf(szItems, L"%s%s", kHostGroupItemsPrefix, pszGroupId);
	CVerticalLayoutUI *pGroupItems = NULL;
	FIND_CONTROL_BY_ID(pGroupItems, CVerticalLayoutUI, pManager, szItems);
	assert(pGroupItems!=NULL);
	
	LPCWSTR pszItemIndex = wcsrchr(sUserData.GetData(), L'_');
	wchar_t szItemIndex[64] = {0};
	assert(wcslen(pszItemIndex)>=2);
	wcscpy(szItemIndex, &pszItemIndex[1]);

	char* szGroupId = StrUtil::w2a(pszGroupId);
	char* szIndex = StrUtil::w2a(szItemIndex);
	BOOL bRet = m_HostsHelper.DelItemById(szGroupId, szIndex);
	delete[] szGroupId;
	delete[] szIndex;
	
	pGroupItems->Remove(pChildLayout);
	AdjustGroupHeight(pManager, pszGroupId);
	m_bModified = TRUE;
	return bRet;
}

BOOL CHosts::ToggleHostItem(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	CDuiString sCtrlName = pSender->GetName();
	CDuiString sUserData = pSender->GetUserData();
	CChildLayoutUI *pChildLayout = NULL;
	FIND_CONTROL_BY_ID(pChildLayout, CChildLayoutUI, pManager, sUserData.GetData());
	assert(pChildLayout!=NULL);
	LPCWSTR pszGroupId = pChildLayout->GetUserData();

	//item index
	LPCWSTR pszItemIndex = wcsrchr(sUserData.GetData(), L'_');
	wchar_t szItemIndex[64] = {0};
	assert(wcslen(pszItemIndex)>=2);
	wcscpy(szItemIndex, &pszItemIndex[1]);

	char* szGroupId = StrUtil::w2a(pszGroupId);
	char* szIndex = StrUtil::w2a(szItemIndex);
	LPHOSTS_ITEM item = m_HostsHelper.FindItemById(szGroupId,szIndex);
	delete[] szGroupId;
	delete[] szIndex;
	
	item->bActive = (!item->bActive);
	m_bModified = TRUE;
	return TRUE;
}

BOOL CHosts::UpdateHostGroupName(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	CDuiString strId = pSender->GetUserData();
	CDuiString strNewName = pSender->GetText();
	char szId[64] = {0}, szNewSection[1024] = {0};
	StrUtil::w2a(strId.GetData(), szId);
	PHOSTS_INFO pHostInfo = m_HostsHelper.FindSectionById(szId);
	assert(pHostInfo!=NULL);
	StrUtil::w2a(strNewName.GetData(), szNewSection);
	memset(pHostInfo->szSection, 0, sizeof(pHostInfo->szSection));
	strcpy(pHostInfo->szSection, szNewSection);
	return TRUE;
}

BOOL CHosts::UpdateHostGroupDesc(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	CDuiString strDesc = pSender->GetText();
	CDuiString strGroupId = pSender->GetUserData();
	wchar_t szName[64] = {0};
	swprintf(szName, L"%s%s", kHostGroupNamePrefix, strGroupId.GetData());
	CEditUI *pNameEdit = NULL;
	FIND_CONTROL_BY_ID(pNameEdit, CEditUI, pManager, szName);
	assert(pNameEdit!=NULL);
	CDuiString strName = pNameEdit->GetText();
	char szSection[1024] = {0};
	StrUtil::w2a(strName.GetData(), szSection);
	PHOSTS_INFO pHostInfo = m_HostsHelper.FindSection(szSection);
	assert(pHostInfo!=NULL);
	char szDesc[1024] = {0};
	StrUtil::w2a(strDesc.GetData(), szDesc);
	memset(pHostInfo->szDesc, 0, sizeof(pHostInfo->szDesc));
	strncpy(pHostInfo->szDesc, szDesc, _countof(pHostInfo->szDesc)-1);
	return TRUE;
}

BOOL CHosts::UpdateHostItemAddr(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	BOOL bRet = FALSE;
	CDuiString strText = pSender->GetText();
	CDuiString sUserData = pSender->GetUserData();
	bRet = PCREUtil::IsIPv4(strText.GetData());
	if(!bRet)
	{
//#pragma message("CHosts::UpdateHostItemAddr - IP地址输入无效，提示用户")
		//RDMsgBox(hWnd, MSG_IPV4_ERR, MSG_ERR, MB_OK);
		wchar_t szErr[1024] = {0};
		Utility::GetINIStr(g_pLangManager->GetLangName(), GET_ASSOC_SECTION(g_LangIDs, MSG_IPV4_ERR), GET_ASSOC_ID(g_LangIDs, MSG_IPV4_ERR), szErr);
		m_pHostTipText->SetText(szErr);
		m_pHostTipText->SetVisible();
		pSender->SetFocus();
		return FALSE;
	}
	CChildLayoutUI *pChildLayout = NULL;
	FIND_CONTROL_BY_ID(pChildLayout, CChildLayoutUI, pManager, sUserData.GetData());
	assert(pChildLayout!=NULL);
	LPCWSTR pszGroupId = pChildLayout->GetUserData();
	LPCWSTR pszItemIndex = wcsrchr(sUserData.GetData(), L'_');
	wchar_t szItemIndex[64] = {0};
	assert(wcslen(pszItemIndex)>=2);
	wcscpy(szItemIndex, &pszItemIndex[1]);
	char szGroupId[64] = {0}, szItemId[64] = {0};
	sprintf(szGroupId, "%d", _wtoi(pszGroupId));
	sprintf(szItemId, "%d", _wtoi(szItemIndex));
	PHOSTS_ITEM pHostsItem = m_HostsHelper.FindItemById(szGroupId, szItemId);
	assert(pHostsItem!=NULL);
	memset(pHostsItem->szAddr, 0, sizeof(pHostsItem->szAddr));
	StrUtil::w2a(strText.GetData(), pHostsItem->szAddr);
	if(m_pHostTipText->IsVisible())
		m_pHostTipText->SetVisible(false);
	return TRUE;
}

BOOL CHosts::UpdateHostItemDomain(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	BOOL bRet = FALSE;
	CDuiString strText = pSender->GetText();
	CDuiString sUserData = pSender->GetUserData();
	bRet = PCREUtil::IsDomain(strText.GetData());
	if(!bRet)
	{
//#pragma message("CHosts::UpdateHostItemAddr - 域名输入无效，提示用户")
		//RDMsgBox(hWnd, MSG_DOMAIN_ERR, MSG_ERR, MB_OK);
		wchar_t szErr[1024] = {0};
		Utility::GetINIStr(g_pLangManager->GetLangName(), GET_ASSOC_SECTION(g_LangIDs, MSG_DOMAIN_ERR), GET_ASSOC_ID(g_LangIDs, MSG_DOMAIN_ERR), szErr);
		wcscat_s(szErr,1024,L" : ");
		wcscat_s(szErr,1024,strText.GetData());
		m_pHostTipText->SetText(szErr);
		m_pHostTipText->SetVisible();
		pSender->SetFocus();
		return FALSE;
	}
	CChildLayoutUI *pChildLayout = NULL;
	FIND_CONTROL_BY_ID(pChildLayout, CChildLayoutUI, pManager, sUserData.GetData());
	assert(pChildLayout!=NULL);
	LPCWSTR pszGroupId = pChildLayout->GetUserData();
	LPCWSTR pszItemIndex = wcsrchr(sUserData.GetData(), L'_');
	wchar_t szItemIndex[64] = {0};
	assert(wcslen(pszItemIndex)>=2);
	wcscpy(szItemIndex, &pszItemIndex[1]);
	char szGroupId[64] = {0}, szItemId[64] = {0};
	sprintf(szGroupId, "%d", _wtoi(pszGroupId));
	sprintf(szItemId, "%d", _wtoi(szItemIndex));
	PHOSTS_ITEM pHostsItem = m_HostsHelper.FindItemById(szGroupId, szItemId);
	assert(pHostsItem!=NULL);
	memset(pHostsItem->szDomain, 0, sizeof(pHostsItem->szDomain));
	StrUtil::w2a(strText.GetData(), pHostsItem->szDomain);
	if(m_pHostTipText->IsVisible())
		m_pHostTipText->SetVisible(false);
	return TRUE;
}

BOOL CHosts::UpdateHostItemDesc(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pSender)
{
	CDuiString strText = pSender->GetText();
	CDuiString sUserData = pSender->GetUserData();
	CChildLayoutUI *pChildLayout = NULL;
	FIND_CONTROL_BY_ID(pChildLayout, CChildLayoutUI, pManager, sUserData.GetData());
	assert(pChildLayout!=NULL);
	LPCWSTR pszGroupId = pChildLayout->GetUserData();
	LPCWSTR pszItemIndex = wcsrchr(sUserData.GetData(), L'_');
	wchar_t szItemIndex[64] = {0};
	assert(wcslen(pszItemIndex)>=2);
	wcscpy(szItemIndex, &pszItemIndex[1]);
	char szGroupId[64] = {0}, szItemId[64] = {0};
	sprintf(szGroupId, "%d", _wtoi(pszGroupId));
	sprintf(szItemId, "%d", _wtoi(szItemIndex));
	PHOSTS_ITEM pHostsItem = m_HostsHelper.FindItemById(szGroupId, szItemId);
	assert(pHostsItem!=NULL);
	memset(pHostsItem->szDesc, 0, sizeof(pHostsItem->szDesc));
	StrUtil::w2a(strText.GetData(), pHostsItem->szDesc);
	return TRUE;
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
	wchar_t szGroupId[64] = {0};
	wchar_t szGroupName[64] = {0};
	if(pszGroupId)
		wcscpy(szGroupId, pszGroupId);
	else
		wcscpy(szGroupId, m_pHostGroupContainerLayout->GetUserData());
	if(pszGroupName)
		wcscpy(szGroupName, pszGroupName);
	else
		swprintf(szGroupName, L"%s%s", kHostSectionPrefix, szGroupId);
	wchar_t szChildLayout[64] = {0};
	swprintf(szChildLayout, L"%s%s", kChildHostGroupPrefix, szGroupId);
	pChildLayout->SetName(szChildLayout);
	pChildLayout->SetChildLayoutXML(kHostGroupXmlPath, (DuiLib::IDialogBuilderCallback *)g_pMainFrame);
	m_pHostGroupContainerLayout->AddAt(pChildLayout, m_pHostGroupContainerLayout->GetCount() - 1);

	// Reset SubControl Name
	CHostGroupLayoutUI *pGroupLayout = (CHostGroupLayoutUI *)pChildLayout->GetItemAt(0);
	assert(pGroupLayout!=NULL);
	wchar_t szHostGroup[64] = {0}, szDescTopic[64] = {0}, szDescState[64] = {0}, szDescEdit[64] = {0}, szState[64] = {0}, 
		szName[64] = {0}, szNew[64] = {0}, szSave[64] = {0}, szDel[64] = {0}, szItems[64] = {0};
	swprintf(szHostGroup, L"%s%s", kHostGroupPrefix, szGroupId);
	swprintf(szDescTopic, L"%s%s", kHostGroupDescTopicPrefix, szGroupId);
	swprintf(szDescState, L"%s%s", kHostGroupDescStatePrefix, szGroupId);
	swprintf(szDescEdit, L"%s%s", kHostGroupDescEditPrefix, szGroupId);
	swprintf(szState, L"%s%s", kHostGroupStatePrefix, szGroupId);
	swprintf(szName, L"%s%s", kHostGroupNamePrefix, szGroupId);
	swprintf(szNew, L"%s%s", kHostGroupNewPrefix, szGroupId);
	swprintf(szSave, L"%s%s", kHostGroupSavePrefix, szGroupId);
	swprintf(szDel, L"%s%s", kHostGroupDelPrefix, szGroupId);
	swprintf(szItems, L"%s%s", kHostGroupItemsPrefix, szGroupId);
	pGroupLayout->SetName(szHostGroup);
	CControlUI *pSubControl = NULL;
	pSubControl = pGroupLayout->DescTopicCtrl();
	if(pSubControl)
		pSubControl->SetName(szDescTopic);
	pSubControl = pGroupLayout->DescStateCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDescState);
		pSubControl->SetUserData(szDescEdit); // 保存描述编辑框id，用于快速定位编辑框控件，以便于进行显示隐藏
		SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSubControl,kHostGroupDescShowTip)
	}
	pSubControl = pGroupLayout->DescEditCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDescEdit);
		if(pszDesc)
			pSubControl->SetText(pszDesc);
		pSubControl->SetUserData(szGroupId);
	}
	pSubControl = pGroupLayout->StateCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szState);
		pSubControl->SetUserData(szItems); // 保存HostsItems容器id，用于快速定位HostsItems容器，以便于进行显示隐藏
		SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSubControl,kHostItemsHideTip)
	}
	pSubControl = pGroupLayout->NameCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szName);
		pSubControl->SetText(szGroupName);
		pSubControl->SetUserData(szGroupId); // 保存HostsGroup分组id，用于查找HostInfo信息，以便于修改内存数据
	}
	pSubControl = pGroupLayout->NewCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szNew);
		pSubControl->SetUserData(szGroupId); // 保存HostsGroup分组id，用于快速定位HostsGroup容器，以便于新增节点后，重新调整分组高度
		SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSubControl,kHostNewItemTip)
	}
	pSubControl = pGroupLayout->SaveCtrl(); // 该控件目前被隐藏
	if(pSubControl)
		pSubControl->SetName(szSave);
	pSubControl = pGroupLayout->DelCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDel);
		pSubControl->SetUserData(szGroupId); // 保存HostsGroup分组id，用于快速定位HostsGroup容器，以便于删除节点后，重新调整分组高度
		SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSubControl,kHostGroupDeleteTip)
	}
	pSubControl = pGroupLayout->ItemsLayout();
	if(pSubControl)
	{
		pSubControl->SetName(szItems);
		pSubControl->SetUserData(L"0"); // 保存HostsItems子节点索引起始值，用于HostsItem节点id命名
	}
	int nGroupIndex = _wtoi(m_pHostGroupContainerLayout->GetUserData().GetData());
	wchar_t szNewGroupIndex[32] = {0};
	swprintf(szNewGroupIndex, L"%d", nGroupIndex+1);
	m_pHostGroupContainerLayout->SetUserData(szNewGroupIndex); // Update Host Group's Index
	pManager->UpdateControls(pGroupLayout);
	if(pszGroupId==NULL)
		AdjustGroupHeight(pManager, szGroupId, TRUE);
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
	SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSubControl,kHostGroupDeleteTip)
	pSubControl = NULL;
	FIND_CONTROL_BY_ID(pSubControl, CControlUI, pManager, szItems);
	assert(pSubControl!=NULL);
	SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSubControl,kHostItemsShowTip)
#endif
	bRet = TRUE;
	return bRet;
}

BOOL CHosts::CreateItem(CPaintManagerUI* pManager, bool active,const wchar_t* pszGroupId, const wchar_t* pszAddr, const wchar_t* pszDomain, const wchar_t* pszDesc /*= NULL*/)
{
	BOOL bRet = FALSE;
	wchar_t szItems[64] = {0};
	swprintf(szItems, L"%s%s", kHostGroupItemsPrefix, pszGroupId);
	CVerticalLayoutUI *pGroupItems = NULL;
	FIND_CONTROL_BY_ID(pGroupItems, CVerticalLayoutUI, pManager, szItems);
	assert(pGroupItems!=NULL);
	int nItemIndex = _wtoi(pGroupItems->GetUserData());
	CChildLayoutUI *pChildLayout = new CChildLayoutUI();
	if(!pChildLayout)
		return bRet;
	wchar_t szChildLayout[64] = {0};
	swprintf(szChildLayout, L"%s%s_%d", kChildHostItemPrefix, pszGroupId, nItemIndex);
	pChildLayout->SetName(szChildLayout);
	pChildLayout->SetUserData(pszGroupId); // 保存HostsGroup分组id，用于快速定位HostsGroup容器，以便于删除节点后，重新调整分组高度
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
	swprintf(szHostItem, L"%s%s_%d", kHostItemPrefix, pszGroupId, nItemIndex);
	swprintf(szState, L"%s%s_%d", kHostItemStatePrefix, pszGroupId, nItemIndex);
	swprintf(szAddr, L"%s%s_%d", kHostItemAddrPrefix, pszGroupId, nItemIndex);
	swprintf(szDomain, L"%s%s_%d", kHostItemDomainPrefix, pszGroupId, nItemIndex);
	swprintf(szDesc, L"%s%s_%d", kHostItemDescPrefix, pszGroupId, nItemIndex);
	swprintf(szSave, L"%s%s_%d", kHostItemSavePrefix, pszGroupId, nItemIndex);
	swprintf(szDel, L"%s%s_%d", kHostItemDelPrefix, pszGroupId, nItemIndex);
	pItemLayout->SetName(szHostItem);
	CControlUI *pSubControl = NULL;
	pSubControl = pItemLayout->StateCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szState);
		pSubControl->SetUserData(szChildLayout); // 保存host_item_child容器id，用于找回分组id、节点id
		CCheckBoxUI* cb = (CCheckBoxUI*)pSubControl;
		cb->SetCheck(active);
	}
	pSubControl = pItemLayout->AddrCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szAddr);
		if(pszAddr)
			pSubControl->SetText(pszAddr);
		pSubControl->SetUserData(szChildLayout); // 保存host_item_child容器id，用于找回分组id、节点id
	}
	pSubControl = pItemLayout->DomainCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDomain);
		if(pszDomain)
			pSubControl->SetText(pszDomain);
		pSubControl->SetUserData(szChildLayout); // 保存host_item_child容器id，用于找回分组id、节点id
	}
	pSubControl = pItemLayout->DescCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDesc);
		if(pszDesc)
			pSubControl->SetText(pszDesc);
		pSubControl->SetUserData(szChildLayout); // 保存host_item_child容器id，用于找回分组id、节点id
	}
	pSubControl = pItemLayout->SaveCtrl(); // 该控件目前被隐藏
	if(pSubControl)
		pSubControl->SetName(szSave);
	pSubControl = pItemLayout->DelCtrl();
	if(pSubControl)
	{
		pSubControl->SetName(szDel);
		pSubControl->SetUserData(szChildLayout); // 保存host_item_child容器id，用于快速定位host_item_child容器，以便于删除该容器
		SET_CONTROL_TIP3(LS_HOSTADMINPANEL,pSubControl,kHostItemDeleteTip)
	}
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

BOOL CHosts::AdjustGroupHeight(CPaintManagerUI* pManager, const wchar_t* pszGroupId, BOOL bInit /*= FALSE*/)
{
	BOOL bRet = FALSE;
	CChildLayoutUI *pChildLayout = NULL;
	CRichEditUI *pDescEdit = NULL;
	CVerticalLayoutUI *pHostsItems = NULL;
	wchar_t szChildLayout[64] = {0}, szDescEdit[64] = {0}, szHostsItems[64] = {0};
	swprintf(szChildLayout, L"child_host_group_%s", pszGroupId);
	swprintf(szDescEdit, L"host_group_desc_edit_%s", pszGroupId);
	swprintf(szHostsItems, L"host_group_items_%s", pszGroupId);
	FIND_CONTROL_BY_ID(pChildLayout, CChildLayoutUI, pManager, szChildLayout);
	FIND_CONTROL_BY_ID(pDescEdit, CRichEditUI, pManager, szDescEdit);
	FIND_CONTROL_BY_ID(pHostsItems, CVerticalLayoutUI, pManager, szHostsItems);
	if(!pChildLayout)
		return bRet;
	CHostGroupLayoutUI *pGroupLayout = (CHostGroupLayoutUI *)pChildLayout->GetItemAt(0);
	assert(pGroupLayout!=NULL);
	assert(pDescEdit!=NULL);
	assert(pHostsItems!=NULL);
	int nCount = pHostsItems->GetCount();
	RECT rtInset = pGroupLayout->GetInset();
	RECT rtPadding = pGroupLayout->GetPadding();
	int nChildPadding = pGroupLayout->GetChildPadding();
	int nBorderSize = pGroupLayout->GetBorderSize();
	int nMinHeight = pGroupLayout->GetMinHeight() + (rtInset.bottom + rtInset.top) + (rtPadding.bottom + rtPadding.top) + nChildPadding*2 + nBorderSize*2;
	rtInset = pDescEdit->GetInset();
	rtPadding = pDescEdit->GetPadding();
	nBorderSize = pDescEdit->GetBorderSize();
	int nEditHeight = pDescEdit->GetFixedHeight() + (rtInset.bottom + rtInset.top) + (rtPadding.bottom + rtPadding.top) + nBorderSize*2;
	int nItemHeight = _wtoi(pGroupLayout->GetUserData());
	if(nItemHeight==0)
		nItemHeight = 26;
	rtInset = pHostsItems->GetInset();
	rtPadding = pHostsItems->GetPadding();
	nChildPadding = pHostsItems->GetChildPadding();
	nBorderSize = pHostsItems->GetBorderSize();
	int nBlankHeight = _wtoi(pHostsItems->GetUserData());
	nItemHeight = nCount * nItemHeight + (rtInset.bottom - rtInset.top) + (rtPadding.bottom + rtPadding.top) + nChildPadding*2 + nBorderSize*2 + nBlankHeight;
	int nHeight = nMinHeight;
	if(bInit)
	{
		nHeight += nItemHeight;
	}
	else
	{
		if(pDescEdit->IsVisible())
			nHeight += nEditHeight;
		if(pHostsItems->IsVisible())
			nHeight += nItemHeight;
	}
	pGroupLayout->SetFixedHeight(nHeight);
	pChildLayout->SetFixedHeight(pGroupLayout->GetFixedHeight());
	bRet = TRUE;
	return bRet;
}