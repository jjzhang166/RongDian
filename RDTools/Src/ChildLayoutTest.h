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

//////////////////////////////////////////////////////////////////////////
// CChildLayoutTest
class CChildLayoutTest : public CBaseTool
{
public:
	CChildLayoutTest();
	virtual ~CChildLayoutTest() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/);

	CControlUI* OnCreateControl(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszClass*/, CControlUI* /*pParent*/);
	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	BOOL CreateTestContainer(CPaintManagerUI* /*pManager*/);
};
// CChildLayoutTest
//////////////////////////////////////////////////////////////////////////