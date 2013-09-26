#include "stdafx.h"
#include "IHostAdmin.h"

// 头部
const wchar_t* const kHostPathText = L"host_path_text";
const wchar_t* const kHostPathEdit = L"host_path_edit";		
const wchar_t* const kHostOpenBtn = L"host_open";
const wchar_t* const kHostOpenTipBtn = L"host_open_tip";
const wchar_t* const kHostBackupBtn = L"host_backup";
const wchar_t* const kHostBackupTipBtn = L"host_backup_tip";
const wchar_t* const kHostNewGroupBtn = L"host_new_group";
const wchar_t* const kHostNewGroupTipBtn = L"host_new_group_tip";

IHostAdmin::IHostAdmin()
{
	m_hHostAdminOwner = NULL;
	m_pHostAdminManager = NULL;
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
	return TRUE;
}

BOOL IHostAdmin::SetHostAdminLang(LPCWSTR lpszLang)
{
	if(!m_pHostAdminManager)
		return FALSE;
	memset(m_szHandleText, 0, sizeof(m_szHandleText));
	memset(m_szNoHandleText, 0, sizeof(m_szNoHandleText));
	Utility::GetINIStr(lpszLang, LS_MSG, kHandleText, m_szHandleText);
	Utility::GetINIStr(lpszLang, LS_MSG, kNoHandleText, m_szNoHandleText);

SET_CONTROL_BEGIN(m_pHostAdminManager, lpszLang, LS_HOSTADMINPANEL)
	SET_CONTROL_TEXT2(kHostPathText)
	SET_CONTROL_TEXT_AND_TIP(kHostOpenBtn,kHostOpenTipBtn)
	SET_CONTROL_TEXT_AND_TIP(kHostBackupBtn,kHostBackupTipBtn)
	SET_CONTROL_TEXT_AND_TIP(kHostNewGroupBtn,kHostNewGroupTipBtn)
SET_CONTROL_END()

	return TRUE;
}

void IHostAdmin::OnHostAdminClick(TNotifyUI& msg, BOOL& bHandled)
{
	if(!m_pHostAdminManager)
		return;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kHostOpenBtn)
	{

	}
	else if (sCtrlName == kHostBackupBtn)
	{
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
