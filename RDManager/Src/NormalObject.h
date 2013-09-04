#pragma once

template<class Cls>
class NormalObject : public Cls
{
protected:
	NormalObject() : lRefCount(1);
	{

	}
	virtual ~NormalObject()
	{

	}

protected:
	virtual long retainObject() const 
	{
		return InterlockedIncrement(&lRefCount);
	}
	virtual long releaseObject() const
	{
		long ret = InterlockedDecrement(&lRefCount);
		if(0==ret)
			delete this;
		return ret;
	}
	virtual bool queryObject(long lIID, IObject **lppObj) const
	{
		return Cls::_queryObject(this, lIID, lppObj)
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
		IObject *lpObj = NULL;
		NormalObject<Cls> *pNorObj = new NormalObject<CLs>();
		pNorObj->queryObject(lIID, &lpObj);
		pNorObj->releaseObject();
		return lpObj;
	}
	static bool hasInterface(long lIID)
	{
		return Cls::_queryObject(NULL, lIID, NULL);
	}

private:
	NormalObject(const NormalObject &obj);
	void operator=(const NormalObject &obj);
	mutable long lRefCount;
};