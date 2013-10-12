#include "stdafx.h"
#include "ITidy.h"

const wchar_t* const kTidyList = L"tidy_list";
const wchar_t* const kTidyTest = L"tidy_test";

ITidy::ITidy()
{
	hTidyOwner = NULL;
	pTidyManager = NULL;
	pTidyList = NULL;
}

ITidy::~ITidy()
{

}

LPCWSTR ITidy::GetTidyListName()
{
	return kTidyList;
}

BOOL ITidy::InitTidy(IListCallbackUI* pIListCallback)
{
	if(!pTidyManager)
		return FALSE;
	return TRUE;
}

BOOL ITidy::SetTidyLang(LPCWSTR lpszLang)
{
	if(!pTidyManager)
		return FALSE;

	return TRUE;
}

CControlUI* ITidy::OnTidyCreateControl(LPCTSTR pstrClass, CControlUI *pParent)
{
	return NULL;
}

void ITidy::OnTidyClick(TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kTidyTest)
	{
		bHandled = TRUE;
	}
}

void ITidy::OnTidyItemActive(TNotifyUI& msg)
{

}