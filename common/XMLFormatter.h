#pragma once
#include "Formatter.h"
/***
分析xml，转成vector<wchar_t*>(Format方法)
例如：
<note><to>George</to><from>John</from><body>Don't forget the meeting!</body></note>
转成：
1：<note>
2：<to>
3：George
4：</to>
5：<from>
6：John
7：</from>
8：<body>
9：Don't forget the meeting!
10：</body>
11：</note>
***/
class CXMLFormatter : public CFormatter
{
public:
	CXMLFormatter();
	~CXMLFormatter();

	vector<wchar_t*> GetRows();
	void CalSpaceForRow(LPCWSTR lpszRowText);
	/**
	判断是否开始标签如：<note>、<to>。。。
	*/
	bool IsBeginTag(LPCWSTR lpszText);
	/**
	判断是否结束标签如：</note>、</to>。。。
	*/
	bool IsEndTag(LPCWSTR lpszText);
	/**
	判断是否封闭标签如：<note name=""/>
	*/
	bool IsClosedTag(LPCWSTR lpszText);
};
