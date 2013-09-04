#pragma once

#include <list>
using namespace std;

class ISkinUI
{
public:
	ISkinUI() { };
	virtual ~ISkinUI() {};

public:
	virtual LONG SetSkin() = 0;
};

class CSkinManager
{
protected:
	CSkinManager();
	virtual ~CSkinManager();

private:
	CSkinManager(const CSkinManager& ) {};
	CSkinManager& operator= (const CSkinManager &) {};

public:
	static CSkinManager* Instance();
	static void Release();
	static void SetBkImage(LPCWSTR lpszBkImage);
	static void SetBkColor(DWORD dwColor);
	static LPCWSTR GetBkImage() { return szBkImage; };
	static DWORD GetBkColor() { return dwBkColor; };
	static DWORD GetBkColor2() { return dwBkColor2; };
	static DWORD GetBorderColor() { return dwBorderColor; };
	static void AddFrame(ISkinUI *pFrame);
	static void DelFrame(ISkinUI *pFrame);
	static void UpdateBkImage(LPCWSTR lpszBkImage);
	static void UpdateBkColor(DWORD dwColor);
	static BOOL IsBkColor() { return bBkColor; };

private:
	static CSkinManager *pInstance;
	static list<ISkinUI *> lstFrames;
	static wchar_t szBkImage[1024];
	static DWORD dwBkColor;
	static DWORD dwBkColor2;
	static DWORD dwBorderColor;
	static BOOL bBkColor;
};