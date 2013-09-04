#include "Plugins.h"

CPlugins::CPlugins()
{

}

CPlugins::~CPlugins()
{

}


// IPlugin's Interface
int CPlugins::GetCount() const
{
	CLockRW locker(vecPlugins.locker);
	return locker.CanRead() ? (1 + get_size(vecPlugins)) : 1;
}

void CPlugins::GetFiles(std::vector<std::string>& files) const
{
	char szName[1024] = {0};
	files.clear();
	GetModuleFileNameA(getModuleHandle(), szName, _countof(szName));
	files.push_back(szName);

	CLockRW locker(vecPlugins.locker);
	if(locker.CanRead())
	{
		for(std::vector<PAIR_PLUGIN>::const_iterator it=vecPlugins.begin();
			it!=vecPlugins.end(); ++it)
		{
			GetModuleFileNameA(it->second, szName, _countof(szName));
			files.push_back(szName);
		}
	}
}

// IRegister's Interface
bool CPlugins::Register(LPFNCreate lpfnCreate, HMODULE hModule, const char **lppszCLSIDs)
{
	bool bNeedInit = true;
	CLockRW locker(vecPlugins.locker, true);
	if(locker.CanWrite() && find_value(vecPlugins, PAIR_PLUGIN(lpfnCreate, hModule))<0)
	{
		vecPlugins.push_back(PAIR_PLUGIN(lpfnCreate, hModule));
		CLockRW locker2(mapCLSID.locker, true);
		for(; *lppszCLSIDs && locker2.CanWrite(); lppszCLSIDs++)
			mapCLSID[*lppszCLSIDs] = lpfnCreate;
	}
	return bNeedInit;
}

void CPlugins::UnRegister(LPFNCreate lpfnCreate)
{
	CLockRW locker(vecPlugins.locker, true);
	if(locker.CanWrite())
	{
		for(std::vector<PAIR_PLUGIN>::iterator it=vecPlugins.begin();
			it!=vecPlugins.end(); ++it)
		{
			if(it->first == lpfnCreate)
			{
				vecPlugins.erase(it);
				break;
			}
		}
	}

	CLockRW locker2(mapCLSID.locker, true);
	if(locker2.CanWrite())
	{
		hash_map<std::string, LPFNCreate>::iterator it = mapCLSID.begin();
		while(it!=mapCLSID.end())
		{
			if(it->second==lpfnCreate)
			{
				mapCLSID.erase(it);
				it = mapCLSID.begin();
			}
			else
			{
				++it;
			}
		}
	}

	CLockRW locker3(mapObservers.locker, true);
	if(locker3.CanRead())
	{
		for(OBSERVER_ITEM_ITER it=mapObservers.begin(); it!=mapObservers.end(); )
		{
			if(it->second.lpfnCreate==lpfnCreate)
			{
				mapObservers.erase(it);
				it = mapObservers.begin();
			}
			else
			{
				it->second = OBSERVER_ITEM();
				++it;
			}
		}
	}
}

bool CPlugins::CreateFromOthers(const char *lpszClsid, long lIid, IObject **lppIObject)
{
	return 0;
}

HMODULE CPlugins::FindModuleByFileName(const char *lpszFileName)
{
	return 0;
}

bool CPlugins::AddObserver(const char *lpszType, PROC lpfnProc, LPFNCreate lpfnCreate)
{
	return 0;
}

bool CPlugins::AddObserver(const char *lpszType, CObserver *lpObserver, LPFNOnEvent lpfnOnEvent, LPFNCreate lpfnCreate)
{
	return 0;
}

void CPlugins::DelObserver(CObserver *lpObserver)
{

}

int CPlugins::FireEvent(const char *lpszType, LPFNEventDispatcher lpfnDispatcher, void *lpData)
{
	return 0;
}

int CPlugins::FireEvent(const char *lpszType, LPFNObjectEventDispatcher lpfnEventDispatcher, void *lpData)
{
	return 0;
}

// 
LPFNCreate CPlugins::FindPluginByCLSID(const char *lpszCSLID) const
{
	return 0;
}
