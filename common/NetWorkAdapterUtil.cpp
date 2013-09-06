#include "NetWorkAdapterUtil.h"
#include "StrUtil.h"
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Mprapi.lib")

#define IP_LENGTH 129

CNetWorkAdapterUtil::CNetWorkAdapterUtil():m_adaptersCount(0):m_isNewAdapterInfo(FALSE)
{
	m_pIpadapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(m_pIpadapterInfo,&stSize);
	int netCardNum = 0;
	int IPnumPerNetCard = 0;
	BOOL isNewIPInfo = FALSE;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete m_pIpadapterInfo;
		m_pIpadapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel=GetAdaptersInfo(m_pIpadapterInfo,&stSize); 
		isNewIPInfo=TRUE;
	}
	//所有的适配器
	m_allAdapters = new vector();
	//所有的连接名称
	m_connectNames = new vector();
	CountAdapters();
}

CNetWorkAdapterUtil::~CNetWorkAdapterUtil()
{
	
	if (m_pIpadapterInfo&&m_isNewAdapterInfo)
	{
		delete[] m_pIpadapterInfo;
	}
	else if(m_pIpadapterInfo)
	{
		delete m_pIpadapterInfo;
	}
	//连接名
	if (!m_connectNames.empty())
	{
		for (UINT i=0;i<m_connectNames.size();i++)
		{
			delete m_connectNames.at(i);
		}
	}
	delete m_connectNames;
	//适配器
	if (!m_allAdapters.empty())
	{
		for (UINT i=0;i<m_allAdapters.size();i++)
		{
			delete m_allAdapters.at(i);
		}
	}
	delete m_allAdapters;
}

BOOL CNetWorkAdapterUtil::GetDns(const int adapterIndex,LPWSTR lpszDns[])
{
	//DNS,pIpAdapterInfo为网卡适配器信息结构PIP_ADAPTER_INFO
	IP_PER_ADAPTER_INFO* pPerAdapt	= NULL;
	ULONG ulLen = 0;
	int err = GetPerAdapterInfo(adapterIndex, pPerAdapt, &ulLen);
	if( err == ERROR_BUFFER_OVERFLOW ) {
		pPerAdapt = ( IP_PER_ADAPTER_INFO* ) HeapAlloc(GetProcessHeap(), 
			HEAP_ZERO_MEMORY, ulLen);
		err = GetPerAdapterInfo(adapterIndex, pPerAdapt, &ulLen );
		if( err == ERROR_SUCCESS ) {
			IP_ADDR_STRING* pNext = &( pPerAdapt->DnsServerList );
			if (pNext && strcmp(pNext->IpAddress.String, "") != 0)
			{
				LPWSTR dns = StrUtil::a2w(pNext->IpAddress.String);//主dns
				lpszDns[0]=dns;
				if (pNext = pNext->Next)
				{
					LPWSTR dns = StrUtil::a2w(pNext->IpAddress.String);
					lpszDns[1]=dns;
				}
					
			}
		}
		HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pPerAdapt);
	}	
	return TRUE;
}

int CNetWorkAdapterUtil::CountAdapters()
{
	m_adaptersCount = 0;
	/*******************************************
	*通过Iphlpapi库获取网卡信息和个数
	********************************************/
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	ULONG stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);    //获得其大小

	if (ERROR_BUFFER_OVERFLOW == nRel)                      //重新申请所需要的空间
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		nRel=GetAdaptersInfo(pIpAdapterInfo, &stSize); 
	}
	if (ERROR_SUCCESS == nRel)                          //获取信息成功
	{
		while (pIpAdapterInfo)                          //获取网卡个数
		{
			m_allAdapters.push_back(pIpAdapterInfo);
			m_adaptersCount++;
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	return count;
}

BOOL CNetWorkAdapterUtil::GetConnectNames(LPWSTR lpszConnectNames[])
{
	m_connectNames.clear();
	/*******************************************
	*通过mprapi库获取连接名称
	*并通过index将网卡信息和连接名称相关联
	********************************************/
	HANDLE   hMprConfig;                    //连接信息的句柄
	DWORD   dwRet=0;                        //返回值
	PIP_INTERFACE_INFO   plfTable = NULL;   //接口信息表
	DWORD   dwBufferSize=0;                 //接口信息表空间大小

	dwRet = MprConfigServerConnect(NULL, &hMprConfig);  //获得句柄
	dwRet = GetInterfaceInfo(NULL, &dwBufferSize);      //获得接口信息表大小

	if(dwRet == ERROR_INSUFFICIENT_BUFFER)              //获得接口信息
	{ 
		plfTable = (PIP_INTERFACE_INFO)HeapAlloc(GetProcessHeap(), 
			HEAP_ZERO_MEMORY, dwBufferSize); 
		GetInterfaceInfo(plfTable, &dwBufferSize); 
	} 

	wchar_t  szConnectName = new wchar_t[256];                   //接口名称
	DWORD   tchSize = sizeof(wchar_t)*256; 
	ZeroMemory(&szConnectName, tchSize);  
	
	for (UINT i = 0; i < plfTable->NumAdapters; i++) 
	{ 
		IP_ADAPTER_INDEX_MAP adaptMap;         //接口信息,索引adaptMap.Index
		adaptMap = plfTable->Adapter[i]; 
		dwRet = MprConfigGetFriendlyName(hMprConfig, adaptMap.Name,(wchar_t)szConnectName, tchSize);
		m_connectNames.push_back(szConnectName);
	} 
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, plfTable);
	return TRUE;
}

BOOL CNetWorkAdapterUtil::GetAdapterName(const int adapterIndex,LPWSTR lpszName)
{
	PIP_ADAPTER_INFO pAdapterInfo = m_allAdapters.at(adapterIndex);
	lpszName = StrUtil::a2w(pAdapterInfo->AdapterName);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetAdapterType(const int adapterIndex,LPWSTR lpszType)
{
	PIP_ADAPTER_INFO pAdapterInfo = m_allAdapters.at(adapterIndex);
	wchar_t type[20] = {0};
	switch(pAdapterInfo->Type)
	{
	case MIB_IF_TYPE_OTHER:
		type = L"Other";
		break;
	case MIB_IF_TYPE_ETHERNET:
		type = L"Ethernet";
		break;
	case MIB_IF_TYPE_TOKENRING:
		type = L"Token Ring";
		break;
	case MIB_IF_TYPE_FDDI:
		type = L"FDDI";
		break;
	case MIB_IF_TYPE_PPP:
		type = L"PPP";
		break;
	case MIB_IF_TYPE_LOOPBACK:
		type = L"Loopback";
		break;
	case MIB_IF_TYPE_SLIP:
		type = L"SLIP";
		break;
	default:
		break;
	}
	wcscpy_s(lpszType,sizeof(lpszType),type);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetDesc(const int adapterIndex,LPWSTR lpszDesc)
{
	PIP_ADAPTER_INFO pAdapterInfo = m_allAdapters.at(adapterIndex);
	lpszDesc = StrUtil::a2w(pAdapterInfo->Description);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetIP(const int adapterIndex,LPWSTR lpszIp[])
{
	PIP_ADAPTER_INFO pAdapterInfo = m_allAdapters.at(adapterIndex);
	IP_ADDR_STRING *pIpAddrString =&(pAdapterInfo->IpAddressList);
	int i=0;
	do 
	{
		lpszIp[i] = StrUtil::a2w(pIpAddrString->IpAddress.String);
		pIpAddrString=pIpAddrString->Next;
	} while (pIpAddrString);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetMask(const int adapterIndex,LPWSTR lpszMask)
{
	PIP_ADAPTER_INFO pAdapterInfo = m_allAdapters.at(adapterIndex);
	IP_ADDR_STRING *pIpAddrString =&(pAdapterInfo->IpAddressList);
	lpszMask = StrUtil::a2w(pIpAddrString->IpMask.String);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetGateway(const int adapterIndex,LPWSTR lpszGateway)
{
	PIP_ADAPTER_INFO pAdapterInfo = m_allAdapters.at(adapterIndex);
	lpszGateway = StrUtil::a2w(pAdapterInfo->GatewayList.IpAddress.String);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::IsDHCPEnabled(const int adapterIndex)
{
	PIP_ADAPTER_INFO pAdapterInfo = m_allAdapters.at(adapterIndex);
	return pAdapterInfo->DhcpEnabled;
}