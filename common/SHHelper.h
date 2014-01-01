#pragma once

#include <tchar.h>
#include <Windows.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#pragma comment(lib, "shell32.lib")

namespace SHHelper
{
BOOL DelFile(HWND hWnd, LPCWSTR lpszFile, BOOL bShowUI = TRUE);
BOOL CopyFile(HWND hWnd, LPCWSTR  lpszFromPath, LPCWSTR  lpszToPath, BOOL bShowUI = TRUE);
BOOL MoveFile(HWND hWnd, LPCWSTR  lpszFromPath, LPCWSTR  lpszToPath, BOOL bShowUI = TRUE);
BOOL RenameFile(HWND hWnd, LPCWSTR lpszFromPath, LPCWSTR lpszToPath);

BOOL OpenFile(HWND hWnd, LPCWSTR lpszFile);
BOOL OpenFileEx(HWND hWnd, LPCWSTR lpszFile, LPCWSTR lpszParameters);
BOOL OpenFolder(HWND hWnd, LPCWSTR lpszPath);
// 选择文件对话框
BOOL SelectFile(HWND hWnd, LPWSTR lpszFile, LPCWSTR lpszFilter);
BOOL SelectFolder(HWND hWnd, LPWSTR lpszFolder, LPCWSTR lpszTitle = NULL);
};