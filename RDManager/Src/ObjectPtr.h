#pragma once

#include "IObject.h"

bool createObject(const char* lpszCLSID, long lIID, IObject **lppObj);

struct NullPointerError {};

#ifndef RDTHROW_NULLPOINTERERROR
#define RDTHROW_NULLPOINTERERROR(name) throw NullPointerError()
#endif

template<class PI>
class Object
{
public:
	Object(const char* lpszCLSID) : pPI(0)
	{
		createObject(lpszCLSID, PI::getIID(), address());
	}
	Object(const IObject *pObj) : pPI(0)
	{
		operator=(pObj);
	}
#if !defined(_MSC_VER) || _MSC_VER>1200
	Object(const Object<PI>& pObj) : pPI(0)
	{
		operator=(pObj.p());
	}
#endif
	bool valid() const
	{
		return this && pPI;
	}
	PI* p() const
	{
		return this ? pPI : 0;
	}
	PI* operator->() const
	{
		if(!pPI) 
			RDTHROW_NULLPOINTERERROR(PI::getInterfaceName());
		return pPI;
	}
	Object() : pPI(0)
	{
	}
	template<class PI2>
	Object(const Object<PI2>& pObj2) : pPI(0)
	{
		operator=(pObj2->p());
	}
	~Object()
	{
		release();
	}
	void release()
	{
		if(this && pPI)
		{
			pPI->releaseObject();
			pPI = 0;
		}
	}
	Object<PI>& create(const char lpszCLSID = "")
	{
		createObject(lpszCLSID, PI::getIID(), address());
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER>1200
	Object<PI>& operator=(const Object<PI>& obj)
	{
		if(obj)
			obj.p()->retainObject();
		if(pPI)
			pPI->releaseObject();
		pPI = obj.p();

		return *this;
	}
#endif

	template<class PI2>
	Object<PI>& operator=(const Object<PI2>&obj)
	{
		if(PI::getIID() == PI2::getIID() ||
			PI::getIID() == IObject::getIID())
		{
			if(obj)
				obj.p()->retainObject();
			if(pPI)
				pPI->releaseObject();
			pPI = obj.p();
			return *this;
		}

		return operator=(obj.p());
	}
	Object<PI> &operator=(const IObject *pObj)
	{
		if(pPI!=pObj)
		{
			release();
			if(PI::getIID()==IObject::getIID() && pObj)
			{
				pObj->retainObject();
				pPI = static_cast<PI*>(const_cast<IObject*>(pObj));
			}
			else if(pObj)
			{
				pObj->queryObject(PI::getIID(), address());
			}
		}
		return *this;
	}
	operator bool() const
	{
		return this && pPI;
	}
	bool operator!() const 
	{
		return !(this && pPI);
	}
	bool operator==(const Object<PI>&obj) const
	{
		return pPI==obj.pPI;
	}
	bool operator!=(const Object<PI>&obj) const
	{
		return pPI!=obj.pPI;
	}
	IObject ** address()
	{
		release();
		return reinterpret_cast<IObject **>(&pPI);
	}

private:
	PI *pPI;
};

typedef Object<IObject> AnyObject;