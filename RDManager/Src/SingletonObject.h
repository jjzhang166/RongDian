#pragma once

#include "ModuleItem.h"
#include "IObject.h"

template<class Cls>
class SingletonObject : public Cls, public ModuleItem
{
protected:
	SingletonObject() : lRefCount(1)
	{

	}
	virtual ~SingletonObject()
	{
		if(Instance()==this)
			Instance() = NULL;
	}

protected:
	virtual long retainObject() const
	{
		return InterlockedIncrement(&lRefCount);
	}
	virtual long releaseObject() const
	{
		return InterlockedDecrement(&lRefCount);
	}
	virtual bool queryObject(long lIID, IObject **lppObj) const
	{
		return Cls::_queryObject(this, lIID, lppObj);
	}
	virtual const char* getClassID() const
	{
		return Cls::_getClassID();
	}
	virtual const char* getClassName() const
	{
		return Cls::_getClassName();
	}

public:
	static IObject* create(long lIID)
	{
		if(!Instance())
		{
			SingletonObject<Cls>* pObj = new SingletonObject<Cls>();
			if(1==InterlockedIncrement(&locker()))
			{
				Instance() = pObj;
				pObj->AddModuleItem();
			}
			else
			{
				delete pObj;
			}
			InterlockedDecrement(&locker());
		}
		IObject *pRet = NULL;
		Instance()->queryObject(lIID, &pRet);

		return pRet;
	}

private:
	SingletonObject(const SingletonObject &obj);
	void operator=(const SingletonObject &obj);

	mutable long lRefCount;

	static SingletonObject<Cls>*& Instance()
	{
		static SingletonObject<Cls>* pObj = NULL;
		return pObj;
	}
	static long& locker()
	{
		static long lCount = 0;
		return lCount;
	}
};