#pragma once

#include "IObject.h"

typedef IObject *(*LPFNObjectCreater)(long lIID);
typedef bool (*LPFNHasIID)(long lIID);

enum { MIN_SINGLETON_TYPE = 10 };

class ClassEntry
{
public:
	int nType;
	const char *lpszName;
	const char *lpszCLSID;
	LPFNObjectCreater lpfnCreater;
	LPFNHasIID lpfnHasIID;

	ClassEntry(int _type, const char* _name, const char *_clsid, 
		LPFNObjectCreater _creater, LPFNHasIID _hasiid)
		: nType(_type), lpszName(_name), lpszCLSID(_clsid)
		, lpfnCreater(_creater), lpfnHasIID(_hasiid)
	{

	}
	ClassEntry() : nType(0), lpszName(""), lpszCLSID(""), lpfnCreater(NULL), lpfnHasIID(NULL)
	{

	}

	static const ClassEntry *const lpClasses[];
};

#if !defined(OUTAPI) && defined(_WIN32)
#ifndef _WINDEF_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#define OUTAPI extern "C" __declspec(dllexport)
#define RDFreeLibrary(h) FreeLibrary(h)
#define RDLoadLibrary(f) LoadLibraryExA(f, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#endif
