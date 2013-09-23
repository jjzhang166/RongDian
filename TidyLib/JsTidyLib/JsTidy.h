#pragma once
#include <string>
using namespace std;

namespace JsTidyLib{
class JsTidy
{
public:
	JsTidy(void);
	~JsTidy(void);
	bool Format(const char* pSourceIn, const char *pOptions, string &strOut, string &strErr);
};
}