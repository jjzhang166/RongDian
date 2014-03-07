#include "stdafx.h"
#include "PanelXml.h"

CPanelXml::CPanelXml()
{
	bLoad = FALSE;
}

CPanelXml::~CPanelXml()
{

}

BOOL CPanelXml::Open(LPCWSTR lpszPath)
{
	bLoad = xmlDoc.loadFromFile(lpszPath);
	return bLoad;
}

BOOL CPanelXml::LoadPanels(LPCWSTR lpszRoot, list<LPMAIN_PANEL> &lstPanels)
{
	BOOL bRet = FALSE;
	if(!bLoad)
		return bRet;
	XmlNode *pRoot = xmlDoc.findChild(L"Root");
	if(!pRoot)
		return bRet;
	wchar_t szTabNameKey[256],szTabName[256];
	wchar_t szTabDescKey[1024],szTabDesc[1024];
	NodeIterator iter;
	LPCWSTR lpszLayout = NULL, lpszXml = NULL, lpszTab = NULL, 
		lpszPanel = NULL, lpszName = NULL, lpszDesc = NULL, lpszDll = NULL;
	XmlNode *pLayout = NULL, *pXml = NULL, *pTab = NULL,
		*pPanel = NULL, *pName = NULL, *pDesc = NULL, *pDll = NULL;
	LPMAIN_PANEL lpPanelInfo = NULL;
	wchar_t szPath[1024];
	XmlNode *pPlugin = pRoot->findFirstChild(L"Plugin", iter);
	if(!pPlugin)
		return TRUE;
	while(pPlugin)
	{
		pLayout = pPlugin->findChild(L"layout");
		pXml = pPlugin->findChild(L"xml");
		pTab = pPlugin->findChild(L"tab");
		pDll = pPlugin->findChild(L"dll");
		if(pLayout && pXml && pTab)
		{
			lpszLayout = pLayout->getString();
			lpszXml = pXml->getString();
			lpszTab = pTab->getString();
			if(pDll)
				lpszDll = pDll->getString();
			swprintf(szPath, L"%s\\%s", lpszRoot, lpszXml);
			if(PathFileExists(szPath)) // 检测界面xml文集是否存在
			{
				lpPanelInfo = new MAIN_PANEL();
				if(lpPanelInfo)
				{
					memset(lpPanelInfo, 0, sizeof(MAIN_PANEL));
					wcscpy(lpPanelInfo->szLayout, lpszLayout);
					wcscpy(lpPanelInfo->szXml, lpszXml);
					wcscpy(lpPanelInfo->szTab, lpszTab);
					wsprintf(szTabNameKey,L"%s_name",lpPanelInfo->szTab);
					wsprintf(szTabDescKey,L"%s_desc",lpPanelInfo->szTab);
					//wcscpy(lpPanelInfo->szPanel, lpszPanel);
					Utility::GetINIStr(g_pLangManager->GetLangName(), lpszTab, szTabNameKey, szTabName);
					Utility::GetINIStr(g_pLangManager->GetLangName(), lpszTab, szTabDescKey, szTabDesc);
					wcscpy(lpPanelInfo->szName, szTabName);
					wcscpy(lpPanelInfo->szDesc, szTabDesc);
					if(lpszDll)
						wcscpy(lpPanelInfo->szDll, lpszDll);
					else
						memset(lpPanelInfo->szDll, 0, sizeof(lpPanelInfo->szDll));
					lstPanels.push_back(lpPanelInfo);
				}
			}
			lpszLayout = NULL;
			lpszXml = NULL;
			lpszTab = NULL;
			lpszPanel = NULL;
			lpszName = NULL;
			lpszDesc = NULL;
			lpszDll = NULL;
		}

		pPlugin = pRoot->findNextChild(L"Plugin", iter);
	}
	bRet = TRUE;
	return bRet;
}