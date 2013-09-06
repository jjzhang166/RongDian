#pragma once
#include <windows.h>
#include <vector>
using namespace std;

class CNetWorkAdapterUtil
{
public:
	CNetWorkAdapterUtil();
	~CNetWorkAdapterUtil();
	BOOL	GetAdapterName(const int adapterIndex,LPWSTR lpszName);	//根据适配器index获取适配器名字
	BOOL	GetAdapterType(const int adapterIndex,LPWSTR lpszType);	//根据适配器index获取适配器类型
	BOOL	GetDesc(const int adapterIndex,LPWSTR lpszDns);			//根据适配器index获取适配器描述
	BOOL	GetIP(const int adapterIndex,LPWSTR lpszIp[]);			//根据适配器index获取IP列表
	BOOL	GetMask(const int adapterIndex,LPWSTR lpszMask);		//根据适配器index获取子网掩码
	BOOL	GetGateway(const int adapterIndex,LPWSTR lpszGateway);	//根据适配器index获取网关
	BOOL	IsDHCPEnabled(const int adapterIndex);					//根据适配器index获取判断是否开启DHCP
	BOOL	GetDns(const int adapterIndex,LPWSTR lpszDns[]);		//根据适配器index获取对应的dns
	int		CountAdapters();										//适配器数量
	BOOL	GetConnectNames(LPWSTR lpszConnectNames[]);				//获取本机所有的网络连接名称
public:
	int m_adaptersCount;		//适配器数量
	BOOL m_isNewAdapterInfo;
	PIP_ADAPTER_INFO m_pIpadapterInfo;
	vector<LPWSTR> m_connectNames;	//网络连接名称
	vector<PIP_ADAPTER_INFO> m_allAdapters;
}