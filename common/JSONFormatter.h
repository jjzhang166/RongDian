#pragma once
#include "formatter.h"

class CJSONFormatter : public CFormatter
{
public:
	CJSONFormatter();
	~CJSONFormatter();

	void CalSpaceForRow(LPCWSTR lpszRowText);
	vector<wchar_t*> GetRows();
	/**
	判断是否开始标签
	*/
	bool IsBeginTag(LPCWSTR lpszText);
	/**
	判断是否对象结束标签
	*/
	bool IsObjectEndTag(LPCWSTR lpszText);
	bool IsArrayEndTag(LPCWSTR lpszText);
};
