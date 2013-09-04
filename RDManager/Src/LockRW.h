#pragma once

enum LOCKRW_TYPE
{
	LOCKRW_UNDEFINE = 0,
	LOCKRW_READER = 1,
	LOCKRW_WRITER = 2
};

class CLockRW
{
public:
	struct DATA
	{
		LONG lRead;
		LONG lWrite;
		DATA() : lRead(0), lWrite(0) {}
	};

	CLockRW(DATA &data, bool bWrite = false) : daData(data), nType(LOCKRW_UNDEFINE)
	{
		if(bWrite)
		{
			if(0==daData.lRead && 0==daData.lWrite)
			{
				if(1==InterlockedIncrement(&daData.lWrite))
					nType = LOCKRW_WRITER;
				else
					InterlockedDecrement(&daData.lWrite);
			}
		}
		if(LOCKRW_UNDEFINE==nType)
		{
			if(0==daData.lWrite)
			{
				InterlockedIncrement(&daData.lRead);
				if(0==daData.lWrite)
					nType = LOCKRW_READER;
				else
					InterlockedDecrement(&daData.lRead);
			}
		}
	}
	~CLockRW()
	{
		Free();
	}
	void Free()
	{
		if(LOCKRW_READER==nType)
			InterlockedDecrement(&daData.lRead);
		else if(LOCKRW_WRITER==nType)
			InterlockedDecrement(&daData.lWrite);
		nType = LOCKRW_UNDEFINE;
	}
	bool CanRead() const
	{
		return nType > 0;
	}
	bool CanWrite() const
	{
		return (LOCKRW_WRITER==nType);
	}

private:
	CLockRW(const CLockRW&);
	CLockRW& operator=(const CLockRW &);

	DATA daData;
	int nType;
};

template <typename T>
struct LOCKRW : public T
{
	mutable CLockRW::DATA locker;
};
