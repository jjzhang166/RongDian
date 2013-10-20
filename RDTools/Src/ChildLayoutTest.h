#pragma once

//////////////////////////////////////////////////////////////////////////
// CChildLayoutTest
RD_DECLARE_BEGIN(ChildLayoutTest)
	RD_DECLARE_MEMBER(CControlUI*, OnCreateControl)(LPCTSTR pstrClass, CControlUI *pParent);
	RD_DECLARE_MEMBER(void, OnClick)(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled);
	RD_DECLARE_MEMBER(void, OnItemActive) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	RD_DECLARE_MEMBER(BOOL, CreateTestContainer) (CPaintManagerUI* pManager);
RD_DECLARE_END(ChildLayoutTest)
// CChildLayoutTest
//////////////////////////////////////////////////////////////////////////