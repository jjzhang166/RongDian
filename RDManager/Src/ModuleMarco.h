#pragma once

#include "ClassEntry.h"
#include "NormalObject.h"
#include "SingletonObject.h"

#define RDBEGIN_DEFINE_MODULE() \ 
	static const ClassEntry g_Classes[] = {
#define RDDEFINE_CLASSMAP_ENTRY(cls) \
	ClassEntry(1, "NormalObject<" #cls ">", cls::_getClassID(), \
		(LPFNObjectCreater)(&NormalObject<cls>::create), \
		(LPFNHasIID)(&NormalObject<cls>::hasInterface)),

#define RDDEFINE_CLASSMAP_ENTRY_SINGLETON(cls) \
	ClassEntry(MIN_SINGLETON_TYPE, \
		"SingletonObject<" #cls ">", cls::_getClassID(), \
		(LPFNObjectCreater)(&SingletonObject<cls>::create), \
		(LPFNHasIID)(&SingletonObject<cls>::hasInterface)),

#if !defined(_LIB) && !defined(RDUSE_LIB_PLUGIN)
#define RDEND_DEFINE_MODULE() \
	ClassEntry() \
	}; \
	const ClassEntry* const ClassEntry::lpClasses[] = { \
		g_Classes, NULL \
	};
#else
#define RDEND_DEFINED_MODULE() ClassEntry() };
#endif

#ifdef _LIB
#define RDEND_DEFINE_MODULE_LIB(szName) \
	ClassEntry() }; \
	extern const ClassEntry* const classes_##szName = g_Classes;
#define RDEND_DEFINE_MODULE_DLL()
#define RDEND_DEFINE_MODULE_MFCEXTDLL()
#define RDEND_DEFINE_MODULE_MFCDLL()
#else
#define RDEND_DEFINE_MODULE_LIB(szName) ClassEntry() };

OUTAPI bool RDInitPlugin(HMODULE hModule, HMODULE hManager);
OUTAPI bool RDFreePlugin();

#define RDDllMain(hInstance, dwReason) \
	(dwReason==DLL_PROCESS_ATTACH && ::RDInitPlugin((HMODULE)hInstance, NULL) \
	|| dwReason==DLL_PROCESS_DETACH && (::RDFreePlugin() || true))

#define RDEND_DEFINE_MODULE_MFCEXTDLL() \ 
	RDEND_DEFINE_MODULE() \
static AFX_EXTENSION_MODULE MFCExtDLL = { NULL, NULL }; \
extern "C" int APIENTRY \
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpData) \
{ \
	if(dwReason==DLL_PROCESS_ATTACH) \
	{ \
		if(!AfxInitExtensionModule(MFCExtDLL, hInstance)) \
			return 0; \
		new CDynLinkLibrary(MFCExtDLL); \
			return ::RDInitPlugin(MFCExtDLL.hModule, NULL) ? 1 : 0; \
	} \
	else if(dwReason==DLL_PROCESS_DETACH) \
	{ \
		::RDFreePlugin(); \
		AfxTermExtensionModule(MFCExtDLL); \
	} \
	return 1; \
}

#define RDEND_DEFINE_MODULE_MFCDLL() \
	RDEND_DEFINE_MODULE() \
class CPluginApp : public CWinApp \
{ \
public: \
	CPluginApp() \
	{ \
	} \
	virtual BOOL InitInstance() \
	{ \
		CWinApp::InitInstance(); \
		return ::RDInitPlugin(m_hInstance, NULL); \
	} \
	virtual int ExitInstance() \
	{ \
		::RDFreePlugin(); \
		return CWinApp::ExitInstance(); \
	} \
};

#if defined(_AFXEXT) && defined(_AFXDLL) && defined(APIENTRY)
#define RDEND_DEFINE_MODULE_DLL() \
	RDEND_DEFINE_MODULE_MFCEXTDLL()
#elif defined(_AFXDLL) && defined(__AFXWIN_H__)
#define RDEND_DEFINE_MODULE_DLL() \
	RDEND_DEFINE_MODULE_MFCDLL() \
	CPluginApp theApp;
#elif defined(_USRDLL) && defined(APIENTRY)
#ifdef _ATL_DLL
#define RDEND_DEFINE_MODULE_DLL() \
	RDEND_DEFINE_MODULE() \
extern "C" BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpData) \
{ \
	if(dwReason==DLL_PROCESS_ATTACH) \
	{ \
		return ::RDInitPlugin((HMODULE)hModule, NULL); \
	} \
	else if(dwReason==DLL_PROCESS_DETACH) \
	{ \
		::RDFreePlugin(); \
	} \
	return TRUE; \
}
#else
#define RDEND_DEFINE_MODULE_DLL() please_use_RDEND_DEFINE_MODULE_and_call_RDDllMain
#endif

#else
#define RDEND_DEFINE_MODULE_DLL() RDEND_DEFINE_MODULE()
#endif

#define RDDEFINE_EMPTY_MODULE() \
	RDBEGIN_DEFINE_MODULE() \
	RDEND_DEFINE_MODULE_DLL() \
	OUTAPI bool RDInitializePlugin() { return true; } \
	OUTAPI void RDUnInitializePlugin() {} 
#endif
	