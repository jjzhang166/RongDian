#include <windows.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cassert>
using namespace std;

#include <JsonTidyLib/json.h>
#ifdef _DEBUG
#pragma comment(lib, "../../RongDian/JsonTidyLibd.lib")
#else
#pragma comment(lib, "../../RongDian/JsonTidyLib.lib")
#endif

int main()
{
	/*ifstream ifs;
	ifs.open("testjson.json");
	assert(ifs.is_open());*/
	string json = "{ \"version_name\":\"1.0.0.1\", \"version_code\":2 }";
	JsonTidyLib::Reader reader;
	JsonTidyLib::Value root;
	if (!reader.parse(json,root))
	{
		return -1;
	}
	//string name = root.get("name", "UTF-8" ).asString();
	string name = root["version_name"].asString();
	char cc[50] = {0};
	strcpy_s(cc,50,name.c_str());
	int age = root["version_code"].asInt();
	std::cout<<name<<std::endl;
	std::cout<<age<<std::endl;
	return 0;

}