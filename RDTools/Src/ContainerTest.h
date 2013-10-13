#pragma once

class CContainerTest : public CVerticalLayoutUI
{
public:
	CContainerTest();
	~CContainerTest();

	LPCTSTR GetClass() const;

	void SaveButton(CControlUI *pButton) { pSubButton = pButton; };
	void SaveLabel(CControlUI *pLabel) { pSubLabel = pLabel; };
	void SaveEdit(CControlUI *pEdit) { pSubEdit = pEdit; };
	CControlUI* GetButton() { return pSubButton; };
	CControlUI* GetLabel() { return pSubLabel; };
	CControlUI* GetEdit() { return pSubEdit; };

private:
	CControlUI *pSubButton;
	CControlUI *pSubLabel;
	CControlUI *pSubEdit;
};