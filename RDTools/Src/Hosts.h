#pragma once

#include "HostsHelper.h"

//////////////////////////////////////////////////////////////////////////
// CHosts
RD_DECLARE_BEGIN(Hosts)
	RD_DECLARE_MEMBER(CControlUI*, OnCreateControl)(CPaintManagerUI* /*pManager*/, LPCTSTR /*pstrClass*/, CControlUI* /*pParent*/);

	RD_DECLARE_MEMBER(void, OnClick)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnTextChanged) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	RD_DECLARE_MEMBER(BOOL, InitHosts) (CPaintManagerUI* /*pManager*/);

	RD_DECLARE_MEMBER(BOOL, OpenHostsFile) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, BackupHostsFile) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, NewHostsGroup) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, LoadHostsFile) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, SaveHostsFile) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);

	RD_DECLARE_MEMBER(BOOL, ShowHostItems) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, ShowHostDesc) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, NewHostItem) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, DelHostGroup) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, DelHostItem) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostGroupName) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostGroupDesc) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostItemAddr) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostItemDomain) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostItemDesc) (CControlUI* /*pSender*/, CPaintManagerUI* /*pManager*/);

	RD_DECLARE_MEMBER(BOOL, CreateGroup)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, const wchar_t* /*pszGroupName*/, const wchar_t* pszDesc = NULL);
	RD_DECLARE_MEMBER(BOOL, CreateItem)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, const wchar_t* /*pszAddr*/, const wchar_t* /*pszDomain*/, const wchar_t* pszDesc = NULL);

	RD_DECLARE_MEMBER(BOOL, AdjustGroupHeight)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, BOOL bInit = FALSE);

public:
	RD_DECLARE_MEMBER(CEditUI*, m_pHostPathEdit);
	RD_DECLARE_MEMBER(CVerticalLayoutUI*, m_pHostGroupContainerLayout);
	RD_DECLARE_MEMBER(CHostsHelper, HostsHelper);
	RD_DECLARE_MEMBER(BOOL, bModified);
RD_DECLARE_END()
// CHosts
//////////////////////////////////////////////////////////////////////////