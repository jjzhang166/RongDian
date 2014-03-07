#include "stdafx.h"
#include "PanelRegister.h"

//panels
#include "ColorPicker.h"
#include "UrlEncoder.h"
#include "Coder.h"
#include "Finder.h"
#include "Tidy.h"
#include "IPConfig.h"
#include "Hosts.h"
#include "ChildLayoutTest.h"
#include "Setting.h"
#include "About.h"

CPanelRegister::CPanelRegister()
{
}

CPanelRegister::~CPanelRegister()
{
}

BOOL CPanelRegister::InitTools()
{
	RD_TOOL(L"plugin_picker",CColorPicker)
	RD_TOOL(L"plugin_coder",CCoder)
	RD_TOOL(L"plugin_urlencoder",CUrlEncoder)
	RD_TOOL(L"plugin_finder",CFinder)
	RD_TOOL(L"plugin_tidy",CTidy)
	RD_TOOL(L"plugin_ipconfig",CIPConfig)
	RD_TOOL(L"plugin_hosts",CHosts)
	//RD_TOOL(CChildLayoutTest)
	RD_TOOL(L"plugin_setttings",CSetting)
	RD_TOOL(L"plugin_about",CAbout)
	return TRUE;
}

void CPanelRegister::ReleaseTools()
{
	LPTOOLS_INFO tool = NULL;
	if(lstToolsEntries.size())
		tool = (LPTOOLS_INFO)lstToolsEntries.back();
	while(tool)
	{
		lstToolsEntries.pop_back();
		if(tool->lpClass)
		{
			if(tool->lpfnDestroy)
				tool->lpfnDestroy(tool->lpClass);
			else
				delete tool->lpClass;
		}
		delete tool;
		tool = NULL;
		if(lstToolsEntries.size())
			tool = (LPTOOLS_INFO)lstToolsEntries.back();
	}
}

BOOL CPanelRegister::OnInit(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			break;
		}
		bRet = tool->lpClass->OnInit(wParam, lParam);
		if(!bRet)
			break;
	}
	return bRet;
}

BOOL CPanelRegister::SetLang(CPaintManagerUI* pPaintManger, LPCWSTR lpszLang)
{
	BOOL bRet = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			break;
		}
		bRet = tool->lpClass->SetLang(pPaintManger, lpszLang ,tool->szName);
		if(!bRet)
			break;
	}
	return bRet;
}

CControlUI* CPanelRegister::OnCreateControl(CPaintManagerUI* pManager, LPCWSTR lpszClass, CControlUI* pParent)
{
	CControlUI *pControl = NULL;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			pControl = NULL;
			break;
		}
		pControl = tool->lpClass->OnCreateControl(pManager, lpszClass, pParent);
		if(pControl)
			return pControl;
	}
	return pControl;
}

void CPanelRegister::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			bHandled = FALSE;
			break;
		}
		tool->lpClass->OnClick(hWnd, pManager, msg, bHandled);
		if(bHandled)
			break;
	}
}

void CPanelRegister::OnItemActive(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			bHandled = FALSE;
			break;
		}
		tool->lpClass->OnItemActive(hWnd, pManager, msg, bHandled);
		if(bHandled)
			break;
	}
}

void CPanelRegister::OnItemClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			bHandled = FALSE;
			break;
		}
		tool->lpClass->OnItemClick(hWnd, pManager, msg, bHandled);
		if(bHandled)
			break;
	}
}

void CPanelRegister::OnItemSelected(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			bHandled = FALSE;
			break;
		}
		tool->lpClass->OnItemSelected(hWnd, pManager, msg, bHandled);
		if(bHandled)
			break;
	}
}

void CPanelRegister::OnTextChanged(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			bHandled = FALSE;
			break;
		}
		tool->lpClass->OnTextChanged(hWnd, pManager, msg, bHandled);
		if(bHandled)
			break;
	}
}

LPCWSTR CPanelRegister::GetItemText(HWND hWnd, CPaintManagerUI* pManager, CControlUI* pControl, int iIndex, int iSubItem)
{
	LPCWSTR lpData = L"";
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			break;
		}
		lpData = tool->lpClass->GetItemText(hWnd, pManager, pControl, iIndex, iSubItem);
		if(wcscmp(lpData, L"")!=0)
			break;
	}
	return lpData;
}

BOOL CPanelRegister::IsCanQuit(HWND hWnd, CPaintManagerUI* pPaintManager)
{
	BOOL bIsCanQuit = TRUE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			bIsCanQuit = FALSE;
			break;
		}
		bIsCanQuit = tool->lpClass->IsCanQuit(hWnd, pPaintManager);
		if(!bIsCanQuit)
			break;
	}
	return bIsCanQuit;
}

void CPanelRegister::OnQuit()
{
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			return;
		}
		tool->lpClass->OnQuit();
	}
}

LRESULT CPanelRegister::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HRESULT hRes = 0;
	BOOL bHandle = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			continue;
		}
		hRes = tool->lpClass->HandleCustomMessage(uMsg, wParam, lParam, bHandle);
		if(hRes)
			break;
	}
	return hRes;
}

LRESULT CPanelRegister::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;
	list<LPTOOLS_INFO>::iterator iter;
	LPTOOLS_INFO tool = NULL;
	for(iter=lstToolsEntries.begin(); iter!=lstToolsEntries.end(); iter++)
	{
		tool = (*iter);
		if(!tool->lpClass)
		{
			continue;
		}
		bRet = tool->lpClass->OnCopyData(wParam, lParam);
		if(bRet)
			break;
	}
	return bRet;
}
