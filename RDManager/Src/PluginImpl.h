#pragma once

#include "ClassEntry.h"
#include "ModuleItem.h"
#ifdef RDCORE_PORTABILITY_H
#include "PortImpl.h"
#endif

static HMODULE g_hModule = NULL;
static HMODULE g_hManager = NULL;
static bool g_bLoadMgr = false;
static LONG g_lRefCount = 0;

OUTAPI bool RDInitializePlugin();
OUTAPI void RDUnInitializePlugin();

HMODULE getModuleHandle()
{
	return g_hModule;
}

HMODULE getMgrModule()
{
	return g_hManager;
}

static int getClassCount(int nType = 0)
{
	int nCount = 0;
	for(const ClassEntry* const * arr = ClassEntry::lpClasses; *arr; arr++)
	{
		for(const ClassEntry* cls = *arr; cls->lpfnCreater; cls++)
		{
			if(cls->nType >= nType)
				nCount ++;
		}
	}

	return nCount;
}

static const char** getClassIDs(const char** lpszCLSIDs, int nCount)
{
	const ClassEntry * const * arr = ClassEntry::lpClasses;
	int nIndex = 0;
	for(; *arr && nIndex<nCount; arr++)
	{
		for(const ClassEntry* cls = *arr; cls->lpfnCreater; cls++, nIndex++)
			lpszCLSIDs[nIndex] = cls->lpszCLSID;
	}
	lpszCLSIDs[nIndex] = NULL;

	return lpszCLSIDs;
}

static bool getDefaultClassID(long lIID, const char* &lpszCLSID)
{
	for(const ClassEntry * const * arr = ClassEntry::lpClasses; *arr; arr++)
	{
		for(const ClassEntry* cls = *arr; cls->lpfnCreater; cls++)
		{
			if(cls->lpfnHasIID(lIID))
			{
				lpszCLSID = cls->lpszCLSID;
				return true;
			}
		}
	}
	return false;
}

OUTAPI bool RDInternalCreate(const char *lpszCLSID, long lIID, IObject **lppObj)
{
	*lppObj = NULL;
	if(NULL==lpszCLSID)
		getDefaultClassID(lIID, lpszCLSID);
	for(const ClassEntry * const * arr = ClassEntry::lpClasses; *arr; arr++)
	{
		for(const ClassEntry* cls=*arr; cls->lpfnCreater; cls++)
		{
			if(strcmp(cls->lpszCLSID, lpszCLSID)==0)
			{
				*lppObj = cls->lpfnCreater(lIID);
				return (*lppObj!=NULL);
			}
		}
	}
	return false;
}

OUTAPI bool RDFreePlugin()
{
	if(--g_lRefCount!=0)
		return false;
	bool bNeedFree = true;
	if(g_hManager && g_hManager!=g_hModule)
	{
		typedef bool (*LPFNRDInternalCreate)(const char *lpszCLSID, long lIID, IObject **lppObj);
		typedef bool (*LPFNRDUnRegisterPlugin)(LPFNRDInternalCreate);
		LPFNRDUnRegisterPlugin lpfnRDUnRegisterPlugin = (LPFNRDUnRegisterPlugin)GetProcAddress(g_hManager, "RDUnRegisterPlugin");
		bNeedFree = !lpfnRDUnRegisterPlugin || lpfnRDUnRegisterPlugin(RDInternalCreate);
	}
	if(bNeedFree)
		RDUnInitializePlugin();
	if(g_bLoadMgr)
	{
		RDFreePlugin();
		g_bLoadMgr = false;
	}
	ModuleItem::release();

	return true;
}

#ifndef RD_CLASS_MAXCOUNT
#define RD_CLASS_MAXCOUNT 64
#endif
#ifndef RDMANAGER_PLNAME
#define RDMANAGER_PLNAME "rdmanager.pln"
#endif

OUTAPI bool RDInitPlugin(HMODULE hModule, HMODULE hManager)
{
	if(++g_lRefCount!=1)
		return true;
	int nSingletonClassCount = getClassCount(MIN_SINGLETON_TYPE);
	ModuleItem::init(nSingletonClassCount);

	const char *lpszTmpCLSID = NULL;
	getDefaultClassID(IObject::getIID(), lpszTmpCLSID);
	g_hModule = hModule;
	if(!g_hManager)
	{
		hManager = hManager ? hManager : GetModuleHandleA(RDMANAGER_PLNAME);
		if(!hManager)
		{
			hManager = RDLoadLibrary(RDMANAGER_PLNAME);
			g_bLoadMgr = true;
		}
		g_hManager = hManager;
	}
	bool bNeedInit = true;
	if(g_hManager && g_hManager!=g_hModule)
	{
		typedef bool (*LPFNRDInternalCreate)(const char *lpszCLSID, long lIID, IObject **lppObj);
		typedef bool (*LPFNRDRegisterPlugin)(LPFNRDInternalCreate lpfnRDInternalCreate, HMODULE hModule, const char **lpszCLSID);
		LPFNRDRegisterPlugin lpfnRDRegisterPlugin = (LPFNRDRegisterPlugin)GetProcAddress(g_hManager, "RDRegisterPlugin");
		const char* lpszCLSIDs[RD_CLASS_MAXCOUNT + 1] = {NULL};
		bNeedInit = !lpfnRDRegisterPlugin || lpfnRDRegisterPlugin(RDInternalCreate, g_hModule,
			getClassIDs(lpszCLSIDs, RD_CLASS_MAXCOUNT));
	}

	return !bNeedInit || RDInitializePlugin();
}

#ifndef CREATEOBJECTIMPL
#define CREATEOBJECTIMPL
bool createObject(const char* lpszCLSID, long lIID, IObject **lppObj)
{
	if(!RDInternalCreate(lpszCLSID, lIID, lppObj) && *lpszCLSID && g_hManager)
	{
		typedef bool (*LPFNRDCreateObject)(const char *lpszCLSID, long lIID, IObject **lppObj);
		LPFNRDCreateObject lpfnRDCreateObject = (LPFNRDCreateObject)GetProcAddress(g_hManager, "RDCreateObject");
		return lpfnRDCreateObject && lpfnRDCreateObject(lpszCLSID, lIID, lppObj);
	}

	return (*lppObj!=NULL);
}
#endif

#ifndef RD_EXCLUDE_CREATEOBJECT
OUTAPI bool RDCreateObject(const char* lpszCLSID, long lIID, IObject **lppObj)
{
	return createObject(lpszCLSID, lIID, lppObj);
}
#endif
