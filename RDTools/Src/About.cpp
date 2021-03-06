#include "StdAfx.h"
#include "About.h"

// About's Frame
const wchar_t* const kAboutName = L"about_name";
const wchar_t* const kAboutVer = L"about_ver";
const wchar_t* const kAboutMail = L"about_mail";
const wchar_t* const kAboutSupport = L"about_support";
const wchar_t* const kAboutDesc = L"about_desc";
const wchar_t* const kAboutUrl = L"about_url";
const wchar_t* const kAboutCopyRight = L"about_copyright";

CAbout::CAbout()
{

}

BOOL CAbout::IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/)
{
	return TRUE;
}

void CAbout::OnQuit()
{

}

BOOL CAbout::OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

BOOL CAbout::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang, LPCWSTR lpszLangSection)
{
	if(!pManager)
		return FALSE;
SET_CONTROL_BEGIN(pManager, lpszLang, lpszLangSection)
	SET_CONTROL_TEXT(kAboutName, g_szAppName)
	SET_CONTROL_TEXT3(kAboutVer, g_szAppVer)
	//SET_CONTROL_TEXT3(kAboutMail, kAuthorMail)
	//SET_CONTROL_TIP(kAboutMail, kAuthorMail)
	SET_CONTROL_TEXT3(kAboutUrl, kAuthorUrl)
	SET_CONTROL_TIP(kAboutUrl, kAuthorUrl)
	SET_CONTROL_TEXT(kAboutCopyRight, kCopyRight)
SET_CONTROL_END()

	return TRUE;
}

void CAbout::OnClick(HWND hWnd, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(wcsstr(sCtrlName.GetData(), kAboutMail))
	{
		bHandled = TRUE;
		Utility::OpenMail(hWnd, msg.pSender->GetText().GetData());
	}
	else if(sCtrlName == kAboutUrl)
	{
		bHandled = TRUE;
		Utility::OpenUrl(hWnd, kAuthorUrl);
	}
}