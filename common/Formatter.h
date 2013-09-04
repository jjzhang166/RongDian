#pragma once

#include <vector>

typedef enum _tagTAG_STATE
{
	TAG_BEGIN = 1,		// 开始标签
	TAG_CONTENT = 2,	// 内容
	TAG_END = 3,		// 关闭标签
	TAG_FOURTH = 4		// 未知状态 - ?? - 2013-08-27 - LIng
} TAG_STATE;

class CFormatter
{
public:
	CFormatter();
	virtual ~CFormatter();

	int Format(LPCWSTR lpszText);
	void Reset(); // 重置成员变量
	bool IsMatch(LPCSTR lpszPattern, LPCWSTR lpszText);
	virtual vector<wchar_t*> GetRows() = 0;
	virtual void CalSpaceForRow(LPCWSTR lpszRowText) = 0; // 处理缩进

public:
	char *m_lpszRegex; // 正则表达式规则
	wchar_t *m_lpszText;
	TAG_STATE m_nPrevTagState;
	int m_nSpaceNum;
	vector<wchar_t*> m_vecRows; // 记录格式化后的每行
};
