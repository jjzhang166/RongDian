#include "stdafx.h"
#include "HostsParser.h"

CHostsParser::CHostsParser()
{
	m_pGroupReg = "^#\\s+(.+)";
	m_pActiveHostReg = "^([^#\\s]+)\\s+([^#\\s]+)\\s*#{0,1}(.*)";
	m_pStopHostReg =  "^#([^#\\s]+)\\s+([^#\\s]+)\\s*#{0,1}(.*)";
	m_lpszHostsFilePath = L"c:\\windows\\system32\\drivers\\etc\\hosts";
	m_szHostFilePath = "c:\\windows\\system32\\drivers\\etc\\hosts";
	LoadHostsFile();
}
CHostsParser::~CHostsParser()
{
	list<PHOSTS_INFO>::iterator iter;
	for (iter = m_rows.begin();iter!=m_rows.end();iter++)
	{
		PHOSTS_INFO hosts_row = PHOSTS_INFO(*iter);
		delete hosts_row;
	}
}

void CHostsParser::LoadHostsFile()
{
	m_rows.clear();
	char line[1024] = {0};
	ifstream fin(m_szHostFilePath, ios::in);
	while(fin.getline(line, sizeof(line)))  
	{
		if (strlen(line)>0)
		{
			//line字符串为gbk
			//bool gbkCheck = StrUtil::is_gbk(line);
			//if (gbkCheck)
			//{
			//	char* utf8 = StrUtil::a2u8(line);
			//	strcpy_s(line,sizeof(line),utf8);
			//	delete[] utf8;
			//}
			wchar_t* row;
			int utf8Type = StrUtil::is_utf8(line);
			if (utf8Type==UTF8_BOM||utf8Type==UTF8_NOBOM)
			{
				row = StrUtil::u82u(line);
			}
			else
			{
				row = StrUtil::a2w(line);
			}
			PHOSTS_INFO pHostsRow = new HOSTS_INFO();
			Parser(row,pHostsRow);
			delete[] row;
		}
	}  
	fin.clear();  
	fin.close();
}

void CHostsParser::SaveHostsFile(wchar_t* content)
{
	locale &loc=locale::global(locale(locale(),"",LC_CTYPE));
	wofstream fon(m_szHostFilePath, ios::out);
	locale::global(loc);
	if (fon.is_open())
	{
		fon<<content;
		fon.close();
	}
}

BOOL CHostsParser::Parser(LPWSTR row,PHOSTS_INFO pHosts)
{
	list<char*> hostParts;
	StrUtil::wcstrim(row,wcslen(row));
	char* row_a = StrUtil::w2a(row);

	
	if (PCREUtil::find(m_pGroupReg,row_a,&hostParts))
	{
		pHosts->type = TAG_GROUP;
	}
	else if (PCREUtil::find(m_pActiveHostReg,row_a,&hostParts))
	{
		pHosts->type = TAG_ACTIVE_HOST;
	}
	else if (PCREUtil::find(m_pStopHostReg,row_a,&hostParts))
	{
		pHosts->type = TAG_STOP_HOST;
		
	}
	//格式不对
	else
	{
		return FALSE;
	}

	list<char*>::iterator it;
	switch(pHosts->type)
	{
	case TAG_GROUP:
		{
			wchar_t* desc = StrUtil::a2w(*hostParts.begin());
			wcscpy_s(pHosts->szDesc,_countof(pHosts->szDesc),desc);
			delete[] desc;
		}
		break;
	case TAG_ACTIVE_HOST:
	case TAG_STOP_HOST:
		{
			it = hostParts.begin();
			wchar_t* ip = StrUtil::a2w(*it);
			wcscpy_s(pHosts->szIP,_countof(pHosts->szIP),ip);
			it++;
			wchar_t* domain = StrUtil::a2w(*it);
			wcscpy_s(pHosts->szDomain,_countof(pHosts->szDomain),domain);
			delete[] ip;
			delete[] domain;

			if (hostParts.size()>2)
			{
				it++;
				wchar_t* desc = StrUtil::a2w(*it);
				wcscpy_s(pHosts->szDesc,_countof(pHosts->szDesc),desc);
				delete[] desc;
			}
		}
		break;
	}
	m_rows.push_back(pHosts);

	for (it = hostParts.begin();it!=hostParts.end();it++)
	{
		delete[] *it;
	}
	delete[] row_a;
	return TRUE;
}

