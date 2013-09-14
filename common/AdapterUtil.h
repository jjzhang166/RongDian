#pragma once
#include <windows.h>
#include <Iphlpapi.h>

typedef LONG (*LPFNAdapterInfoCallBack)(WPARAM wParam, LPARAM lParam);
typedef LONG (*LPFNAdapterNameCallBack)(LPVOID lParam, LPCWSTR lpszName, int nIndex);

namespace AdapterUtil
{
BOOL LoadAdapters(LPFNAdapterInfoCallBack lpCallBack, WPARAM wParam, LPARAM lParam);
BOOL GetName(LPFNAdapterNameCallBack lpCallBack, LPVOID lpParam);
BOOL GetClsid(PIP_ADAPTER_INFO pAdapter, LPSTR lpszClsid);
BOOL GetType(PIP_ADAPTER_INFO pAdapter, LPSTR lpszType);
BOOL GetDesc(PIP_ADAPTER_INFO pAdapter, LPSTR lpszDesc);
BOOL GetAddr(PIP_ADAPTER_INFO pAdapter, LPSTR lpszAddr, int nSize, int nCount);
BOOL GetMask(PIP_ADAPTER_INFO pAdapter, LPSTR lpszMask);
BOOL GetGateway(PIP_ADAPTER_INFO pAdapter, LPSTR lpszGateway);
BOOL GetDns(PIP_ADAPTER_INFO pAdapter, LPSTR szDns, int nSize, int nCount);
BOOL GetMac(PIP_ADAPTER_INFO pAdapter, LPSTR lpszMac);
}