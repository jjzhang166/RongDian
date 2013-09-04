#include <Windows.h>
#include "RDCoder.h"
#include "md5.h"

int APIENTRY wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	std::string strSrc = "Œ“ «À≠";
	std::string strDigU = MD5Str(strSrc, true);
	std::string strDigL = MD5Str(strSrc);
	return 0;
}