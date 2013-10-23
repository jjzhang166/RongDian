#pragma once

class CHostAdminGroupContainer : public CVerticalLayoutUI
{
public:
	CHostAdminGroupContainer();
	~CHostAdminGroupContainer();

	LPCTSTR GetClass() const;
	void SaveVisibleBtn(CControlUI* pButton){pVisibleBtn = pButton;};
	void SaveNewRowBtn(CControlUI* pButton){pNewRowBtn = pButton;};
	void SaveSaveBtn(CControlUI* pButton){pSaveBtn = pButton;};
	void SaveDeleteBtn(CControlUI* pButton){pDeleteBtn = pButton;};
	void SaveTitleEdit(CControlUI* pControl){pTitleEdit = pControl;};
	void SaveRowsVLayout(CControlUI* pLayout){pRowsVLayout = pLayout;};

	CControlUI* GetVisibleBtn(){return pVisibleBtn;};
	CControlUI* GetNewRowBtn(){return pNewRowBtn;};
	CControlUI* GetSaveBtn(){return pSaveBtn;};
	CControlUI* GetDeleteBtn(){return pDeleteBtn;};
	CControlUI* GetTitleEdit(){return pTitleEdit;};
	CControlUI* GetRowsLayout(){return pRowsVLayout;};

public:
	CControlUI* pVisibleBtn;
	CControlUI* pNewRowBtn;
	CControlUI* pSaveBtn;
	CControlUI* pDeleteBtn;
	CControlUI* pTitleEdit;
	CControlUI* pRowsVLayout;
};
