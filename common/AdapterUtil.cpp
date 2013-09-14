#include "AdapterUtil.h"
#include <assert.h>
#include <stdio.h>
#include <Mprapi.h>
#pragma comment(lib, "Mprapi.lib")

BOOL AdapterUtil::LoadAdapters(LPFNAdapterInfoCallBack /*lpCallBack*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

BOOL AdapterUtil::GetName(LPFNAdapterNameCallBack lpCallBack, LPVOID lpParam)
{
	HANDLE hMprConfig = NULL;
	DWORD dwRet = 0;
	PIP_INTERFACE_INFO plfTable = NULL;
	DWORD dwSize = 0;
	IP_ADAPTER_INDEX_MAP adaptMap;
	int tchSize = 256;
	wchar_t szName[1024] = {0};
	dwRet = MprConfigServerConnect(NULL, &hMprConfig);
	dwRet = GetInterfaceInfo(NULL, &dwSize);
	if(dwRet == ERROR_INSUFFICIENT_BUFFER)
	{
		plfTable = (PIP_INTERFACE_INFO)malloc(dwSize);
		if(!plfTable)
			return FALSE;
		GetInterfaceInfo(plfTable, &dwSize);
	}
	for (LONG i = 0; i < plfTable->NumAdapters; i++)
	{
		adaptMap = plfTable->Adapter[i];
		dwRet = MprConfigGetFriendlyName(hMprConfig, adaptMap.Name, szName, tchSize);
		if(lpCallBack)
			lpCallBack(lpParam, szName, adaptMap.Index);
		memset(szName, 0, sizeof(szName));
	}
	free(plfTable);
	return TRUE;
}

BOOL AdapterUtil::GetClsid(PIP_ADAPTER_INFO pAdapter, LPSTR lpszClsid)
{
	assert(pAdapter!=NULL);
	if(!lpszClsid)
		return FALSE;
	strcpy(lpszClsid, pAdapter->AdapterName);
	return TRUE;
}

BOOL AdapterUtil::GetType(PIP_ADAPTER_INFO pAdapter, LPSTR lpszType)
{
	assert(pAdapter!=NULL);
	if(!lpszType)
		return FALSE;
	switch(pAdapter->Type)
	{
	case MIB_IF_TYPE_OTHER:
		strcpy(lpszType, "Other");
		break;
	case MIB_IF_TYPE_ETHERNET:
		strcpy(lpszType, "Ethernet");
		break;
	case MIB_IF_TYPE_TOKENRING:
		strcpy(lpszType, "Token Ring");
		break;
	case MIB_IF_TYPE_FDDI:
		strcpy(lpszType, "FDDI");
		break;
	case MIB_IF_TYPE_PPP:
		strcpy(lpszType, "PPP");
		break;
	case MIB_IF_TYPE_LOOPBACK:
		strcpy(lpszType, "Lookback");
		break;
	case MIB_IF_TYPE_SLIP:
		strcpy(lpszType, "Slip");
		break;
	case IF_TYPE_IEEE80211:
		strcpy(lpszType, "IEEE80211");
		break;
	default:
		strcpy(lpszType, "UnKnown");
	}
	return TRUE;
}

BOOL AdapterUtil::GetDesc(PIP_ADAPTER_INFO pAdapter, LPSTR lpszDesc)
{
	assert(pAdapter!=NULL);
	if(!lpszDesc)
		return FALSE;
	strcpy(lpszDesc, pAdapter->Description);
	return TRUE;
}

BOOL AdapterUtil::GetAddr(PIP_ADAPTER_INFO pAdapter, LPSTR lpszAddr, int nSize, int nCount)
{
	assert(pAdapter!=NULL);
	if(!lpszAddr)
		return FALSE;
	IP_ADDR_STRING *pIpAddrString = &(pAdapter->IpAddressList);
	for(int i=0; i<nCount-1 && pIpAddrString; i++)
	{
		strcpy(lpszAddr, pIpAddrString->IpAddress.String);
		lpszAddr += nSize;
		pIpAddrString = pIpAddrString->Next;
	}
	return TRUE;
}

BOOL AdapterUtil::GetMask(PIP_ADAPTER_INFO pAdapter, LPSTR lpszMask)
{
	assert(pAdapter!=NULL);
	if(!lpszMask)
		return FALSE;
	strcpy(lpszMask, pAdapter->IpAddressList.IpMask.String);
	return TRUE;
}

BOOL AdapterUtil::GetGateway(PIP_ADAPTER_INFO pAdapter, LPSTR lpszGateway)
{
	assert(pAdapter!=NULL);
	if(!lpszGateway)
		return FALSE;
	strcpy(lpszGateway, pAdapter->GatewayList.IpAddress.String);
	return TRUE;
}

BOOL AdapterUtil::GetDns(PIP_ADAPTER_INFO pAdapter, LPSTR szDns, int nSize, int nCount)
{
	assert(pAdapter!=NULL);
	if(!szDns)
		return FALSE;
	if(nCount<2)
		return FALSE;
	IP_PER_ADAPTER_INFO* pPerAdapt	= NULL;
	DWORD dwIndex = pAdapter->Index;
	ULONG ulLen = 0;
	DWORD dwRet = GetPerAdapterInfo(dwIndex, pPerAdapt, &ulLen);
	if(dwRet!=ERROR_BUFFER_OVERFLOW)
		return FALSE;
	pPerAdapt = (IP_PER_ADAPTER_INFO *)malloc(ulLen);
	if(!pPerAdapt)
		return FALSE;
	dwRet = GetPerAdapterInfo(dwIndex, pPerAdapt, &ulLen);
	if(dwRet!=ERROR_SUCCESS)
	{
		free(pPerAdapt);
		return FALSE;
	}
	IP_ADDR_STRING* pAddrString = &(pPerAdapt->DnsServerList);
	if(pAddrString && stricmp(pAddrString->IpAddress.String, "")!=0)
	{
		strcpy(szDns, pAddrString->IpAddress.String);
		szDns += nSize;
		pAddrString = pAddrString->Next;
		if(pAddrString)
			strcpy(szDns, pAddrString->IpAddress.String);
	}
	free(pPerAdapt);
	return TRUE;
}

BOOL AdapterUtil::GetMac(PIP_ADAPTER_INFO pAdapter, LPSTR lpszMac)
{
	assert(pAdapter!=NULL);
	if(!lpszMac)
		return FALSE;
	char szAddr[8] = {0};
	sprintf(lpszMac, "%.2X-", (int)pAdapter->Address[0]);
	for(int i = 1; i <(int)pAdapter->AddressLength; ++i)
	{
		if(i == (int)(pAdapter->AddressLength -1))
			sprintf(szAddr, "%.2X", (int)pAdapter->Address[i]);
		else
			sprintf(szAddr, "%.2X-", (int)pAdapter->Address[i]);
		strcat(lpszMac,	szAddr);
		memset(szAddr, 0, sizeof(szAddr));
	}
	return TRUE;
}