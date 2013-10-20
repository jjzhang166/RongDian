#include "StdAfx.h"
#include "Hosts.h"

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

CHosts::CHosts()
{
	m_groupIndex = 0;
	m_rowIndex = 0;
	m_pHostGroupContainerLayout = NULL;
}

BOOL CHosts::IsCanQuit(HWND hWnd)
{
	return TRUE;
}

void CHosts::OnQuit()
{

}

BOOL CHosts::OnInit(WPARAM wParam, LPARAM lParam)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	if(!pManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_pHostPathEdit, CEditUI, pManager, kHostPathEdit)
	FIND_CONTROL_BY_ID(m_pHostGroupContainerLayout, CVerticalLayoutUI, pManager, kHostGroupContainer)

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
SET_CONTROL_END()

	return TRUE;
}

CControlUI* CHosts::OnCreateControl(LPCTSTR pstrClass, CControlUI *pParent)
{
	return NULL;
}

void CHosts::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
}

void CHosts::OnItemSelect(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg)
{
	CComboUI* pCombo = (CComboUI*)msg.pSender;
	CDuiString sItemName = pCombo->GetName();
	
}