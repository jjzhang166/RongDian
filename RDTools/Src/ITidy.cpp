#include "stdafx.h"
#include "ITidy.h"

const wchar_t* const kTidyList = L"tidy_list";

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

void ITidy::OnTidyClick(TNotifyUI& msg, BOOL& bHandled)
{

}

void ITidy::OnTidyItemActive(TNotifyUI& msg)
{

}