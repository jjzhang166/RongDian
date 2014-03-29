#include "stdafx.h"
#include "PanelXml.h"

CPanelXml::CPanelXml()
{
	memset(m_lpszRoot, 0, sizeof(m_lpszRoot));
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

BOOL CPanelXml::LoadPanels(LPCWSTR lpszRoot)
{
	wcscpy(m_lpszRoot,lpszRoot);
	BOOL bRet = FALSE;
	if(!bLoad)
		return bRet;
	XmlNode *pRoot = xmlDoc.findChild(L"Root");
	if(!pRoot)
		return bRet;
	NodeIterator iter;
	XmlNode *pPlugin = pRoot->findFirstChild(L"Plugin", iter);
	if(!pPlugin)
		return TRUE;
	while(pPlugin)
	{
		LoadPanel(pPlugin,NULL);
		pPlugin = pRoot->findNextChild(L"Plugin", iter);
	}
	bRet = TRUE;
	return bRet;
}

BOOL CPanelXml::LoadPanel(XmlNode *pPlugin,LPMAIN_PANEL lpParentPanelInfo)
{
	LPCWSTR lpszLayout = NULL, lpszXml = NULL, lpszTab = NULL, 
		lpszPanel = NULL, lpszName = NULL, lpszDesc = NULL, lpszDll = NULL, lpszSubPlugin = NULL;
	XmlNode *pLayout = NULL, *pXml = NULL, *pTab = NULL,
		*pPanel = NULL, *pName = NULL, *pDesc = NULL, *pDll = NULL, *pSubPlugin = NULL;
	wchar_t szPath[1024];
	wchar_t szTabNameKey[256],szTabName[256];
	wchar_t szTabDescKey[1024],szTabDesc[1024];
	LPMAIN_PANEL lpPanelInfo;
	//memset(lpPanelInfo, 0, sizeof(MAIN_PANEL));
	if(pPlugin)
	{	
		pTab = pPlugin->findChild(L"tab");
		if(pTab)
		{
			lpPanelInfo = new MAIN_PANEL();
			pLayout = pPlugin->findChild(L"layout");
			pXml = pPlugin->findChild(L"xml");
			pDll = pPlugin->findChild(L"dll");
			lpszTab = pTab->getString();
			wsprintf(lpPanelInfo->szLayout,L"%s_layout",lpszTab);
			if(pXml)
			{
				lpszXml = pXml->getString();
				wcscpy(lpPanelInfo->szXml, lpszXml);
			}
			if(pDll)
				lpszDll = pDll->getString();
			if (pXml)
				swprintf(szPath, L"%s\\%s", m_lpszRoot, lpszXml);
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
			if (lpParentPanelInfo!=NULL)
			{
				lpParentPanelInfo->childs.push_back(lpPanelInfo);
			}
			else
			{
				g_lstPanelInfo.push_back(lpPanelInfo);
			}
			lpszLayout = NULL;
			lpszXml = NULL;
			lpszTab = NULL;
			lpszPanel = NULL;
			lpszName = NULL;
			lpszDesc = NULL;
			lpszDll = NULL;
		}
		int subPluginSize = pPlugin->getChildCount();
		for(int i=0;i<subPluginSize;i++)
		{
			XmlNode* pSubPlugin = pPlugin->findChild(i);
			if(wcsicmp(pSubPlugin->getName(),L"Plugin")==0&&lpPanelInfo)
			{
				LoadPanel(pSubPlugin,lpPanelInfo);
			}
		}
	}
	return TRUE;
}

BOOL CPanelXml::GetPanel(LPCWSTR lpszPluginName,LPMAIN_PANEL &lpPanelInfo)
{
	LPMAIN_PANEL lpInfo;
	list<LPMAIN_PANEL>::iterator iter;
	for(iter=g_lstPanelInfo.begin(); iter!=g_lstPanelInfo.end(); iter++)
	{
		lpInfo = (*iter);
		if(wcsicmp(lpszPluginName,lpInfo->szTab)==0)
		{
			lpPanelInfo = lpInfo;
			break;
		}
		else if(!lpInfo->childs.empty())
		{
			LoopGetPanel(lpInfo->childs,lpszPluginName,lpPanelInfo);
		}
	}
	return TRUE;
}

BOOL CPanelXml::LoopGetPanel(list<LPMAIN_PANEL> panels,LPCWSTR lpszPluginName,LPMAIN_PANEL &lpPanelInfo)
{
	LPMAIN_PANEL lpInfo;
	list<LPMAIN_PANEL>::iterator iter;
	for(iter=panels.begin(); iter!=panels.end(); iter++)
	{
		lpInfo = (*iter);
		if(wcsicmp(lpszPluginName,lpInfo->szTab)==0)
		{
			lpPanelInfo = lpInfo;
			break;
		}
		else if(!lpInfo->childs.empty())
		{
			LoopGetPanel(lpInfo->childs,lpszPluginName,lpPanelInfo);
		}
	}
	return TRUE;
}