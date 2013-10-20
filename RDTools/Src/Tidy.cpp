#include "StdAfx.h"
#include "Tidy.h"

const wchar_t* const kTidyList = L"tidy_list";
const wchar_t* const kTidyTest = L"tidy_test";

CTidy::CTidy()
{
	pTidyList = NULL;

}

BOOL CTidy::IsCanQuit(HWND hWnd)
{
	return TRUE;
}

void CTidy::OnQuit()
{

}

BOOL CTidy::OnInit(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

BOOL CTidy::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang)
{
	return TRUE;
}

void CTidy::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kTidyTest)
	{
		bHandled = TRUE;
	}
}

void CTidy::OnItemActive(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(!pParent)
		return;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return;
	if(pList->GetName()!=kTidyList)
		return;
	CDuiString sCtrlName = msg.pSender->GetClass();
	if(sCtrlName==L"ListLabelElementUI")
	{

	}
}
