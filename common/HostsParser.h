#pragma once
#include <list>
#include <fstream>
#include <list>
using namespace std;

typedef enum _tagHOSTS_STATE
{
	TAG_GROUP = 1,
	TAG_STOP_HOST = 2,
	TAG_ACTIVE_HOST = 3
} HOSTS_STATE;

class CHostsParser
{
public:
	CHostsParser();
	virtual ~CHostsParser();
	void LoadHostsFile();
	void SaveHostsFile(wchar_t* content);
public:
	list<PHOSTS_INFO> m_rows;
	LPSTR m_pGroupReg;			//БъЬт
	LPSTR m_pStopHostReg;
	LPSTR m_pActiveHostReg;
	LPWSTR m_lpszHostsFilePath;
	LPSTR m_szHostFilePath;
private:
	void Parser(LPWSTR row,PHOSTS_INFO pHosts);
};