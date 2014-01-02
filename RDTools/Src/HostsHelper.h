#pragma once
#include <string>

typedef struct _tagHOSTS_ITEM
{
	char szAddr[64];
	char szDomain[256];
	char szDesc[1024];
	_tagHOSTS_ITEM *pNext;
} HOSTS_ITEM, *PHOSTS_ITEM, *LPHOSTS_ITEM;

typedef struct _tagHOSTS_INFO
{
	char szSection[1024];
	char szDesc[1024];
	_tagHOSTS_INFO *pNext;
	int nCount;
	LPHOSTS_ITEM pItem;
} HOSTS_INFO, *PHOSTS_INFO, *LPHOSTS_INFO;

class CHostsHelper
{
public:
	CHostsHelper();
	~CHostsHelper();

	bool Load(const char* pszHosts = NULL);
	bool Parse(const char* pszStr, const char* pszDelim);
	//
	LPHOSTS_INFO FindSection(const char* pszSection);
	LPHOSTS_ITEM FindItem(const char* pszSection, const char* pszAddr);
	bool AddItem(const char* pszSection, const char* pszAddr, const char* pszDomain);
	bool AddSection(const char* pszSection);
	bool DelItem(const char* pszSection, const char* pszAddr);
	bool DelSection(const char* pszSection);
	bool ModifyItem(const char* pszSection, const char* pszAddr, const char* pszDomain);
	bool Build(std::string &strContent);
	bool Save();
	bool SaveAs(const char* lpszPath);

public:
	std::string strHeader;
	LPHOSTS_INFO pHosts;
	int nCount;
};
