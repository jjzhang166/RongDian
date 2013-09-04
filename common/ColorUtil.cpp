#include "ColorUtil.h"

#pragma warning(push)
#pragma warning(disable:4996)

BOOL ColorUtil::RGBToHTML(DWORD dwColor, LPWSTR lpszHtml)
{
	if(!lpszHtml)
		return FALSE;
	swprintf(lpszHtml, L"#%02X%02X%02X", GetRValue(dwColor), GetGValue(dwColor), GetBValue(dwColor));
	return TRUE;
}

BOOL ColorUtil::HtmlToRGB(LPCWSTR lpszHtml, DWORD &dwColor)
{
	if(!lpszHtml)
		return FALSE;
	LPWSTR lpszEnd = NULL;
	dwColor = wcstoul(lpszHtml, &lpszEnd, 16);
	return TRUE;
}

BOOL ColorUtil::RGBToHSL(DWORD dwColor, double &h, double &s, double &l)
{
	double R,G,B,Max,Min,del_R,del_G,del_B,del_Max;
	R = GetRValue(dwColor) / 255.0; // Where RGB values = 0 / 255
	G = GetGValue(dwColor) / 255.0;
	B = GetBValue(dwColor) / 255.0;

	Min = min(R, min(G, B)); // Min. value of RGB
	Max = max(R, max(G, B)); // Max. value of RGB
	del_Max = Max - Min; // Delta RGB value

	l = (Max + Min) / 2.0;
	if (del_Max == 0) // This is a gray, no chroma...
	{
		//H = 2.0/3.0; // Windows下S值为0时，H值始终为160（2/3*240）
		h = 0; // HSL results = 0 / 1
		s = 0;
	}
	else // Chromatic data...
	{
		if (l < 0.5) 
			s = del_Max / (Max + Min);
		else
			s= del_Max / (2 - Max - Min);

		del_R = (((Max - R) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_G = (((Max - G) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_B = (((Max - B) / 6.0) + (del_Max / 2.0)) / del_Max;

		if (R == Max)
			h = del_B - del_G;
		else if (G == Max)
			h = (1.0 / 3.0) + del_R - del_B;
		else if (B == Max)
			h = (2.0 / 3.0) + del_G - del_R;

		if (h< 0) 
			h += 1;
		if (h > 1)  
			h -= 1;
	}
	return TRUE;
}

BOOL ColorUtil::RGBToHSV(DWORD dwColor, double &h, double &s, double &v)
{
	// r,g,b values are from 0 to 1  
	// h = [0,360], s = [0,1], v = [0,1]  
	// if s == 0, then h = -1 (undefined) 
	double R = 0.0, G = 0.0, B = 0.0;

	R = GetRValue(dwColor) / 255.0; // RGB from 0 to 1
	G = GetGValue(dwColor) / 255.0;
	B = GetBValue(dwColor) / 255.0;

	double min, max, delta,tmp;  
	tmp = R>G?G:R;  
	min = tmp>B?B:tmp;  
	tmp = R>G?R:G;  
	max = tmp>B?tmp:B;  
	v = max; // v  
	delta = max - min;  
	if( max != 0 )  
		s = delta / max; // s  
	else  
	{  
		// r = g = b = 0 // s = 0, v is undefined  
		s = 0;  
		h = 0;  
		return TRUE;  
	}  
	if (delta == 0){  
		h = 0;  
		return TRUE;  
	}  
	else if(R == max){  
		if (G >= B)  
			h = (G - B) / delta; // between yellow & magenta  
		else  
			h = (G - B) / delta + 6.0;  
	}  
	else if( G == max )  
		h = 2.0 + ( B - R ) / delta; // between cyan & yellow  
	else if (B == max)  
		h = 4.0 + ( R - G ) / delta; // between magenta & cyan  
	h *= 60.0; // degrees 
	return TRUE;
}

BOOL ColorUtil::RGBToHex(DWORD dwColor, LPWSTR lpszHex)
{
	if(!lpszHex)
		return FALSE;
	swprintf(lpszHex, L"%02X%02X%02X", GetRValue(dwColor), GetGValue(dwColor), GetBValue(dwColor));
	return TRUE;
}

BOOL ColorUtil::RGBToDelphi(DWORD dwColor, LPWSTR lpszDelphi)
{
	if(!lpszDelphi)
		return FALSE;
	wchar_t szHex[32] = {0};
	RGBToHex(dwColor, szHex);
	swprintf(lpszDelphi, L"$00%s", szHex);
	return TRUE;
}

BOOL ColorUtil::RGBToStr(DWORD dwColor, LPWSTR lpszRGB)
{
	if(!lpszRGB)
		return FALSE;
	BYTE bRed = GetRValue(dwColor);
	BYTE bGreen = GetGValue(dwColor);
	BYTE bBlue = GetBValue(dwColor);
	swprintf(lpszRGB, L"RGB(%d,%d,%d)", bRed, bGreen, bBlue);
	return TRUE;
}

BOOL ColorUtil::StrToRGB(LPCWSTR lpszRGB, DWORD &dwColor)
{
	if(!lpszRGB)
		return FALSE;
	wchar_t szColor[1024] = {0};
	wcscpy(szColor, lpszRGB);
	BYTE bColors[3] = {0};
	int i = 0;
	wchar_t *pStr = wcstok(szColor, L",");
	while(pStr && i<3)
	{
		bColors[i] = (BYTE)_wtoi(pStr);
		i++;
		pStr = wcstok(NULL, L",");
	}
	if(pStr || i!=3)
		return FALSE;
	BYTE bAlpha = 0xff;
	dwColor = (COLORREF)(((BYTE)(bColors[2])|((WORD)((BYTE)(bColors[1]))<<8))|(((DWORD)(BYTE)(bColors[0]))<<16)|(((DWORD)(BYTE)(bAlpha))<<24));
	return TRUE;
}

BOOL ColorUtil::AdjustColor(DWORD dwColor, DWORD &dwFixColor)
{
	BYTE bAlpha = 0xff;
	BYTE bRed = GetRValue(dwColor);
	BYTE bGreen = GetGValue(dwColor);
	BYTE bBlue = GetBValue(dwColor);
	dwFixColor = (COLORREF)(((BYTE)(bBlue)|((WORD)((BYTE)(bGreen))<<8))|(((DWORD)(BYTE)(bRed))<<16)|(((DWORD)(BYTE)(bAlpha))<<24));
	return TRUE;
}
#pragma warning(pop)
