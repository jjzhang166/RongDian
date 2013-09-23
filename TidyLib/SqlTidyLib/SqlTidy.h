#pragma once
#include <string>
using namespace std;

namespace SqlTidyLib{
class SqlTidy
{
public:
	SqlTidy();
	~SqlTidy();
	bool Format(const char* pSourceIn, const char *pOptions, string &strOut, string &strErr);
};
}