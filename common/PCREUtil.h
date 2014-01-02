#pragma once
#include "stdafx.h"
#include <Windows.h>
#include <list>
#include <pcre/pcre.h>

using namespace std;
namespace PCREUtil
{
	bool IsMatch(const char* lpszPattern, const char* lpszText);
	bool find(const char* lpszPattern,char* lpszText,list<char*>* result);
}