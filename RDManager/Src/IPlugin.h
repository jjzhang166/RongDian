#pragma once

#include <vector>
#include <string>

class IPlugin : public IObject
{
	RDDEFINE_IID(IPlugin);
	virtual int GetCount() const = 0;
	virtual void GetFiles(std::vector<std::string>& files) const = 0;

	virtual int LoadPlugins(const char* folder) = 0;
	virtual int UnLoadPlugins() = 0;
};