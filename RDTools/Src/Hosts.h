#pragma once

#include "HostsHelper.h"

//////////////////////////////////////////////////////////////////////////
// CHosts
class CHosts : public CBaseTool
{
public:
	CHosts();
	virtual ~CHosts() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/, LPCWSTR /*lpszLangSection*/);

	CControlUI* OnCreateControl(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszClass*/, CControlUI* /*pParent*/);
	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnTextChanged(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

	BOOL InitHosts(CPaintManagerUI* /*pManager*/);

	BOOL OpenHostsFile(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	BOOL BackupHostsFile(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	BOOL NewHostsGroup(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	BOOL LoadHostsFile(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	BOOL SaveHostsFile(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);

	BOOL ShowHostItems(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL ShowHostDesc(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL NewHostItem(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL DelHostGroup(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL DelHostItem(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL ToggleHostItem(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL UpdateHostGroupName(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL UpdateHostGroupDesc(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL UpdateHostItemAddr(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL UpdateHostItemDomain(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);
	BOOL UpdateHostItemDesc(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pSender*/);

	BOOL CreateGroup(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, const wchar_t* /*pszGroupName*/, const wchar_t* pszDesc = NULL);
	BOOL CreateItem(CPaintManagerUI* /*pManager*/, bool active,const wchar_t* /*pszGroupId*/, const wchar_t* /*pszAddr*/, const wchar_t* /*pszDomain*/, const wchar_t* pszDesc = NULL);

	BOOL AdjustGroupHeight(CPaintManagerUI* /*pManager*/, const wchar_t* /*pszGroupId*/, BOOL bInit = FALSE);

public:
	CEditUI*			m_pHostPathEdit;
	CVerticalLayoutUI*  m_pHostGroupContainerLayout;
	CLabelUI*			m_pHostTipText;
	CHostsHelper		m_HostsHelper;
	BOOL				m_bModified;
};
// CHosts
//////////////////////////////////////////////////////////////////////////