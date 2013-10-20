#pragma once

typedef enum _tagCHARSET_TYPE
{
	CHARSET_INPUT = 0,
	CHARSET_OUTPUT = 1,
	CHARSET_BOTH = 2
} CHARSET_TYPE;

typedef struct _tagCHARSET_TABLE
{
	_tagCHARSET_TABLE()
	{
		nId = 0;
		nType = CHARSET_INPUT;
		memset(szName, 0, sizeof(szName));
		memset(szValue, 0, sizeof(szValue));
		memset(szDesc, 0, sizeof(szDesc));
	}
	int nId;
	int nType;
	wchar_t szName[260];
	wchar_t szValue[260];
	wchar_t szDesc[260];
} CHARSET_TABLE, *PCHARSET_TABLE, *LPCHARSET_TABLE;

class CCharSetTableDB
{
public:
	explicit CCharSetTableDB(CSQLite *pSQLite);
	~CCharSetTableDB();

	BOOL Init();
	BOOL Query(LPCHARSET_TABLE lpTable);

	int GetRows() { return m_nRows; };
	LPCHARSET_TABLE GetResults() { return m_pResults; };

private:
	int m_nRows;
	LPCHARSET_TABLE m_pResults;
	CSQLite *m_pSQLite;
};