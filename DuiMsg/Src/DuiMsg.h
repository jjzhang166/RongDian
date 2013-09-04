#pragma once
#include <Windows.h>

//#define USE_ZIPRES

void __stdcall InitDuiMsg(HINSTANCE hInst, LPVOID pLangMan, LPVOID pSkinMan, LPCWSTR lpszIconName);

int __stdcall DuiMsgBox(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszCaption, UINT uType);

int __stdcall DuiPopupMsg(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszCaption);