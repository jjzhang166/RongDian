#pragma once

class IPanelXml
{
public:
	IPanelXml() { };
	virtual ~IPanelXml() { };

public:
	BOOL Open(LPCWSTR lpszPath);
	BOOL LoadPanels();
	BOOL Write(LPCWSTR lpszSection, LPCWSTR lpszKeyName, LPCWSTR lpszValue);
	BOOL Flush();
	BOOL IsLoad() { return bLoad; };
	BOOL Reset();

private:
	XmlDocument xmlDoc;
	BOOL bLoad;
	wchar_t szXmlPath[1024];
};