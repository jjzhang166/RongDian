#pragma once

#include <Windows.h>
#include <tchar.h>

namespace ColorUtil
{
	BOOL RGBToHTML(DWORD dwColor, LPWSTR lpszHtml);
	BOOL HtmlToRGB(LPCWSTR lpszHtml, DWORD &dwColor);

	BOOL RGBToHSL(DWORD dwColor, double &h, double &s, double &l);
	BOOL HSLToRGB(double h, double s, double l, DWORD &dwColor);

	BOOL RGBToHSV(DWORD dwColor, double &h, double &s, double &v);
	BOOL HSVToRGB(double h, double s, double v, DWORD &dwColor);

	BOOL RGBToHex(DWORD dwColor, LPWSTR lpszHex);

	BOOL RGBToDelphi(DWORD dwColor, LPWSTR lpszDelphi);

	BOOL RGBToStr(DWORD dwColor, LPWSTR lpszRGB);
	BOOL StrToRGB(LPCWSTR lpszRGB, DWORD &dwColor);

	BOOL AdjustColor(DWORD dwColor, DWORD &dwFixColor);
}
