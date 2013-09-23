#pragma once
#include <string>
using namespace std;

namespace JsonTibLib{
class JsonTidy
{
public:
	JsonTidy();
	~JsonTidy();
	bool Format(const char* pSourceIn, const char *pOptions, string &strOut, string &strErr);
};
}