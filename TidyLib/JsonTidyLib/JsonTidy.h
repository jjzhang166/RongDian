#pragma once
#include <string>
using namespace std;

namespace JsonTidyLib{
class JsonTidy
{
public:
	JsonTidy();
	~JsonTidy();
	bool Format(const char* pSourceIn, const char *pOptions, string &strOut, string &strErr);
};
}