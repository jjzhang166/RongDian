#include "NetWorkAdapterUtil.h"

#define IP_LENGTH 129
CNetWorkAdapterUtil::CNetWorkAdapterUtil():m_adaptersCount(0),m_isNewAdapterInfo(FALSE)
{
	m_allAdapters.reserve(5);
	CountAdapters();
	GetConnectNames();
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
	//������
	if (!m_connectNames.empty())
	{
		for (UINT i=0;i<m_connectNames.size();i++)
		{
			delete m_connectNames.at(i);
		}
	}
}



void CNetWorkAdapterUtil::CountAdapters()
{
	BOOL isNewIPInfo = FALSE;
	m_adaptersCount = 0;
	/*******************************************
	*ͨ��Iphlpapi���ȡ������Ϣ�͸���
	********************************************/
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	m_pIpadapterInfo = pIpAdapterInfo;
	ULONG stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);    //������С

	if (ERROR_BUFFER_OVERFLOW == nRel)                      //������������Ҫ�Ŀռ�
	{
		delete pIpAdapterInfo;
		isNewIPInfo = TRUE;
		pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
		m_pIpadapterInfo = pIpAdapterInfo;
		nRel=GetAdaptersInfo(pIpAdapterInfo, &stSize); 
	}
	if (ERROR_SUCCESS == nRel)                          //��ȡ��Ϣ�ɹ�
	{
		while (pIpAdapterInfo)                          //��ȡ��������
		{
			m_allAdapters.push_back(pIpAdapterInfo);
			m_adaptersCount++;
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
}

BOOL CNetWorkAdapterUtil::GetConnectNames()
{
	m_connectNames.clear();
	/*******************************************
	*ͨ��mprapi���ȡ��������
	*��ͨ��index��������Ϣ���������������
	********************************************/
	HANDLE   hMprConfig;                    //������Ϣ�ľ��
	DWORD   dwRet=0;                        //����ֵ
	PIP_INTERFACE_INFO   plfTable = NULL;   //�ӿ���Ϣ��
	DWORD   dwBufferSize=0;                 //�ӿ���Ϣ��ռ��С

	dwRet = MprConfigServerConnect(NULL, &hMprConfig);  //��þ��
	dwRet = GetInterfaceInfo(NULL, &dwBufferSize);      //��ýӿ���Ϣ���С

	if(dwRet == ERROR_INSUFFICIENT_BUFFER)              //��ýӿ���Ϣ
	{ 
		plfTable = (PIP_INTERFACE_INFO)HeapAlloc(GetProcessHeap(), 
			HEAP_ZERO_MEMORY, dwBufferSize); 
		GetInterfaceInfo(plfTable, &dwBufferSize); 
	} 
    
	map<int,LPWSTR> map;
	for (LONG i = 0; i < plfTable->NumAdapters; i++) 
	{ 
		//�ӿ�����
		int tchSize = 256; 
		wchar_t* szConnectName = new wchar_t[tchSize];
		wmemset(szConnectName,0,tchSize);

		IP_ADAPTER_INDEX_MAP adaptMap = plfTable->Adapter[i];   //�ӿ���Ϣ,����adaptMap.Index
		dwRet = MprConfigGetFriendlyName(hMprConfig, adaptMap.Name,szConnectName, tchSize);
		//map[adaptMap.Name] = szConnectName;

		//����������������(key��ֵ��ʾ���)����������˳��һһ��Ӧ
		for(UINT i=0;i<m_allAdapters.size();i++)
		{
			char *an = m_allAdapters.at(i)->AdapterName;
			wchar_t* adapterName = StrUtil::a2w(an);
			wchar_t* isContain = wcsstr(adaptMap.Name,adapterName);//�Ƿ�ͬ������������(adaptMap.Name��adapterName����dev...������)
			delete adapterName;
			if (isContain!=NULL)
			{
				map[i] = szConnectName;
				break;
			}
		}
	} 
	HeapFree(GetProcessHeap(), 0, plfTable);

	//���Ѿ��ź����map��ֵ��m_connectNames
	for (UINT i=0;i<map.size();i++)
	{
		m_connectNames.push_back(map[i]);
	}
	return TRUE;
}

BOOL CNetWorkAdapterUtil::GetAdapterName(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszName)
{
	lpszName = StrUtil::a2w(pAdapterInfo->AdapterName);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetAdapterType(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszType,int len)
{
	wchar_t type[100] = {0};
	switch(pAdapterInfo->Type)
	{
	case MIB_IF_TYPE_OTHER:
		wcscpy_s(type,100,L"Other");
		break;
	case MIB_IF_TYPE_ETHERNET:
		wcscpy_s(type,100,L"Ethernet");
		break;
	case MIB_IF_TYPE_TOKENRING:
		wcscpy_s(type,100,L"Token Ring");
		break;
	case MIB_IF_TYPE_FDDI:
		wcscpy_s(type,100,L"FDDI");
		break;
	case MIB_IF_TYPE_PPP:
		wcscpy_s(type,100,L"PPP");
		break;
	case MIB_IF_TYPE_LOOPBACK:
		wcscpy_s(type,100,L"Loopback");
		break;
	case MIB_IF_TYPE_SLIP:
		wcscpy_s(type,100,L"SLIP");
		break;
	case IF_TYPE_IEEE80211:
		wcscpy_s(type,100,L"IEEE 802.11");
		break;
	default:
		break;
	}
	wcscpy_s(lpszType,len,type);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetDesc(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszDesc,int len)
{
	wchar_t* desc = StrUtil::a2w(pAdapterInfo->Description);
	wcscpy_s(lpszDesc,len,desc);
	delete[] desc;
	return TRUE;
}
int CNetWorkAdapterUtil::GetIPs(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszIp[])
{
	IP_ADDR_STRING *pIpAddrString = &(pAdapterInfo->IpAddressList);
	int i=0;
	do 
	{
		lpszIp[i] = StrUtil::a2w(pIpAddrString->IpAddress.String);
		pIpAddrString=pIpAddrString->Next;
		i++;
	} while (pIpAddrString);
	return i;
}
BOOL CNetWorkAdapterUtil::GetMask(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszMask,int len)
{
	IP_ADDR_STRING *pIpAddrString =&(pAdapterInfo->IpAddressList);
	wchar_t* mask = StrUtil::a2w(pIpAddrString->IpMask.String);
	wcscpy_s(lpszMask,len,mask);
	delete[] mask;
	return TRUE;
}
BOOL CNetWorkAdapterUtil::GetGateway(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszGateway,int len)
{
	wchar_t* gateway = StrUtil::a2w(pAdapterInfo->GatewayList.IpAddress.String);
	wcscpy_s(lpszGateway,len,gateway);
	return TRUE;
}
BOOL CNetWorkAdapterUtil::IsDHCPEnabled(PIP_ADAPTER_INFO pAdapterInfo)
{
	return pAdapterInfo->DhcpEnabled;
}
BOOL CNetWorkAdapterUtil::GetDns(PIP_ADAPTER_INFO pAdapterInfo,LPWSTR lpszDns[])
{
	DWORD adapterIndex = pAdapterInfo->Index;
	//DNS,pIpAdapterInfoΪ������������Ϣ�ṹPIP_ADAPTER_INFO
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
				LPWSTR dns = StrUtil::a2w(pNext->IpAddress.String);//��dns
				lpszDns[0] = dns;
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

BOOL  CNetWorkAdapterUtil::GetPhysicalAddress(PIP_ADAPTER_INFO pAdapterInf,LPWSTR lpszMac,int len)
{
	for(unsigned int i = 0; i < pAdapterInf->AddressLength; ++i){
		if(i == (pAdapterInf->AddressLength -1 ) ){ // ���һ�����뻻��
			//printf("%.2X\n", (int)pAdapterInf->Address[i]);
			wchar_t mac[3] = {0};
			swprintf(mac,3,L"%.2X",(int)pAdapterInf->Address[i]);
			wcscat_s(lpszMac,len,mac);
		}
		else
		{
			//printf("%.2X-", (int)pAdapterInf->Address[i]);
			wchar_t mac[4] = {0};
			swprintf(mac,4,L"%.2X-",(int)pAdapterInf->Address[i]);
			wcscat_s(lpszMac,len,mac);
		}
	}
	return TRUE;
}