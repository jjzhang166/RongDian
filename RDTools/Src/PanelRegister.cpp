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

RD_BEGIN_TOOLS_MAP(CPanelRegister)
	RD_TOOL(CColorPicker)
	RD_TOOL(CCoder)
	RD_TOOL(CUrlEncoder)
	RD_TOOL(CFinder)
	RD_TOOL(CTidy)
	RD_TOOL(CIPConfig)
	RD_TOOL(CHosts)
	//RD_TOOL(CChildLayoutTest)
	RD_TOOL(CSetting)
	RD_TOOL(CAbout)
RD_END_TOOLS_MAP()

CPanelRegister::CPanelRegister()
{
	
}

CPanelRegister::~CPanelRegister()
{
}

void CPanelRegister::setLang(CPaintManagerUI* pPaintManger,LPCWSTR lpszLang)
{
	RD_ON_LANG_MSG(pPaintManger,lpszLang);
}

BOOL CPanelRegister::isCanQuit(HWND hwnd,CPaintManagerUI* pPaintManager,BOOL isCanQuit)
{
	RD_ISCAN_QUIT(hwnd, pPaintManager, isCanQuit)
	return isCanQuit;
}

void CPanelRegister::quit()
{
	RD_ON_QUIT()
}