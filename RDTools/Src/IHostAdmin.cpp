#include "stdafx.h"
#include "IHostAdmin.h"
#include <time.h>

// 头部
const wchar_t* const kHostPathText = L"host_path_text";
const wchar_t* const kHostPathEdit = L"host_path_edit";		
const wchar_t* const kHostOpenBtn = L"host_open";
const wchar_t* const kHostOpenTip = L"host_open_tip";
const wchar_t* const kHostBackupBtn = L"host_backup";
const wchar_t* const kHostBackupTip = L"host_backup_tip";
const wchar_t* const kHostSaveBtn = L"host_save";
const wchar_t* const kHostSaveTip = L"host_save_tip";
const wchar_t* const kHostNewGroupBtn = L"host_new_group";
const wchar_t* const kHostNewGroupTip = L"host_new_group_tip";
const wchar_t* const kHostDeleteGroupTip = L"host_delete_group_tip";
const wchar_t* const kHostGroupRowsBox = L"host_groups_box";
const wchar_t* const kHostRowsHideBox = L"host_rows_hide";
const wchar_t* const kHostRowsShowBox = L"host_rows_show";
const wchar_t* const kHostNewRowTip = L"host_new_row_tip";
const wchar_t* const kHostDeleteRowTip = L"host_delete_row_tip";
//xml路径
const wchar_t* const kHostGroupXmlPath = L"HostGroup.xml";
const wchar_t* const kHostGroupRowXmlPath = L"HostGroupRow.xml";
//动态控件名
const wchar_t* const kHostGroupContainer = L"HostGroupContainer";
const wchar_t* const kHostGroup = L"host_group";
const wchar_t* const kHostGroupVisibleContentBtn = L"HostGroupVisibleButton";
const wchar_t* const kHostGroupTitleEdit = L"HostGroupTitleEdit";
const wchar_t* const kHostGroupNewRowBtn = L"HostGroupNewRowButton";
const wchar_t* const kHostGroupSaveBtn = L"HostGroupSaveButton";
const wchar_t* const kHostGroupDeleteBtn = L"HostGroupDeleteButton";
const wchar_t* const kHostGroupRowsVLayout = L"HostGroupRowsVLayout";

const wchar_t* const kHostRowContainer = L"HostRowContainer";
const wchar_t* const kHostRowHLayout = L"host_row";
const wchar_t* const kHostRowActiveCB = L"HostRowCheckBox";
const wchar_t* const kHostRowIPEdit = L"HostRowIPEdit";
const wchar_t* const kHostRowDomainEdit = L"HostRowDomainEdit";
const wchar_t* const kHostRowDescEdit = L"HostRowDescEdit";
const wchar_t* const kHostRowSaveBtn = L"HostRowSaveButton";
const wchar_t* const kHostRowDeleteBtn = L"HostRowDeleteButton";

const wchar_t* const kHostSaveSuccessWord = L"host_save_success";

IHostAdmin::IHostAdmin():m_groupIndex(0),m_rowIndex(0)
{
	m_hHostAdminOwner = NULL;
	m_pHostAdminManager = NULL;

	m_pHostPathEdit = NULL;
	m_pHostOpenBtn = NULL;
	m_pHostBackupBtn = NULL;
	m_pHostNewGroupBtn = NULL;
	m_pHostGroupsBox = NULL;
	m_pLastRowsBox = NULL;

	m_pHostParser = new CHostsParser();
}

IHostAdmin::~IHostAdmin()
{
	delete m_pHostParser;
}

BOOL IHostAdmin::InitHostAdmin()
{
	if(!m_pHostAdminManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_pHostPathEdit, CEditUI, m_pHostAdminManager, kHostPathEdit)
	FIND_CONTROL_BY_ID(m_pHostOpenBtn, CButtonUI, m_pHostAdminManager, kHostOpenBtn)
	FIND_CONTROL_BY_ID(m_pHostBackupBtn, CButtonUI, m_pHostAdminManager, kHostBackupBtn)
	FIND_CONTROL_BY_ID(m_pHostSaveBtn, CButtonUI, m_pHostAdminManager, kHostSaveBtn)
	FIND_CONTROL_BY_ID(m_pHostNewGroupBtn, CButtonUI, m_pHostAdminManager, kHostNewGroupBtn)
	FIND_CONTROL_BY_ID(m_pHostGroupsBox, CVerticalLayoutUI, m_pHostAdminManager, kHostGroupRowsBox)

	m_pHostPathEdit->SetText(m_pHostParser->m_lpszHostsFilePath);

	list<PHOSTS_INFO> rows = m_pHostParser->m_rows;
	list<PHOSTS_INFO>::iterator it;
	for(it=rows.begin();it!=rows.end();it++)
	{
		PHOSTS_INFO row = *it;
		if(row->type==TAG_GROUP)
		{
			m_pLastRowsBox = CreateGroup(row->szDesc);
		}
		else if (row->type==TAG_ACTIVE_HOST)
		{
			if(m_pLastRowsBox==0)
			{
				m_pLastRowsBox = CreateGroup(L"");
			}
			CreateGroupRow(m_pLastRowsBox,TRUE,row->szIP,row->szDomain,row->szDesc);
		}
		else if (row->type==TAG_STOP_HOST)
		{
			if(m_pLastRowsBox==0)
			{
				m_pLastRowsBox = CreateGroup(L"");
			}
			CreateGroupRow(m_pLastRowsBox,FALSE,row->szIP,row->szDomain,row->szDesc);
		}
	}
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
	SET_CONTROL_TEXT_AND_TIP(kHostSaveBtn,kHostSaveTip)
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
		ShellExecute(NULL,L"open",L"notepad.exe",m_pHostParser->m_lpszHostsFilePath,NULL,SW_SHOWNORMAL);
		//SHHelper::OpenFile(m_hHostAdminOwner,m_pHostParser->m_lpszHostsFilePath);
	}
	else if (sCtrlName == kHostBackupBtn)
	{
		time_t time_million;
		time(&time_million);
		struct tm *now;
		now = localtime(&time_million); 

		wchar_t* backUpPath = new wchar_t[MAX_PATH];
		wmemset(backUpPath,0,MAX_PATH);
		wsprintf(backUpPath,L"%s_%d%02d%02d%02d%02d%02d",m_pHostParser->m_lpszHostsFilePath,now->tm_year+1900,now->tm_mon,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		SHHelper::CopyFile(m_hHostAdminOwner,m_pHostParser->m_lpszHostsFilePath,backUpPath);
		delete[] backUpPath;
		RDMsgBox(m_hHostAdminOwner,MSG_OPT_SUCCESS,MSG_SUCCESS, MB_OK);
	}
	else if (sCtrlName == kHostNewGroupBtn)
	{
		CContainerUI* rowsBox = CreateGroup(L"");
		CreateGroupRow(rowsBox,FALSE,L"",L"",L"");
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostGroupVisibleContentBtn))//收起或者展开内容
	{
		LPCTSTR szGroupIndex = msg.pSender->GetUserData().GetData();
		//隐藏当前组列表
		CCheckBoxUI* pVisibleContentBtn = (CCheckBoxUI*)msg.pSender;
		CVerticalLayoutUI* groupLayout = (CVerticalLayoutUI*)pVisibleContentBtn->GetParent()->GetParent();
		CVerticalLayoutUI* pGroupRowsVLayout = (CVerticalLayoutUI*)groupLayout->GetItemAt(3);
		if (pGroupRowsVLayout->IsVisible())
		{
			pGroupRowsVLayout->SetVisible(false);

			wchar_t cbTip[50] = {0};
			Utility::GetINIStr(g_pLangManager->GetLangName(),LS_HOSTADMINPANEL,kHostRowsShowBox,cbTip);
			pVisibleContentBtn->SetToolTip(cbTip);
		}
		else
		{
			pGroupRowsVLayout->SetVisible(true);

			wchar_t cbTip[50] = {0};
			Utility::GetINIStr(g_pLangManager->GetLangName(),LS_HOSTADMINPANEL,kHostRowsHideBox,cbTip);
			pVisibleContentBtn->SetToolTip(cbTip);
		}
		ResetRowsPos(pGroupRowsVLayout);
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostGroupNewRowBtn))//为该组添加新行
	{
		LPCTSTR szGroupIndex = msg.pSender->GetUserData().GetData();
		char* aGroupIndex = StrUtil::w2a(szGroupIndex);
		int groupIndex = atoi(aGroupIndex);

		CHostAdminGroupContainer* pGroupVlayout = (CHostAdminGroupContainer*)msg.pSender->GetParent()->GetParent();
		CContainerUI* pRowsBox = (CContainerUI*)pGroupVlayout->GetRowsLayout();
		CreateGroupRow(pRowsBox,FALSE,L"",L"",L"");
		delete[] aGroupIndex;
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostSaveBtn))//保存
	{
		SaveAll();
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostGroupDeleteBtn))//删除该组
	{
		CHostAdminGroupContainer* pGroup = (CHostAdminGroupContainer*)msg.pSender->GetParent()->GetParent();
		CContainerUI* pGroupWindow = (CContainerUI*)pGroup->GetParent();
		m_pHostGroupsBox->Remove(pGroupWindow);
	}
	else if (StrUtil::is_wsubstr(ctrlName,kHostRowDeleteBtn))//删除该组的一行
	{
		CHostAdminRowContainer* pHostRowVLayout = (CHostAdminRowContainer*)msg.pSender->GetParent();
		CContainerUI* pRowWindow = (CContainerUI*)pHostRowVLayout->GetParent();
		CContainerUI* pRowsBox = (CContainerUI*)pRowWindow->GetParent();
		pRowsBox->Remove(pRowWindow);
		ResetRowsPos(pRowsBox);
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

CControlUI*	IHostAdmin::OnHostAdminCreateControl(LPCTSTR pstrClass, CControlUI *pParent)
{
	BOOL bSubControl = FALSE;
	int nSubType = 0; // 自定义标识
	CControlUI* pControl = NULL;
	if (wcsicmp(pstrClass, kHostGroupContainer)==0)
	{
		pControl = new CHostAdminGroupContainer();
	}
	else if(wcsicmp(pstrClass, kHostRowContainer)==0)
	{
		pControl = new CHostAdminRowContainer();
	}
	//group
	else if (wcsicmp(pstrClass, kHostGroupVisibleContentBtn)==0)
	{
		pControl = new CCheckBoxUI();
		nSubType = 1;
	}
	else if (wcsicmp(pstrClass, kHostGroupNewRowBtn)==0)
	{
		pControl = new CButtonUI();
		nSubType = 2;
	}
	else if (wcsicmp(pstrClass, kHostGroupDeleteBtn)==0)
	{
		pControl = new CButtonUI();
		nSubType = 3;
	}
	else if (wcsicmp(pstrClass, kHostGroupRowsVLayout)==0)
	{
		pControl = new CVerticalLayoutUI();
		nSubType = 4;
	}
	else if (wcsicmp(pstrClass, kHostGroupTitleEdit)==0)
	{
		pControl = new CEditUI();
		nSubType = 5;
	}
	//row
	else if (wcsicmp(pstrClass, kHostRowActiveCB)==0)
	{
		pControl = new CCheckBoxUI();
		nSubType = 1;
	}
	else if (wcsicmp(pstrClass, kHostRowIPEdit)==0)
	{
		pControl = new CEditUI();
		nSubType = 2;
	}
	else if (wcsicmp(pstrClass, kHostRowDomainEdit)==0)
	{
		pControl = new CEditUI();
		nSubType = 3;
	}
	else if (wcsicmp(pstrClass, kHostRowDescEdit)==0)
	{
		pControl = new CEditUI();
		nSubType = 4;
	}
	else if (wcsicmp(pstrClass, kHostRowDeleteBtn)==0)
	{
		pControl = new CButtonUI();
		nSubType = 5;
	}
	if(pControl && nSubType!=0)
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
		if(wcsicmp(pContainer->GetClass(), L"HostAdminGroupContainerUI")==0)
		{
			CHostAdminGroupContainer *pGroupConainer = static_cast<CHostAdminGroupContainer *>(pContainer);
			if(nSubType==1)
				pGroupConainer->SaveVisibleBtn(pControl);
			else if(nSubType==2)
				pGroupConainer->SaveNewRowBtn(pControl);
			else if(nSubType==3)
				pGroupConainer->SaveDeleteBtn(pControl);
			else if (nSubType==4)
				pGroupConainer->SaveRowsVLayout(pControl);
			else if(nSubType==5)
				pGroupConainer->SaveTitleEdit(pControl);
		}
		else if (wcsicmp(pContainer->GetClass(), L"HostAdminRowContainerUI")==0)
		{
			CHostAdminRowContainer* pRowConainer = static_cast<CHostAdminRowContainer*>(pContainer);
			if(nSubType==1)
				pRowConainer->SaveCheckBox(pControl);
			else if(nSubType==2)
				pRowConainer->SaveIPEdit(pControl);
			else if(nSubType==3)
				pRowConainer->SaveDomainEdit(pControl);
			else if(nSubType==4)
				pRowConainer->SaveDescEdit(pControl);
			else if (nSubType==5)
				pRowConainer->SaveDeleteBtn(pControl);
		}
	}
	return pControl;
}
CContainerUI* IHostAdmin::CreateGroup(LPCWSTR lpszGroupTitle)
{
	m_groupIndex++;
	wchar_t szGroupIndex[3] = {0};
	wsprintf(szGroupIndex,L"%d",m_groupIndex);

	CChildLayoutUI* pGroupPanel = new CChildLayoutUI();
	if(!pGroupPanel)
		return FALSE;
	wchar_t szGroupName[25] = {0};
	wsprintf(szGroupName,L"%s_%d",kHostGroup,m_groupIndex);
	pGroupPanel->SetName(szGroupName);
	pGroupPanel->SetChildLayoutXML(kHostGroupXmlPath,(DuiLib::IDialogBuilderCallback *)g_pMainFrame);

	if (!m_pHostGroupsBox->AddAt(pGroupPanel,m_pHostGroupsBox->GetCount()))
	{
		return FALSE;
	}
	//重设父容器高度
	pGroupPanel->SetFixedHeight(pGroupPanel->GetItemAt(0)->GetFixedHeight());

	CHostAdminGroupContainer *pGroupContainer = (CHostAdminGroupContainer *)pGroupPanel->GetItemAt(0);
	
	wchar_t szHostGroupVLayoutName[50] = {0};
	wsprintf(szHostGroupVLayoutName,L"%s_%d",kHostGroup,m_groupIndex);
	pGroupContainer->SetName(szHostGroupVLayoutName);
	pGroupContainer->SetUserData(szGroupIndex);
	
	CCheckBoxUI* pVisibleBtn = (CCheckBoxUI*)pGroupContainer->GetVisibleBtn();
	wchar_t szGroupVisibleBtnName[50] = {0};
	wsprintf(szGroupVisibleBtnName,L"%s_%d",kHostGroupVisibleContentBtn,m_groupIndex);
	pVisibleBtn->SetName(szGroupVisibleBtnName);
	pVisibleBtn->SetUserData(szGroupIndex);
	wchar_t hideTip[50] = {0};
	Utility::GetINIStr(g_pLangManager->GetLangName(),LS_HOSTADMINPANEL,kHostRowsHideBox,hideTip);
	pVisibleBtn->SetToolTip(hideTip);

	CControlUI* pGroupTitleEdit = pGroupContainer->GetTitleEdit();
	wchar_t szGroupTitleEditName[50] = {0};
	wsprintf(szGroupTitleEditName,L"%s_%d",kHostGroupTitleEdit,m_groupIndex);
	pGroupTitleEdit->SetName(szGroupTitleEditName);
	pGroupTitleEdit->SetUserData(szGroupIndex);
	pGroupTitleEdit->SetText(lpszGroupTitle);

	CControlUI* pGroupNewRowBtn = pGroupContainer->GetNewRowBtn();
	wchar_t szGroupNewRowBtnName[50] = {0};
	wsprintf(szGroupNewRowBtnName,L"%s_%d",kHostGroupNewRowBtn,m_groupIndex);
	pGroupNewRowBtn->SetName(szGroupNewRowBtnName);
	pGroupNewRowBtn->SetUserData(szGroupIndex);
	wchar_t addRowTip[50] = {0};
	Utility::GetINIStr(g_pLangManager->GetLangName(),LS_HOSTADMINPANEL,kHostNewRowTip,addRowTip);
	pGroupNewRowBtn->SetToolTip(addRowTip);

	CControlUI* pGroupDeleteBtn = pGroupContainer->GetDeleteBtn();
	wchar_t szGroupDeleteBtnName[50] = {0};
	wsprintf(szGroupDeleteBtnName,L"%s_%d",kHostGroupDeleteBtn,m_groupIndex);
	pGroupDeleteBtn->SetName(szGroupDeleteBtnName);
	pGroupDeleteBtn->SetUserData(szGroupIndex);
	wchar_t deleteGroupTip[50] = {0};
	Utility::GetINIStr(g_pLangManager->GetLangName(),LS_HOSTADMINPANEL,kHostDeleteGroupTip,deleteGroupTip);
	pGroupDeleteBtn->SetToolTip(deleteGroupTip);

	//组内容，包含多行
	CContainerUI* pGroupRowsVLayout = (CContainerUI*)pGroupContainer->GetRowsLayout();
	wchar_t szGroupRowsVLayoutName[50] = {0};
	wsprintf(szGroupRowsVLayoutName,L"%s_%d",kHostGroupRowsVLayout,m_groupIndex);
	pGroupRowsVLayout->SetName(szGroupRowsVLayoutName);
	pGroupRowsVLayout->SetUserData(szGroupIndex);
	pGroupRowsVLayout->SetVisible(true);
	pGroupRowsVLayout->SetInternVisible(true);
	//CreateGroupRow(pGroupRowsVLayout,FALSE,L"",L"",L"");
	return pGroupRowsVLayout;
}

BOOL IHostAdmin::CreateGroupRow(CContainerUI* pParentContainer,BOOL isActive,LPCWSTR lpszIP,LPCWSTR lpszDomain,LPCWSTR lpszDesc)
{
	LPCTSTR groupIndex = pParentContainer->GetUserData().GetData();

	m_rowIndex++;
	wchar_t sDataIndex[7] = {0};
	wsprintf(sDataIndex,L"%s_%d",groupIndex,m_rowIndex);

	CChildLayoutUI* pRowPanel = new CChildLayoutUI();
	if(!pRowPanel)
		return FALSE;
	wchar_t szRowName[25] = {0};
	wsprintf(szRowName,L"host_row_%s_%d",groupIndex,m_rowIndex);
	pRowPanel->SetName(szRowName);
	pRowPanel->SetChildLayoutXML(kHostGroupRowXmlPath,(DuiLib::IDialogBuilderCallback *)g_pMainFrame);

	if(!pParentContainer->Add(pRowPanel))
	{
		return FALSE;
	}

	//重置控件高度（group、row）
	int rowHeight = pRowPanel->GetItemAt(0)->GetFixedHeight();
	pRowPanel->SetFixedHeight(rowHeight);

	/**重新指定控件名**/
	CHostAdminRowContainer* pHostRowHLayout = (CHostAdminRowContainer*)pRowPanel->GetItemAt(0);
	wchar_t szHostRowVLayoutName[50] = {0};
	wsprintf(szHostRowVLayoutName,L"%s_%s_%d",kHostRowHLayout,groupIndex,m_rowIndex);
	pHostRowHLayout->SetName(szHostRowVLayoutName);
	pHostRowHLayout->SetUserData(sDataIndex);

	CCheckBoxUI* pActiveCB = (CCheckBoxUI*)pHostRowHLayout->GetCheckBox();
	wchar_t szRowActiveCBName[50] = {0};
	wsprintf(szRowActiveCBName,L"%s_%s_%d",kHostRowActiveCB,groupIndex,m_rowIndex);
	pActiveCB->SetName(szRowActiveCBName);
	pActiveCB->SetUserData(sDataIndex);
	pActiveCB->SetCheck(isActive==TRUE?true:false);

	CControlUI* pIPEdit = pHostRowHLayout->GetIPEdit();
	wchar_t szRowIPEditName[50] = {0};
	wsprintf(szRowIPEditName,L"%s_%s_%d",kHostRowIPEdit,groupIndex,m_rowIndex);
	pIPEdit->SetName(szRowIPEditName);
	pIPEdit->SetUserData(sDataIndex);
	pIPEdit->SetText(lpszIP);

	CControlUI* pDomainEdit = pHostRowHLayout->GetDomainEdit();
	wchar_t szRowDomainEditName[50] = {0};
	wsprintf(szRowDomainEditName,L"%s_%s_%d",kHostRowDomainEdit,groupIndex,m_rowIndex);
	pDomainEdit->SetName(szRowDomainEditName);
	pDomainEdit->SetUserData(sDataIndex);
	pDomainEdit->SetText(lpszDomain);

	CControlUI* pDescEdit = pHostRowHLayout->GetDescEdit();
	wchar_t szRowDescEditName[50] = {0};
	wsprintf(szRowDescEditName,L"%s_%s_%d",kHostRowDescEdit,groupIndex,m_rowIndex);
	pDescEdit->SetName(szRowDescEditName);
	pDescEdit->SetUserData(sDataIndex);
	pDescEdit->SetText(lpszDesc);

	CControlUI* pDeleteBtn = pHostRowHLayout->GetDeleteBtn();
	wchar_t szRowDeleteBtnName[50] = {0};
	wsprintf(szRowDeleteBtnName,L"%s_%s_%d",kHostRowDeleteBtn,groupIndex,m_rowIndex);
	pDeleteBtn->SetName(szRowDeleteBtnName);
	pDeleteBtn->SetUserData(sDataIndex);
	wchar_t deleteTip[50] = {0};
	Utility::GetINIStr(g_pLangManager->GetLangName(),LS_HOSTADMINPANEL,kHostDeleteRowTip,deleteTip);
	pDeleteBtn->SetToolTip(deleteTip);

	m_pHostAdminManager->Init(m_hHostAdminOwner);

	ResetRowsPos(pParentContainer);
	return TRUE;
}

BOOL IHostAdmin::ResetRowsPos(CContainerUI* pRowsBox)
{
	int rowHeight = 30;
	int y = 85;
	int count = pRowsBox->GetCount();
	for (int i=0;i<count;i++)
	{
		CControlUI* child = pRowsBox->GetItemAt(i);
		RECT rect = child->GetPos();
		rect.top = y;
		rect.bottom = rect.top+rowHeight;
		child->SetPos(rect);

		y += rect.bottom;
	}

	int rowsHeight = count*rowHeight;
	if (!pRowsBox->IsVisible())
	{
		rowsHeight=0;
	}
	int groupHeight = rowsHeight+40;

	RECT rowsBoxRect = pRowsBox->GetPos();
	rowsBoxRect.bottom = rowsBoxRect.top+rowsHeight;
	pRowsBox->SetFixedHeight(rowsHeight);
	pRowsBox->SetPos(rowsBoxRect);

	CHostAdminRowContainer* pGroupContainer = (CHostAdminRowContainer*)pRowsBox->GetParent();
	RECT groupRect = pGroupContainer->GetPos();
	groupRect.bottom = groupRect.top+groupHeight;
	pGroupContainer->SetFixedHeight(groupHeight);
	pGroupContainer->SetPos(groupRect);

	CContainerUI* pGroupWindow = (CContainerUI*)pGroupContainer->GetParent();
	pGroupWindow->SetFixedHeight(groupHeight);
	pGroupWindow->SetPos(groupRect);
	return TRUE;
}

BOOL IHostAdmin::SaveAll()
{
	wchar_t* hostsFileContent = new wchar_t[10240];
	int contentLenght = 10240;
	wmemset(hostsFileContent,0,contentLenght);
	int groupsCount = m_pHostGroupsBox->GetCount();
	for (int i=0;i<groupsCount;i++)
	{
		CContainerUI* groupWindow = (CContainerUI*)m_pHostGroupsBox->GetItemAt(i);
		CHostAdminGroupContainer* groupContainer = (CHostAdminGroupContainer*)groupWindow->GetItemAt(0);
		LPCTSTR groupTitle = groupContainer->GetTitleEdit()->GetText().GetData();

		wcscat_s(hostsFileContent,contentLenght,L"# ");
		wcscat_s(hostsFileContent,contentLenght,groupTitle);
		wcscat_s(hostsFileContent,contentLenght,L"\n");
		
		CContainerUI* rowsBox = (CContainerUI*)groupContainer->GetRowsLayout();
		int rowsCount = rowsBox->GetCount();
		for (int j=0;j<rowsCount;j++)
		{
			CContainerUI* rowWindow = (CContainerUI*)rowsBox->GetItemAt(j);
			CHostAdminRowContainer* rowContainer = (CHostAdminRowContainer*)rowWindow->GetItemAt(0);
			CCheckBoxUI* activeCB = (CCheckBoxUI*)rowContainer->GetCheckBox();
			if (!activeCB->IsSelected())
			{
				wcscat_s(hostsFileContent,contentLenght,L"#");
			}
			LPCTSTR ip =rowContainer->GetIPEdit()->GetText().GetData();
			LPCTSTR domain =rowContainer->GetDomainEdit()->GetText().GetData();
			LPCTSTR desc =rowContainer->GetDescEdit()->GetText().GetData();
			wcscat_s(hostsFileContent,contentLenght,ip);
			wcscat_s(hostsFileContent,contentLenght,L" ");
			wcscat_s(hostsFileContent,contentLenght,domain);
			wcscat_s(hostsFileContent,contentLenght,L" #");
			wcscat_s(hostsFileContent,contentLenght,desc);
			wcscat_s(hostsFileContent,contentLenght,L"\n");
		}
	}

	char* utf8 = StrUtil::u2u8(hostsFileContent);
	m_pHostParser->SaveHostsFile(utf8);
	delete[] utf8;
	delete[] hostsFileContent;
	RDMsgBox(m_hHostAdminOwner,MSG_OPT_SUCCESS,MSG_SUCCESS, MB_OK);
	return TRUE;
}
