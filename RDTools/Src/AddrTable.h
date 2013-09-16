#pragma once

typedef struct _tagADDR_TABLE
{
	_tagADDR_TABLE()
	{
		nId = 0;
		nValid = 1;
		nAddrType = 0;
		nDnsType = 0;
		memset(szSolution, 0, sizeof(szSolution));
		memset(szAddr, 0, sizeof(szAddr));
		memset(szMask, 0, sizeof(szMask));
		memset(szGateway, 0, sizeof(szGateway));
		memset(szDns1, 0, sizeof(szDns1));
		memset(szDns2, 0, sizeof(szDns2));
	}
	int nId;
	int nValid;
	int nAddrType;
	int nDnsType;
	wchar_t szSolution[260];
	wchar_t szAddr[260];
	wchar_t szMask[260];
	wchar_t szGateway[260];
	wchar_t szDns1[260];
	wchar_t szDns2[260];
} ADDR_TABLE, *PADDR_TABLE, *LPADDR_TABLE;

class CAddrTableDB
{
public:
	explicit CAddrTableDB(CSQLite *pSQLite);
	~CAddrTableDB();

	BOOL Init();
	BOOL Query(LPADDR_TABLE lpTable);
	BOOL Insert(LPADDR_TABLE lpTable);
	BOOL Update(LPADDR_TABLE lpTable);
	BOOL Delete(LPADDR_TABLE lpTable);

	int GetRows() { return m_nRows; };
	LPADDR_TABLE GetResults() { return m_pResults; };

private:
	int m_nRows;
	LPADDR_TABLE m_pResults;
	CSQLite *m_pSQLite;
};

