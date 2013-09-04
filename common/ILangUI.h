#pragma once

#include <list>
using namespace std;

class ILangUI
{
public:
	ILangUI() { };
	virtual ~ILangUI() {};

public:
	virtual LONG SetLang(LPCWSTR lpszLang) = 0;
};

class CLangManager
{
protected:
	CLangManager();
	virtual ~CLangManager();

private:
	CLangManager(const CLangManager& ) {};
	CLangManager& operator= (const CLangManager &) {};

public:
	static CLangManager* Instance();
	static void Release();
	static void SetLangName(LPCWSTR lpszLangName);
	static LPCWSTR GetLangName() { return szLangName; };
	static void AddFrame(ILangUI *pFrame);
	static void DelFrame(ILangUI *pFrame);
	static void UpdateLang(LPCWSTR lpszLang);

private:
	static CLangManager *pInstance;
	static list<ILangUI *> lstFrames;
	static wchar_t szLangName[1024];
};