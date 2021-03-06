#include "jsontidy.h"
#include "json.h"

namespace JsonTidyLib{
JsonTidy::JsonTidy()
{

}

JsonTidy::~JsonTidy()
{

}

bool JsonTidy::Format(const char* pSourceIn, const char *pOptions, string &strOut, string &strErr)
{
	string strOptions = pOptions;
	int nChPerInd = 3;
	string::size_type nPos = strOptions.find("-cn");
	if (string::npos != nPos)
	{
		string strIN = "";
		for (string::size_type i = nPos + 3; i < strOptions.size(); ++i)
		{
			if (!isdigit(strOptions[i]))
			{
				break;
			}
			strIN.append(1, strOptions[i]);
		}
		nChPerInd = atoi(strIN.c_str());
	}

	Reader jReader;
	Value jValue;
	bool parsingSuccessful = jReader.parse(pSourceIn, jValue);
	if (!parsingSuccessful)
	{
		strErr = jReader.getFormattedErrorMessages();
		return false;
	} 

	strOut = jValue.toMyStyledString(nChPerInd);
	return true;
}
}
