#include "stdafx.h"
#include "IPanelXml.h"

BOOL IPanelXml::Open(LPCWSTR lpszPath)
{
	memset(szXmlPath, 0, sizeof(szXmlPath));
	_tcscpy(szXmlPath, lpszPath);
	bLoad = xmlDoc.loadFromFile(szXmlPath);

	return bLoad;
}

BOOL IPanelXml::LoadPanels()
{
	BOOL bRet = FALSE;
	if(!bLoad)
		return bRet;
	XmlNode *pRoot = xmlDoc.findChild(L"Root");
	if(!pRoot)
		return bRet;
	NodeIterator iter;
	LPCWSTR lpszLayout = NULL, lpszXml = NULL, lpszTab = NULL, 
		lpszPanel = NULL, lpszName = NULL, lpszDesc = NULL;
	XmlNode *pLayout = NULL, *pXml = NULL, *pTab = NULL,
		*pPanel = NULL, *pName = NULL, *pDesc = NULL;
	LPPANEL_INFO lpPanelInfo = NULL;
	wchar_t szPath[1024];
	XmlNode *pPlugin = pRoot->findFirstChild(L"Plugin", iter);
	if(!pPlugin)
		return TRUE;
	while(pPlugin)
	{
		pLayout = pPlugin->findChild(L"layout");
		pXml = pPlugin->findChild(L"xml");
		pTab = pPlugin->findChild(L"tab");
		pPanel = pPlugin->findChild(L"panel");
		pName = pPlugin->findChild(L"name");
		pDesc = pPlugin->findChild(L"desc");
		if(pLayout && pXml && pTab /*&& pPanel*/ && pName /*&& pDesc*/)
		{
			lpszLayout = pLayout->getString();
			lpszXml = pXml->getString();
			lpszTab = pTab->getString();
			//lpszPanel = pPanel->getString();
			lpszName = pName->getString();
			//lpszDesc = pDesc->getString();
			
			swprintf(szPath, L"%s\\%s", g_szResPath, lpszXml);
			if(PathFileExists(szPath)) // 检测界面xml文集是否存在
			{
				lpPanelInfo = new PANEL_INFO();
				if(lpPanelInfo)
				{
					memset(lpPanelInfo, 0, sizeof(PANEL_INFO));
					wcscpy(lpPanelInfo->szLayout, lpszLayout);
					wcscpy(lpPanelInfo->szXml, lpszXml);
					wcscpy(lpPanelInfo->szTab, lpszTab);
					//wcscpy(lpPanelInfo->szPanel, lpszPanel);
					wcscpy(lpPanelInfo->szName, lpszName);
					//wcscpy(lpPanelInfo->szDesc, lpszDesc);
					g_lstPanelInfo.push_back(lpPanelInfo);
				}
			}
		}

		pPlugin = pRoot->findNextChild(L"Plugin", iter);
	}
	return bRet;
}

BOOL IPanelXml::Write(LPCWSTR lpszSection, LPCWSTR lpszKeyName, LPCWSTR lpszValue)
{
	BOOL bRet = FALSE;
	if(!bLoad)
		return bRet;
	XmlNode *pRoot = xmlDoc.findChild(L"Root");
	if(!pRoot)
		return bRet;
	XmlNode *pSection = pRoot->findChild(lpszSection);
	if(!pSection)
		pSection = pRoot->addChild(lpszSection);
	if(!pSection)
		return bRet;
	XmlNode *pKey = pSection->findChild(lpszKeyName);
	if(!pKey)
		pKey = pSection->addChild(lpszKeyName);
	if(!pKey)
		return bRet;
	pKey->setString(lpszValue);
	bRet = TRUE;
	return bRet;
}

BOOL IPanelXml::Flush()
{
	return (xmlDoc.save(szXmlPath, UTF_16)==true);
}

BOOL IPanelXml::Reset()
{
	BOOL bRet = FALSE;
	if(!bLoad)
		return bRet;
	static char szTemplate[1024] = "<?xml version=\"1.0\" encoding=\"UTF-16\"?><Root></Root>";
	xmlDoc.loadFromMemory(szTemplate, strlen(szTemplate)*sizeof(char));
	return (xmlDoc.save(szXmlPath, UTF_16)==true);
}