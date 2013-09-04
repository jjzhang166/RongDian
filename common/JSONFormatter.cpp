#include "stdafx.h"
#include "JSONFormatter.h"

#define OVECCOUNT 30
CJSONFormatter::CJSONFormatter()
{
	//①处理开始标签{、[，②处理内容 ③处理结束标签
	m_lpszRegex = new char[256];
	assert(m_lpszRegex!=NULL);
	memset(m_lpszRegex, 0, sizeof(char)*256);
	strcpy(m_lpszRegex, "([\\[\\{])\\s*|([^\\{\\[\\]\\},]+,*)\\s*|([\\]\\}]\\s*,*)\\s*");
}

CJSONFormatter::~CJSONFormatter()
{
}

void CJSONFormatter::CalSpaceForRow(LPCWSTR lpszRowText)
{
	if(IsBeginTag(lpszRowText))
	{
		if(m_nPrevTagState==1)
			m_nSpaceNum += 2;
		m_nPrevTagState = TAG_BEGIN;
	}
	else if(IsObjectEndTag(lpszRowText))
	{
		if(m_nPrevTagState==3)
			m_nSpaceNum -= 2;
		m_nSpaceNum -= 2;
		m_nPrevTagState = TAG_CONTENT;
	}
	else if(IsArrayEndTag(lpszRowText))
	{
		if(m_nPrevTagState==1)
			m_nSpaceNum += 2;
		else if(m_nPrevTagState==2||m_nPrevTagState==3)
			m_nSpaceNum -= 2;
		m_nPrevTagState = TAG_END;
	}
	else if(m_nPrevTagState==1) // 内容（上一行为开始，则缩进）
	{
		m_nSpaceNum += 2;
		m_nPrevTagState = TAG_FOURTH;
	}
	else //内容
	{
		m_nPrevTagState = TAG_FOURTH;
	}
	m_nSpaceNum = m_nSpaceNum<0 ? 0 : m_nSpaceNum;
}

vector<wchar_t*> CJSONFormatter::GetRows()
{
	return this->m_vecRows;
}

bool CJSONFormatter::IsBeginTag(LPCWSTR lpszText)
{
	return IsMatch("[^\\{\\[]*[\\{\\[]\\s*", lpszText);
}

bool CJSONFormatter::IsObjectEndTag(LPCWSTR lpszText)
{
	return IsMatch("\\}\\s*,{0,1}", lpszText);
}

bool CJSONFormatter::IsArrayEndTag(LPCWSTR lpszText)
{
	return IsMatch("[^\\]]*\\]\\s*,{0,1}", lpszText);
}
