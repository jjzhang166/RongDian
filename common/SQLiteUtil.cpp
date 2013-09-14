#include <SQLiteUtil.h>

#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

const char* GetSQLiteErr(int nErrCode)
{
	switch (nErrCode)
	{
	case SQLITE_OK:
		return "SQLITE_OK";
	case SQLITE_ERROR:
		return "SQLITE_ERROR";
	case SQLITE_INTERNAL:
		return "SQLITE_INTERNAL";
	case SQLITE_PERM:
		return "SQLITE_PERM";
	case SQLITE_ABORT:
		return "SQLITE_ABORT";
	case SQLITE_BUSY:
		return "SQLITE_BUSY";
	case SQLITE_LOCKED:
		return "SQLITE_LOCKED";
	case SQLITE_NOMEM:
		return "SQLITE_NOMEM";
	case SQLITE_READONLY:
		return "SQLITE_READONLY";
	case SQLITE_INTERRUPT   :
		return "SQLITE_INTERRUPT";
	case SQLITE_IOERR:
		return "SQLITE_IOERR";
	case SQLITE_CORRUPT:
		return "SQLITE_CORRUPT";
	case SQLITE_NOTFOUND:
		return "SQLITE_NOTFOUND";
	case SQLITE_FULL:
		return "SQLITE_FULL";
	case SQLITE_CANTOPEN:
		return "SQLITE_CANTOPEN";
	case SQLITE_PROTOCOL:
		return "SQLITE_PROTOCOL";
	case SQLITE_EMPTY:
		return "SQLITE_EMPTY";
	case SQLITE_SCHEMA:
		return "SQLITE_SCHEMA";
	case SQLITE_TOOBIG:
		return "SQLITE_TOOBIG";
	case SQLITE_CONSTRAINT:
		return "SQLITE_CONSTRAINT";
	case SQLITE_MISMATCH:
		return "SQLITE_MISMATCH";
	case SQLITE_MISUSE:
		return "SQLITE_MISUSE";
	case SQLITE_NOLFS:
		return "SQLITE_NOLFS";
	case SQLITE_AUTH:
		return "SQLITE_AUTH";
	case SQLITE_FORMAT:
		return "SQLITE_FORMAT";
	case SQLITE_RANGE:
		return "SQLITE_RANGE";
	case SQLITE_ROW:
		return "SQLITE_ROW";
	case SQLITE_DONE:
		return "SQLITE_DONE";
	case CPPSQLITE_ERROR:
		return "CPPSQLITE_ERROR";
	default: return "UNKNOWN_ERROR";
	}
}
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes for SQLite functions not included in SQLite DLL, but copied below
// from SQLite encode.c
////////////////////////////////////////////////////////////////////////////////
int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out);
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out);

/*
** Encode a binary buffer "in" of size n bytes so that it contains
** no instances of characters '\'' or '\000'.  The output is 
** null-terminated and can be used as a string value in an INSERT
** or UPDATE statement.  Use sqlite3_decode_binary() to convert the
** string back into its original binary.
**
** The result is written into a preallocated output buffer "out".
** "out" must be able to hold at least 2 +(257*n)/254 bytes.
** In other words, the output will be expanded by as much as 3
** bytes for every 254 bytes of input plus 2 bytes of fixed overhead.
** (This is approximately 2 + 1.0118*n or about a 1.2% size increase.)
**
** The return value is the number of characters in the encoded
** string, excluding the "\000" terminator.
*/
int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out)
{
	int i, j, e, m;
	int cnt[256];
	if( n<=0 ){
		out[0] = 'x';
		out[1] = 0;
		return 1;
	}
	memset(cnt, 0, sizeof(cnt));
	for(i=n-1; i>=0; i--){ cnt[in[i]]++; }
	m = n;
	for(i=1; i<256; i++){
		int sum;
		if( i=='\'' ) continue;
		sum = cnt[i] + cnt[(i+1)&0xff] + cnt[(i+'\'')&0xff];
		if( sum<m ){
			m = sum;
			e = i;
			if( m==0 ) break;
		}
	}
	out[0] = (unsigned char)e;
	j = 1;
	for(i=0; i<n; i++){
		int c = (in[i] - e)&0xff;
		if( c==0 ){
			out[j++] = 1;
			out[j++] = 1;
		}else if( c==1 ){
			out[j++] = 1;
			out[j++] = 2;
		}else if( c=='\'' ){
			out[j++] = 1;
			out[j++] = 3;
		}else{
			out[j++] = (unsigned char)c;
		}
	}
	out[j] = 0;
	return j;
}

/*
** Decode the string "in" into binary data and write it into "out".
** This routine reverses the encoding created by sqlite3_encode_binary().
** The output will always be a few bytes less than the input.  The number
** of bytes of output is returned.  If the input is not a well-formed
** encoding, -1 is returned.
**
** The "in" and "out" parameters may point to the same buffer in order
** to decode a string in place.
*/
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out)
{
	int i, c, e;
	e = *(in++);
	i = 0;
	while( (c = *(in++))!=0 ){
		if( c==1 ){
			c = *(in++);
			if( c==1 ){
				c = 0;
			}else if( c==2 ){
				c = 1;
			}else if( c==3 ){
				c = '\'';
			}else{
				return -1;
			}
		}
		out[i++] = (c + e)&0xff;
	}
	return i;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CSQLiteBinary
CSQLiteBinary::CSQLiteBinary()
	: m_pBuffer(NULL)
	, m_nBinaryLen(0)
	, m_nBufferLen(0)
	, m_nEncodedLen(0)
	, m_bEncoded(false)
{

}

CSQLiteBinary::~CSQLiteBinary()
{
	Clear();
}

bool CSQLiteBinary::SetBinary(const unsigned char* pBuf, int nLen)
{
	m_pBuffer = Alloc(nLen);
	if(!m_pBuffer)
		return false;
	memcpy(m_pBuffer, pBuf, nLen);
	return true;
}

bool CSQLiteBinary::SetEncoded(const unsigned char* pBuf)
{
	Clear();
	m_nEncodedLen = strlen((const char*)pBuf);
	m_nBufferLen = m_nEncodedLen + 1; // Allow for NULL terminator
	m_pBuffer = (unsigned char*)malloc(m_nBufferLen);
	if (!m_pBuffer)
		return false;
	memcpy(m_pBuffer, pBuf, m_nBufferLen);
	m_bEncoded = true;
	return true;
}

const unsigned char* CSQLiteBinary::GetEncoded()
{
	if (!m_bEncoded)
	{
		unsigned char* ptmp = (unsigned char*)malloc(m_nBinaryLen);
		assert(ptmp!=NULL);
		memcpy(ptmp, m_pBuffer, m_nBinaryLen);
		m_nEncodedLen = sqlite3_encode_binary(ptmp, m_nBinaryLen, m_pBuffer);
		free(ptmp);
		m_bEncoded = true;
	}

	return m_pBuffer;
}

const unsigned char* CSQLiteBinary::GetBinary()
{
	if (m_bEncoded)
	{
		// in/out buffers can be the same
		m_nBinaryLen = sqlite3_decode_binary(m_pBuffer, m_pBuffer);
		if (m_nBinaryLen == -1)
			return NULL;
		m_bEncoded = false;
	}

	return m_pBuffer;
}

int CSQLiteBinary::GetLength()
{
	GetBinary();
	return m_nBinaryLen;
}

unsigned char* CSQLiteBinary::Alloc(int nLen)
{
	Clear();
	// Allow extra space for encoded binary as per comments in
	// SQLite encode.c See bottom of this file for implementation
	// of SQLite functions use 3 instead of 2 just to be sure ;-)
	m_nBinaryLen = nLen;
	m_nBufferLen = 3 + (257*nLen)/254;
	m_pBuffer = (unsigned char*)malloc(m_nBufferLen);
	if (!m_pBuffer)
	{
		return NULL;
	}
	m_bEncoded = false;

	return m_pBuffer;
}

void CSQLiteBinary::Clear()
{
	if (m_pBuffer)
	{
		m_nBinaryLen = 0;
		m_nBufferLen = 0;
		m_nEncodedLen = 0;
		free(m_pBuffer);
		m_pBuffer = 0;
	}
}
// CSQLiteBinary
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CSQLiteQuery
CSQLiteQuery::CSQLiteQuery()
	: m_pSQLite3(NULL)
	, m_pStmt(NULL)
	, m_bEof(false)
	, m_nCols(0)
	, m_bOwnVM(false)
{

}

CSQLiteQuery::CSQLiteQuery(const CSQLiteQuery& stQuery)
{
	m_pSQLite3 = NULL;
	m_pStmt = stQuery.m_pStmt;
	// Only one object can own the VM
	const_cast<CSQLiteQuery&>(stQuery).m_pStmt = 0;
	m_bEof = stQuery.m_bEof;
	m_nCols = stQuery.m_nCols;
	m_bOwnVM = stQuery.m_bOwnVM;
}

CSQLiteQuery::CSQLiteQuery(sqlite3* pSQLite, sqlite3_stmt* pStmt, bool bEof, bool bOwnVM /*= true*/)
{
	m_pSQLite3 = pSQLite;
	m_pStmt = pStmt;
	m_bEof = bEof;
	m_nCols = sqlite3_column_count(m_pStmt);
	m_bOwnVM = bOwnVM;
}

CSQLiteQuery::~CSQLiteQuery()
{
	Finalize();
}

CSQLiteQuery& CSQLiteQuery::operator=(const CSQLiteQuery& stQuery)
{
	Finalize();
	m_pStmt = stQuery.m_pStmt;
	// Only one object can own the VM
	const_cast<CSQLiteQuery&>(stQuery).m_pStmt = 0;
	m_bEof = stQuery.m_bEof;
	m_nCols = stQuery.m_nCols;
	m_bOwnVM = stQuery.m_bOwnVM;
	return *this;
}

int CSQLiteQuery::FieldIndex(const char* szField)
{
	if(!CheckStmt())
		return -1;
	if (!szField)
		return -1;
	for (int nField = 0; nField < m_nCols; nField++)
	{
		const char* szTemp = sqlite3_column_name(m_pStmt, nField);
		if (strcmp(szField, szTemp) == 0)
			return nField;
	}
	return -1;
}

const char* CSQLiteQuery::FieldName(int nCol)
{
	if(!CheckStmt())
		return NULL;

	if (nCol < 0 || nCol > m_nCols-1)
		return NULL;

	return sqlite3_column_name(m_pStmt, nCol);
}

const char* CSQLiteQuery::FieldDeclType(int nCol)
{
	if(!CheckStmt())
		return NULL;

	if (nCol < 0 || nCol > m_nCols-1)
		return NULL;

	return sqlite3_column_decltype(m_pStmt, nCol);
}

int CSQLiteQuery::FieldDataType(int nCol)
{
	if(!CheckStmt())
		return SQLITE_NULL;

	if (nCol < 0 || nCol > m_nCols-1)
		return SQLITE_NULL;

	return sqlite3_column_type(m_pStmt, nCol);
}

const char* CSQLiteQuery::FieldValue(int nField)
{
	if(!CheckStmt())
		return NULL;

	if (nField < 0 || nField > m_nCols-1)
		return NULL;

	return (const char*)sqlite3_column_text(m_pStmt, nField);
}

const char* CSQLiteQuery::FieldValue(const char* szField)
{
	int nField = FieldIndex(szField);
	if(nField==-1)
		return NULL;
	return (const char*)sqlite3_column_text(m_pStmt, nField);
}

int CSQLiteQuery::GetIntField(int nField, int nNullValue /*= 0*/)
{
	if (FieldDataType(nField) == SQLITE_NULL)
		return nNullValue;
	
	return sqlite3_column_int(m_pStmt, nField);
}

int CSQLiteQuery::GetIntField(const char* szField, int nNullValue /*= 0*/)
{
	int nField = FieldIndex(szField);
	if(nField==-1)
		return nNullValue;
	return GetIntField(nField, nNullValue);
}

sqlite_int64 CSQLiteQuery::GetInt64Field(int nField, sqlite_int64 nNullValue /*= 0*/)
{
	if (FieldDataType(nField) == SQLITE_NULL)
		return nNullValue;
	return sqlite3_column_int64(m_pStmt, nField);
}

sqlite_int64 CSQLiteQuery::GetInt64Field(const char* szField, sqlite_int64 nNullValue /*= 0*/)
{
	int nField = FieldIndex(szField);
	if(nField==-1)
		return nNullValue;
	return GetInt64Field(nField, nNullValue);
}

double CSQLiteQuery::GetFloatField(int nField, double fNullValue /*= 0.0*/)
{
	if (FieldDataType(nField) == SQLITE_NULL)
		return fNullValue;
	return sqlite3_column_double(m_pStmt, nField);
}

double CSQLiteQuery::GetFloatField(const char* szField, double fNullValue /*= 0.0*/)
{
	int nField = FieldIndex(szField);
	if(nField==-1)
		return fNullValue;
	return GetFloatField(nField, fNullValue);
}

const char* CSQLiteQuery::GetStringField(int nField, const char* szNullValue /*= ""*/)
{
	if (FieldDataType(nField) == SQLITE_NULL)
		return szNullValue;
	return (const char*)sqlite3_column_text(m_pStmt, nField);
}

const char* CSQLiteQuery::GetStringField(const char* szField, const char* szNullValue /*= ""*/)
{
	int nField = FieldIndex(szField);
	if(nField==-1)
		return szNullValue;
	return GetStringField(nField, szNullValue);
}

const unsigned char* CSQLiteQuery::GetBlobField(int nField, int& nLen)
{
	if(!CheckStmt())
		return NULL;
	if (nField < 0 || nField > m_nCols-1)
		return NULL;
	nLen = sqlite3_column_bytes(m_pStmt, nField);
	return (const unsigned char*)sqlite3_column_blob(m_pStmt, nField);
}

const unsigned char* CSQLiteQuery::GetBlobField(const char* szField, int& nLen)
{
	int nField = FieldIndex(szField);
	if(nField==-1)
		return NULL;
	return GetBlobField(nField, nLen);
}

bool CSQLiteQuery::FieldIsNull(int nField)
{
	return (FieldDataType(nField) == SQLITE_NULL);
}

bool CSQLiteQuery::FieldIsNull(const char* szField)
{
	int nField = FieldIndex(szField);
	return (FieldDataType(nField) == SQLITE_NULL);
}

bool CSQLiteQuery::CheckStmt()
{
	if(!m_pStmt)
		return false;
	return true;
}

int CSQLiteQuery::NumFields()
{
	if(CheckStmt())
		return m_nCols;
	return -1;
}

bool CSQLiteQuery::Eof()
{
	if(CheckStmt())
		return false;
	return m_bEof;
}

bool CSQLiteQuery::NextRow()
{
	if(CheckStmt())
		return false;

	int nRet = sqlite3_step(m_pStmt);
	if (nRet == SQLITE_DONE)
	{
		// no rows
		m_bEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = sqlite3_finalize(m_pStmt);
		m_pStmt = 0;
		if (nRet == SQLITE_OK)
			return false;
		//const char* szError = sqlite3_errmsg(m_pSQLite3);
	}
	return true;
}

bool CSQLiteQuery::Finalize()
{
	if (m_pStmt && m_bOwnVM)
	{
		int nRet = sqlite3_finalize(m_pStmt);
		m_pStmt = 0;
		if (nRet != SQLITE_OK)
		{
			//const char* szError = sqlite3_errmsg(mpDB);
			return false;
		}
	}
	return true;
}
// CSQLiteQuery
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CSQLiteTable
CSQLiteTable::CSQLiteTable()
	: m_paszResults(NULL)
	, m_nCols(0)
	, m_nRows(0)
	, m_nCurrentRow(0)
{

}

CSQLiteTable::CSQLiteTable(const CSQLiteTable& stTable)
{
	m_paszResults = stTable.m_paszResults;
	// Only one object can own the results
	const_cast<CSQLiteTable&>(stTable).m_paszResults = 0;
	m_nRows = stTable.m_nRows;
	m_nCols = stTable.m_nCols;
	m_nCurrentRow = stTable.m_nCurrentRow;
}

CSQLiteTable::CSQLiteTable(char** paszResults, int nRows, int nCols)
{
	m_paszResults = paszResults;
	m_nRows = nRows;
	m_nCols = nCols;
	m_nCurrentRow = 0;
}

CSQLiteTable::~CSQLiteTable()
{
	Finalize();
}

CSQLiteTable& CSQLiteTable::operator=(const CSQLiteTable& stTable)
{
	Finalize();
	m_paszResults = stTable.m_paszResults;
	// Only one object can own the results
	const_cast<CSQLiteTable&>(stTable).m_paszResults = 0;
	m_nRows = stTable.m_nRows;
	m_nCols = stTable.m_nCols;
	m_nCurrentRow = stTable.m_nCurrentRow;
	return *this;
}


int CSQLiteTable::NumFields()
{
	if(!IsValid())
		return -1;
	return m_nCols;
}

int CSQLiteTable::NmRows()
{
	if(!IsValid())
		return -1;
	return m_nRows;
}

const char* CSQLiteTable::FieldName(int nCol)
{
	if(!IsValid())
		return NULL;
	if (nCol < 0 || nCol > m_nCols-1)
		return NULL;

	return m_paszResults[nCol];
}

const char* CSQLiteTable::FieldValue(int nField)
{
	if(!IsValid())
		return NULL;
	if (nField < 0 || nField > m_nCols-1)
		return NULL;

	int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
	return m_paszResults[nIndex];
}

const char* CSQLiteTable::FieldValue(const char* szField)
{
	if(!IsValid())
		return NULL;
	if (!szField)
		return NULL;
	for (int nField = 0; nField < m_nCols; nField++)
	{
		if (stricmp(szField, m_paszResults[nField]) == 0)
		{
			int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
			return m_paszResults[nIndex];
		}
	}
	return NULL;
}

int CSQLiteTable::GetIntField(int nField, int nNullValue /*= 0*/)
{
	if (FieldIsNull(nField))
		return nNullValue;
	const char *szVal = FieldValue(nField);
	if(!szVal)
		return nNullValue;
	return atoi(szVal);
}

int CSQLiteTable::GetIntField(const char* szField, int nNullValue /*= 0*/)
{
	if (FieldIsNull(szField))
		return nNullValue;
	const char *szVal = FieldValue(szField);
	if(!szVal)
		return nNullValue;
	return atoi(szVal);
}

double CSQLiteTable::GetFloatField(int nField, double fNullValue /*= 0.0*/)
{
	if (FieldIsNull(nField))
		return fNullValue;
	const char *szVal = FieldValue(nField);
	if(!szVal)
		return fNullValue;
	return atof(szVal);
}

double CSQLiteTable::GetFloatField(const char* szField, double fNullValue /*= 0.0*/)
{
	if (FieldIsNull(szField))
		return fNullValue;
	const char *szVal = FieldValue(szField);
	if(!szVal)
		return fNullValue;
	return atof(szVal);
}

const char* CSQLiteTable::GetStringField(int nField, const char* szNullValue /*= ""*/)
{
	if (FieldIsNull(nField))
		return szNullValue;
	return FieldValue(nField);
}

const char* CSQLiteTable::GetStringField(const char* szField, const char* szNullValue /*= ""*/)
{
	if (FieldIsNull(szField))
		return szNullValue;
	return FieldValue(szField);
}

bool CSQLiteTable::IsValid()
{
	if (!m_paszResults)
		return false;
	return true;
}

bool CSQLiteTable::FieldIsNull(int nField)
{
	if(!IsValid())
		return false;
	return (FieldValue(nField) == 0);
}

bool CSQLiteTable::FieldIsNull(const char* szField)
{
	if(!IsValid())
		return false;
	return (FieldValue(szField) == 0);
}

bool CSQLiteTable::SetRow(int nRow)
{
	if(!IsValid())
		return false;
	if (nRow < 0 || nRow > m_nRows-1)
		return false;

	m_nCurrentRow = nRow;
	return true;
}

bool CSQLiteTable::Finalize()
{
	if (m_paszResults)
	{
		sqlite3_free_table(m_paszResults);
		m_paszResults = 0;
	}
	return true;
}
// CSQLiteTable
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CSQLiteStmt
CSQLiteStmt::CSQLiteStmt()
	: m_pSQLite3(NULL)
	, m_pStmt(NULL)
{

}

CSQLiteStmt::CSQLiteStmt(const CSQLiteStmt& stStmt)
{
	m_pSQLite3 = stStmt.m_pSQLite3;
	m_pStmt = stStmt.m_pStmt;
	// Only one object can own VM
	const_cast<CSQLiteStmt&>(stStmt).m_pStmt = 0;
}

CSQLiteStmt::CSQLiteStmt(sqlite3* pSQLite, sqlite3_stmt* pStmt)
{
	m_pSQLite3 = pSQLite;
	m_pStmt = pStmt;
}

CSQLiteStmt::~CSQLiteStmt()
{
	Finalize();
}

CSQLiteStmt& CSQLiteStmt::operator=(const CSQLiteStmt& stStmt)
{
	m_pSQLite3 = stStmt.m_pSQLite3;
	m_pStmt = stStmt.m_pStmt;
	// Only one object can own VM
	const_cast<CSQLiteStmt&>(stStmt).m_pStmt = 0;
	return *this;
}

int CSQLiteStmt::ExecDML()
{
	if(!CheckSQLite())
		return -1;
	if(!CheckStmt())
		return -1;
	int nRowsChanged = -1;
	const char* szError = 0;
	int nRet = sqlite3_step(m_pStmt);
	if (nRet == SQLITE_DONE)
	{
		nRowsChanged = sqlite3_changes(m_pSQLite3);
		nRet = sqlite3_reset(m_pStmt);
		if (nRet != SQLITE_OK)
			szError = sqlite3_errmsg(m_pSQLite3);
	}
	else
	{
		nRet = sqlite3_reset(m_pStmt);
		szError = sqlite3_errmsg(m_pSQLite3);
	}
	return nRowsChanged;
}

CSQLiteQuery CSQLiteStmt::ExecQuery()
{
	CSQLiteQuery stQuery;
	if(!CheckSQLite())
		return stQuery;
	if(!CheckStmt())
		return stQuery;
	int nRet = sqlite3_step(m_pStmt);
	if (nRet == SQLITE_DONE)
		return CSQLiteQuery(m_pSQLite3, m_pStmt, true/*eof*/, false); // no rows
	else if (nRet == SQLITE_ROW)
		return CSQLiteQuery(m_pSQLite3, m_pStmt, false/*eof*/, false); // at least 1 row
	nRet = sqlite3_reset(m_pStmt);
	/*const char* szError = */sqlite3_errmsg(m_pSQLite3);
	return stQuery;
}

void CSQLiteStmt::Bind(int nParam, const char* szValue)
{
	if(!CheckStmt())
		return ;
	int nRes = sqlite3_bind_text(m_pStmt, nParam, szValue, -1, SQLITE_TRANSIENT);
	assert(nRes!=SQLITE_OK);
}

void CSQLiteStmt::Bind(int nParam, const int nValue)
{
	if(!CheckStmt())
		return ;
	int nRes = sqlite3_bind_int(m_pStmt, nParam, nValue);
	assert(nRes!=SQLITE_OK);
}

void CSQLiteStmt::Bind(int nParam, const double fValue)
{
	if(!CheckStmt())
		return ;
	int nRes = sqlite3_bind_double(m_pStmt, nParam, fValue);
	assert(nRes!=SQLITE_OK);
}

void CSQLiteStmt::Bind(int nParam, const unsigned char* blobValue, int nLen)
{
	if(!CheckStmt())
		return ;
	int nRes = sqlite3_bind_blob(m_pStmt, nParam, (const void*)blobValue, nLen, SQLITE_TRANSIENT);
	assert(nRes!=SQLITE_OK);
}

void CSQLiteStmt::BindNull(int nParam)
{
	if(!CheckStmt())
		return ;
	int nRes = sqlite3_bind_null(m_pStmt, nParam);
	assert(nRes!=SQLITE_OK);
}

int CSQLiteStmt::BindParamIndex(const char* szParam)
{
	if(!CheckStmt())
		return -1;
	int nParam = sqlite3_bind_parameter_index(m_pStmt, szParam);
	/*int nn = */sqlite3_bind_parameter_count(m_pStmt);
	/*const char* sz1 = */sqlite3_bind_parameter_name(m_pStmt, 1);
	/*const char* sz2 = */sqlite3_bind_parameter_name(m_pStmt, 2);
	if (!nParam)
	{
		//char buf[128];
		//sprintf(buf, "Parameter '%s' is not valid for this statement", szParam);
	}

	return nParam;
}

void CSQLiteStmt::Bind(const char* szParam, const char* szValue)
{
	int nParam = BindParamIndex(szParam);
	Bind(nParam, szValue);
}

void CSQLiteStmt::Bind(const char* szParam, const int nValue)
{
	int nParam = BindParamIndex(szParam);
	Bind(nParam, nValue);
}

void CSQLiteStmt::Bind(const char* szParam, const double fValue)
{
	int nParam = BindParamIndex(szParam);
	Bind(nParam, fValue);
}

void CSQLiteStmt::Bind(const char* szParam, const unsigned char* blobValue, int nLen)
{
	int nParam = BindParamIndex(szParam);
	Bind(nParam, blobValue, nLen);
}

void CSQLiteStmt::BindNull(const char* szParam)
{
	int nParam = BindParamIndex(szParam);
	BindNull(nParam);
}

bool CSQLiteStmt::CheckSQLite()
{
	if (!m_pSQLite3)
		return false;
	return true;
}

bool CSQLiteStmt::CheckStmt()
{
	if (!m_pStmt)
		return false;
	return true;
}

bool CSQLiteStmt::Reset()
{
	if (!m_pStmt)
		return false;
	int nRet = sqlite3_reset(m_pStmt);
	if (nRet != SQLITE_OK)
	{
		//const char* szError = sqlite3_errmsg(m_pSQLite3);
		return false;
	}
	return true;
}

bool CSQLiteStmt::Finalize()
{
	if (!m_pStmt)
		return false;
	int nRet = sqlite3_finalize(m_pStmt);
	m_pStmt = 0;
	if (nRet != SQLITE_OK)
	{
		//const char* szError = sqlite3_errmsg(m_pSQLite3);
		return false;
	}
	return true;
}
// CSQLiteStmt
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CSQLite
CSQLite::CSQLite()
{
	m_pSQLite3 = 0;
	m_nBusyTimeoutMs = 60000; // 60 seconds
}

CSQLite::CSQLite(const CSQLite& stSQLite)
{
	m_pSQLite3 = stSQLite.m_pSQLite3;
	m_nBusyTimeoutMs = 60000; // 60 seconds
}

CSQLite& CSQLite::operator=(const CSQLite& stSQLite)
{
	m_pSQLite3 = stSQLite.m_pSQLite3;
	m_nBusyTimeoutMs = 60000; // 60 seconds
	return *this;
}

CSQLite::~CSQLite()
{
	Close();
}

bool CSQLite::Open(const char* szFile)
{
	int nRet = sqlite3_open(szFile, &m_pSQLite3);
	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pSQLite3);
		return false;
	}

	SetBusyTimeout(m_nBusyTimeoutMs);
	return true;
}

void CSQLite::Close()
{
	if (!m_pSQLite3)
		return;
	if (sqlite3_close(m_pSQLite3) == SQLITE_OK)
		m_pSQLite3 = 0;
}

bool CSQLite::TableExists(const char* szTable)
{
	char szSQL[256];
	sprintf(szSQL, "select count(*) from sqlite_master where type='table' and name='%s'", szTable);
	int nRet = ExecScalar(szSQL);
	return (nRet > 0);
}

int CSQLite::ExecDML(const char* szSQL)
{
	if(!IsValid())
		return -1;
	char* szError = 0;
	int nRet = sqlite3_exec(m_pSQLite3, szSQL, 0, 0, &szError);
	if (nRet == SQLITE_OK)
		return sqlite3_changes(m_pSQLite3);
	return -1;
}

CSQLiteQuery CSQLite::ExecQuery(const char* szSQL)
{
	CSQLiteQuery stQuery;
	if(!IsValid())
		return stQuery;
	sqlite3_stmt* pStmt = Compile(szSQL);
	int nRet = sqlite3_step(pStmt);
	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CSQLiteQuery(m_pSQLite3, pStmt, true/*eof*/);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CSQLiteQuery(m_pSQLite3, pStmt, false/*eof*/);
	}
	else
	{
		nRet = sqlite3_finalize(pStmt);
		//const char* szError= sqlite3_errmsg(m_pSQLite3);
		return stQuery;
	}
	return stQuery;
}

int CSQLite::ExecScalar(const char* szSQL, int nNullValue /*= 0*/)
{
	CSQLiteQuery query = ExecQuery(szSQL);
	if (query.Eof() || query.NumFields() < 1)
		return -1;

	return query.GetIntField(0, nNullValue);
}

CSQLiteTable CSQLite::GetTable(const char* szSQL)
{
	CSQLiteTable stTable;
	if(!IsValid())
		return stTable;

	char* szError = 0;
	char** paszResults = 0;
	int nRet;
	int nRows = 0;
	int nCols = 0;
	nRet = sqlite3_get_table(m_pSQLite3, szSQL, &paszResults, &nRows, &nCols, &szError);
	if (nRet == SQLITE_OK)
		return CSQLiteTable(paszResults, nRows, nCols);

	return stTable;
}

CSQLiteStmt CSQLite::CompileStmt(const char* szSQL)
{
	CSQLiteStmt stStmt;
	if(!IsValid())
		return stStmt;

	sqlite3_stmt* pStmt = Compile(szSQL);
	return CSQLiteStmt(m_pSQLite3, pStmt);
}

sqlite_int64 CSQLite::LastRowId()
{
	return sqlite3_last_insert_rowid(m_pSQLite3);
}

void CSQLite::SetBusyTimeout(int nMillisecs)
{
	m_nBusyTimeoutMs = nMillisecs;
	sqlite3_busy_timeout(m_pSQLite3, m_nBusyTimeoutMs);
}

bool CSQLite::IsValid()
{
	if (!m_pSQLite3)
		return false;
	return true;
}

bool CSQLite::IsAutoCommitOn()
{
	if(!IsValid())
		return false;
	return sqlite3_get_autocommit(m_pSQLite3) ? true : false;
}

sqlite3_stmt* CSQLite::Compile(const char* szSQL)
{
	if(!IsValid())
		return NULL;
	const char* szTail = 0;
	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(m_pSQLite3, szSQL, -1, &pStmt, &szTail);

	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pSQLite3);
		return NULL;
	}
	return pStmt;
}
// CSQLite
//////////////////////////////////////////////////////////////////////////