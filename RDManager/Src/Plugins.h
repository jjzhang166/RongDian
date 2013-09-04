#pragma once

#include "ClassMarco.h"
#include "IObject.h"
#include "IPlugin.h"
#include "IRegister.h"
#include "LockRW.h"
#include "stlfunc.h"

#if defined(_MSC_VER) && _MSC_VER > 1200    // VC8/9
#include <hash_map>
using stdext::hash_multimap;
using stdext::hash_map;
#else                                       // VC6, GCC or others
#define hash_multimap std::multimap
#define hash_map std::map
#endif

class CPlugins : public IPlugin, public IRegister
{
	RDBEGIN_CLASS_DECLARE0(CPlugins)
		RDDEFINE_INTERFACE_ENTRY(IPlugin)
		RDDEFINE_INTERFACE_ENTRY(IRegister)
	RDEND_CLASS_DECLARE()

protected:
	CPlugins();
	virtual ~CPlugins();

private:
	// IPlugin's Interface
	virtual int GetCount() const;
	virtual void GetFiles(std::vector<std::string>& files) const;
	//virtual int LoadPlugins(const char* folder);
	//virtual int UnLoadPlugins();

	// IRegister's Interface
	virtual bool Register(LPFNCreate lpfnCreate, HMODULE hModule, const char **lppszCLSIDs);
	virtual void UnRegister(LPFNCreate lpfnCreate);
	virtual bool CreateFromOthers(const char *lpszClsid, long lIid, IObject **lppIObject);
	virtual HMODULE FindModuleByFileName(const char *lpszFileName);
	virtual bool AddObserver(const char *lpszType, PROC lpfnProc, LPFNCreate lpfnCreate);
	virtual bool AddObserver(const char *lpszType, CObserver *lpObserver, LPFNOnEvent lpfnOnEvent, LPFNCreate lpfnCreate);
	virtual void DelObserver(CObserver *lpObserver);
	virtual int FireEvent(const char *lpszType, LPFNEventDispatcher lpfnDispatcher, void *lpData);
	virtual int FireEvent(const char *lpszType, LPFNObjectEventDispatcher lpfnEventDispatcher, void *lpData);

	// 
	virtual LPFNCreate FindPluginByCLSID(const char *lpszCSLID) const;

private:
	typedef std::pair<LPFNCreate, HMODULE> PAIR_PLUGIN;
	LOCKRW<std::vector<PAIR_PLUGIN> > vecPlugins;
	LOCKRW<hash_map<std::string, LPFNCreate> > mapCLSID;

	typedef struct _tagOBSERVER_ITEM {
		LPFNCreate		lpfnCreate;
		PROC			lpfnProc;
		CObserver		*pObserver;
		LPFNOnEvent		lpfnOnEvent;

		_tagOBSERVER_ITEM() : lpfnCreate(NULL), lpfnProc(NULL), pObserver(NULL), lpfnOnEvent(NULL) {}
	} OBSERVER_ITEM, *POBSERVER_ITEM, *LPOBSERVER_ITEM;

	typedef hash_multimap<std::string, OBSERVER_ITEM> OBSERVER_ITEM_MAP;
	typedef std::pair<std::string, OBSERVER_ITEM> OBSERVER_ITEM_PAIR;
	typedef OBSERVER_ITEM_MAP::iterator OBSERVER_ITEM_ITER;

	LOCKRW<OBSERVER_ITEM_MAP> mapObservers;
};
