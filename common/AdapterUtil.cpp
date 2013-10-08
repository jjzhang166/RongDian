#include "AdapterUtil.h"
#include <assert.h>
#include <stdio.h>
#include <Mprapi.h>
#pragma comment(lib, "Mprapi.lib")
#pragma comment(lib, "Iphlpapi.lib")

#pragma comment(lib, "Wbemuuid.lib")
#pragma comment(lib, "Credui.lib")
#ifdef _DEBUG
#pragma comment(lib, "comsuppwd.lib")
#else
#pragma comment(lib, "comsuppw.lib")
#endif

//////////////////////////////////////////////////////////////////////////
// AdapterUtil
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
// AdapterUtil
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CWbemObject
CWbemObject::CWbemObject(IWbemClassObject *pIWbem /*= NULL*/)
{
	m_pClassObject = pIWbem;
}

CWbemObject::~CWbemObject()
{
	Clean();
}

void CWbemObject::Attatch(IWbemClassObject* pWbemObj)
{
	if(m_pClassObject)
		m_pClassObject->Release();
	m_pClassObject = pWbemObj;
}

IWbemClassObject* CWbemObject::Detach()
{
	IWbemClassObject* pWbemObj = m_pClassObject;
	m_pClassObject = NULL;
	return pWbemObj;
}

CWbemObject::operator IWbemClassObject* ()
{
	return m_pClassObject;
}

IWbemClassObject* CWbemObject::SpawnInstanceHandle()
{
	assert(m_pClassObject==NULL);
	IWbemClassObject *pObj = NULL;
	m_pClassObject->SpawnInstance(0, &pObj);
	return pObj;
}

IWbemClassObject* CWbemObject::GetIWbemObject()
{
	return m_pClassObject;
}

HRESULT CWbemObject::SpwanInstanceObject(CWbemObject& wbemObj)
{
	HRESULT hRes = S_FALSE;
	assert(m_pClassObject!=NULL);
	IWbemClassObject* pWbemObj = NULL;
	hRes = m_pClassObject->SpawnInstance(0, &pWbemObj);
	wbemObj.Attatch(pWbemObj);
	return hRes;
}

HRESULT CWbemObject::GetClassPath(LPWSTR lpszPath)
{
	assert(m_pClassObject!=NULL);
	assert(lpszPath!=NULL);
	VARIANT pVal;
	HRESULT hRes = m_pClassObject->Get(L"__Path", 0, &pVal, 0, 0);
	if(SUCCEEDED(hRes))
	{
		assert(pVal.vt==VT_BSTR);
		wcscpy(lpszPath, pVal.bstrVal);
	}
	return hRes;
}

HRESULT CWbemObject::Get(LPCWSTR lpszName, VARIANT *pVal, LONG *plFlavor /*= NULL*/)
{
	assert(m_pClassObject!=NULL);
	assert(lpszName!=NULL);
	assert(pVal!=NULL);
	return m_pClassObject->Get(lpszName, 0, pVal, NULL, plFlavor);
}

HRESULT CWbemObject::Put(LPCWSTR lpszName, VARIANT *pVal, CIMTYPE vtType /*= 0*/)
{
	assert(m_pClassObject!=NULL);
	assert(lpszName!=NULL);
	return m_pClassObject->Put(lpszName, 0, pVal, vtType);
}

HRESULT CWbemObject::BeginEnumeration(LONG lEnumFlags /*= WBEM_FLAG_KEYS_ONLY*/)
{
	assert(m_pClassObject!=NULL);
	return m_pClassObject->BeginEnumeration(lEnumFlags);
}

HRESULT CWbemObject::Next(LPCWSTR lpszName, VARIANT* pVal, CIMTYPE* pvtType /*= NULL*/, LONG* plFlavor /*= NULL*/)
{
	assert(m_pClassObject!=NULL);
	assert(lpszName!=NULL);
	assert(pvtType!=NULL);
	return m_pClassObject->Next(00, (BSTR *)lpszName, pVal, pvtType, plFlavor);
}

HRESULT CWbemObject::GetInMethod(CWbemObject& wbemObj, IWbemServices *pServices, LPCWSTR lpszMethod)
{
	HRESULT hRes = S_FALSE;
	assert(m_pClassObject!=NULL);
	assert(pServices!=NULL);
	IWbemClassObject* pWbemObj = NULL;
	IWbemClassObject* pMethodObj = NULL;
	VARIANT vtClassName = {0};
	hRes = Get(L"__Class", &vtClassName);
	if(FAILED(hRes))
		return hRes;
	hRes = pServices->GetObject(vtClassName.bstrVal, 0, NULL, &pWbemObj, NULL);
	if(FAILED(hRes))
		return hRes;
	hRes = pWbemObj->GetMethod(lpszMethod, 0, &pMethodObj, NULL);
	pWbemObj->Release();
	wbemObj.Attatch(pMethodObj);
	return hRes;
}

HRESULT CWbemObject::GetOutMethod(CWbemObject& wbemObj, IWbemServices *pServices, LPCWSTR lpszMethod)
{
	HRESULT hRes = S_FALSE;
	assert(m_pClassObject!=NULL);
	assert(pServices!=NULL);
	IWbemClassObject* pWbemObj = NULL;
	IWbemClassObject* pMethodObj = NULL;
	VARIANT vtClassName = {0};
	hRes = Get(L"__Class", &vtClassName);
	if(FAILED(hRes))
		return hRes;
	hRes = pServices->GetObject(vtClassName.bstrVal, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pWbemObj, NULL);
	if(FAILED(hRes))
		return hRes;
	hRes = pWbemObj->GetMethod(lpszMethod, 0, NULL, &pMethodObj);
	pWbemObj->Release();
	wbemObj.Attatch(pMethodObj);
	return hRes;
}

HRESULT CWbemObject::ExecInMethod(
					 IWbemServices* pServices,
					 LPCWSTR lpszMethod,
					 CWbemObject* pWbemIn /*= NULL*/,
					 CWbemObject* pWbemOut /*= NULL*/,
					 IWbemCallResult** ppResult /*= NULL*/,
					 IWbemContext *pContext /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	assert(pServices!=NULL);
	assert(lpszMethod!=NULL);
	wchar_t szPath[1024] = {0};
	wchar_t szMethod[1024] = {0};
	wcscpy(szMethod, lpszMethod);
	hRes = GetClassPath(szPath);
	if(FAILED(hRes))
		return hRes;
	IWbemClassObject* pWbemObj = NULL;
	if(pWbemIn!=NULL)
		pWbemObj = pWbemIn->GetIWbemObject();
	if(pWbemOut!=NULL)
	{
		IWbemClassObject* pWbemTemp = NULL;
		hRes = pServices->ExecMethod(szPath, szMethod, 0, pContext, pWbemObj, &pWbemTemp, ppResult);
		pWbemOut->Attatch(pWbemTemp);
		return hRes;
	}
	hRes = pServices->ExecMethod(szPath, szMethod, 0, pContext, pWbemObj, NULL, ppResult);
	return hRes;
}

void CWbemObject::Clean()
{
	if(m_pClassObject)
	{
		m_pClassObject->Release();
		m_pClassObject = NULL;
	}
}
// CWbemObject
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CWbemUtil
CWbemUtil::CWbemUtil()
{

}

CWbemUtil::CWbemUtil(IWbemClassObject* pWbemObj)
	: CWbemObject(pWbemObj)
{

}

CWbemUtil::~CWbemUtil()
{

}

BOOL CWbemUtil::GetBSTR(LPCWSTR lpszName, bstr_t &bstrValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && vtValue.vt==VT_BSTR)
	{
		//bstrValue = SysAllocString(vtValue.bstrVal);
		bstrValue = vtValue;
		return TRUE;
	}
	return FALSE;
}

BOOL CWbemUtil::GetLONG(LPCWSTR lpszName, LONG &lValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_I4 == vtValue.vt))
	{
		lValue = V_I4(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	lValue = 0;
	return FALSE;
}

BOOL CWbemUtil::GetULONG(LPCWSTR lpszName, ULONG &ulValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_UI4 == vtValue.vt))
	{
		ulValue = V_UI4(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	ulValue = 0;
	return FALSE;
}

BOOL CWbemUtil::GetUINT(LPCWSTR lpszName, UINT &uiValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_UINT == vtValue.vt))
	{
		uiValue = V_UINT(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	uiValue = 0;
	return FALSE;
}

BOOL CWbemUtil::GetUINT64(LPCWSTR lpszName, UINT64 &ullValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_UI8 == vtValue.vt))
	{
		ullValue = V_UI8(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	ullValue = 0;
	return FALSE;
}

BOOL CWbemUtil::GetUSHORT(LPCWSTR lpszName, USHORT &ubValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_UI2 == vtValue.vt))
	{
		ubValue = V_UI2(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	ubValue = 0;
	return FALSE;
}

BOOL CWbemUtil::GetSHORT(LPCWSTR lpszName, SHORT &bValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_I2 == vtValue.vt))
	{
		bValue = V_I2(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	bValue = 0;
	return FALSE;
}

BOOL CWbemUtil::GetBOOL(LPCWSTR lpszName, BOOL &bValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_BOOL == vtValue.vt))
	{
		bValue = V_BOOL(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	bValue = 0;
	return FALSE;
}

BOOL CWbemUtil::GetDATE(LPCWSTR lpszName, DOUBLE& dValue)
{
	assert(lpszName!=NULL);
	VARIANT vtValue;
	HRESULT hRes = Get(lpszName, &vtValue);
	if(SUCCEEDED(hRes) && (VT_DATE == vtValue.vt))
	{
		dValue = V_DATE(&vtValue);
		return TRUE;
	}
	assert(vtValue.vt == VT_NULL);
	dValue = 0;
	return FALSE;
}
// CWbemUtil
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CAdapterUtil
CAdapterUtil::CAdapterUtil()
{
	m_pServices = NULL;
	m_pEnumObject = NULL;
}

CAdapterUtil::~CAdapterUtil()
{
	ClearEnum();
	Disconnect();
}

IWbemServices* CAdapterUtil::GetServices()
{
	return m_pServices;
}

HRESULT CAdapterUtil::Init()
{
	//return ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	return S_OK;
}

void CAdapterUtil::UnInit()
{
	//CoUninitialize();
}

HRESULT CAdapterUtil::ConnectLocal(LPCWSTR lpszRes)
{
	HRESULT hRes = S_FALSE;
	assert(m_pServices==NULL);
	// Set General COM Security Levels
	hRes = CoInitializeSecurity(
		NULL,							
		-1, // COM Authentication
		NULL, // Authentication Services
		NULL, // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT, // Default Authentication
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
		NULL, // Authentication Info
		EOAC_NONE, // Additional Capabilities
		NULL); // Reserved
	if(FAILED(hRes) && hRes!=RPC_E_TOO_LATE) 
		return hRes;
	// Obtain the Initial Localtor to WMI
	IWbemLocator *pLocator = NULL;
	hRes = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID *)&pLocator);
	if(FAILED(hRes))
		return hRes;
	// Connect to WMI through the IWbemLocator::ConnectServer method
	wchar_t szRes[1024] = {0};
	wcscpy(szRes, lpszRes);
	hRes = pLocator->ConnectServer(
		szRes, // Object Path of WMI namespace
		NULL, // User name, NULL = Current User
		NULL, // User Password, NULL = Current User
		0, // Locale, NULL indicates Current User
		NULL, // Security flags.
		0, // Authority
		0, // Context Object
		&m_pServices); // Pointer to IWbemServices Proxy
	pLocator->Release();
	pLocator = NULL;
	if(FAILED(hRes))
		return hRes;
	// Set Security Levels on the Proxy
	hRes = CoSetProxyBlanket(
		m_pServices, // Indicates the proxy to set
		RPC_C_AUTHN_WINNT, // RPC_C_AUTHN_XXX
		RPC_C_AUTHZ_NONE, // RPC_C_AUTHZ_XXX
		NULL, // Server Principal Name
		RPC_C_AUTHN_LEVEL_CALL, // RPC_C_AUTHN_LEVEL_XXX
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_XXX
		NULL, // Client identity
		EOAC_NONE); // Proxy Capabilities
	if(FAILED(hRes))
		m_pServices->Release();

	return hRes;
}

HRESULT CAdapterUtil::ConnectRemote(
									LPCWSTR lpszHost, 
									LPCWSTR lpszRes, 
									LPCWSTR lpszUser, 
									LPCWSTR lpszPwd,
									LPCWSTR lpszLocale /*= L"MS_409"*/, 
									LPCWSTR lpszAuthority /*= L"ntlmdomain:domain"*/, 
									BOOL bShowDlg /*= FALSE*/)
{
	HRESULT hRes = S_FALSE;
	assert(m_pServices!=NULL);
	// Set General COM Security Levels
	hRes = CoInitializeSecurity(
		NULL,
		-1, // COM Authentication
		NULL, // Authentication Services
		NULL, // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT, // Default Authentication
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
		NULL, // Authentication Info
		EOAC_NONE, // Additional Capabilities
		NULL); // Reserved
	if(FAILED(hRes))
		return hRes;
	// Obtain the initial Locator to WMI
	IWbemLocator *pLocator = NULL;
	hRes = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(LPVOID *)&pLocator);
	if(FAILED(hRes))
		return hRes;
	// Connect to WMI through the IWbemLocator::ConnectServer method
	CREDUI_INFO cui;
	wchar_t szName[CREDUI_MAX_USERNAME_LENGTH+1] = {0};
	wchar_t szPwd[CREDUI_MAX_USERNAME_LENGTH+1] = {0};
	BOOL bSave = FALSE;
	DWORD dwErr = 0;
	cui.cbSize = sizeof(CREDUI_INFO);
	cui.hwndParent = NULL;
	cui.hbmBanner = NULL;
	cui.pszMessageText = L"Remote Computer Account Information";
	cui.pszCaptionText = L"Enter Account Information";
	if(lpszUser)
		wcscpy(szName, lpszUser);
	if(lpszPwd)
		wcscpy(szPwd, lpszPwd);
	if(bShowDlg)
	{
		dwErr = CredUIPromptForCredentials(
			&cui,
			L"",
			NULL,
			0,
			szName,
			_countof(szName),
			szPwd,
			_countof(szPwd),
			&bSave,
			CREDUI_FLAGS_GENERIC_CREDENTIALS | CREDUI_FLAGS_ALWAYS_SHOW_UI | CREDUI_FLAGS_DO_NOT_PERSIST);
		if(dwErr)
		{
			pLocator->Release();
			return hRes;
		}
	}
	wchar_t szRes[2048], szLocale[1024] = {0}, szAuthority[1024] = {0};
	swprintf(szRes, L"\\\\%s\\%s", lpszHost, lpszRes);
	wcscpy(szLocale, lpszLocale);
	wcscpy(szAuthority, lpszAuthority);
	hRes = pLocator->ConnectServer(
		szRes,
		szName,
		szPwd,
		szLocale,
		NULL,
		szAuthority,
		NULL,
		&m_pServices);
	pLocator->Release();
	SecureZeroMemory(szName, sizeof(szName));
	SecureZeroMemory(szPwd, sizeof(szPwd));
	if(FAILED(hRes))
		return hRes;
	hRes = CoSetProxyBlanket(
		m_pServices,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE);
	if(FAILED(hRes))
		pLocator->Release();
	return hRes;
}

void CAdapterUtil::Disconnect()
{
	if(m_pServices)
		m_pServices->Release();
	m_pServices = NULL;
}

HRESULT CAdapterUtil::EnumExecQuery(
									LPCWSTR lpszQuery, 
									const LONG& lFlags /*= WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY*/, 
									IWbemContext* pContext /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	assert(lpszQuery!=NULL);
	assert(m_pServices!=NULL);
	ClearEnum();
	wchar_t szQuery[1024] = {0};
	wcscpy(szQuery, lpszQuery);
	hRes = m_pServices->ExecQuery(L"WQL", szQuery, lFlags, pContext, &m_pEnumObject);
	return hRes;
}

HRESULT CAdapterUtil::GetClassObject(
									 CWbemObject& wbemObj, 
									 LPCWSTR lpszPath, 
									 const LONG& lFlags /*= 0*/, 
									 IWbemContext* pContext /*= NULL*/, 
									 IWbemCallResult** lppResult /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	assert(lpszPath!=NULL);
	assert(m_pServices!=NULL);
	IWbemClassObject* pWbemObj = NULL;
	wchar_t szPath[1024] = {0};
	wcscpy(szPath, lpszPath);
	hRes = m_pServices->GetObject(szPath, lFlags, pContext, &pWbemObj, lppResult);
	wbemObj.Attatch(pWbemObj);
	return hRes;
}

HRESULT CAdapterUtil::NexClassObject(
									 CWbemObject& wbemObj, 
									 const LONG& lTimeOut /*= WBEM_INFINITE*/, 
									 const ULONG& uCount /*= 1*/)
{
	HRESULT hRes = S_FALSE;
	assert(m_pEnumObject!=NULL);
	assert(m_pServices!=NULL);
	IWbemClassObject *pWbemObj = NULL;
	ULONG uRet = 0;
	hRes = m_pEnumObject->Next(lTimeOut, uCount, &pWbemObj, &uRet);
	if(uRet==0)
		return -1;
	wbemObj.Attatch(pWbemObj);
	return hRes;
}

BOOL CAdapterUtil::IsConnect()
{
	if(!m_pServices)
		return FALSE;
	return TRUE;
}

HRESULT CAdapterUtil::ResetEnum()
{
	HRESULT hRes = S_FALSE;
	assert(m_pEnumObject!=NULL);
	assert(m_pServices!=NULL);
	hRes = m_pEnumObject->Reset();
	return hRes;
}

void CAdapterUtil::ClearEnum()
{
	if(m_pEnumObject)
		m_pEnumObject->Release();
	m_pEnumObject = NULL;
}

HRESULT CAdapterUtil::ExecMethod(
								 LPCWSTR lpszPath, 
								 LPCWSTR lpszMethod, 
								 IWbemContext* pContext /*= NULL*/, 
								 IWbemClassObject* pWbemIn /*= NULL*/, 
								 IWbemClassObject** ppWbemOut /*= NULL*/, 
								 IWbemCallResult** ppCallResult /*= NULL*/)
{
	assert(lpszPath!=NULL);
	assert(lpszMethod!=NULL);
	assert(m_pServices!=NULL);
	wchar_t szPath[1024] = {0}, szMethod[1024] = {0};
	wcscpy(szPath, lpszPath);
	wcscpy(szMethod, lpszMethod);
	return m_pServices->ExecMethod(szPath, szMethod, 0, pContext, pWbemIn, ppWbemOut, ppCallResult);
}

HRESULT CAdapterUtil::GetClassHandle(
									 IWbemClassObject** ppWbemObject, 
									 LPCWSTR lpszPath, 
									 const LONG& lFalgs /*= 0*/,
									 IWbemContext* pContext /*= NULL*/,
									 IWbemCallResult** ppCallResult /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	assert(m_pEnumObject!=NULL);
	assert(m_pServices!=NULL);
	assert(ppWbemObject!=NULL);
	IWbemClassObject* pWbemObj = NULL;
	wchar_t szPath[1024] = {0};
	wcscpy(szPath, lpszPath);
	hRes = m_pServices->GetObject(szPath, lFalgs, pContext, &pWbemObj, ppCallResult);
	*ppWbemObject = pWbemObj;
	return hRes;
}

HRESULT CAdapterUtil::NextClassHandle(
									  IWbemClassObject* ppWbemObject, 
									  const LONG& lTimeOut /*= WBEM_INFINITE*/,
									  const ULONG& uCount /*= 1*/)
{
	HRESULT hRes = S_FALSE;
	assert(m_pEnumObject!=NULL);
	assert(m_pServices!=NULL);
	assert(ppWbemObject!=NULL);
	IWbemClassObject *pWbemObj = NULL;
	ULONG uRet = 0;
	hRes = m_pEnumObject->Next(lTimeOut, uCount, &pWbemObj, &uRet);
	if(uRet==0)
		return hRes;
	ppWbemObject = pWbemObj;
	return hRes;
}
// CAdapterUtil
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CAdapterCfgObj
CAdapterCfgObj::CAdapterCfgObj(IWbemServices *pServices /*= NULL*/)
{
	m_pServices = pServices;
	m_pClassObject = NULL;
}

CAdapterCfgObj::CAdapterCfgObj(IWbemClassObject *pWbemObject, IWbemServices *pServices)
{
	m_pServices = pServices;
	m_pClassObject = pWbemObject;
}

CAdapterCfgObj::~CAdapterCfgObj()
{

}

/*************************************************************************
 * Method:    		SetAddr
 * Description:		设置IP地址、子网掩码
 * ParameterList:	LPCWSTR lpszAddr, LPCWSTR lpszMask, LONG *plRet
 * Parameter:       lpszAddr:IP地址字符串
 * Parameter:       lpszMask:子网掩码字符串
 * Parameter:       plRet:返回值，成功返回0，失败返回相应错误值(WBEMSTATUS枚举类型，在WbemCli.h文件定义)
 * Return Value:	HRESULT
 * Date:        	2013:10:06 22:38:14
 * Author:			LIng
 * CopyRight:		Copyright (C) 2013
 ************************************************************************/
HRESULT CAdapterCfgObj::SetAddr(LPCWSTR lpszAddr, LPCWSTR lpszMask, LONG *plRet /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	CAdapterCfgObj adaMethod;
	CAdapterCfgObj adaParam;
	wchar_t szMethod[] = L"EnableStatic";
	hRes = GetInMethod(adaMethod, m_pServices, szMethod);
	if(FAILED(hRes))
		return hRes;
	hRes = adaMethod.SpwanInstanceObject(adaParam);
	if(FAILED(hRes))
		return hRes;
	VARIANT vtAddr = {0}, vtMask = {0};
	SAFEARRAYBOUND saBoundAddr = {0}, saBoundMask = {0};
	SAFEARRAY *psaAddr = NULL, *psaMask = NULL;
	saBoundAddr.cElements = 1;
	saBoundAddr.lLbound = 0;
	psaAddr = SafeArrayCreate(VT_BSTR, 1, &saBoundAddr);
	if(!psaAddr)
		goto cleanup;
	((BSTR*)psaAddr->pvData)[0] = SysAllocString(lpszAddr);
	vtAddr.vt = VT_BSTR | VT_ARRAY;
	vtAddr.parray = psaAddr;
	hRes = adaParam.Put(L"IPAddress", &vtAddr, 0);
	if(FAILED(hRes))
		goto cleanup;

	saBoundMask.cElements = 1;
	saBoundMask.lLbound = 0;
	psaMask = SafeArrayCreate(VT_BSTR, 1, &saBoundMask);
	if(!psaMask)
		goto cleanup;
	((BSTR*)psaMask->pvData)[0] = SysAllocString(lpszMask);
	vtMask.vt = VT_BSTR | VT_ARRAY;
	vtMask.parray = psaMask;
	hRes = adaParam.Put(L"SubnetMask", &vtMask, 0);
	if(FAILED(hRes))
		goto cleanup;
	if(plRet==NULL)
	{
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam);
	}
	else
	{
		CAdapterCfgObj adaRet;
		*plRet = 64;
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam, &adaRet);
		adaRet.GetLONG(L"ReturnValue", *plRet);
		if(*plRet!=S_OK)
			hRes = *plRet;
	}
	
cleanup:
	if(psaAddr)
	{
		SafeArrayUnaccessData(psaAddr);
		SafeArrayDestroy(psaAddr);
	}
	if(psaMask)
	{
		SafeArrayUnaccessData(psaMask);
		SafeArrayDestroy(psaMask);
	}
	return hRes;
}

/*************************************************************************
 * Method:    		SetGateway
 * Description:		设置网关
 * ParameterList:	LPCWSTR lpszGateway, LONG *plRet
 * Parameter:       lpszGateway:网关字符串
 * Parameter:       plRet:返回值，成功返回0，失败返回相应错误值(WBEMSTATUS枚举类型，在WbemCli.h文件定义)
 * Return Value:	HRESULT
 * Date:        	2013:10:06 22:37:03
 * Author:			LIng
 * CopyRight:		Copyright (C) 2013
 ************************************************************************/
HRESULT CAdapterCfgObj::SetGateway(LPCWSTR lpszGateway, LONG *plRet /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	CAdapterCfgObj adaMethod;
	CAdapterCfgObj adaParam;
	wchar_t szMethod[] = L"SetGateways";
	hRes = GetInMethod(adaMethod, m_pServices, szMethod);
	if(FAILED(hRes))
		return hRes;
	hRes = adaMethod.SpwanInstanceObject(adaParam);
	if(FAILED(hRes))
		return hRes;
	VARIANT vtGateway = {0}, vtMetric = {0};
	SAFEARRAYBOUND saGateway = {0}, saMetric = {0};
	SAFEARRAY *psaGateway = NULL, *psaMetric = NULL;
	saGateway.cElements = 1;
	saGateway.lLbound = 0;
	psaGateway = SafeArrayCreate(VT_BSTR, 1, &saGateway);
	if(!psaGateway)
		goto cleanup;
	((BSTR*)psaGateway->pvData)[0] = SysAllocString(lpszGateway);
	vtGateway.vt = VT_BSTR | VT_ARRAY;
	vtGateway.parray = psaGateway;
	hRes = adaParam.Put(L"DefaultIPGateway", &vtGateway, 0);
	if(FAILED(hRes))
		goto cleanup;

	BYTE bMetric[1] = {1};
	saMetric.cElements = 1;
	saMetric.lLbound = 0;
	psaMetric = SafeArrayCreate(VT_UI1, 1, &saMetric);
	if(!psaMetric)
		goto cleanup;
	((BYTE*)psaMetric->pvData)[0] = bMetric[0];
	vtMetric.vt = VT_UI1 | VT_ARRAY;
	vtMetric.parray = psaMetric;
	hRes = adaParam.Put(L"GatewayCostMetric", &vtMetric, 0);
	if(FAILED(hRes))
		goto cleanup;
	if(plRet==NULL)
	{
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam);
	}
	else
	{
		CAdapterCfgObj adaRet;
		*plRet = 64;
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam, &adaRet);
		adaRet.GetLONG(L"ReturnValue", *plRet);
		if(*plRet!=S_OK)
			hRes = *plRet;
	}

cleanup:
	if(psaGateway)
	{
		SafeArrayUnaccessData(psaGateway);
		SafeArrayDestroy(psaGateway);
	}
	if(psaMetric)
	{
		SafeArrayUnaccessData(psaMetric);
		SafeArrayDestroy(psaMetric);
	}
	return hRes;
}

/*************************************************************************
 * Method:    		SetDns
 * Description:		设置DNS
 * ParameterList:	LPBYTE lpszDns, int nCount, LONG *plRet
 * Parameter:       lpszDns:dns字符串数组，字符串数组长度为260，使用前需转换为LPCWSTR，通过指针偏移(MAX_PATH*sizeof(wchar_t)*i)访问字符串数组元素
 * Parameter:       nCount:DNS个数
 * Parameter:       plRet:返回值，成功返回0，失败返回相应错误值(WBEMSTATUS枚举类型，在WbemCli.h文件定义)
 * Return Value:	HRESULT
 * Date:        	2013:10:06 22:32:16
 * Author:			LIng
 * CopyRight:		Copyright (C) 2013
 ************************************************************************/
HRESULT CAdapterCfgObj::SetDns(LPBYTE lpszDns, int nCount, LONG *plRet /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	CAdapterCfgObj adaMethod;
	CAdapterCfgObj adaParam;
	wchar_t szMethod[] = L"SetDNSServerSearchOrder";
	hRes = GetInMethod(adaMethod, m_pServices, szMethod);
	if(FAILED(hRes))
		return hRes;
	hRes = adaMethod.SpwanInstanceObject(adaParam);
	if(FAILED(hRes))
		return hRes;
	VARIANT vtDns = {0};
	SAFEARRAYBOUND saBoundDns = {0};
	SAFEARRAY *psaDns = NULL;
	LPBYTE pDns = lpszDns;
	if(pDns==NULL)
	{
		vtDns.vt = VT_NULL;
	}
	else
	{
		saBoundDns.cElements = nCount;
		saBoundDns.lLbound = 0;
		psaDns = SafeArrayCreate(VT_BSTR, 1, &saBoundDns);
		if(!psaDns)
			goto cleanup;
		for(int i=0; i<nCount; i++)
		{
			((BSTR*)psaDns->pvData)[i] = SysAllocString((LPCWSTR)pDns);
			pDns+=(MAX_PATH*sizeof(wchar_t));
		}
		vtDns.vt = VT_BSTR | VT_ARRAY;
		vtDns.parray = psaDns;
	}
	hRes = adaParam.Put(L"DNSServerSearchOrder", &vtDns, 0);
	if(FAILED(hRes))
		goto cleanup;
	if(plRet==NULL)
	{
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam);
	}
	else
	{
		CAdapterCfgObj adaRet;
		*plRet = 64;
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam, &adaRet);
		adaRet.GetLONG(L"ReturnValue", *plRet);
		if(*plRet!=S_OK)
			hRes = *plRet;
	}
cleanup:
	if(psaDns)
	{
		SafeArrayUnaccessData(psaDns);
		SafeArrayDestroy(psaDns);
	}
	return hRes;
}

/*************************************************************************
 * Method:    		EnableDHCP
 * Description:		自动获取IP、DNS模式
 * ParameterList:	LONG plRet
 * Parameter:       plRet:返回值，成功返回0，失败返回相应错误值(WBEMSTATUS枚举类型，在WbemCli.h文件定义)
 * Return Value:	HRESULT
 * Date:        	2013:10:06 22:30:45
 * Author:			LIng
 * CopyRight:		Copyright (C) 2013
 ************************************************************************/
HRESULT CAdapterCfgObj::EnableDHCP(LONG *plRet /*= NULL*/)
{
	HRESULT hRes = S_FALSE;
	CAdapterCfgObj adaMethod;
	CAdapterCfgObj adaParam;
	wchar_t szMethod[] = L"EnableDHCP";
	hRes = SetDns(NULL, 0, plRet);
	if(FAILED(hRes))
		return hRes;
	hRes = GetInMethod(adaMethod, m_pServices, szMethod);
	if(FAILED(hRes))
		return hRes;
	if(plRet==NULL)
	{
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam);
	}
	else
	{
		CAdapterCfgObj adaRet;
		*plRet = 64;
		hRes = ExecInMethod(m_pServices, szMethod, &adaParam, &adaRet);
		adaRet.GetLONG(L"ReturnValue", *plRet);
		if(*plRet!=S_OK)
			hRes = *plRet;
	}

	return hRes;
}
// CAdapterCfgObj
//////////////////////////////////////////////////////////////////////////