#pragma once
#include <Windows.h>

//#define USE_ZIPRES

#ifndef MB_POPUP
#define MB_POPUP 0xFFFFFFFFL
#endif

#ifndef MB_LOADING
#define MB_LOADING 0x0FFFFFFFL
#endif

int __stdcall InitDuiMsg(HINSTANCE hInst, LPVOID pLangMan, LPVOID pSkinMan, LPCWSTR lpszIconName);

int __stdcall DuiMsgBox(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszCaption, UINT uType);

int __stdcall DuiPopupMsg(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszCaption);

int __stdcall DuiShowLoading(HWND hWnd, LPCWSTR lpszText, LPCWSTR lpszLoadImg, LPVOID *ppParam);

int __stdcall DuiCancelLoading(LPVOID lpParam);