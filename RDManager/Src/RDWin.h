#pragma once

#if !defined(_MSC_VER) || _MSC_VER>1200

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT WINVER
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS WINVER
#endif
#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#endif 

#ifdef AFXDLL
#define VC_EXTRALEAN
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS
#include <afxwin.h>
#include <afxext.h>
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdisp.h>
#endif
#else
#define _WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef _MSC_VER
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#define OUTAPI exter "C" __declspec(dllexport)

#endif
