#include "stdafx.h"
#include <ConfigTable.h>
#include <StrUtil.h>

 CConfigTableDB::CConfigTableDB(CSQLite *pSQLite)
 {
	 m_pSQLite = pSQLite;
	 m_nRows = 0;
	 m_pResults = NULL;
 }

 CConfigTableDB::~CConfigTableDB()
 {
	 if(m_pResults)
		 delete[] m_pResults;
 }

BOOL CConfigTableDB::Init()
{
	if(!m_pSQLite)
		return FALSE;
	if(!m_pSQLite->TableExists("config"))
	{
		m_pSQLite->ExecQuery("create table config(id_ integer primary key, name_ varchar(100) unique, value_ varchar(255));");
		char szSQL_u8[1024];
		char szDefLang[1024] = {0};
		char szBack[1024];
		StrUtil::w2a(kDefaultLangIni, szDefLang);
		sprintf(szBack, "%0x", kDefBackground);
		sprintf(szSQL_u8, "insert into config values(null, 'Background', '%s');", szBack);
		m_pSQLite->ExecQuery(szSQL_u8);
		sprintf(szSQL_u8, "insert into config values(null, 'Lang', '%s');", szDefLang);
		m_pSQLite->ExecQuery(szSQL_u8);
	}
	return TRUE;
}

BOOL CConfigTableDB::Query(LPCONFIG_TABLE lpTable)
{
	if(!m_pSQLite)
		return FALSE;
	char szSQL_u8[2048] = {0};
	CSQLiteQuery query;
	if(!lpTable)
	{
		sprintf(szSQL_u8, "select * from config");
		query = m_pSQLite->ExecQuery("select count(*) from config;");
		m_nRows = atoi(query.FieldValue(0));
	}
	else
	{
		wchar_t szSQL[2048];
		CDuiString strName = lpTable->szName;
		EscapeSQLite(strName);
		if(strName.IsEmpty())
			return FALSE;
		swprintf(szSQL, L"select * from config where name_='%s'", strName.GetData());
		StrUtil::u2u8(szSQL, szSQL_u8);
		m_nRows = 1;
	}
	if(m_nRows<=0)
		return FALSE;
	query = m_pSQLite->ExecQuery(szSQL_u8);
	if(m_pResults)
		delete[] m_pResults;
	m_pResults = NULL;
	m_pResults = new CONFIG_TABLE[m_nRows];
	if(!m_pResults)
		return FALSE;
	int nIndex = 0;
	while(!query.Eof())
	{
		StrUtil::u82u(query.FieldValue("name_"), m_pResults[nIndex].szName);
		StrUtil::u82u(query.FieldValue("value_"), m_pResults[nIndex].szValue);
		nIndex++;
		query.NextRow();
	}

	return TRUE;
}

BOOL CConfigTableDB::Insert(LPCONFIG_TABLE lpTable)
{
	if(!m_pSQLite || !lpTable)
		return FALSE;
	wchar_t szSQL[2048];
	char szSQL_u8[2048] = {0};
	CDuiString strName = lpTable->szName;
	EscapeSQLite(strName);
	swprintf(szSQL, L"select count(*) from config where name_='%s';", strName.GetData());
	StrUtil::u2u8(szSQL, szSQL_u8);
	CSQLiteQuery query = m_pSQLite->ExecQuery(szSQL_u8);
	int nRows = atoi(query.FieldValue(0));
	if(nRows>0)
		return FALSE;
	CDuiString strValue = lpTable->szValue;
	EscapeSQLite(strValue);
	swprintf(szSQL, L"insert into config values(null, '%s', '%s');", strName.GetData(), strValue.GetData());
	StrUtil::u2u8(szSQL, szSQL_u8);
	m_pSQLite->ExecQuery(szSQL_u8);
	return TRUE;
}

BOOL CConfigTableDB::Update(LPCONFIG_TABLE lpTable)
{
	if(!m_pSQLite || !lpTable)
		return FALSE;
	wchar_t szSQL[2048];
	char szSQL_u8[2048] = {0};
	CDuiString strName = lpTable->szName;
	CDuiString strValue = lpTable->szValue;
	EscapeSQLite(strName);
	EscapeSQLite(strValue);
	swprintf(szSQL, L"update config set value_='%s' where name_=%s", strName.GetData(), strValue.GetData());
	StrUtil::u2u8(szSQL, szSQL_u8);
	m_pSQLite->ExecQuery(szSQL_u8);
	return TRUE;
}

BOOL CConfigTableDB::Delete(LPCONFIG_TABLE lpTable)
{
	if(!m_pSQLite || !lpTable)
		return FALSE;
	wchar_t szSQL[2048];
	char szSQL_u8[2048] = {0};
	CDuiString strName = lpTable->szName;
	EscapeSQLite(strName);
	swprintf(szSQL, L"delete from config where name_='%s'", strName.GetData());
	StrUtil::u2u8(szSQL, szSQL_u8);
	m_pSQLite->ExecQuery(szSQL_u8);
	return TRUE;
}