#pragma once

typedef struct _tagCONFIG_TABLE
{
	_tagCONFIG_TABLE()
	{
		nId = 0;
		memset(szName, 0, sizeof(szName));
		memset(szValue, 0, sizeof(szValue));
	}
	int nId;
	wchar_t szName[260];
	wchar_t szValue[260];
} CONFIG_TABLE, *PCONFIG_TABLE, *LPCONFIG_TABLE;

class CConfigTableDB
{
public:
	explicit CConfigTableDB(CSQLite *pSQLite);
	~CConfigTableDB();

	BOOL Init();
	BOOL Query(LPCONFIG_TABLE lpTable);
	BOOL Insert(LPCONFIG_TABLE lpTable);
	BOOL Update(LPCONFIG_TABLE lpTable);
	BOOL Delete(LPCONFIG_TABLE lpTable);

	int GetRows() { return m_nRows; };
	LPCONFIG_TABLE GetResults() { return m_pResults; };

private:
	int m_nRows;
	LPCONFIG_TABLE m_pResults;
	CSQLite *m_pSQLite;
};
