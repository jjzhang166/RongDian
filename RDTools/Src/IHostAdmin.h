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

	void			OnHostAdminClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnHostAdminItemSelect(TNotifyUI& msg);
	BOOL			CreateGroup(CContainerUI* pParentContainer);
	BOOL			CreateGroupRow(CContainerUI* pParentContainer,int groupIndex);
public:
	HWND m_hHostAdminOwner;
	CPaintManagerUI *m_pHostAdminManager;

	CEditUI*   m_pHostPathEdit;
	CButtonUI* m_pHostOpenBtn;
	CButtonUI* m_pHostBackupBtn;
	CButtonUI* m_pHostNewGroupBtn;
	CVerticalLayoutUI* m_pHostGroupContainerLayout;

	int m_groupIndex;
	int m_rowIndex;
};
