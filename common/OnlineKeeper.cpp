#include "OnlineKeeper.h"
#include <string>
#include <process.h>
#include <libcurl/curl/curl.h>
#include <jsoncpp/json/json.h>
using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "../RongDian/bin/libcurld.lib")
#pragma comment(lib, "../RongDian/bin/jsoncppd.lib")
#else
#pragma comment(lib, "../RongDian/bin/libcurl.lib")
#pragma comment(lib, "../RongDian/bin/jsoncpp.lib")
#endif

#define WM_VERSION_NOTIFY WM_USER+113

COnlineKeeper* 	COnlineKeeper::s_instance = NULL;
const size_t 	INI_ARR_LEN = 256;
COnlineKeeper::COnlineKeeper():m_countOfContent(INI_ARR_LEN),m_newVersionAlarm(FALSE)
{
	m_url = NULL;
	m_szContent = new char[INI_ARR_LEN];
	memset(m_szContent,0,sizeof(m_szContent));
	m_szVersionName = new char[INI_ARR_LEN];
	memset(m_szVersionName,0,sizeof(m_szVersionName));
	m_versionId = 0;
	curl_global_init(CURL_GLOBAL_WIN32);
}

COnlineKeeper::~COnlineKeeper()
{
	curl_global_cleanup();
	if (NULL!=m_szContent)
	{
		delete[] m_szContent;
	}
	if (NULL!=m_url)
	{
		delete[] m_url;
	}
}

void COnlineKeeper::Reset()
{
	if (NULL!=m_szContent)
	{
		delete[] m_szContent;
	}
	m_szContent = new char[INI_ARR_LEN];
	memset(m_szContent,0,sizeof(m_szContent));
	m_countOfContent = INI_ARR_LEN;
}

COnlineKeeper* COnlineKeeper::GetInstance()
{
	if (NULL==COnlineKeeper::s_instance)
	{
		COnlineKeeper::s_instance = new COnlineKeeper();
	}
	return COnlineKeeper::s_instance;
}

void COnlineKeeper::RebuildContent(size_t new_len)
{
	m_countOfContent = new_len;
	char* tmpContent = new char[new_len];
	memset(tmpContent,0,new_len);
	strcpy_s(tmpContent,new_len,m_szContent);
	delete[] m_szContent;
	m_szContent = tmpContent;
}

BOOL COnlineKeeper::SetRequest(HWND hWnd,char* url,BOOL needHeader)
{
	m_hWnd = hWnd;
	m_url = url;
	if (NULL==m_url)
	{
		int url_len = strlen(url)+1;
		m_url = new char[url_len];
		memset(m_url,0,url_len);
		strcpy_s(m_url,url_len,url);
	}

	m_needHttpHeader = needHeader;

	unsigned threadID;
	_beginthreadex(NULL, 0, &COnlineKeeper::SendHttpRequest, (LPVOID)this, 0, &threadID);
	return TRUE;
}

size_t WINAPI COnlineKeeper::SendHttpRequest(void* lpParam)
{
	COnlineKeeper* onlineKeeper = (COnlineKeeper*)lpParam;
	CURL* curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, onlineKeeper->m_url); 		//设置下载的URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        			//设置超时
    //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        			//屏蔽其它信号
    if (onlineKeeper->m_needHttpHeader)
    {
    	 curl_easy_setopt(curl, CURLOPT_HEADER, 1);          		//下载数据包括HTTP头部
    }
   
    //curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);				//每次执行完请求，连接就会马上关闭
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetHttpResponse); //设置下载数据的回调函数
    while(TRUE)
    {
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, onlineKeeper->m_szContent);
    	curl_easy_perform(curl);
    	if (!onlineKeeper->m_newVersionAlarm&&!onlineKeeper->m_newVersionAlarm)	//如果已经提醒过，则不再提醒
    	{
    		onlineKeeper->ParseResponseStr(onlineKeeper->m_szContent);
			PostMessage(onlineKeeper->m_hWnd,WM_VERSION_NOTIFY,NULL,NULL);
    	}
    	Sleep(4*60*1000);			//4分钟一次
    	onlineKeeper->Reset();
    }
    curl_easy_cleanup(curl);

	return 0;
}

size_t COnlineKeeper::GetHttpResponse(char *ptr, size_t size, size_t nmemb, void* /*userdata*/)
{
	BOOL rebuildArray = FALSE;	
	size_t old_arrayLen = COnlineKeeper::GetInstance()->m_countOfContent;
	size_t old_strLen = strlen(COnlineKeeper::GetInstance()->m_szContent); //已接收到的数据长度
	size_t min_len = nmemb+old_strLen+1;

	//计算追加新的字符串所需要的数组长度
	while(old_arrayLen < min_len)
	{
		old_arrayLen = old_arrayLen*2;
		rebuildArray = TRUE;
	}

	if (rebuildArray)
	{
		COnlineKeeper::GetInstance()->RebuildContent(old_arrayLen);
	}

	strcat_s(COnlineKeeper::GetInstance()->m_szContent,old_arrayLen,ptr);
	//memcpy(COnlineKeeper::GetInstance()->m_szContent+old_strLen,ptr,size*nmemb);//追加
    return size*nmemb;
}

BOOL COnlineKeeper::ParseResponseStr(char* str)
{
	Json::Reader reader;
	Json::Value root;
	string json = str;
	if (reader.parse(json, root)&&root["version_id"].type()!=Json::nullValue)
	{
		string versioinName = root["version_name"].asString();
		m_versionId = root["version_id"].asInt();
		strcpy_s(m_szVersionName,INI_ARR_LEN,versioinName.c_str());
	}
	return TRUE;
}

