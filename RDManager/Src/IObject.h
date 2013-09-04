#pragma once

inline long _hashkey(const char* str)
{
	unsigned long value = 0;
	while (*str)
		value = (value<<5) + value + *str++;
	return value;
}

#define RDDEFINE_IID(_Interface)     \
public: \
	_Interface() {}		\
	virtual ~_Interface() {}         \
	static const char* getInterfaceName() { return #_Interface; }   \
	static long getIID() { static long iid = _hashkey(#_Interface); return iid; } \
	typedef void dummy

class IObject
{
	RDDEFINE_IID(IObject);
	virtual long retainObject() const = 0;
	virtual long releaseObject() const = 0;
	virtual bool queryObject(long iid, IObject** p) const = 0;
	virtual const char* getClassID() const = 0;
	virtual const char* getClassName() const = 0;
};
