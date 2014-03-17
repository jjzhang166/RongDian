#include "StdAfx.h"
#include <AddrTable.h>
#include <StrUtil.h>

CAddrTableDB::CAddrTableDB(CSQLite *pSQLite)
{
	m_pSQLite = pSQLite;
	m_nRows = 0;
	m_pResults = NULL;
}

CAddrTableDB::~CAddrTableDB()
{
	if(m_pResults)
		delete[] m_pResults;
}

BOOL CAddrTableDB::Init()
{
	if(!m_pSQLite)
		return FALSE;
	if(!m_pSQLite->TableExists("addr"))
		m_pSQLite->ExecQuery("create table addr(id_ integer primary key, solution_ varchar(100) unique, atype_ integer, addr_ varchar(255), mask_ varchar(100), gateway_ varchar(100), dtype_ integer, dns1_ varchar(100), dns2_ varchar(100), valid_ integer);");
	return TRUE;
}

BOOL CAddrTableDB::Query(LPADDR_TABLE lpTable)
{
	if(!m_pSQLite)
		return FALSE;
	char szSQL_u8[2048] = {0};
	CSQLiteQuery query;
	if(!lpTable)
	{
		sprintf(szSQL_u8, "select * from addr");
		query = m_pSQLite->ExecQuery("select count(*) from addr;");
		m_nRows = atoi(query.FieldValue(0));
	}
	else
	{
		wchar_t szSQL[2048];
		CDuiString strSolution = lpTable->szSolution;
		EscapeSQLite(strSolution);
		if(strSolution.IsEmpty())
			return FALSE;
		swprintf(szSQL, L"select * from addr where solution_='%s'", strSolution);
		StrUtil::u2u8(szSQL, szSQL_u8);
		m_nRows = 1;
	}
	if(m_nRows<=0)
		return FALSE;
	query = m_pSQLite->ExecQuery(szSQL_u8);
	if(m_pResults)
		delete[] m_pResults;
	m_pResults = NULL;
	m_pResults = new ADDR_TABLE[m_nRows];
	if(!m_pResults)
		return FALSE;
	int nIndex = 0;
	while(!query.Eof())
	{
		StrUtil::u82u(query.FieldValue("solution_"), m_pResults[nIndex].szSolution);
		StrUtil::u82u(query.FieldValue("addr_"), m_pResults[nIndex].szAddr);
		StrUtil::u82u(query.FieldValue("mask_"), m_pResults[nIndex].szMask);
		StrUtil::u82u(query.FieldValue("gateway_"), m_pResults[nIndex].szGateway);
		StrUtil::u82u(query.FieldValue("dns1_"), m_pResults[nIndex].szDns1);
		StrUtil::u82u(query.FieldValue("dns2_"), m_pResults[nIndex].szDns2);
		m_pResults[nIndex].nAddrType = atoi(query.FieldValue("atype_"));
		m_pResults[nIndex].nDnsType = atoi(query.FieldValue("dtype_"));
		m_pResults[nIndex].nValid = atoi(query.FieldValue("valid_"));
		nIndex++;
		query.NextRow();
	}

	return TRUE;
}

BOOL CAddrTableDB::Insert(LPADDR_TABLE lpTable)
{
	if(!m_pSQLite || !lpTable)
		return FALSE;
	wchar_t szSQL[2048];
	char szSQL_u8[2048] = {0};
	CDuiString strSolution = lpTable->szSolution;
	EscapeSQLite(strSolution);
	swprintf(szSQL, L"select count(*) from addr where solution_='%s';", strSolution.GetData());
	StrUtil::u2u8(szSQL, szSQL_u8);
	CSQLiteQuery query = m_pSQLite->ExecQuery(szSQL_u8);
	int nRows = atoi(query.FieldValue(0));
	if(nRows>0)
		return FALSE;
	CDuiString strAddr = lpTable->szAddr;
	CDuiString strMask = lpTable->szMask;
	CDuiString strGateway = lpTable->szGateway;
	CDuiString strDns1 = lpTable->szDns1;
	CDuiString strDns2 = lpTable->szDns2;
	EscapeSQLite(strAddr);
	EscapeSQLite(strMask);
	EscapeSQLite(strGateway);
	EscapeSQLite(strDns1);
	EscapeSQLite(strDns2);
	memset(szSQL, 0, sizeof(szSQL));
	memset(szSQL_u8, 0, sizeof(szSQL_u8));
	swprintf(szSQL, L"insert into addr values(null, '%s', %d, '%s', '%s', '%s', %d, '%s', '%s', %d);", 
		strSolution.GetData(), lpTable->nAddrType, strAddr.GetData(), strMask.GetData(), strGateway.GetData(), 
		lpTable->nDnsType, strDns1.GetData(), strDns2.GetData(), lpTable->nValid);
	StrUtil::u2u8(szSQL, szSQL_u8);
	m_pSQLite->ExecQuery(szSQL_u8);
	return TRUE;
}

BOOL CAddrTableDB::Update(LPADDR_TABLE lpTable)
{
	if(!m_pSQLite || !lpTable)
		return FALSE;
	wchar_t szSQL[2048];
	char szSQL_u8[2048] = {0};
	CDuiString strSolution = lpTable->szSolution;
	CDuiString strAddr = lpTable->szAddr;
	CDuiString strMask = lpTable->szMask;
	CDuiString strGateway = lpTable->szGateway;
	CDuiString strDns1 = lpTable->szDns1;
	CDuiString strDns2 = lpTable->szDns2;
	EscapeSQLite(strSolution);
	EscapeSQLite(strAddr);
	EscapeSQLite(strMask);
	EscapeSQLite(strGateway);
	EscapeSQLite(strDns1);
	EscapeSQLite(strDns2);
	swprintf(szSQL, L"update addr set atype_=%d, addr_='%s', mask_='%s', gateway_='%s', dtype_=%d, dns1_='%s', dns2_='%s' where solution_='%s'", 
		lpTable->nAddrType, strAddr.GetData(), strMask.GetData(), strGateway.GetData(), 
		lpTable->nDnsType, strDns1.GetData(), strDns2.GetData(), strSolution.GetData());
	StrUtil::u2u8(szSQL, szSQL_u8);
	m_pSQLite->ExecQuery(szSQL_u8);
	return TRUE;
}

BOOL CAddrTableDB::Delete(LPADDR_TABLE lpTable)
{
	if(!m_pSQLite || !lpTable)
		return FALSE;
	wchar_t szSQL[2048];
	char szSQL_u8[2048] = {0};
	CDuiString strSolution = lpTable->szSolution;
	EscapeSQLite(strSolution);
	swprintf(szSQL, L"delete from addr where solution_='%s'", strSolution.GetData());
	StrUtil::u2u8(szSQL, szSQL_u8);
	m_pSQLite->ExecQuery(szSQL_u8);
	return TRUE;
}
