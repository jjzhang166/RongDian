#pragma once

#ifdef KEYHOOK_DLL
#define KEYHOOK_EXPORT extern "C" __declspec(dllexport)
#else
#define KEYHOOK_EXPORT extern "C" __declspec(dllimport)
#endif

KEYHOOK_EXPORT LONG __cdecl AttatchHook(HWND hInvoker);
KEYHOOK_EXPORT LONG __cdecl DetatchHook();

