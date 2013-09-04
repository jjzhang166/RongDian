#pragma once
#include <Windows.h>

HMODULE getModuleHandle();
HMODULE getManagerModule();

#define RDBEGIN_CLASS_DECLARE0(Cls) \
public: \
	static bool _queryObject(const Cls* self, long iid, IObject** p)  \
	{

#define RDBEGIN_CLASS_DECLARE(Cls, clsid)  \
	public: \
	static const char* _getClassID() { return clsid; }   \
	static const char* _getClassName() { return #Cls; }   \
	static bool _queryObject(const Cls* self, long iid, IObject** p)  \
	{

#define RDDEFINE_INTERFACE_ENTRY(_Interface)    \
	if (iid == _Interface::getIID())        \
		{   \
		if (self) {     \
		*p = (IObject*)(_Interface*)(self); \
		(*p)->retainObject(); \
		}   \
		return true;    \
		}

#define RDUSE_INTERFACE_ENTRY(_BaseClass)   \
	if (_BaseClass::_queryObject(self, iid, p))   \
		{   \
		return true;    \
		}

#define RDEND_CLASS_DECLARE() \
	return false; \
	}   \
	protected:  // Ensure the following constructor is protected.
