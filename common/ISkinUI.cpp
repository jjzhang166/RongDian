#include "stdafx.h"
#include "ISkinUI.h"

CSkinManager * CSkinManager::pInstance = NULL;
list<ISkinUI *> CSkinManager::lstFrames;
wchar_t CSkinManager::szBkImage[1024] = {0};
DWORD CSkinManager::dwBkColor = 0xFF32506E;
DWORD CSkinManager::dwBkColor2 = 0xFF466482;
DWORD CSkinManager::dwBorderColor = 0xFF32506E;
BOOL CSkinManager::bBkColor = TRUE;

CSkinManager::CSkinManager()
{

}

CSkinManager::~CSkinManager()
{
	//Release();
}

CSkinManager* CSkinManager::Instance()
{
	if(!pInstance)
		pInstance = new CSkinManager();
	return pInstance;
}

void CSkinManager::Release()
{
	if(pInstance)
		delete pInstance;
	pInstance = NULL;
}

void CSkinManager::SetBkImage(LPCWSTR lpszBkImage)
{
	memset(szBkImage, 0, sizeof(szBkImage));
	wcscpy(szBkImage, lpszBkImage);
	bBkColor = FALSE;
}

void CSkinManager::SetBkColor(DWORD dwColor)
{
	dwBkColor = dwColor;
	dwBorderColor = dwColor;
	BYTE bMaxValue = 235, bIncrement = 20;
	BYTE bAlpha = ((BYTE)((((DWORD_PTR)(dwColor)) >> 24) & 0xffffff));
	BYTE bRed = GetRValue(dwBkColor);
	if(bRed>=bMaxValue)
		bRed = bIncrement + (bMaxValue - bRed);
	else
		bRed += bIncrement;
	BYTE bGreen = GetGValue(dwBkColor);
	if(bGreen>=bMaxValue)
		bGreen = bIncrement + (bMaxValue - bGreen);
	else
		bGreen += bIncrement;
	BYTE bBlue = GetBValue(dwBkColor);
	if(bBlue>=bMaxValue)
		bBlue = bIncrement + (bMaxValue - bBlue);
	else
		bBlue += bIncrement;
	dwBkColor2 = (COLORREF)(((BYTE)(bRed)|((WORD)((BYTE)(bGreen))<<8))|(((DWORD)(BYTE)(bBlue))<<16)|(((DWORD)(BYTE)(bAlpha))<<24));
	bBkColor = TRUE;
}

void CSkinManager::AddFrame(ISkinUI *pFrame)
{
	ISkinUI * lpItem = NULL;
	list<ISkinUI *>::iterator iter;
	for(iter=lstFrames.begin();iter!=lstFrames.end();++iter)
	{
		lpItem = (*iter);
		if(lpItem==pFrame)
			break;
		else
			lpItem = NULL;
	}
	if(!lpItem)
		lstFrames.push_back(pFrame);
}

void CSkinManager::DelFrame(ISkinUI *pFrame)
{
	ISkinUI * lpItem = NULL;
	list<ISkinUI *>::iterator iter;
	for(iter=lstFrames.begin();iter!=lstFrames.end();++iter)
	{
		lpItem = (*iter);
		if(lpItem==pFrame)
		{
			lstFrames.erase(iter);
			delete lpItem;
			break;
		}
	}
}

void CSkinManager::UpdateBkImage(LPCWSTR lpszBkImage)
{
	SetBkImage(lpszBkImage);
	ISkinUI * lpItem = NULL;
	list<ISkinUI *>::iterator iter;
	for(iter=lstFrames.begin();iter!=lstFrames.end();++iter)
	{
		lpItem = (*iter);
		if(lpItem)
			lpItem->SetSkin();
	}
}

void CSkinManager::UpdateBkColor(DWORD dwColor)
{
	SetBkColor(dwColor);
	ISkinUI * lpItem = NULL;
	list<ISkinUI *>::iterator iter;
	for(iter=lstFrames.begin();iter!=lstFrames.end();++iter)
	{
		lpItem = (*iter);
		if(lpItem)
			lpItem->SetSkin();
	}
}