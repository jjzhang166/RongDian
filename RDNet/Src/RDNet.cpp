#include <Windows.h>
#include <string>

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wldap32.lib")

#define CURL_STATICLIB
#define BUILDING_LIBCURL
#define HTTP_ONLY

#include <curl/curl.h>

#define NET_DLL

#include <RDNet.h>

// Link Lib
#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/curld.lib")
#else
#pragma comment(lib, "../RongDian/bin/curl.lib")
#endif

const int		MAX_RESPONE_SIZE = 4096;

HINSTANCE g_hInst = NULL;
volatile LONG g_lNetTasks = 0; // 维护当前正在执行的任务数，防止退出时有任务正在执行，导致curl未能正常退出，导致内存泄露

static size_t CheckVersionCallBack(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	PRESPONE_INFO pszInfo = (PRESPONE_INFO)userdata;
	unsigned long ulSize = size * nmemb;
	if(!pszInfo || pszInfo->ulSeek<0 || pszInfo->ulSeek>pszInfo->ulSize) // 用户数据异常
	{
		if(pszInfo)
			pszInfo->nRet = -1;
		return 0;
	}
	if(pszInfo->ulSize<ulSize) // 用户缓冲区过小
	{
		pszInfo->nRet = -1;
		return 0;
	}
	if(pszInfo->ulSize<(pszInfo->ulSeek+ulSize)) // 用户剩余缓冲区不足
	{
		pszInfo->nRet = -1;
		return 0;
	}
	memcpy(&pszInfo->pszData[pszInfo->ulSeek], ptr, ulSize);
	pszInfo->nRet = 0;
	pszInfo->ulSeek += ulSize;
	return ulSize;
}

LONG __cdecl RDCheckVersion(HWND /*hInvoker*/, const char* pszUrl, char* pszRespone, unsigned long* pnSize)
{
	if(!pszUrl)
		return -1;
	InterlockedIncrement(&g_lNetTasks);
	char szData[MAX_RESPONE_SIZE] = {0};
	RESPONE_INFO stResponeInfo;
	stResponeInfo.pszData = szData;
	stResponeInfo.ulSeek = 0;
	stResponeInfo.ulSize = MAX_RESPONE_SIZE;
	stResponeInfo.nRet = 0;
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, pszUrl);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CheckVersionCallBack);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stResponeInfo);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if(pnSize)
		*pnSize = stResponeInfo.ulSeek;
	if(pszRespone)
		memcpy(pszRespone, stResponeInfo.pszData, stResponeInfo.ulSeek);
	InterlockedDecrement(&g_lNetTasks);
	return 0;
}

LONG __cdecl RDDownLoad(HWND hInvoker, const char* pszUrl, char* pszSavePath, int nMsg)
{
	InterlockedIncrement(&g_lNetTasks);
	InterlockedDecrement(&g_lNetTasks);
	return 0;
}

LONG __cdecl RDTaskCount()
{
	return g_lNetTasks;
}

BOOL WINAPI DllMain(HMODULE hModule, ULONG dwReason, LPVOID /*lpReserved*/)
{
	if(dwReason==DLL_PROCESS_ATTACH)
	{
		g_hInst = (HINSTANCE)hModule;
	}
	return TRUE;
}