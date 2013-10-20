#pragma once

//////////////////////////////////////////////////////////////////////////
// CTidy
RD_DECLARE_BEGIN(Tidy)
	RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	RD_DECLARE_MEMBER(void, OnItemActive) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);

public:
	RD_DECLARE_MEMBER(CListUI *, pTidyList);
RD_DECLARE_END(Tidy)
// CTidy
//////////////////////////////////////////////////////////////////////////