#pragma once

class CHostAdminRowContainer : public CHorizontalLayoutUI
{
public:
	CHostAdminRowContainer();
	~CHostAdminRowContainer();

	LPCTSTR GetClass() const;
	void SaveCheckBox(CControlUI* pControl){pCheckBox = pControl;};
	void SaveIPEdit(CControlUI* pControl){pIPEdit = pControl;};
	void SaveDomainEdit(CControlUI* pControl){pDomainEdit = pControl;};
	void SaveDescEdit(CControlUI* pControl){pDescEdit = pControl;};
	void SaveSaveBtn(CControlUI* pControl){pSaveBtn = pControl;};
	void SaveDeleteBtn(CControlUI* pControl){pDeleteBtn = pControl;};

	CControlUI* GetCheckBox(){return pCheckBox;};
	CControlUI* GetIPEdit(){return pIPEdit;};
	CControlUI* GetDomainEdit(){return pDomainEdit;};
	CControlUI* GetDescEdit(){return pDescEdit;};
	CControlUI* GetSaveBtn(){return pSaveBtn;};
	CControlUI* GetDeleteBtn(){return pDeleteBtn;};

public:
	CControlUI* pCheckBox;
	CControlUI* pIPEdit;
	CControlUI* pDomainEdit;
	CControlUI* pDescEdit;
	CControlUI* pSaveBtn;
	CControlUI* pDeleteBtn;
};
