#pragma once

class CPanelRegister
{
public:
	CPanelRegister();
	~CPanelRegister();
	RD_DECLARE_TOOLS()
public:
	void setLang(CPaintManagerUI* pPaintManger,LPCWSTR lpszLang);
	BOOL isCanQuit(HWND hwnd,CPaintManagerUI* pPaintManager,BOOL isCanQuit);
	void quit();
};