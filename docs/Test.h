#pragma once

//////////////////////////////////////////////////////////////////////////
// CTest
RD_DECLARE_BEGIN(Test) --> 此宏定义调用后，会声明一个CTest类，并且该类继承于CBaseTool类
	RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled);
	
	RD_DECLARE_MEMBER(BOOL, OnTest) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	
public:
	RD_DECLARE_MEMBER(LPBYTE, lpData);
	RD_DECLARE_MEMBER(CControlUI*, pTest);
RD_DECLARE_END(Test)
// CTest
//////////////////////////////////////////////////////////////////////////