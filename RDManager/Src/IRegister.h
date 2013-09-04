#pragma once

#include <windows.h>

class CObserver
{
public:
	virtual ~CObserver() {}
};

typedef void (CObserver::*LPFNOnEvent)();

typedef bool (*LPFNEventDispatcher)(PROC lpfnProc, void *lpData);
typedef bool (*LPFNObjectEventDispatcher)(CObserver *lpObserver, LPFNOnEvent lpfnOnEvent, void *lpData);
typedef bool (*LPFNCreate)(const char*, long, IObject**);

class IRegister : public IObject
{
	RDDEFINE_IID(IRegister);
	virtual bool Register(LPFNCreate lpfnCreate, HMODULE hModule, const char **lppszClsid) = 0;
	virtual void UnRegister(LPFNCreate lpfnCreate) = 0;
	virtual bool CreateFromOthers(const char *lpszClsid, long lIid, IObject **lppIObject) = 0;
	virtual HMODULE FindModuleByFileName(const char *lpszFileName) = 0;

	virtual bool AddObserver(const char *lpszType, PROC lpfnProc, LPFNCreate lpfnCreate) = 0;
	virtual bool AddObserver(const char *lpszType, CObserver *lpObserver, LPFNOnEvent lpfnOnEvent, LPFNCreate lpfnCreate) = 0;
	virtual void DelObserver(CObserver *lpObserver) = 0;
	virtual int FireEvent(const char *lpszType, LPFNEventDispatcher lpfnDispatcher, void *lpData) = 0;
	virtual int FireEvent(const char *lpszType, LPFNObjectEventDispatcher lpfnEventDispatcher, void *lpData) = 0;
};