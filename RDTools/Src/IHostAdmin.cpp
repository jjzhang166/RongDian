#include "stdafx.h"
#include "IHostAdmin.h"

// 头部
const wchar_t* const kHostPathText = L"host_path_text";
const wchar_t* const kHostPathEdit = L"host_path_edit";		
const wchar_t* const kHostOpenBtn = L"host_open";
const wchar_t* const kHostOpenTip = L"host_open_tip";
const wchar_t* const kHostBackupBtn = L"host_backup";
const wchar_t* const kHostBackupTip = L"host_backup_tip";
const wchar_t* const kHostNewGroupBtn = L"host_new_group";
const wchar_t* const kHostNewGroupTip = L"host_new_group_tip";
const wchar_t* const kHostGroupContainer = L"host_group_container";
//xml路径
const wchar_t* const kHostGroupXmlPath = L"HostGroup.xml";
const wchar_t* const kHostGroupRowXmlPath = L"HostGroupRow.xml";
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
const wchar_t* const kHostRowDeleteBtn = L"host_row_delete";

const wchar_t* const kHostDownArrowImage = L"default\\arrow_down.png";
const wchar_t* const kHostUpArrowImage = L"default\\arrow_up.png";

IHostAdmin::IHostAdmin():m_groupIndex(0),m_rowIndex(0)
{
	m_hHostAdminOwner = NULL;
	m_pHostAdminManager = NULL;
	m_pHostGroupContainerLayout = NULL;
}

IHostAdmin::~IHostAdmin()
{

}

BOOL IHostAdmin::InitHostAdmin()
{
	if(!m_pHostAdminManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_pHostPathEdit, CEditUI, m_pHostAdminManager, kHostPathEdit)
	FIND_CONTROL_BY_ID(m_pHostOpenBtn, CButtonUI, m_pHostAdminManager, kHostOpenBtn)
	FIND_CONTROL_BY_ID(m_pHostBackupBtn, CButtonUI, m_pHostAdminManager, kHostBackupBtn)
	FIND_CONTROL_BY_ID(m_pHostNewGroupBtn, CButtonUI, m_pHostAdminManager, kHostNewGroupBtn)
	FIND_CONTROL_BY_ID(m_pHostGroupContainerLayout, CVerticalLayoutUI, m_pHostAdminManager, kHostGroupContainer)
	return TRUE;
}

BOOL IHostAdmin::SetHostAdminLang(LPCWSTR lpszLang)
{
	if(!m_pHostAdminManager)
		return FALSE;

SET_CONTROL_BEGIN(m_pHostAdminManager, lpszLang, LS_HOSTADMINPANEL)
	SET_CONTROL_TEXT2(kHostPathText)
	SET_CONTROL_TEXT_AND_TIP(kHostOpenBtn,kHostOpenTip)
	SET_CONTROL_TEXT_AND_TIP(kHostBackupBtn,kHostBackupTip)
	SET_CONTROL_TEXT_AND_TIP(kHostNewGroupBtn,kHostNewGroupTip)
SET_CONTROL_END()

	return TRUE;
}

void IHostAdmin::OnHostAdminClick(TNotifyUI& msg, BOOL& bHandled)
{
	if(!m_pHostAdminManager)
		return;
	CDuiString sCtrlName = msg.pSender->GetName();

	LPCTSTR ctrlName = sCtrlName.GetData();

	if(sCtrlName == kHostOpenBtn)
	{

	}
	else if (sCtrlName == kHostBackupBtn)
	{
	}
	else if (sCtrlName == kHostNewGroupBtn)
	{
		CreateGroup(m_pHostGroupContainerLayout);
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostGroupVisibleContentBtn))//收起或者展开内容
	{
		LPCTSTR szGroupIndex = msg.pSender->GetUserData().GetData();
		//隐藏当前组列表
		wchar_t name1[50] = {0};
		wsprintf(name1,L"%s_%s",kHostGroupVisibleContentBtn,szGroupIndex);
		CCheckBoxUI* pVisibleContentBtn = NULL;
		FIND_CONTROL_BY_ID(pVisibleContentBtn, CCheckBoxUI, m_pHostAdminManager,name1)

		wchar_t name[50] = {0};
		wsprintf(name,L"%s_%s",kHostGroupRowsVLayout,szGroupIndex);
		CVerticalLayoutUI* pGroupRowsVLayout = NULL;
		FIND_CONTROL_BY_ID(pGroupRowsVLayout, CVerticalLayoutUI, m_pHostAdminManager, name)
		
		if (pGroupRowsVLayout->IsVisible())
		{
			pGroupRowsVLayout->SetVisible(false);
			//将箭头变为向下
			pVisibleContentBtn->SetBkImage(kHostDownArrowImage);
		}
		else
		{
			pGroupRowsVLayout->SetVisible(true);
			//将箭头变为向上
			pVisibleContentBtn->SetBkImage(kHostUpArrowImage);
		}
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostGroupNewRowBtn))//为该组添加新行
	{
		LPCTSTR szGroupIndex = msg.pSender->GetUserData().GetData();
		char* aGroupIndex = StrUtil::w2a(szGroupIndex);
		int groupIndex = atoi(aGroupIndex);

		CVerticalLayoutUI* pGroup = (CVerticalLayoutUI*)msg.pSender->GetParent()->GetParent();
		CreateGroupRow(pGroup,groupIndex);
		delete[] aGroupIndex;
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostGroupDeleteBtn))//删除该组
	{
		CVerticalLayoutUI* pGroup = (CVerticalLayoutUI*)msg.pSender->GetParent()->GetParent();
		CVerticalLayoutUI* pGroupContainer = (CVerticalLayoutUI*)pGroup->GetParent();
		pGroupContainer->Remove(pGroup);
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostRowDeleteBtn))//删除该组的一行
	{
		CVerticalLayoutUI* pHostRowVLayout = (CVerticalLayoutUI*)msg.pSender->GetParent();
		CVerticalLayoutUI* pParentLayout = (CVerticalLayoutUI*)pHostRowVLayout->GetParent();
		pParentLayout->Remove(pHostRowVLayout);
	}
}

void IHostAdmin::OnHostAdminItemSelect(TNotifyUI& msg)
{
	CComboUI* pCombo = (CComboUI*)msg.pSender;
	CDuiString sItemName = pCombo->GetName();
	// if(sItemName == kHostOpenBtn)
	// {

	// }
	// else if (sItemName == kHostBackupBtn)
	// {
	// }
}

BOOL IHostAdmin::CreateGroup(CContainerUI* pParentContainer)
{
	m_groupIndex++;
	wchar_t szGroupIndex[3] = {0};
	wsprintf(szGroupIndex,L"%d",m_groupIndex);

	CChildLayoutUI* pGroupPanel = new CChildLayoutUI();
	if(!pGroupPanel)
		return FALSE;
	wchar_t szGroupName[25] = {0};
	wsprintf(szGroupName,L"%s_%d",kHostGroup,m_groupIndex);
	//pGroupPanel->SetName(szGroupName);
	pGroupPanel->SetChildLayoutXML(kHostGroupXmlPath);
	pGroupPanel->SetVisible(true);
	pParentContainer->AddAt(pGroupPanel,pParentContainer->GetCount());
	/**指定各控件的名字**/
	CVerticalLayoutUI* pHostGroupVLayout = NULL;
	FIND_CONTROL_BY_ID(pHostGroupVLayout, CVerticalLayoutUI, m_pHostAdminManager,kHostGroup)
	wchar_t szHostGroupVLayoutName[50] = {0};
	wsprintf(szHostGroupVLayoutName,L"%s_%d",kHostGroup,m_groupIndex);
	pHostGroupVLayout->SetName(szHostGroupVLayoutName);
	pHostGroupVLayout->SetUserData(szGroupIndex);

	CCheckBoxUI* pVisibleContentBtn = NULL;
	FIND_CONTROL_BY_ID(pVisibleContentBtn, CCheckBoxUI, m_pHostAdminManager,kHostGroupVisibleContentBtn)
	wchar_t szVisibleContentBtnName[50] = {0};
	wsprintf(szVisibleContentBtnName,L"%s_%d",kHostGroupVisibleContentBtn,m_groupIndex);
	pVisibleContentBtn->SetName(szVisibleContentBtnName);
	pVisibleContentBtn->SetUserData(szGroupIndex);

	CEditUI* pGroupTitleEdit = NULL;
	FIND_CONTROL_BY_ID(pGroupTitleEdit, CEditUI, m_pHostAdminManager, kHostGroupTitleEdit)
	wchar_t szGroupTitleEditName[50] = {0};
	wsprintf(szGroupTitleEditName,L"%s_%d",kHostGroupTitleEdit,m_groupIndex);
	pGroupTitleEdit->SetName(szGroupTitleEditName);
	pGroupTitleEdit->SetUserData(szGroupIndex);

	CButtonUI* pGroupNewRowBtn = NULL;
	FIND_CONTROL_BY_ID(pGroupNewRowBtn, CButtonUI, m_pHostAdminManager, kHostGroupNewRowBtn)
	wchar_t szGroupNewRowBtnName[50] = {0};
	wsprintf(szGroupNewRowBtnName,L"%s_%d",kHostGroupNewRowBtn,m_groupIndex);
	pGroupNewRowBtn->SetName(szGroupNewRowBtnName);
	pGroupNewRowBtn->SetUserData(szGroupIndex);

	CButtonUI* pGroupSaveBtn = NULL;
	FIND_CONTROL_BY_ID(pGroupSaveBtn, CButtonUI, m_pHostAdminManager, kHostGroupSaveBtn)
	wchar_t szGroupSaveBtnName[50] = {0};
	wsprintf(szGroupSaveBtnName,L"%s_%d",kHostGroupSaveBtn,m_groupIndex);
	pGroupSaveBtn->SetName(szGroupSaveBtnName);
	pGroupSaveBtn->SetUserData(szGroupIndex);

	CButtonUI* pGroupDeleteBtn = NULL;
	FIND_CONTROL_BY_ID(pGroupDeleteBtn, CButtonUI, m_pHostAdminManager, kHostGroupDeleteBtn)
	wchar_t szGroupDeleteBtnName[50] = {0};
	wsprintf(szGroupDeleteBtnName,L"%s_%d",kHostGroupDeleteBtn,m_groupIndex);
	pGroupDeleteBtn->SetName(szGroupDeleteBtnName);
	pGroupDeleteBtn->SetUserData(szGroupIndex);

	CVerticalLayoutUI* pGroupRowsVLayout = NULL;
	FIND_CONTROL_BY_ID(pGroupRowsVLayout, CVerticalLayoutUI, m_pHostAdminManager, kHostGroupRowsVLayout)
	wchar_t szGroupRowsVLayoutName[50] = {0};
	wsprintf(szGroupRowsVLayoutName,L"%s_%d",kHostGroupRowsVLayout,m_groupIndex);
	pGroupRowsVLayout->SetName(szGroupRowsVLayoutName);
	pGroupRowsVLayout->SetUserData(szGroupIndex);

	CVerticalLayoutUI* pGroupRowsVLayout2 = NULL;
	FIND_CONTROL_BY_ID(pGroupRowsVLayout2, CVerticalLayoutUI, m_pHostAdminManager, szGroupRowsVLayoutName)

	CreateGroupRow(pGroupRowsVLayout,m_groupIndex);
	return TRUE;
}

BOOL IHostAdmin::CreateGroupRow(CContainerUI* pParentContainer,int groupIndex)
{
	m_rowIndex++;
	wchar_t sDataIndex[7] = {0};
	wsprintf(sDataIndex,L"%d_%d",m_groupIndex,m_rowIndex);

	CChildLayoutUI* pRowPanel = new CChildLayoutUI();
	if(!pRowPanel)
		return FALSE;
	wchar_t szRowName[25] = {0};
	wsprintf(szRowName,L"host_row_%d_%d",groupIndex,m_rowIndex);
	//pRowPanel->SetName(szRowName);
	pRowPanel->SetChildLayoutXML(kHostGroupRowXmlPath);
	pRowPanel->SetVisible(true);
	pParentContainer->AddAt(pRowPanel,pParentContainer->GetCount());

	/**重新指定控件名**/
	CVerticalLayoutUI* pHostRowVLayout = NULL;
	FIND_CONTROL_BY_ID(pHostRowVLayout, CVerticalLayoutUI, m_pHostAdminManager,kHostRowVLayout)
	wchar_t szHostRowVLayoutName[50] = {0};
	wsprintf(szHostRowVLayoutName,L"%s_%d_%d",kHostRowVLayout,m_groupIndex,m_rowIndex);
	pHostRowVLayout->SetName(szHostRowVLayoutName);
	pHostRowVLayout->SetUserData(sDataIndex);

	CCheckBoxUI* pActiveCB = NULL;
	FIND_CONTROL_BY_ID(pActiveCB, CCheckBoxUI, m_pHostAdminManager, kHostRowActiveCB)
	wchar_t szRowActiveCBName[50] = {0};
	wsprintf(szRowActiveCBName,L"%s_%d_%d",kHostRowActiveCB,groupIndex,m_rowIndex);
	pActiveCB->SetName(szRowActiveCBName);
	pActiveCB->SetUserData(sDataIndex);

	CEditUI* pIPEdit = NULL;
	FIND_CONTROL_BY_ID(pIPEdit, CEditUI, m_pHostAdminManager, kHostRowIPEdit)
	wchar_t szRowIPEditName[50] = {0};
	wsprintf(szRowIPEditName,L"%s_%d_%d",kHostRowIPEdit,groupIndex,m_rowIndex);
	pIPEdit->SetName(szRowIPEditName);
	pIPEdit->SetUserData(sDataIndex);

	CEditUI* pDomainEdit = NULL;
	FIND_CONTROL_BY_ID(pDomainEdit, CEditUI, m_pHostAdminManager, kHostRowDomainEdit)
	wchar_t szRowDomainEditName[50] = {0};
	wsprintf(szRowDomainEditName,L"%s_%d_%d",kHostRowDomainEdit,groupIndex,m_rowIndex);
	pDomainEdit->SetName(szRowDomainEditName);
	pDomainEdit->SetUserData(sDataIndex);

	CEditUI* pDescEdit = NULL;
	FIND_CONTROL_BY_ID(pDescEdit, CEditUI, m_pHostAdminManager, kHostRowDescEdit)
	wchar_t szRowDescEditName[50] = {0};
	wsprintf(szRowDescEditName,L"%s_%d_%d",kHostRowDescEdit,groupIndex,m_rowIndex);
	pDescEdit->SetName(szRowDescEditName);
	pDescEdit->SetUserData(sDataIndex);

	CButtonUI* pSaveBtn = NULL;
	FIND_CONTROL_BY_ID(pSaveBtn, CButtonUI, m_pHostAdminManager, kHostRowSaveBtn)
	wchar_t szRowSaveBtnName[50] = {0};
	wsprintf(szRowSaveBtnName,L"%s_%d_%d",kHostRowSaveBtn,groupIndex,m_rowIndex);
	pSaveBtn->SetName(szRowSaveBtnName);
	pSaveBtn->SetUserData(sDataIndex);

	CButtonUI* pDeleteBtn = NULL;
	FIND_CONTROL_BY_ID(pDeleteBtn, CButtonUI, m_pHostAdminManager, kHostRowDeleteBtn)
	wchar_t szRowDeleteBtnName[50] = {0};
	wsprintf(szRowDeleteBtnName,L"%s_%d_%d",kHostRowDeleteBtn,groupIndex,m_rowIndex);
	pDeleteBtn->SetName(szRowDeleteBtnName);
	pDeleteBtn->SetUserData(sDataIndex);

	return TRUE;
}
