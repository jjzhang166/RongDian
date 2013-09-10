#pragma once
#include <string>
using namespace std;

namespace PhpTidyLib{
class PhpTidy
{
public:
	PhpTidy();
	~PhpTidy();
	bool Format(const char* pSourceIn, const char *pOptions, string &strOut, string &strErr);
};
}