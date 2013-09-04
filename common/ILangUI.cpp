#include "stdafx.h"
#include "ILangUI.h"

CLangManager * CLangManager::pInstance = NULL;
list<ILangUI *> CLangManager::lstFrames;
wchar_t CLangManager::szLangName[1024] = {0};

CLangManager::CLangManager()
{

}

CLangManager::~CLangManager()
{
	//Release();
}

CLangManager* CLangManager::Instance()
{
	if(!pInstance)
		pInstance = new CLangManager();
	return pInstance;
}

void CLangManager::Release()
{
	if(pInstance)
		delete pInstance;
	pInstance = NULL;
}

void CLangManager::SetLangName(LPCWSTR lpszLangName)
{
	memset(szLangName, 0, sizeof(szLangName));
	wcscpy(szLangName, lpszLangName);
}

void CLangManager::AddFrame(ILangUI *pFrame)
{
	ILangUI * lpItem = NULL;
	list<ILangUI *>::iterator iter;
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

void CLangManager::DelFrame(ILangUI *pFrame)
{
	ILangUI * lpItem = NULL;
	list<ILangUI *>::iterator iter;
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

void CLangManager::UpdateLang(LPCWSTR lpszLang)
{
	SetLangName(lpszLang);
	ILangUI * lpItem = NULL;
	list<ILangUI *>::iterator iter;
	for(iter=lstFrames.begin();iter!=lstFrames.end();++iter)
	{
		lpItem = (*iter);
		if(lpItem)
			lpItem->SetLang(lpszLang);
	}
}
