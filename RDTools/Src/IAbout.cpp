#include "stdafx.h"
#include "IAbout.h"

// About's Frame
const wchar_t* const kAboutName = L"about_name";
const wchar_t* const kAboutVer = L"about_ver";
const wchar_t* const kAboutMail = L"about_mail";
const wchar_t* const kAboutSupport = L"about_support";
const wchar_t* const kAboutDesc = L"about_desc";
const wchar_t* const kAboutUrl = L"about_url";
const wchar_t* const kAboutCopyRight = L"about_copyright";

IAbout::IAbout()
{
	pAboutManager = NULL;
	hAboutOwner = NULL;
}

IAbout::~IAbout()
{

}

BOOL IAbout::InitAbout()
{
	return TRUE;
}

BOOL IAbout::SetAboutLang(LPCWSTR lpszLang)
{
	if(!pAboutManager)
		return FALSE;
	
SET_CONTROL_BEGIN(pAboutManager, lpszLang, LS_ABOUTPANEL)
	SET_CONTROL_TEXT(kAboutName, g_szAppName)
	SET_CONTROL_TEXT3(kAboutVer, g_szAppVer)
	SET_CONTROL_TEXT3(kAboutMail, kAuthorMail)
	SET_CONTROL_TIP(kAboutMail, kAuthorMail)
	SET_CONTROL_TEXT3(kAboutUrl, kAuthorUrl)
	SET_CONTROL_TIP(kAboutUrl, kAuthorUrl)
	SET_CONTROL_TEXT(kAboutCopyRight, kCopyRight)
SET_CONTROL_END()

	return TRUE;
}

void IAbout::OnAboutClick(TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kAboutMail)
	{
		bHandled = TRUE;
		Utility::OpenMail(hAboutOwner, kAuthorMail);
	}
	else if(sCtrlName == kAboutUrl)
	{
		bHandled = TRUE;
		//Utility::OpenUrl(hAboutOwner, kAuthorUrl);
		DuiLoading(hAboutOwner, L"Test...", NULL);
	}
}
