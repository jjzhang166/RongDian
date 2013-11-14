#pragma once

//////////////////////////////////////////////////////////////////////////
// CHosts
RD_DECLARE_BEGIN(Hosts)
	RD_DECLARE_MEMBER(CControlUI*, OnCreateControl)(CPaintManagerUI* /*pManager*/, LPCTSTR /*pstrClass*/, CControlUI* /*pParent*/);

	RD_DECLARE_MEMBER(void, OnClick)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemSelect)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/);

	RD_DECLARE_MEMBER(BOOL, CreateGroup)(CContainerUI* pParentContainer);
	RD_DECLARE_MEMBER(BOOL, CreateGroupRow)(CContainerUI* pParentContainer,int groupIndex);

public:
	RD_DECLARE_MEMBER(CEditUI*, m_pHostPathEdit);
	RD_DECLARE_MEMBER(CVerticalLayoutUI*, m_pHostGroupContainerLayout);
	RD_DECLARE_MEMBER(int, m_groupIndex);
	RD_DECLARE_MEMBER(int, m_rowIndex);
RD_DECLARE_END()
// CHosts
//////////////////////////////////////////////////////////////////////////