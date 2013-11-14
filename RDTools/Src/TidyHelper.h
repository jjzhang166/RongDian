#pragma once
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
//
class AsTidy
{
public:
	AsTidy();
	~AsTidy();

	bool Format(const char* pszTextIn, const char *pszOptions, string &strOut, string &strErr);

private:
	static void  PrintError(LPCTSTR pszErr);
	// callback functions for Artistic Style
	static void  STDCALL ErrorHandler(int nErr, const char* pszErr);
	static char* STDCALL MemoryAlloc(unsigned long nSize);
};
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
class HtmlTidy
{
public:
	HtmlTidy(UINT uLang);
	~HtmlTidy();

	bool Format(const char* pszTextIn, const char *pszOptions, string &strOut, string &strErr);

private:
	void SetTidyConfig(TidyDoc tdoc);
	void SetTidyControl(TidyDoc tdoc, LPCTSTR lpszTidy, int nPos, int nSize);
	LPCSTR GetEncodeByIndex(int nIndex);
	void InitTidyDefault(TidyDoc tdoc);

private:
	UINT m_nHtmlXml;
};
//
//////////////////////////////////////////////////////////////////////////

class CTidyHelper
{
public:
	CTidyHelper();
	~CTidyHelper();

	BOOL DoTidy(UINT nLang, const char *pTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut = NULL);

private:
	BOOL TidyCpp(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyCs(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyJava(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyHtml(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyXml(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyPhp(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyJs(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyCss(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidyJson(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
	BOOL TidySql(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut);
};