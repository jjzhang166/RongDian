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

public:
	HWND m_hHostAdminOwner;
	CPaintManagerUI *m_pHostAdminManager;

	CEditUI*   m_pHostPathEdit;
	CButtonUI* m_pHostOpenBtn;
	CButtonUI* m_pHostBackupBtn;
	CButtonUI* m_pHostNewGroupBtn;
	wchar_t m_szHandleText[1024];
	wchar_t m_szNoHandleText[1024];
};
