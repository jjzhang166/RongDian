#pragma once

class CPanelXml
{
public:
	CPanelXml();
	virtual ~CPanelXml();

public:
	BOOL Open(LPCWSTR lpszPath);
	BOOL LoadPanels(LPCWSTR lpszRoot, list<LPMAIN_PANEL> &lstPanels);
	BOOL IsLoad() { return bLoad; };

private:
	XmlDocument xmlDoc;
	BOOL bLoad;
};