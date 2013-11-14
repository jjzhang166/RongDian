#include "StdAfx.h"
#include "Setting.h"

CSetting::CSetting()
{

}

BOOL CSetting::IsCanQuit(HWND /*hWnd*/)
{
	return TRUE;
}

void CSetting::OnQuit()
{

}

BOOL CSetting::OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

BOOL CSetting::SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/)
{
	return TRUE;
}

BOOL CSetting::Apply()
{
	return TRUE;
}
