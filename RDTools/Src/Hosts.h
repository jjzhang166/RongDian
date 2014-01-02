#pragma once

#include "HostsHelper.h"

//////////////////////////////////////////////////////////////////////////
// CHosts
RD_DECLARE_BEGIN(Hosts)
	RD_DECLARE_MEMBER(CControlUI*, OnCreateControl)(CPaintManagerUI* /*pManager*/, LPCTSTR /*pstrClass*/, CControlUI* /*pParent*/);

	RD_DECLARE_MEMBER(void, OnClick)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemSelect)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);

	RD_DECLARE_MEMBER(BOOL, InitHosts) (CPaintManagerUI* /*pManager*/);

	RD_DECLARE_MEMBER(BOOL, OpenHostsFile) (HWND /*hWnd*/);
	RD_DECLARE_MEMBER(BOOL, BackupHostsFile) (HWND /*hWnd*/);
	RD_DECLARE_MEMBER(BOOL, CreateNewHosts) (HWND /*hWnd*/);

	RD_DECLARE_MEMBER(BOOL, CreateGroup)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, const wchar_t* /*pszGroupName*/, const wchar_t* pszDesc = NULL);
	RD_DECLARE_MEMBER(BOOL, CreateItem)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, const wchar_t* /*pszAddr*/, const wchar_t* /*pszDomain*/, const wchar_t* pszDesc = NULL);

public:
	RD_DECLARE_MEMBER(CEditUI*, m_pHostPathEdit);
	RD_DECLARE_MEMBER(CVerticalLayoutUI*, m_pHostGroupContainerLayout);
	RD_DECLARE_MEMBER(int, m_groupIndex);
	RD_DECLARE_MEMBER(int, m_rowIndex);
	RD_DECLARE_MEMBER(CHostsHelper, HostsHelper);
RD_DECLARE_END()
// CHosts
//////////////////////////////////////////////////////////////////////////