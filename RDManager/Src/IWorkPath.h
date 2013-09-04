#pragma once

class IWorkPath : public IObject
{
public:
	RDDEFINE_IID(IWorkPath);

    virtual std::wstring GetWorkPath() = 0;
    virtual void GetWorkPath(const std::wstring& path) = 0;
    virtual std::wstring GetConfigPath(const wchar_t* subfolder) = 0;
};
