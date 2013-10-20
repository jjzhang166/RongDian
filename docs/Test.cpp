#include "StdAfx.h"
#include "Test.h"

//////////////////////////////////////////////////////////////////////////
// 以上为必须实现函数
CTest::CTest()
{
	// 在这里初始化成员变量
	lpData = malloc(4096);
	pTest = NULL;
}

/* 子面板类析构函数，不在这里实现，在宏定义里面实现，相对应要实现的是OnQuit */

// 必须实现函数
BOOL CTest::IsCanQuit(HWND hWnd)
{
	// 通常情况下应该返回TRUE
	// 若此面板可以随时随地退出，则返回TRUE
	// 若此面板有功能需要异步操作，则需要考虑当前异步操作是否完成，对退出是否有影响
	return TRUE;
}

// 必须实现函数
// 退出时进行资源释放，代替子面板的析构函数的作用
void CTest::OnQuit()
{
	if(lpData)
		free(lpData);
}

// 必须实现函数
BOOL CTest::OnInit(WPARAM wParam, LPARAM lParam)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam; // 第一个参数为CPaintManagerUI *指针，该指针是主窗体的CPaintManagerUI *
	IListCallbackUI* pIListCallback = (IListCallbackUI *)lParam; // 第二个参数是IListCallbackUI *接口指针，实际上是CMainFrame*指针(CMainFrame继承于IListCallbackUI接口)，此参数在多列List控件时使用
	
	FIND_CONTROL_BY_ID(pTest, CControlUI, pManager, kTest)
	return TRUE;
}

// 必须实现函数
BOOL CTest::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang)
{
	if(!pManager)
		return FALSE;
SET_CONTROL_BEGIN(pManager, lpszLang, LS_TESTPANEL)
	SET_CONTROL_TEXT2(kTestText)
SET_CONTROL_END()

	return TRUE;
}
// 以上为必须实现函数
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 消息处理
void CTest::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kTest)
	{
		// 若成员方法中不需要访问主窗口句柄，或不需要查找子面板控件操作等(访问CPaintManagerUI*指针)，则可考虑不需要传递这两个参数
		OnTest(hWnd, pManager);
		bHandled = TRUE; // 注意此处，若已经处理过改点击事件，则返回TRUE，通知主窗体点击消息已被处理，结束当前消息处理
	}
// 消息处理
//////////////////////////////////////////////////////////////////////////

// 自定义成员函数
BOOL CTest::OnTest(HWND hWnd, CPaintManagerUI* pManager)
{
	BOOL bRet = FALSE;
	RDMsgBox(hWnd, MSG_ERR, MSG_ERR, MB_OK);
	return bRet;
}