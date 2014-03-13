#include "stdafx.h"
#include "TidyHelper.h"

//////////////////////////////////////////////////////////////////////////
//
AsTidy::AsTidy()
{

}

AsTidy::~AsTidy()
{

}

bool AsTidy::Format(const char* pszTextIn, const char *pszOptions, string &strOut, string &strErr)
{
	strOut = AStyleMain(pszTextIn, pszOptions, ErrorHandler, MemoryAlloc);
	return true;
}

void AsTidy::PrintError(LPCTSTR pszErr)
{
	OutputDebugString(pszErr);
}

// callback functions for Artistic Style
void  STDCALL AsTidy::ErrorHandler(int nErr, const char* pszErr)
{
	char szMsg[1024] = {0};
	sprintf(szMsg, "astyle error %d\n%s", nErr, pszErr);
}

char* STDCALL AsTidy::MemoryAlloc(unsigned long nSize)
{
	// error condition should be checked by calling procedure
	char* buffer = new(std::nothrow) char [nSize];
	return buffer;
}

//////////////////////////////////////////////////////////////////////////
//
HtmlTidy::HtmlTidy(UINT uLang)
	: m_nHtmlXml(uLang)
{

}

HtmlTidy::~HtmlTidy()
{

}

bool HtmlTidy::Format(const char* pszTextIn, const char *pszOptions, string &strOut, string &strErr)
{
	TidyBuffer output;
	TidyBuffer errbuf;	
	int rc = -1;
	Bool ok = yes;

	TidyDoc tdoc = tidyCreate();                     // Initialize "document"
	tidyBufInit(&output);
	tidyBufInit(&errbuf);

	InitTidyDefault(tdoc);
	SetTidyConfig(tdoc);

	ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );  // Convert to XHTML
	if ( ok )
		rc = tidySetErrorBuffer(tdoc, &errbuf);      // Capture diagnostics
	if ( rc >= 0 )
		rc = tidyParseString(tdoc, pszTextIn);           // Parse the input
	if ( rc >= 0 )
		rc = tidyCleanAndRepair(tdoc);               // Tidy it up!
	if ( rc >= 0 )
		rc = tidyRunDiagnostics(tdoc);               // Kvetch
	if ( rc > 1 )                                    // If error, force output.
		rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
	if ( rc >= 0 )
		rc = tidySaveBuffer(tdoc, &output);          // Pretty Print

	if ( rc >= 0 && output.bp!=NULL)
	{		
		strOut = reinterpret_cast< char const* >(output.bp);
	}

	strErr = reinterpret_cast< char const* >(errbuf.bp);
	string strEmpty = "No warnings or errors were found.\r\n\r\n";
	if (strEmpty == strErr)
	{
		strErr = "";
	}
	tidyBufFree(&output);
	tidyBufFree(&errbuf);
	tidyRelease(tdoc);
	return true;
}

void HtmlTidy::SetTidyConfig(TidyDoc tdoc)
{
	tstring strTidy;
	if (TIDY_HTML == m_nHtmlXml)
	{
		strTidy = g_TidyInfo[TIDY_HTML].szName;
	}
	else
	{
		strTidy = g_TidyInfo[TIDY_HTML].szName;
	}

	int lenTidy = strTidy.length();
	if (lenTidy <= 0 || strTidy[0] != '-')
	{
		return;
	}

	int nOption = -1;
	for (int i = 0; i < lenTidy; ++i)
	{
		if (strTidy[i] == '-')
		{
			if (-1 != nOption)
			{
				SetTidyControl(tdoc, strTidy.c_str(), nOption, i - nOption);
			}
			nOption = i;
		}
	}
	SetTidyControl(tdoc, strTidy.c_str(), nOption, lenTidy - nOption);
}

void HtmlTidy::SetTidyControl(TidyDoc tdoc, LPCTSTR lpszTidy, int nPos, int nSize)
{
	if (lpszTidy[nPos] != '-' || nSize < 2)
	{
		return;
	}

	int nNumValue = nSize;
	for (int i = nPos + 1; i < nPos + nSize; ++i)
	{
		if (!_istalpha(lpszTidy[i]))
		{
			nNumValue = i - nPos;
			break;
		}
	}
	tstring strParam(lpszTidy + nPos + 1, nNumValue - 1);
	if (nNumValue != nSize)
	{
		tstring strNum(lpszTidy + nPos + nNumValue, nSize - nNumValue);
		nNumValue = _ttoi(strNum.c_str());
	}
	else
	{
		nNumValue = 0;
	}
	TCHAR tszNumValue[64];
	_stprintf(tszNumValue, _T("%d"), nNumValue);

	string strNothing = "";
	if (_T("axd") == strParam)
	{
		tidyOptSetBool(tdoc, TidyXmlDecl, yes);
	} 
	else if (_T("axs") == strParam)
	{
		tidyOptSetBool(tdoc, TidyXmlSpace, yes);
	}
	else if (_T("aan") == strParam)
	{
		tidyOptSetBool(tdoc, TidyAnchorAsName, yes);
	}
	else if (_T("axp") == strParam)
	{
		tidyOptSetBool(tdoc, TidyXmlPIs, yes);
	}
	else if (_T("b") == strParam)
	{
		tidyOptSetBool(tdoc, TidyMakeBare, yes);
	}
	else if (_T("c") == strParam)
	{
		tidyOptSetBool(tdoc, TidyMakeClean, yes);
	}
	else if (_T("diu") == strParam)
	{
		tidyOptSetBool(tdoc, TidyDecorateInferredUL, yes);
	}
	else if (_T("dep") == strParam)
	{
		tidyOptSetBool(tdoc, TidyDropEmptyParas, yes);
	}
	else if (_T("dft") == strParam)
	{
		tidyOptSetBool(tdoc, TidyDropFontTags, yes);
	}
	else if (_T("dpa") == strParam)
	{
		tidyOptSetBool(tdoc, TidyDropPropAttrs, yes);
	}
	else if (_T("ebt") == strParam)
	{
		tidyOptSetBool(tdoc, TidyEncloseBlockText, yes);
	}
	else if (_T("et") == strParam)
	{
		tidyOptSetBool(tdoc, TidyEncloseBodyText, yes);
	}
	else if (_T("ec") == strParam)
	{
		tidyOptSetBool(tdoc, TidyEscapeCdata, yes);
	}
	else if (_T("fb") == strParam)
	{
		tidyOptSetBool(tdoc, TidyFixBackslash, yes);
	}
	else if (_T("fbc") == strParam)
	{
		tidyOptSetBool(tdoc, TidyFixComments, yes);
	}
	else if (_T("fu") == strParam)
	{
		tidyOptSetBool(tdoc, TidyFixUri, yes);
	}
	else if (_T("hc") == strParam)
	{
		tidyOptSetBool(tdoc, TidyHideComments, yes);
	}
	else if (_T("he") == strParam)
	{
		tidyOptSetBool(tdoc, TidyHideEndTags, yes);
	}
	else if (_T("ic") == strParam)
	{
		tidyOptSetBool(tdoc, TidyIndentCdata, yes);
	}
	else if (_T("ix") == strParam)
	{
		tidyOptSetBool(tdoc, TidyXmlTags, yes);
	}
	else if (_T("jc") == strParam)
	{
		tidyOptSetBool(tdoc, TidyJoinClasses, yes);
	}
	else if (_T("js") == strParam)
	{
		tidyOptSetBool(tdoc, TidyJoinStyles, yes);
	}
	else if (_T("la") == strParam)
	{
		tidyOptSetBool(tdoc, TidyLiteralAttribs, yes);
	}
	else if (_T("le") == strParam)
	{
		tidyOptSetBool(tdoc, TidyLogicalEmphasis, yes);
	}
	else if (_T("ll") == strParam)
	{
		tidyOptSetBool(tdoc, TidyLowerLiterals, yes);
	}
	else if (_T("n") == strParam)
	{
		tidyOptSetBool(tdoc, TidyNCR, yes);
	}
	else if (_T("ne") == strParam)
	{
		tidyOptSetBool(tdoc, TidyNumEntities, yes);
	}
	else if (_T("oh") == strParam)
	{
		tidyOptSetBool(tdoc, TidyHtmlOut, yes);
	}
	else if (_T("ox") == strParam)
	{
		tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
	}
	else if (_T("oxm") == strParam)
	{
		tidyOptSetBool(tdoc, TidyXmlOut, yes);
	}
	else if (_T("pe") == strParam)
	{
		tidyOptSetBool(tdoc, TidyPreserveEntities, yes);
	}
	else if (_T("qa") == strParam)
	{
		tidyOptSetBool(tdoc, TidyQuoteAmpersand, yes);
	}
	else if (_T("qm") == strParam)
	{
		tidyOptSetBool(tdoc, TidyQuoteMarks, yes);
	}
	else if (_T("qn") == strParam)
	{
		tidyOptSetBool(tdoc, TidyQuoteNbsp, yes);
	}
	else if (_T("rc") == strParam)
	{
		tidyOptSetBool(tdoc, TidyReplaceColor, yes);
	}
	else if (_T("ua") == strParam)
	{
		tidyOptSetBool(tdoc, TidyUpperCaseAttrs, yes);
	}
	else if (_T("ut") == strParam)
	{
		tidyOptSetBool(tdoc, TidyUpperCaseTags, yes);
	}
	else if (_T("wo") == strParam)
	{
		tidyOptSetBool(tdoc, TidyWord2000, yes);
	}
	else if (_T("bbb") == strParam)
	{
		tidyOptSetBool(tdoc, TidyBreakBeforeBR, yes);
	} 
	else if (_T("ia") == strParam)
	{
		tidyOptSetBool(tdoc, TidyIndentAttributes, yes);
	}
	else if (_T("m") == strParam)
	{
		tidyOptSetBool(tdoc, TidyShowMarkup, yes);
	}
	else if (_T("pw") == strParam)
	{
		tidyOptSetBool(tdoc, TidyPunctWrap, yes);
	}
	else if (_T("vs") == strParam)
	{
		tidyOptSetBool(tdoc, TidyVertSpace, yes);
	}
	else if (_T("wa") == strParam)
	{
		tidyOptSetBool(tdoc, TidyWrapAsp, yes);
	}
	else if (_T("wat") == strParam)
	{
		tidyOptSetBool(tdoc, TidyWrapAttVals, yes);
	}
	else if (_T("wj") == strParam)
	{
		tidyOptSetBool(tdoc, TidyWrapJste, yes);
	}
	else if (_T("wp") == strParam)
	{
		tidyOptSetBool(tdoc, TidyWrapPhp, yes);
	}
	else if (_T("wsl") == strParam)
	{
		tidyOptSetBool(tdoc, TidyWrapScriptlets, yes);
	}
	else if (_T("ws") == strParam)
	{
		tidyOptSetBool(tdoc, TidyWrapSection, yes);
	}
	else if (_T("ac") == strParam)
	{
		tidyOptSetBool(tdoc, TidyAsciiChars, yes);
	}
	else if (_T("sw") == strParam)
	{
		tidyOptSetBool(tdoc, TidyShowWarnings, yes);
	}
	else if (_T("fo") == strParam)
	{
		tidyOptSetBool(tdoc, TidyForceOutput, yes);
	}
	else if (_T("i") == strParam)
	{
		tidyOptSetInt(tdoc, TidyIndentContent, abs(nNumValue - 2) % 3);
	}
	else if (_T("md") == strParam)
	{
		tidyOptSetInt(tdoc, TidyMergeDivs, abs(nNumValue - 2) % 3);
	}
	else if (_T("ms") == strParam)
	{
		tidyOptSetInt(tdoc, TidyMergeSpans, abs(nNumValue - 2) % 3);
	}
	else if (_T("sbo") == strParam)
	{
		tidyOptSetInt(tdoc, TidyBodyOnly, abs(nNumValue - 2) % 3);
	}
	else if (_T("d") == strParam)
	{
		tidyOptSetInt(tdoc, TidyDoctypeMode, nNumValue % 5);
	}
	else if (_T("ra") == strParam)
	{
		tidyOptSetInt(tdoc, TidyDuplicateAttrs, nNumValue % 2);
	}
	else if (_T("sa") == strParam)
	{
		tidyOptSetInt(tdoc, TidySortAttributes, nNumValue % 2);
	}
	else if (_T("ce") == strParam)
	{		
		tidySetCharEncoding(tdoc, GetEncodeByIndex(nNumValue));
	}
	else if (_T("ie") == strParam)
	{
		tidySetInCharEncoding(tdoc, GetEncodeByIndex(nNumValue));
	}
	else if (_T("oe") == strParam)
	{
		tidySetOutCharEncoding(tdoc, GetEncodeByIndex(nNumValue));
	}
	else if (_T("se") == strParam)
	{
		tidyOptSetInt(tdoc, TidyShowErrors, nNumValue);
	}
	else if (_T("is") == strParam)
	{
		tidyOptSetInt(tdoc, TidyIndentSpaces, nNumValue);
	}
	else if (_T("ts") == strParam)
	{
		tidyOptSetInt(tdoc, TidyTabSize, nNumValue);
	}
	else if (_T("w") == strParam)
	{
		tidyOptSetInt(tdoc, TidyWrapLen, nNumValue);
	}
	else if (_T("at") == strParam)
	{
		if(m_nHtmlXml==TIDY_HTML)
			strNothing = g_TidyInfo[TIDY_HTML_AT].szOption;
		tidyOptSetValue(tdoc, TidyAltText, strNothing.c_str());
	}
	else if (_T("cp") == strParam)
	{
		if(m_nHtmlXml==TIDY_HTML)
			strNothing = g_TidyInfo[TIDY_HTML_CP].szOption;
		tidyOptSetValue(tdoc, TidyCSSPrefix, strNothing.c_str());
	}
	else if (_T("nbt") == strParam)
	{
		if(m_nHtmlXml==TIDY_HTML)
			strNothing = g_TidyInfo[TIDY_HTML_NBT].szOption;
		tidyOptSetValue(tdoc, TidyBlockTags, strNothing.c_str());
	}
	else if (_T("net") == strParam)
	{
		if(m_nHtmlXml==TIDY_HTML)
			strNothing = g_TidyInfo[TIDY_HTML_NET].szOption;
		tidyOptSetValue(tdoc, TidyEmptyTags, strNothing.c_str());
	}
	else if (_T("nit") == strParam)
	{
		if(m_nHtmlXml==TIDY_HTML)
			strNothing = g_TidyInfo[TIDY_HTML_NIT].szOption;
		tidyOptSetValue(tdoc, TidyInlineTags, strNothing.c_str());
	}
	else if (_T("npt") == strParam)
	{
		if(m_nHtmlXml==TIDY_HTML)
			strNothing = g_TidyInfo[TIDY_HTML_NPT].szOption;
		tidyOptSetValue(tdoc, TidyPreTags, strNothing.c_str());
	}
}

LPCSTR HtmlTidy::GetEncodeByIndex(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		return "raw";
	case 1:
		return "ascii";
	case 2:
		return "latin0";
	case 3:
		return "latin1";
	case 4:
		return "utf8";
	case 5:
		return "iso2022";
	case 6:
		return "mac";
	case 7:
		return "win1252";
	case 8:
		return "ibm858";
	case 9:
		return "utf16le";
	case 10:
		return "utf16be";
	case 11:
		return "utf16";
	case 12:
		return "big5";
	case 13:
		return "shiftjis";
	}
	return "raw";
}

void HtmlTidy::InitTidyDefault(TidyDoc tdoc)
{
	tidyOptSetBool(tdoc, TidyShowMarkup, no);
	tidyOptSetBool(tdoc, TidyShowWarnings, no);
	tidyOptSetBool(tdoc, TidyDropEmptyParas, no);
	tidyOptSetBool(tdoc, TidyFixComments, no);
	tidyOptSetBool(tdoc, TidyQuoteNbsp, no);
	tidyOptSetBool(tdoc, TidyQuoteAmpersand, no);
	tidyOptSetBool(tdoc, TidyWrapSection, no);
	tidyOptSetBool(tdoc, TidyWrapAsp, no);
	tidyOptSetBool(tdoc, TidyWrapJste, no);
	tidyOptSetBool(tdoc, TidyWrapPhp, no);
	tidyOptSetBool(tdoc, TidyFixBackslash, no);
	tidyOptSetBool(tdoc, TidyMark, no);
	tidyOptSetBool(tdoc, TidyFixUri, no);
	tidyOptSetBool(tdoc, TidyLowerLiterals, no);
	tidyOptSetBool(tdoc, TidyJoinStyles, no);
	tidyOptSetBool(tdoc, TidyNCR, no);
	tidyOptSetBool(tdoc, TidyAnchorAsName, no);
}
//
//////////////////////////////////////////////////////////////////////////

CTidyHelper::CTidyHelper()
{

}

CTidyHelper::~CTidyHelper()
{

}

BOOL CTidyHelper::DoTidy(UINT nLang, const char *pTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut/* = NULL*/)
{
	switch(nLang)
	{
	case TIDY_CPP:
		return TidyCpp(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_CS:
		return TidyCs(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_JAVA:
		return TidyJava(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_HTML:
		return TidyHtml(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_XML:
		return TidyXml(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_PHP:
		return TidyPhp(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_JAVASCRIPT:
		return TidyJs(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_CSS:
		return TidyCss(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_JSON:
		return TidyJson(pTextIn, pszTextOut, pszMsgOut);
	case TIDY_SQL:
		return TidySql(pTextIn, pszTextOut, pszMsgOut);
	}
	return FALSE;
}

BOOL CTidyHelper::TidyCpp(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_CPP].szOption);
	AsTidy cppTidy;
	BOOL bRet = cppTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyCs(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_CS].szOption);
	strcat(szOption, "--mode=cs");
	AsTidy csTidy;
	BOOL bRet = csTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyJava(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_JAVA].szOption);
	strcat(szOption, "--mode=java");
	AsTidy javaTidy;
	BOOL bRet = javaTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyHtml(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_HTML].szOption);
	HtmlTidy htmlTidy(TIDY_HTML);
	BOOL bRet = htmlTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyXml(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_XML].szOption);
	HtmlTidy xmlTidy(TIDY_XML);
	BOOL bRet = xmlTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyPhp(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_PHP].szOption);
	PhpTidyLib::PhpTidy phpTidy;
	BOOL bRet = phpTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyJs(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_JAVASCRIPT].szOption);
	JsTidyLib::JsTidy jsTidy;
	BOOL bRet = jsTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyCss(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_CSS].szOption);
	CssTidyLib::CssTidy cssTidy;
	BOOL bRet = cssTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidyJson(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_JSON].szOption);
	JsonTidyLib::JsonTidy jsonTidy;
	BOOL bRet = jsonTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}

BOOL CTidyHelper::TidySql(const char *pszTextIn, wchar_t **pszTextOut, wchar_t **pszMsgOut)
{
	string strTextOut, strMsgOut;
	char szOption[1024] = {0};
	strcpy(szOption, g_TidyInfo[TIDY_SQL].szOption);
	SqlTidyLib::SqlTidy sqlTidy;
	BOOL bRet = sqlTidy.Format(pszTextIn, szOption, strTextOut, strMsgOut);
	*pszTextOut = StrUtil::a2w(strTextOut.c_str());
	if(!strMsgOut.empty())
		*pszMsgOut = StrUtil::a2w(strMsgOut.c_str());
	return bRet;
}
