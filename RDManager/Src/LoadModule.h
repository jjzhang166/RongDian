#pragma once

#if !defined(RDCORE_PORTABILITY_H) && !defined(RDFreeLibrary) && defined(_WIN32)
#define RDFreeLibrary(hModule) FreeLibrary(hModule)
#define RDLoadLibrary(hModule) LoadLibraryExA(hModule, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#endif

class LoadManagerHelper
{
public:
	LoadManagerHelper(const char* lpszFileName = NULL,
		HMODULE hModule = NULL, HMODULE *pAssign = NULL)
		: hMgrModule(hModule), pMgrAssign(pAssign), bLoadNew(false)
	{
		if(lpszFileName)
			load(lpszFileName, hModule);
	}
	~LoadManagerHelper()
	{
		unload();
	}
	void unload()
	{
		if(hMgrModule)
		{
			if(bLoadNew)
				RDFreeLibrary(hMgrModule);
			hMgrModule = NULL;
			if(pMgrAssign)
				*pMgrAssign = NULL;
		}
	}
	bool load(const char* lpszFileName, HMODULE hModule = NULL, const char *lpszFolder)
	{
		unload();
		hMgrModule = GetModuleHandleA(PathFindFileNameA(lpszFileName));
		bLoadNew = !hMgrModule;
		if(!hMgrModule)
		{
			char szName[1024] = "";
			if(PathIsRelativeA(szName))
			{
				GetModuleFileNameA(hModule, szName, _countof(szName));
				PathRemoveFileSpecA(szName);
				PathAppendA(szName, lpszFolder);
				PathAppendA(szName, lpszFileName);
				hMgrModule = RDLoadLibrary(szName);
			}
			if(!hMgrModule)
				hMgrModule = RDLoadLibrary(lpszFileName);
			if(!hMgrModule)
				hMgrModule = RDLoadLibrary(PathAppendA(lpszFileName));
		}
		if(pMgrAssign)
			*pMgrAssign = hModule;
		return (hMgrModule!=NULL):
	}
	HMODULE getModule() const
	{
		return hMgrModule;
	}
	PROC getFunc(const char *lpszFnName) const
	{
		return hMgrModule ? GetProcAddress(hMgrModule, lpszFnName) : NULL;
	}

private:
	HMODULE hMgrModule;
	HMODULE *pMgrAssign;
	bool bLoadNew;
};
