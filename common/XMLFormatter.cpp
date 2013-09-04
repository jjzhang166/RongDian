#include "stdafx.h"
#include "XMLFormatter.h"

#define OVECCOUNT 30
CXMLFormatter::CXMLFormatter()
{
	//匹配标签内容：<[^>]+>
	m_lpszRegex = new char[256];
	assert(m_lpszRegex!=NULL);
	memset(m_lpszRegex, 0, sizeof(char)*256);
	strcpy(m_lpszRegex, "\\s*(<[^>]+>)|\\s*([^<\\r\\n]+)");
}

CXMLFormatter::~CXMLFormatter()
{
}

void CXMLFormatter::CalSpaceForRow(LPCWSTR lpszRowText)
{
	if (this->IsEndTag(lpszRowText))
	{
		m_nPrevTagState = TAG_CONTENT;
		m_nSpaceNum = m_nSpaceNum-2;
	}
	//封闭标签不用缩进
	else if (this->IsClosedTag(lpszRowText))
	{
		m_nPrevTagState = TAG_END;
	}
	//开始标签或标签内容，缩进
	else if (m_nPrevTagState==1)
	{
		m_nPrevTagState = TAG_BEGIN;
		m_nSpaceNum += 2;
	}
	else
	{
		m_nPrevTagState = TAG_BEGIN;
	}
	m_nSpaceNum = m_nSpaceNum<0?0:m_nSpaceNum;
}

vector<wchar_t*> CXMLFormatter::GetRows()
{
	return this->m_vecRows;
}

bool CXMLFormatter::IsBeginTag(LPCWSTR lpszText)
{
	//字母数字或下划线
	return IsMatch("<\\s*\\w+\\s*>", lpszText);
}

bool CXMLFormatter::IsEndTag(LPCWSTR lpszText)
{
	return IsMatch("<\\s*/[^>]+>", lpszText);
}

bool CXMLFormatter::IsClosedTag(LPCWSTR lpszText)
{
	return IsMatch("<[^/]+/\\s*>", lpszText);
}
