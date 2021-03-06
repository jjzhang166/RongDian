#pragma once
#include <WinSock2.h>  
#include <WS2tcpip.h>
#include <windows.h>
#include <Iphlpapi.h>

#include <Wbemidl.h>
#include <WinCred.h>
#include <comutil.h>

#include <assert.h>

typedef LONG (CALLBACK *LPFNAdapterInfoCallBack)(WPARAM wParam, LPARAM lParam);
typedef LONG (CALLBACK *LPFNAdapterFriendlyNameAndStateCallBack)(LPVOID lParam, LPCWSTR lpszName, BOOL bConnect, int nIndex);

namespace AdapterUtil
{
BOOL LoadAdapters(LPFNAdapterInfoCallBack lpCallBack, WPARAM wParam, LPARAM lParam);
BOOL GetFriendlyName(LPFNAdapterFriendlyNameAndStateCallBack lpCallBack, LPVOID lpParam);
BOOL GetClsid(PIP_ADAPTER_INFO pAdapter, LPSTR lpszClsid);
BOOL GetType(PIP_ADAPTER_INFO pAdapter, LPSTR lpszType);
BOOL GetDesc(PIP_ADAPTER_INFO pAdapter, LPSTR lpszDesc);
BOOL GetAddr(PIP_ADAPTER_INFO pAdapter, LPSTR lpszAddr, int nSize, int nCount);
BOOL GetMask(PIP_ADAPTER_INFO pAdapter, LPSTR lpszMask);
BOOL GetGateway(PIP_ADAPTER_INFO pAdapter, LPSTR lpszGateway);
BOOL GetDns(PIP_ADAPTER_INFO pAdapter, LPSTR szDns, int nSize, int nCount);
BOOL GetMac(PIP_ADAPTER_INFO pAdapter, LPSTR lpszMac);
}

class CWbemObject
{
public:
	CWbemObject(IWbemClassObject *pIWbem = NULL);
	virtual ~CWbemObject();

	void	Attatch(IWbemClassObject* pWbemObj);
	IWbemClassObject* Detach();
	operator IWbemClassObject* ();

	IWbemClassObject* SpawnInstanceHandle();
	IWbemClassObject* GetIWbemObject();
	HRESULT SpwanInstanceObject(CWbemObject& wbemObj);
	HRESULT GetClassPath(LPWSTR lpszPath);

	HRESULT Get(LPCWSTR lpszName, VARIANT *pVal, LONG *plFlavor = NULL);
	HRESULT Put(LPCWSTR lpszName, VARIANT *pVal, CIMTYPE vtType = 0);
	HRESULT BeginEnumeration(LONG lEnumFlags = WBEM_FLAG_KEYS_ONLY);
	HRESULT Next(LPCWSTR lpszName, VARIANT* pVal, CIMTYPE* pvtType = NULL, LONG* plFlavor = NULL);
	HRESULT GetInMethod(CWbemObject& wbemObj, IWbemServices *pServices, LPCWSTR lpszMethod);
	HRESULT GetOutMethod(CWbemObject& wbemObj, IWbemServices *pServices, LPCWSTR lpszMethod);
	HRESULT ExecInMethod(
				IWbemServices* pServices,
				LPCWSTR lpszMethod,
				CWbemObject* pWbemIn = NULL,
				CWbemObject* pWbemOut = NULL,
				IWbemCallResult** ppResult = NULL,
				IWbemContext *pContext = NULL);
	void Clean();

private:
	CWbemObject(const CWbemObject& /*obj*/) {};
	CWbemObject& operator=(const CWbemObject& /*obj*/) {};

protected:
	IWbemClassObject* m_pClassObject;
};

class CWbemUtil : public CWbemObject
{
public:
	CWbemUtil();
	CWbemUtil(IWbemClassObject* pWbemObj);
	virtual ~CWbemUtil();

public:
	BOOL GetBSTR(LPCWSTR lpszName, bstr_t &bstrValue);
	BOOL GetLONG(LPCWSTR lpszName, LONG &lValue);
	BOOL GetULONG(LPCWSTR lpszName, ULONG &ulValue);
	BOOL GetUINT(LPCWSTR lpszName, UINT &uiValue);
	BOOL GetUINT64(LPCWSTR lpszName, UINT64 &ullValue);
	BOOL GetUSHORT(LPCWSTR lpszName, USHORT &ubValue);
	BOOL GetSHORT(LPCWSTR lpszName, SHORT &bValue);
	BOOL GetBOOL(LPCWSTR lpszName, BOOL &bValue);
	BOOL GetDATE(LPCWSTR lpszName, DOUBLE& dValue);
};

class CAdapterCfgObj : public CWbemUtil
{
public:
	CAdapterCfgObj(IWbemServices *pServices = NULL);
	CAdapterCfgObj(IWbemClassObject *pWbemObject, IWbemServices *pServices);
	~CAdapterCfgObj();

	// 设置IP地址、子网掩码
	HRESULT SetAddr(LPCWSTR lpszAddr, LPCWSTR lpszMask, LONG *plRet = NULL);
	// 设置网关
	HRESULT SetGateway(LPCWSTR lpszGateway, LONG *plRet = NULL);
	// 设置DNS
	HRESULT SetDns(LPBYTE lpszDns, int nCount, LONG *plRet = NULL);
	// 自动获取IP、DNS模式
	HRESULT EnableDHCP(LONG *plRet = NULL);
	
private:
	IWbemServices *m_pServices;
};

class CAdapterUtil
{
public:
	CAdapterUtil();
	virtual ~CAdapterUtil();
	IWbemServices* GetServices();

	HRESULT Init();
	void	UnInit();
	HRESULT ConnectLocal(LPCWSTR lpszRes);
	HRESULT ConnectRemote(
				LPCWSTR lpszHost, 
				LPCWSTR lpszRes, 
				LPCWSTR lpszUser, 
				LPCWSTR lpszPwd,
				LPCWSTR lpszLocale = L"MS_409", 
				LPCWSTR lpszAuthority = L"ntlmdomain:domain", 
				BOOL bShowDlg = FALSE);
	void	Disconnect();
	HRESULT EnumExecQuery(
				LPCWSTR lpszQuery, 
				const LONG& lFlags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
				IWbemContext* pContext = NULL);
	HRESULT GetClassObject(
				CWbemObject& wbemObj, 
				LPCWSTR lpszPath, 
				const LONG& lFlags = 0, 
				IWbemContext* pContext = NULL, 
				IWbemCallResult** lppResult = NULL);
	HRESULT NexClassObject(
				CWbemObject& wbemObj, 
				const LONG& lTimeOut = WBEM_INFINITE, 
				const ULONG& uCount = 1);
	BOOL	IsConnect();
	HRESULT ResetEnum();
	void	ClearEnum();
	HRESULT ExecMethod(
				LPCWSTR lpszPath, 
				LPCWSTR lpszMethod, 
				IWbemContext* pContext = NULL, 
				IWbemClassObject* pWbemIn = NULL, 
				IWbemClassObject** ppWbemOut = NULL, 
				IWbemCallResult** ppCallResult = NULL);
	HRESULT GetClassHandle(
				IWbemClassObject** ppWbemObject, 
				LPCWSTR lpszPath, 
				const LONG& lFalgs = 0,
				IWbemContext* pContext = NULL,
				IWbemCallResult** ppCallResult = NULL);
	HRESULT NextClassHandle(
				IWbemClassObject* ppWbemObject, 
				const LONG& lTimeOut = WBEM_INFINITE,
				const ULONG& uCount = 1);

private:
	IWbemServices* m_pServices;
	IEnumWbemClassObject* m_pEnumObject;
};
