#pragma once
#include <windows.h>
/************************************************************************/
/*
 *每隔4分钟发送http心跳到服务端，以维持在线状态；
 *生命周期为整个应用程序的生命周期，无需析构
 */                
/************************************************************************/
class COnlineKeeper
{
public:
	static COnlineKeeper* 	GetInstance();
	void					Reset();							//重置接收数组内容及长度
	BOOL					SetRequest(HWND hWnd,char* url,BOOL needHttpHeader);
	void					RebuildContent(size_t new_len);		//重新设置http响应内容的接收数组（根据长度来扩容数组）
private:
	COnlineKeeper();
	virtual ~COnlineKeeper();
	BOOL					ParseResponseStr(char* str);
	static size_t WINAPI 	SendHttpRequest(void* lpParam);
	static size_t			GetHttpResponse(char *ptr, size_t size, size_t nmemb, void *userdata);
public:
	HWND 	m_hWnd;
	char* 	m_url;
	char* 	m_szContent;
	BOOL  	m_needHttpHeader;
	size_t 	m_countOfContent;		//接收数据的数组总长度，可变的
	char* 	m_szVersionName;		//服务端返回的版本名字
	int 	m_versionId;			//服务端返回的版本id
	BOOL	m_newVersionAlarm;		//是否已经提醒过有新版本
private:
	static COnlineKeeper* 	s_instance;
};