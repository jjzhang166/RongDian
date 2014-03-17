#pragma once
#include "../../thirdparty/Sqlite/sqlite3.h"

#include <cstdio>
#include <cstring>

#define CPPSQLITE_ERROR 1000

const char* GetSQLiteErr(int nErrCode);

class CSQLiteBinary
{
public:
	CSQLiteBinary();
	~CSQLiteBinary();

	bool SetBinary(const unsigned char* pBuf, int nLen);
	bool SetEncoded(const unsigned char* pBuf);

	const unsigned char* GetEncoded();
	const unsigned char* GetBinary();

	int GetLength();
	unsigned char* Alloc(int nLen);
	void Clear();

private:
	unsigned char* m_pBuffer;
	int m_nBinaryLen;
	int m_nBufferLen;
	int m_nEncodedLen;
	bool m_bEncoded;
};

class CSQLiteQuery
{
public:
	CSQLiteQuery();
	CSQLiteQuery(const CSQLiteQuery& stQuery);
	CSQLiteQuery(sqlite3* pSQLite, sqlite3_stmt* pStmt, bool bEof, bool bOwnVM = true);
	virtual ~CSQLiteQuery();
	CSQLiteQuery& operator=(const CSQLiteQuery& stQuery);

	int FieldIndex(const char* szField);
	const char* FieldName(int nCol);

	const char* FieldDeclType(int nCol);
	int FieldDataType(int nCol);

	const char* FieldValue(int nField);
	const char* FieldValue(const char* szField);

	int GetIntField(int nField, int nNullValue = 0);
	int GetIntField(const char* szField, int nNullValue = 0);

	sqlite_int64 GetInt64Field(int nField, sqlite_int64 nNullValue = 0);
	sqlite_int64 GetInt64Field(const char* szField, sqlite_int64 nNullValue = 0);

	double GetFloatField(int nField, double fNullValue = 0.0);
	double GetFloatField(const char* szField, double fNullValue = 0.0);

	const char* GetStringField(int nField, const char* szNullValue = "");
	const char* GetStringField(const char* szField, const char* szNullValue = "");

	const unsigned char* GetBlobField(int nField, int& nLen);
	const unsigned char* GetBlobField(const char* szField, int& nLen);

	bool FieldIsNull(int nField);
	bool FieldIsNull(const char* szField);

	bool CheckStmt();
	int NumFields();
	bool Eof();
	bool NextRow();
	bool Finalize();

private:
	sqlite3* m_pSQLite3;
	sqlite3_stmt* m_pStmt;
	bool m_bEof;
	int m_nCols;
	bool m_bOwnVM;
};

class CSQLiteTable
{
public:
	CSQLiteTable();
	CSQLiteTable(const CSQLiteTable& stTable);
	CSQLiteTable(char** paszResults, int nRows, int nCols);
	virtual ~CSQLiteTable();
	CSQLiteTable& operator=(const CSQLiteTable& stTable);

	int NumFields();
	int NmRows();

	const char* FieldName(int nCol);

	const char* FieldValue(int nField);
	const char* FieldValue(const char* szField);

	int GetIntField(int nField, int nNullValue = 0);
	int GetIntField(const char* szField, int nNullValue = 0);

	double GetFloatField(int nField, double fNullValue = 0.0);
	double GetFloatField(const char* szField, double fNullValue = 0.0);

	const char* GetStringField(int nField, const char* szNullValue = "");
	const char* GetStringField(const char* szField, const char* szNullValue = "");

	bool IsValid();
	bool FieldIsNull(int nField);
	bool FieldIsNull(const char* szField);

	bool SetRow(int nRow);
	bool Finalize();

private:
	int m_nCols;
	int m_nRows;
	int m_nCurrentRow;
	char** m_paszResults;
};


class CSQLiteStmt
{
public:
	CSQLiteStmt();
	CSQLiteStmt(const CSQLiteStmt& stStmt);
	CSQLiteStmt(sqlite3* pSQLite, sqlite3_stmt* pStmt);
	virtual ~CSQLiteStmt();
	CSQLiteStmt& operator=(const CSQLiteStmt& stStmt);

	int ExecDML();
	CSQLiteQuery ExecQuery();

	void Bind(int nParam, const char* szValue);
	void Bind(int nParam, const int nValue);
	void Bind(int nParam, const double fValue);
	void Bind(int nParam, const unsigned char* blobValue, int nLen);
	void BindNull(int nParam);

	int BindParamIndex(const char* szParam);
	void Bind(const char* szParam, const char* szValue);
	void Bind(const char* szParam, const int nValue);
	void Bind(const char* szParam, const double fValue);
	void Bind(const char* szParam, const unsigned char* blobValue, int nLen);
	void BindNull(const char* szParam);

	bool CheckSQLite();
	bool CheckStmt();
	bool Reset();
	bool Finalize();

private:
	sqlite3* m_pSQLite3;
	sqlite3_stmt* m_pStmt;
};

class CSQLite
{
public:
	CSQLite();
	CSQLite(const CSQLite& stSQLite);
	CSQLite& operator=(const CSQLite& stSQLite);
	virtual ~CSQLite();

	bool Open(const char* szFile);
	void Close();

	bool TableExists(const char* szTable);

	int ExecDML(const char* szSQL);
	CSQLiteQuery ExecQuery(const char* szSQL);
	int ExecScalar(const char* szSQL, int nNullValue = 0);

	CSQLiteTable GetTable(const char* szSQL);

	CSQLiteStmt CompileStmt(const char* szSQL);

	sqlite_int64 LastRowId();

	void Interrupt() { sqlite3_interrupt(m_pSQLite3); }

	void SetBusyTimeout(int nMillisecs);

	static const char* Version() { return SQLITE_VERSION; }
	static const char* HeaderVersion() { return SQLITE_VERSION; }
	static const char* LibraryVersion() { return sqlite3_libversion(); }
	static int VersionNumber() { return sqlite3_libversion_number(); }

	bool IsValid();
	bool IsAutoCommitOn();
	sqlite3_stmt* Compile(const char* szSQL);

private:
	sqlite3* m_pSQLite3;
	int m_nBusyTimeoutMs;
};