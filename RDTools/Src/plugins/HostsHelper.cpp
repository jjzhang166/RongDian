//#include "stadfx.h"
#include "HostsHelper.h"

#include <assert.h>
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "..\common\StrUtil.h"
#include "..\thirdparty\StringTokenizer\StringTokenizer.h"

#pragma warning(push)
#pragma warning(disable:4996)

const char * const kSectionTag = "#Section";
const char * const kCommentTag = "#Comment";
const char * const kDefaultName = "Hosts's Group";
const char * const kDefaultDesc = "Add by HostParser";

CHostsHelper::CHostsHelper()
{
	m_pHosts = NULL;
	m_nCount = 0;
	m_nIndex = 0;
	m_isUtf8 = false;
}

CHostsHelper::~CHostsHelper()
{
	Reset();
}

bool CHostsHelper::Load(const char* pszHosts /*= NULL*/)
{
	bool bRet = false;
	char szSystem[2048] = {0};
	if(pszHosts)
	{
		strcpy(szSystem, pszHosts);
	}
	else
	{
		GetSystemDirectoryA(szSystem, _countof(szSystem));
		strcat(szSystem, "\\drivers\\etc\\hosts");
	}
	Reset();
	FILE *pFile = fopen(szSystem, "r");
	if(!pFile)
		return bRet;
	fseek(pFile, 0, SEEK_END);
	DWORD dwSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	char *pData = (char *)malloc(dwSize+1);
	if(!pData)
	{
		fclose(pFile);
		return bRet;
	}
	memset(pData, 0, dwSize);
	fread(pData, 1, dwSize, pFile);
	pData[dwSize] = '\0';
	//if (pData[0]==0xEF&&pData[1]==0xBB&&pData[2]==0xBF)
	//if (pData[0]==(char)0xEF&&pData[1]==(char)0xBB&&pData[2]==(char)0xBF)
	if (pData[0]=='\xEF'&&pData[1]=='\xBB'&&pData[2]=='\xBF')
	{
		pData[0]=' ';
		pData[1]=' ';
		pData[2]=' ';
	}

	Parse(pData, "\n");
	free(pData);
	fclose(pFile);
	return true;
}

bool CHostsHelper::Parse(const char* pszStr, const char* pszDelim)
{
	char* szContent = new char[strlen(pszStr)+1];
	memset(szContent,0,strlen(pszStr)+1);
	strcpy_s(szContent,strlen(pszStr)+1,pszStr);
	int utf8Type = StrUtil::is_utf8(pszStr);
	if(utf8Type==UTF8_BOM||utf8Type==UTF8_NOBOM)
	{
		m_isUtf8 = true;
		delete[] szContent;
		szContent = StrUtil::u82a(pszStr);
	}
	StringTokenizer strData = StringTokenizer(szContent, pszDelim);
	std::string strTemp = "";
	std::string strSection, strComment;
	std::string strSectionTag, strCommentTag;
	m_pHosts = (LPHOSTS_INFO)malloc(sizeof(HOSTS_INFO));
	if(!m_pHosts)
		return false;
	m_nCount++;
	LPHOSTS_INFO pTail = m_pHosts;
	LPHOSTS_ITEM pItem = NULL;
	memset(pTail, 0, sizeof(HOSTS_INFO));
	sprintf(pTail->szId, "%d", m_nIndex);
	m_nIndex++;
	bool bHeaderEnd = false;
	int nTokensCount = strData.countTokens();
	for(int i = 0; i < nTokensCount; i++)
	{
		strTemp = strData.nextToken();
		char* tmp = const_cast<char*>(strTemp.c_str());
		StrUtil::replace_all(tmp,'\t',' ');
		strSectionTag.assign(strTemp.c_str(), strlen(kSectionTag));
		strCommentTag.assign(strTemp.c_str(), strlen(kCommentTag));
		if(strTemp[0] == '#' && !bHeaderEnd && !isItem(strTemp))
		{
			if(strSectionTag!=kSectionTag) // hosts's Coment
			{
				m_strHeader += strTemp;
				m_strHeader += "\n";
				strSectionTag = "";
				strCommentTag = "";
				continue;
			}
		}

		bHeaderEnd = true;
		if(strSectionTag==kSectionTag) // hosts group section's name
		{
			if(strstr(strTemp.c_str(), kSectionTag))
				strSection.assign(strTemp.c_str(), strlen(kSectionTag)+1, strTemp.length());
			if(pTail->pItem)
			{
				pTail->pNext = (LPHOSTS_INFO)malloc(sizeof(HOSTS_INFO));
				assert(pTail->pNext!=NULL);
				pTail = pTail->pNext;
				memset(pTail, 0, sizeof(HOSTS_INFO));
				sprintf(pTail->szId, "%d", m_nIndex);
				m_nCount++;
				m_nIndex++;
			}
			strcpy(pTail->szSection, strSection.c_str());
		}
		else if(strCommentTag==kCommentTag) // host group's comment
		{
			strComment.assign(strTemp.c_str(), strlen(kCommentTag)+1, strTemp.length());
			strcpy(pTail->szDesc, strComment.c_str());
		}
		else // hosts item
		{
			if (!isItem(strTemp))
			{
				continue;
			}
			if(strlen(pTail->szSection)==0 && strlen(pTail->szDesc)==0)
			{
				strcpy(pTail->szSection, kDefaultName);
				strcpy(pTail->szDesc, kDefaultDesc);
			}
			if(!pTail->pItem)
			{
				pTail->pItem = (LPHOSTS_ITEM)malloc(sizeof(HOSTS_ITEM));
				assert(pTail->pItem!=NULL);
				memset(pTail->pItem, 0, sizeof(HOSTS_ITEM));
				sprintf(pTail->pItem->szId, "%d", pTail->nIndex);
				pTail->nIndex++;
				pItem = pTail->pItem;
			}
			else if(strlen(pItem->szAddr))
			{
				LPHOSTS_ITEM pNewItem = (LPHOSTS_ITEM)malloc(sizeof(HOSTS_ITEM));
				assert(pNewItem!=NULL);
				memset(pNewItem, 0, sizeof(HOSTS_ITEM));
				sprintf(pNewItem->szId, "%d", pTail->nIndex);
				pTail->nIndex++;
				pItem->pNext = pNewItem;
				pItem = pNewItem;
			}
			pTail->nCount++;

			StringTokenizer strItem = StringTokenizer(strTemp, " ");
			std::string strAddrPart = ltrim(strItem.nextToken());
			std::string strDomainPart = ltrim(strItem.nextToken());
			std::string strDescPart = ltrim(strItem.remainingString());
			if (strAddrPart[0]=='#')
			{
				pItem->bActive = false;
				std::string strAddr = strAddrPart.substr(1,strAddrPart.length()-1);
				strcpy(pItem->szAddr, strAddr.c_str());
			}
			else
			{
				pItem->bActive = true;
				strcpy(pItem->szAddr, strAddrPart.c_str());
			}
			strcpy(pItem->szDomain, strDomainPart.c_str());
			if (strDescPart[0]=='#')
			{
				std::string strDesc = strDescPart.substr(1,strDescPart.length()-1);
				strcpy(pItem->szDesc, strDesc.c_str());
			}
			else
			{
				strcpy(pItem->szDesc, strDescPart.c_str());
			}
			
		}

		strSection = "";
		strComment = "";
		strSectionTag = "";
		strCommentTag = "";
	}
	
	delete[] szContent;
	return true;
}

bool CHostsHelper::Reset()
{
	bool bRet = false;
	if(m_pHosts)
	{
		LPHOSTS_INFO pHeader = m_pHosts;
		LPHOSTS_ITEM pItem = NULL;
		while(m_pHosts)
		{
			pHeader = m_pHosts;
			m_pHosts = m_pHosts->pNext;
			while(pHeader->pItem)
			{
				pItem = pHeader->pItem;
				pHeader->pItem = pHeader->pItem->pNext;
				free(pItem);
			}
			free(pHeader);
		}
	}
	m_pHosts = NULL;
	m_nCount = 0;
	m_nIndex = 0;
	m_strHeader = "";
	bRet = true;
	return bRet;
}

LPHOSTS_INFO CHostsHelper::FindSection(const char* pszSection)
{
	if(!m_pHosts)
		return NULL;
	LPHOSTS_INFO pSection =  m_pHosts;
	while(pSection)
	{
		if(stricmp(pSection->szSection, pszSection)==0)
			return pSection;
		pSection = pSection->pNext;
	}
	return NULL;
}


LPHOSTS_INFO CHostsHelper::FindSectionById(const char* pszId)
{
	if(!m_pHosts)
		return NULL;
	LPHOSTS_INFO pSection =  m_pHosts;
	while(pSection)
	{
		if(stricmp(pSection->szId, pszId)==0)
			return pSection;
		pSection = pSection->pNext;
	}
	return NULL;
}

LPHOSTS_ITEM CHostsHelper::FindItem(const char* pszSection, const char* pszDomain)
{
	LPHOSTS_INFO pSection = NULL;
	LPHOSTS_ITEM pItem = NULL;
	if(pszSection)
	{
		pSection = FindSection(pszSection);
		if(!pSection)
			return NULL;
		pItem = pSection->pItem;
		while(pItem)
		{
			if(stricmp(pItem->szDomain, pszDomain)==0)
				return pItem;
			pItem = pItem->pNext;
		}
	}
	else
	{
		pSection = m_pHosts;
		while(pSection)
		{
			pItem = FindItem(pSection->szSection, pszDomain);
			if(pItem)
				return pItem;
			pSection = pSection->pNext;
		}
	}
	return NULL;
}

LPHOSTS_ITEM CHostsHelper::FindItemById(const char* pSid, const char* pszId)
{
	LPHOSTS_INFO pSection = NULL;
	LPHOSTS_ITEM pItem = NULL;
	if(pSid)
	{
		pSection = FindSectionById(pSid);
		if(!pSection)
			return NULL;
		pItem = pSection->pItem;
		while(pItem)
		{
			if(stricmp(pItem->szId, pszId)==0)
				return pItem;
			pItem = pItem->pNext;
		}
	}
	else
	{
		pSection = m_pHosts;
		while(pSection)
		{
			pItem = FindItemById(pSection->szId, pszId);
			if(pItem)
				return pItem;
			pSection = pSection->pNext;
		}
	}
	return NULL;
}

bool CHostsHelper::AddItem(const char* pszSection, const char* pszAddr, const char* pszDomain)
{
	LPHOSTS_INFO pSection = NULL;
	if(pszSection)
	{
		pSection = FindSection(pszSection);
		if(!pSection)
		{
			return false;
		}
		LPHOSTS_ITEM pItem = pSection->pItem;
		while(pItem)
		{
			if(pItem->pNext)
				pItem = pItem->pNext;
			else
				break;
		}
		LPHOSTS_ITEM pNewItem = (LPHOSTS_ITEM)malloc(sizeof(HOSTS_ITEM));
		if(!pNewItem)
			return false;
		memset(pNewItem, 0, sizeof(HOSTS_ITEM));
		strcpy(pNewItem->szAddr, pszAddr);
		strcpy(pNewItem->szDomain, pszDomain);
		sprintf(pNewItem->szId, "%d", pSection->nIndex);
		pSection->nIndex++;
		pItem->pNext = pNewItem;
		return true;
	}
	else
	{
		if(AddItem(kDefaultName, pszAddr, pszDomain))
			return true;
	}

	return false;
}

bool CHostsHelper::AddItemById(const char* pszSid, const char* pszAddr, const char* pszDomain)
{
	LPHOSTS_INFO pSection = NULL;
	if(!pszSid)
		return false;

	pSection = FindSectionById(pszSid);
	if(!pSection)
	{
		return false;
	}
	LPHOSTS_ITEM pItem = pSection->pItem;
	while(pItem)
	{
		if(pItem->pNext)
			pItem = pItem->pNext;
		else
			break;
	}
	LPHOSTS_ITEM pNewItem = (LPHOSTS_ITEM)malloc(sizeof(HOSTS_ITEM));
	if(!pNewItem)
		return false;
	memset(pNewItem, 0, sizeof(HOSTS_ITEM));
	strcpy(pNewItem->szAddr, pszAddr);
	strcpy(pNewItem->szDomain, pszDomain);
	sprintf(pNewItem->szId, "%d", pSection->nIndex);
	pSection->nIndex++;
	if(pItem)
	{
		pItem->pNext = pNewItem;
	}
	else
	{
		pSection->pItem = pNewItem;
	}
	
	
	return true;
}

bool CHostsHelper::AddSection(const char* pszSection)
{
	LPHOSTS_INFO pSection = NULL;
	if(!pszSection)
		return false;
	pSection = (LPHOSTS_INFO)malloc(sizeof(HOSTS_INFO));
	if(!pSection)
		return false;
	memset(pSection, 0, sizeof(HOSTS_INFO));
	sprintf(pSection->szId, "%d", m_nIndex);
	m_nIndex++;
	if(!m_pHosts)
	{
		m_pHosts = pSection;
	}
	else
	{
		LPHOSTS_INFO pTail = m_pHosts;
		while(pTail)
		{
			if(pTail->pNext)
				pTail = pTail->pNext;
			else
				break;
		}
		pTail->pNext = pSection;
	}
	strcpy(pSection->szSection, pszSection);
	m_nCount++;
	return true;
}

bool CHostsHelper::DelItem(const char* pszSection, const char* pszDomain)
{
	LPHOSTS_INFO pSection = NULL;
	LPHOSTS_ITEM pItem = NULL, pPrev = NULL;
	bool bRet = false;
	if(!pszDomain || strlen(pszDomain)==0)
		return bRet;
	if(pszSection)
	{
		pSection = FindSection(pszSection);
		if(!pSection)
			return false;
		pItem = pSection->pItem;
		while(pItem)
		{
			if(stricmp(pItem->szDomain, pszDomain)==0)
			{
				bRet = true;
				break;
			}
			pPrev = pItem;
			pItem = pItem->pNext;
		}
		if(!bRet)
			return false;
		if(pItem)
		{
			if(!pPrev)
				pSection->pItem = pSection->pItem->pNext;
			else
				pPrev->pNext = pItem->pNext;
			pSection->nCount --;
			free(pItem);
		}
	}
	else
	{
		pSection = m_pHosts;
		while(pSection)
		{
			bRet = DelItem(pSection->szSection, pszDomain);
			if(bRet)
				return true;
			pSection = pSection->pNext;
		}
	}
	return false;
}

bool CHostsHelper::DelItemById(const char* pSid, const char* pszId)
{
	LPHOSTS_INFO pSection = NULL;
	LPHOSTS_ITEM pItem = NULL, pPrev = NULL;
	if(pSid)
	{
		pSection = FindSectionById(pSid);
		if(!pSection)
			return false;
		pItem = pSection->pItem;
		while(pItem)
		{
			if(stricmp(pItem->szId, pszId)==0)
			{
				break;
			}
			pPrev = pItem;
			pItem = pItem->pNext;
		}
		if(pItem)
		{
			if(!pPrev)
				pSection->pItem = pSection->pItem->pNext;
			else
				pPrev->pNext = pItem->pNext;
			pSection->nCount --;
			free(pItem);
			return true;
		}
	}
	else
	{
		pSection = m_pHosts;
		while(pSection)
		{
			if(DelItemById(pSection->szId, pszId))
				return true;
			pSection = pSection->pNext;
		}
	}
	return false;
}

bool CHostsHelper::DelSection(const char* pszSection)
{
	if(!m_pHosts)
		return false;
	bool bRet = false;
	LPHOSTS_INFO pPrev = NULL;
	LPHOSTS_INFO pSection =  m_pHosts;
	while(pSection)
	{
		if(stricmp(pSection->szSection, pszSection)==0)
		{
			bRet = true;
			break;
		}
		pPrev = pSection;
		pSection = pSection->pNext;
	}
	if(!bRet)
		return false;
	if(pSection)
	{
		if(!pPrev)
			m_pHosts = m_pHosts->pNext;
		else
			pPrev->pNext = pSection->pNext;
		LPHOSTS_ITEM pItem = pSection->pItem;
		LPHOSTS_ITEM pTemp = NULL;
		while(pItem)
		{
			pTemp = pItem;
			pItem = pItem->pNext;
			free(pTemp);
		}
		free(pSection);
		m_nCount --;
		return true;
	}
	return false;
}

bool CHostsHelper::ModifyItem(const char* pszSection,const bool check, const char* pszAddr, const char* pszDomain)
{
	LPHOSTS_INFO pSection = NULL;
	LPHOSTS_ITEM pItem = NULL;
	if(pszSection)
	{
		pSection = FindSection(pszSection);
		if(!pSection)
			return false;
		pItem = pSection->pItem;
		while(pItem)
		{
			if(stricmp(pItem->szAddr, pszAddr)==0)
			{
				memset(pItem->szDomain, 0, sizeof(pItem->szDomain));
				strcpy(pItem->szDomain, pszDomain);
				return true;
			}
			pItem = pItem->pNext;
		}
	}
	else
	{
		pSection = m_pHosts;
		while(pSection)
		{
			if(ModifyItem(pSection->szSection, check, pszAddr, pszDomain))
				return true;
			pSection = pSection->pNext;
		}
	}
	return false;
}

bool CHostsHelper::Build(std::string &strContent)
{
	LPHOSTS_INFO pSection = m_pHosts;
	LPHOSTS_ITEM pItem = NULL;
	if(!m_strHeader.empty())
	{
		strContent = m_strHeader;
		strContent += "\n";
	}
	while(pSection)
	{
		if(strlen(pSection->szSection))
		{
			strContent += kSectionTag;
			strContent += ":";
			strContent += pSection->szSection;
			strContent += "\n";
		}
		if(strlen(pSection->szDesc))
		{
			strContent += kCommentTag;
			strContent += ":";
			strContent += pSection->szDesc;
			strContent += "\n";
		}
		pItem = pSection->pItem;
		while(pItem)
		{
			if (!pItem->bActive)
				strContent += "#";
			strContent += pItem->szAddr;
			strContent += " ";
			strContent += pItem->szDomain;
			if(strlen(pItem->szDesc))
			{
				strContent += " #";
				strContent += pItem->szDesc;
			}
			strContent += "\n";
			pItem = pItem->pNext;
		}
		strContent += "\n";
		pSection = pSection->pNext;
	}
	if(m_isUtf8)
	{
		const char* uStr = strContent.c_str();
		char* u8Str = StrUtil::a2u8(uStr);
		strContent = u8Str;
		delete[] u8Str;
	}
	return true;
}

bool CHostsHelper::Save()
{
	if(!m_pHosts)
		return false;
	std::string strContent;
	Build(strContent);
	char szModule[1024] = {0};
	GetModuleFileNameA(NULL, szModule, _countof(szModule));
	PathRemoveFileSpecA(szModule);
	if(PathIsRootA(szModule))
		szModule[strlen(szModule)-1] = '\0';
	PathAppendA(szModule, "hosts");
	FILE *pFile = fopen(szModule, "w");
	if(!pFile)
		return false;
	fwrite(strContent.c_str(), 1, strContent.length(), pFile);
	fclose(pFile);
	return true;
}

bool CHostsHelper::SaveAs(const char* lpszPath)
{
	std::string strContent;
	Build(strContent);
	FILE *pFile = fopen(lpszPath, "w");
	if(!pFile)
		return false;
	fwrite(strContent.c_str(), 1, strContent.length(), pFile);
	fclose(pFile);
	return true;
}

const std::string CHostsHelper::trim(const std::string istring)
{
	std::string::size_type first = istring.find_first_not_of(" \n\t\r\0xb");
	if (first == std::string::npos) {
		return std::string();
	}
	else {
		std::string::size_type last = istring.find_last_not_of(" \n\t\r\0xb");
		return istring.substr( first, last - first + 1);
	}
}

const std::string CHostsHelper::ltrim(const std::string istring)
{
	std::string::size_type first = istring.find_first_not_of(" \n\t\r\0xb");
	if (first == std::string::npos) {
		return std::string();
	}
	else {
		return istring.substr( first );
	}
}


const std::string CHostsHelper::rtrim(const std::string istring)
{
	std::string::size_type last = istring.find_last_not_of(" \n\t\r\0xb"); /// must succeed
	return istring.substr( 0, last + 1);
}

bool CHostsHelper::isItem(const std::string strItem)
{
	StringTokenizer itemToken = StringTokenizer(strItem, " ");
	std::string strAddrPart = ltrim(itemToken.nextToken());
	std::string strDomainPart = ltrim(itemToken.nextToken());
	if (strAddrPart[0]=='#')
	{
		strAddrPart = strAddrPart.substr(1,strAddrPart.length()-1);
	}
	if(strAddrPart.length()<7)
	{
		return false;
	}
	if(strDomainPart.length()<5)
	{
		return false;
	}
	return true;
}
#pragma warning(pop)
