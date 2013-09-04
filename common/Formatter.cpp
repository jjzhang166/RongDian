#include "stdafx.h"
#include "Formatter.h"

#define OVECCOUNT 30 /* should be a multiple of 3 */   

CFormatter::CFormatter() : 
	m_lpszRegex(NULL),
	m_nSpaceNum(0),
	m_nPrevTagState(TAG_BEGIN),
	m_lpszText(NULL)
{
	Reset();
}

CFormatter::~CFormatter()
{
	for(unsigned int i=0;i<m_vecRows.size();i++)
		free(m_vecRows.at(i));
	m_vecRows.clear();
	if(m_lpszRegex)
		delete[] m_lpszRegex;
	if(m_lpszText)
		delete[] m_lpszText;
}

int CFormatter::Format(LPCWSTR lpszText)
{
	if (!lpszText)
		return -1;

	Reset();

	m_lpszText = new wchar_t[wcslen(lpszText)+1];
	if(!m_lpszText)
		return -1;
	wcscpy(m_lpszText, lpszText);
	
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc;

	re = pcre_compile(m_lpszRegex, PCRE_MULTILINE, &error, &erroffset, NULL);
	if (re == NULL) //编译失败，返回错误信息
	{
		//printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return -1;
	}
	char *lpszText_a = StrUtil::w2a(lpszText);
	UINT cur = 0;
	while(cur<strlen(lpszText_a) && (rc = pcre_exec(re, NULL, lpszText_a, strlen(lpszText_a), cur, PCRE_NOTEMPTY, ovector, OVECCOUNT)) >= 0)
	{
		for(int j=1; j<rc; j++)
		{
			int substring_length = ovector[2*j+1] - ovector[2*j];//ansi类型的字符串长度
			if (substring_length>0)
			{
				char *substring_start = lpszText_a + ovector[2*j];

				int subLen = substring_length+1;
				char *substring = (char*)new char[sizeof(char)*subLen];
				memset(substring, 0, sizeof(char)*subLen);
				memcpy_s(substring, subLen, substring_start, substring_length);
				StrUtil::strtrim(substring, strlen(substring));

				wchar_t *tmpStr = StrUtil::a2w(substring);

				//*******************  处理缩进  *******************
				this->CalSpaceForRow(tmpStr);
				//*******************  处理缩进  end*******************

				size_t rowLen = wcslen(tmpStr)+m_nSpaceNum*2+1;
				wchar_t *row = (wchar_t*)malloc(sizeof(wchar_t)*rowLen);
				wmemset(row,0,rowLen);
				
				for (int j=0;j<m_nSpaceNum;j++)
				{
					wcscat_s(row,rowLen,L"  ");
				}
				wcscat_s(row,rowLen,tmpStr);
				//存储每行的文本到vector中
				m_vecRows.push_back(row);
				delete[] substring;
				delete[] tmpStr;
			}
			
		}
		cur = ovector[1];
	}
	pcre_free(re); // 编译正则表达式re释放内存
	return 0;
}

void CFormatter::Reset()
{
	if(m_lpszText)
		delete m_lpszText;
	m_lpszText = NULL;
	m_nPrevTagState = TAG_BEGIN;
	m_nSpaceNum = -2;
	for (UINT i=0; i<m_vecRows.size(); i++)
		free(m_vecRows.at(i));
	m_vecRows.clear();
}

bool CFormatter::IsMatch(LPCSTR lpszPattern, LPCWSTR lpszText)
{
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc;
	pcre *re = pcre_compile(lpszPattern, 0, &error, &erroffset, NULL);
	if (re == NULL)
		return false;
	
	char *lpszText_a = StrUtil::w2a(lpszText);
	rc = pcre_exec(re, NULL, lpszText_a, strlen(lpszText_a), 0, 0, ovector, OVECCOUNT);
	delete[] lpszText_a;
	
	return (rc>=0);
}