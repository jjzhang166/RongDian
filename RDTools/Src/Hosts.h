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

	RD_DECLARE_MEMBER(BOOL, ShowHostItems) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, ShowHostDesc) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, NewHostItem) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, DelHostGroup) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, DelHostItem) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostGroupName) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostGroupDesc) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostItemAddr) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostItemDomain) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	RD_DECLARE_MEMBER(BOOL, UpdateHostItemDesc) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);

	RD_DECLARE_MEMBER(BOOL, CreateGroup)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, const wchar_t* /*pszGroupName*/, const wchar_t* pszDesc = NULL);
	RD_DECLARE_MEMBER(BOOL, CreateItem)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, const wchar_t* /*pszAddr*/, const wchar_t* /*pszDomain*/, const wchar_t* pszDesc = NULL);

	RD_DECLARE_MEMBER(BOOL, AdjustGroupHeight)(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, BOOL bInit = FALSE);

public:
	RD_DECLARE_MEMBER(CEditUI*, m_pHostPathEdit);
	RD_DECLARE_MEMBER(CVerticalLayoutUI*, m_pHostGroupContainerLayout);
	RD_DECLARE_MEMBER(CLabelUI*, m_pHostTipText);
	RD_DECLARE_MEMBER(CHostsHelper, HostsHelper);
	RD_DECLARE_MEMBER(BOOL, bModified);
RD_DECLARE_END()
// CHosts
//////////////////////////////////////////////////////////////////////////