#include "StdAfx.h"
#include <CharSetTable.h>
#include <StrUtil.h>

CCharSetTableDB::CCharSetTableDB(CSQLite *pSQLite)
{
	m_pSQLite = pSQLite;
	m_nRows = 0;
	m_pResults = NULL;
}

CCharSetTableDB::~CCharSetTableDB()
{
	if(m_pResults)
		delete[] m_pResults;
}

BOOL CCharSetTableDB::Init()
{
	if(!m_pSQLite)
		return FALSE;
	if(!m_pSQLite->TableExists("charset"))
		return FALSE;
	return TRUE;
}

BOOL CCharSetTableDB::Query(LPCHARSET_TABLE lpTable)
{
	if(!m_pSQLite)
		return FALSE;
	char szCount_u8[256] = {0};
	char szSQL_u8[2048] = {0};
	int nType = CHARSET_INPUT;
	CSQLiteQuery query;
	if(!lpTable)
	{
		sprintf(szSQL_u8, "select * from charset;");
		sprintf(szCount_u8, "select count(*) from charset;");
		query = m_pSQLite->ExecQuery(szCount_u8);
		m_nRows = atoi(query.FieldValue(0));
		if(m_nRows<=0)
			return FALSE;
	}
	else
	{
		wchar_t szSQL[2048] = {0};
		nType = lpTable->nType;
		if(nType!=CHARSET_INPUT && nType!=CHARSET_OUTPUT)
			nType = CHARSET_INPUT;
		CDuiString strName = lpTable->szName;
		EscapeSQLite(strName);
		if(strName.IsEmpty())
			return FALSE;
		swprintf(szSQL, L"select * from charset where name_='%s' and (type_='%d' or type_='2');", strName.GetData(), nType);
		StrUtil::u2u8(szSQL, szSQL_u8);
		m_nRows = 1;
	}
	query = m_pSQLite->ExecQuery(szSQL_u8);
	if(m_pResults)
		delete[] m_pResults;
	m_pResults = NULL;
	m_pResults = new CHARSET_TABLE[m_nRows];
	if(!m_pResults)
		return FALSE;
	int nIndex = 0;
	while(!query.Eof())
	{
		m_pResults[nIndex].nType = atoi(query.FieldValue("type_"));
		StrUtil::u82u(query.FieldValue("name_"), m_pResults[nIndex].szName);
		StrUtil::u82u(query.FieldValue("desc_"), m_pResults[nIndex].szValue);
		nIndex++;
		query.NextRow();
	}

	return TRUE;
}