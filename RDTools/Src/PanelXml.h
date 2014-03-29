#pragma once

class CPanelXml
{
public:
	CPanelXml();
	virtual ~CPanelXml();

public:
	BOOL Open(LPCWSTR lpszPath);
	BOOL LoadPanels(LPCWSTR lpszRoot);
	BOOL LoadPanel(XmlNode *pPlugin,LPMAIN_PANEL lpParentPanelInfo);
	BOOL GetPanel(LPCWSTR lpszPluginName,LPMAIN_PANEL &lpPanelInfo);	// get the panel info by plugin name
	BOOL IsLoad() { return bLoad; };
	
private:
	BOOL LoopGetPanel(list<LPMAIN_PANEL> panels,LPCWSTR lpszPluginName,LPMAIN_PANEL &lpPanelInfo);
	
private:
	XmlDocument xmlDoc;
	BOOL bLoad;
	wchar_t m_lpszRoot[1024];
};