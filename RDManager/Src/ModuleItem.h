#pragma once

class ModuleItem
{
protected:
	ModuleItem() : nIndex(-1)
	{

	}
	virtual ~ModuleItem()
	{

	}
	void AddModuleItem()
	{
		nIndex = InterlockedDecrement(&index());
		if(nIndex>=0)
			items()[nIndex] = this;
	}
public:
	static void release()
	{
		if(items())
		{
			for(int i=index(); i<maxCount(); i++)
			{
				if(i>=0 && items()[i])
				{
					delete items()[i];
					items()[i] = NULL;
				}
			}
			index() = 0;

			delete[] items();
			items() = NULL;
		}
	}

	static void init(int nSingletonClassCount)
	{
		if(!items() && nSingletonClassCount>0)
		{
			maxCount() = (nSingletonClassCount>maxCount())
				? nSingletonClassCount : maxCount();
			index() = maxCount();
			items() = new ModuleItem*[maxCount()];
		}
	}

private:
	int nIndex;

	static long& maxCount()
	{
		static long nCount = 0;
		return nCount;
	}
	static long& index()
	{
		static long n = 0;
		return n;
	}
	static ModuleItem**& items()
	{
		static ModuleItem** pItem = NULL;
		return pItem;
	}
};
