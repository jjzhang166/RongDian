#pragma once
#include <string>

typedef struct _tagHOSTS_ITEM
{
	char szId[64]; // hosts item内部唯一标识
	char szAddr[64];
	char szDomain[256];
	char szDesc[1024];
	_tagHOSTS_ITEM *pNext;
} HOSTS_ITEM, *PHOSTS_ITEM, *LPHOSTS_ITEM;

typedef struct _tagHOSTS_INFO
{
	char szId[64]; // hosts info内部唯一标识
	char szSection[1024];
	char szDesc[1024];
	_tagHOSTS_INFO *pNext;
	int nCount; // host item个数
	int nIndex; // host item索引，用于唯一标识item节点，该字段只增加，不减少
	LPHOSTS_ITEM pItem;
} HOSTS_INFO, *PHOSTS_INFO, *LPHOSTS_INFO;

class CHostsHelper
{
public:
	CHostsHelper();
	~CHostsHelper();

	bool Load(const char* pszHosts = NULL);
	bool Parse(const char* pszStr, const char* pszDelim);
	bool Reset();
	//
	LPHOSTS_INFO FindSection(const char* pszSection);
	LPHOSTS_INFO FindSectionById(const char* pszId);
	LPHOSTS_ITEM FindItem(const char* pszSection, const char* pszDomain);
	LPHOSTS_ITEM FindItemById(const char* pSid, const char* pszId);
	bool AddItem(const char* pszSection, const char* pszAddr, const char* pszDomain);
	bool AddItemById(const char* pszSid, const char* pszAddr, const char* pszDomain);
	bool AddSection(const char* pszSection);
	bool DelItem(const char* pszSection, const char* pszDomain);
	bool DelSection(const char* pszSection);
	bool ModifyItem(const char* pszSection, const char* pszAddr, const char* pszDomain);
	bool Build(std::string &strContent);
	bool Save();
	bool SaveAs(const char* lpszPath);

private:
	const std::string trim(const std::string istring);
	const std::string ltrim(const std::string istring);
	const std::string rtrim(const std::string istring);

public:
	std::string strHeader;
	LPHOSTS_INFO pHosts;
	int nCount; // host info个数
	int nIndex; // host info索引，用于唯一标识info节点，该字段只增加，不减少
};
