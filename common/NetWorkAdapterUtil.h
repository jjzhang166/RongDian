#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include "StrUtil.h"
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Mprapi.lib")

using namespace std;
class CNetWorkAdapterUtil
{
public:
	CNetWorkAdapterUtil();
	virtual ~CNetWorkAdapterUtil();
	BOOL	GetAdapterName(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszName); //根据适配器index获取适配器名字
	BOOL	GetAdapterType(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszType,int len);	//根据适配器index获取适配器类型
	BOOL	GetDesc(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszDns,int len);	//根据适配器index获取适配器描述
	int		GetIPs(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszIp[]);			//根据适配器index获取IP列表,返回某一网卡的实际ip数量
	BOOL	GetMask(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszMask,int len);		//根据适配器index获取子网掩码
	BOOL	GetGateway(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszGateway,int len);	//根据适配器index获取网关
	BOOL	IsDHCPEnabled(PIP_ADAPTER_INFO pAdapterInfo);					//根据适配器index获取判断是否开启DHCP
	BOOL	GetDns(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszDns[]);		//根据适配器index获取对应的dns	
private:
	void	CountAdapters();										//适配器数量
	BOOL	GetConnectNames();										//获取本机所有的网络连接名称
public:
	int m_adaptersCount;					//适配器数量
	BOOL m_isNewAdapterInfo;
	PIP_ADAPTER_INFO m_pIpadapterInfo;
	vector<LPWSTR> m_connectNames;			//连接名称
	vector<PIP_ADAPTER_INFO> m_allAdapters;
};