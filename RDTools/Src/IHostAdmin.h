#pragma once

class IHostAdmin
{
public:
	IHostAdmin();
	virtual ~IHostAdmin();

public:
	void			SetHostAdminOwner(HWND hWnd, CPaintManagerUI *pManager) { m_hHostAdminOwner = hWnd; m_pHostAdminManager = pManager; };
	BOOL			InitHostAdmin();
	BOOL			SetHostAdminLang(LPCWSTR lpszLang);
	CControlUI*		OnHostAdminCreateControl(LPCTSTR pstrClass, CControlUI *pParent);

	void			OnHostAdminClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnHostAdminItemSelect(TNotifyUI& msg);
	CContainerUI*	CreateGroup(LPCWSTR lpszGroupTitle);
	BOOL			CreateGroupRow(CContainerUI* pParentContainer,BOOL isActive,LPCWSTR lpszIP,LPCWSTR lpszDomain,LPCWSTR lpszDesc);
	BOOL			ResetRowsPos(CContainerUI* pContainer);//重置每一行的位置
	BOOL			SaveAll();
public:
	HWND m_hHostAdminOwner;
	CPaintManagerUI *m_pHostAdminManager;

	CEditUI*   m_pHostPathEdit;
	CButtonUI* m_pHostOpenBtn;
	CButtonUI* m_pHostBackupBtn;
	CButtonUI* m_pHostSaveBtn;
	CButtonUI* m_pHostNewGroupBtn;
	CVerticalLayoutUI* m_pHostGroupsBox;
	CContainerUI* m_pLastRowsBox;

	int m_groupIndex;
	int m_rowIndex;

	CHostsParser* m_pHostParser;
};
