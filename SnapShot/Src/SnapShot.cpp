// SnapShot.cpp : 定义应用程序的入口点。
//
#pragma warning(push)
#pragma warning(disable:4996)

#include <windows.h>
#include <Windowsx.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <list>
using namespace std;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#pragma	comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")
#include <Ole2.h>
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "comdlg32.lib")

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include <md5.h>
#include <StrUtil.h>
#include <DibUtil.h>
#include <Util.h>

#include <OptParse.h>
#include <KeyHook.h>

typedef LONG (__cdecl *LPFNAttatchHook)(HWND hInvoker);
typedef LONG (__cdecl *LPFNDetatchHook)();

#include "resource.h"
#include <RongDian.h>

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

#define USE_PALETTE		1

#define unsigned long ULONG_PTR, *PULONG_PTR

#ifdef _DEBUG
const wchar_t*	const	kRDKeyHook			= L"RDKeyHookd.dll";
#else
const wchar_t*	const	kRDKeyHook			= L"RDKeyHook.dll";
#endif
const char*		const	kAttatchHook		= "AttatchHook";
const char*		const	kDetatchHook		= "DetatchHook";
const wchar_t*	const	ARGS_HWND			= L"-h";
const wchar_t*	const	ARGS_LONG_HWND		= L"--hwnd";
const wchar_t*	const	ARGS_HWND_DESC		= L"SnapShot Invoker's HWND.";
const wchar_t*	const	ARGS_PATH			= L"-p";
const wchar_t*	const	ARGS_LONG_PATH		= L"--path";
const wchar_t*	const	ARGS_PATH_DESC		= L"Image Saving Folder.";
const wchar_t*	const	ARGS_MODE			= L"-m";
const wchar_t*	const	ARGS_LONG_MODE		= L"--mode";
const wchar_t*	const	ARGS_MODE_DESC		= L"SnapShot Mode(-m f: Full Screen Mode, -m m: Magnifier mode)";
const wchar_t*	const	ARGS_DEBUG			= L"-d";
const wchar_t*	const	ARGS_LONG_DEBUG		= L"--debug";
const wchar_t*	const	ARGS_DEBUG_DESC		= L"Popup a Message Box for Visual Studio to Attach.";

const int				SNAPSHOT_TIMER		= 1000;
const int				SNAPSHOT_ELAPSE		= 10;
const int				SNAPSHOT_WIDTH		= 160;
const int				SNAPSHOT_HEIGHT		= 164;
const int				PREVIEW_HEIGHT		= 100;

typedef enum _tagSNAPSHOT_MODE
{
	SNAPSHOT_FULLSCREEN = 0,
	SNAPSHOT_MAGNIFIER
} SNAPSHOT_MODE, *PSNAPSHOT_MODE, *LPSNAPSHOT_MODE;

class WinInfo
{
public:
	WinInfo() { Init(); };
	WinInfo(HWND hwnd, LPCWSTR lpszName, LPCWSTR lpszText, const RECT rect)
	{
		SetHWND(hwnd);
		SetName(lpszName);
		SetText(lpszText);
		SetRect(rect);
	};
	~WinInfo() { };

public:
	void Init()
	{
		hWnd = NULL;
		memset(szText, 0, sizeof(szText));
		memset(szName, 0, sizeof(szName));
		rtRect.left = 0;
		rtRect.top = 0;
		rtRect.right = 0;
		rtRect.bottom = 0;
	};
	HWND		GetHWND() { return hWnd; };
	LPCWSTR		GetText() { return szText; };
	LPCWSTR		GetName() { return szName; };
	void		GetRect(LPRECT pRect)
	{
		if(pRect)
		{
			pRect->left = rtRect.left;
			pRect->top = rtRect.top;
			pRect->right = rtRect.right;
			pRect->bottom = rtRect.bottom;
		}
	};
	void			SetHWND(HWND hwnd) { hWnd = hwnd; };
	void			SetText(LPCWSTR lpszText)
	{
		if(!lpszText)
			return;
		memset(szText, 0, sizeof(szText));
		wcscpy(szText, lpszText);
	};
	void			SetName(LPCWSTR lpszName)
	{
		if(!lpszName)
			return;
		memset(szName, 0, sizeof(szName));
		wcscpy(szName, lpszName);
	};
	void			SetRect(int nLeft, int nTop, int nRight, int nBottom)
	{
		rtRect.left = nLeft;
		rtRect.top = nTop;
		rtRect.right = nRight;
		rtRect.bottom = nBottom;
	};
	void			SetRect(const RECT rect)
	{
		rtRect.left = rect.left;
		rtRect.top = rect.top;
		rtRect.right = rect.right;
		rtRect.bottom = rect.bottom;
	};

private:
	HWND		hWnd;
	wchar_t		szText[MAX_PATH];
	wchar_t		szName[MAX_PATH];
	RECT		rtRect;
};

// 全局变量:
Options				g_OptOptions;
Parser				g_OptParser;
HINSTANCE			g_hInst = NULL;
HWND				g_hMainHwnd = NULL;
list<WinInfo *>		g_lstWndInfo;
CRITICAL_SECTION	g_csWndInfo;
ULONG_PTR			g_ulGdiToken;

Bitmap				*g_pCleanBitmap = NULL;
Bitmap				*g_pBkBitmap = NULL;
Bitmap				*g_pRegionBitmap = NULL;
TextureBrush		*g_pCleanBrush = NULL;
TextureBrush		*g_pBkBrush = NULL;
TextureBrush		*g_pRegionBrush = NULL;
Pen					*g_pBorderPen = NULL;
Pen					*g_pCrossPen = NULL;
RECT				g_rtCropRegion = { 0, 0, 0, 0};
POINT				g_ptMouse = { 0, 0 };
POINT				g_ptLastMouse = { 0, 0 };
POINT				g_ptStart = { 0, 0 };
POINT				g_ptEnd = { 0, 0 };
bool				g_bIsMouseDown = false;
bool				g_bIsDrawRegion = false;

DWORD				g_BorderColor = 0xFF6266EE; // ARGB
float				g_fBorderWidth = 1.0;

int					g_nSnapShotRounder = 2;
int					g_nSnapShotScale = 4;
int					g_nSnapShotOffset = 20;
BOOL				g_bRecordColor = FALSE;

int					g_nSnapShotMode = SNAPSHOT_MAGNIFIER;
HWND				g_hInvoker = NULL;
wchar_t				g_szSnapShotPath[1024] = { 0 };
wchar_t				g_szSnapShotFile[1024] = { 0 };
DWORD				g_dwCurColor = 0;

HPALETTE			g_hSnapShotPal = NULL;
HMODULE				g_hHookModule = NULL;
HHOOK				g_hHook = NULL;
LPFNAttatchHook		g_lpfnAttatchHook = NULL;
LPFNDetatchHook		g_lpfnDetatchHook = NULL;


void SetupPalette()
{
#if USE_PALETTE
	if(g_hSnapShotPal)
		DeleteObject(g_hSnapShotPal);
	HDC hDC = CreateDCW(L"DISPLAY", NULL, NULL, NULL);
	if(!hDC)
		return;
	if(GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE)
	{
		WORD nColors = 1 << GetDeviceCaps(hDC, BITSPIXEL);
		LPLOGPALETTE lpLogPal = NULL;
		lpLogPal = (LPLOGPALETTE)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));
		lpLogPal->palNumEntries = nColors;
		lpLogPal->palVersion = 0x300;
		GetSystemPaletteEntries(hDC, 0, nColors, lpLogPal->palPalEntry);
		g_hSnapShotPal = CreatePalette(lpLogPal);
		free(lpLogPal);
	}
	else
	{
		g_hSnapShotPal = NULL;
	}
	DeleteDC(hDC);
#endif
}

/*
image/bmp
image/jpeg
image/gif
image/tiff
image/png
*/
int GetEncoderClsid(LPCTSTR lpszFormat, CLSID* lpCLSID)
{
	UINT uNum = 0; // number of image encoders
	UINT uSize = 0; // size of the image encoder array in bytes
	GetImageEncodersSize(&uNum, &uSize);
	if(uSize == 0)
		return -1; // Failure

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(uSize));
	if(pImageCodecInfo == NULL)
		return -1; // Failure
	GetImageEncoders(uNum, uSize, pImageCodecInfo);
	for(UINT i=0; i<uNum; ++i)
	{
		if(wcsicmp(pImageCodecInfo[i].MimeType, lpszFormat) == 0)
		{
			*lpCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return i; // Success
		}    
	}
	free(pImageCodecInfo);
	return -1;  // Failure
}

// 
INT64 BitmapToBuffer(Bitmap *pBitmap, LPCTSTR lpszFormat, char **lppData)
{
	if(!pBitmap)
		return -1;
	CLSID clsid;
	IStream* pIStream = NULL;
	ULARGE_INTEGER ulSize = {0};
	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
		return -1;
	if(GetEncoderClsid(lpszFormat, &clsid))
	{
		// Save the image to the stream
		Status nSaveStatus = pBitmap->Save(pIStream, &clsid);
		if(nSaveStatus == Ok)
		{
			// get the size of the stream
			LARGE_INTEGER lnOffset = {0};
			if(pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulSize) == S_OK)
			{
				int nLen = (int)ulSize.QuadPart;
				pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL);
				char *pData = new char[nLen];
				memset(pData, 0, (size_t)nLen);
				ULONG ulRead = 0;
				if(pIStream->Read(pData, nLen, &ulRead) == S_OK)
					*lppData = pData;
			}
		}
	}
	pIStream->Release();
	return ulSize.QuadPart;
}

//
HANDLE BitmapToClipBoard(HBITMAP hBitmap)
{
	if(!::OpenClipboard(NULL))
		return NULL;
	::EmptyClipboard();

	HANDLE hDib = NULL;
	HPALETTE ghPal = NULL;
	HANDLE hResult = NULL;
	hDib = BitmapToDIB(hBitmap, ghPal);
	if (hDib)
		hResult = ::SetClipboardData(CF_DIB, hDib);
	::CloseClipboard();
	return hResult;
}

//
void SwapLeftRight(LPRECT lpRect)
{
	LONG temp = lpRect->left; 
	lpRect->left = lpRect->right; 
	lpRect->right = temp;
}

//
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	RECT *pRect = (RECT*)lParam;
	if(::IsWindowVisible(hWnd))
	{
		wchar_t szName[MAX_PATH] = { 0 };
		wchar_t szText[MAX_PATH] = { 0 };
		RECT rtRect = { 0,0,0,0 };
		RECT rtDest = { 0,0,0,0 };
		::GetClassName(hWnd, szName, MAX_PATH);
		::GetWindowText(hWnd, szText, MAX_PATH);
		::GetWindowRect(hWnd, &rtRect);
		IntersectRect(&rtDest, pRect, &rtRect);
		WinInfo *pWinInfo = NULL;
		if(!IsRectEmpty(&rtDest))
			pWinInfo = new WinInfo(hWnd, szName, szText, rtDest);
		EnumChildWindows(hWnd, EnumWindowsProc, (LPARAM)lParam);
		if(!IsRectEmpty(&rtDest) && pWinInfo)
		{
			::EnterCriticalSection(&g_csWndInfo);
			g_lstWndInfo.push_back(pWinInfo);
			::LeaveCriticalSection(&g_csWndInfo);
			return TRUE;
		}
	}
	return TRUE;
}

//
bool IsDragging(POINT ptPoint)
{
	int nCheckSize = 15;
	return g_bIsMouseDown && (ptPoint.x > g_ptStart.x + nCheckSize || ptPoint.y > g_ptStart.y + nCheckSize ||
		ptPoint.x < g_ptStart.x - nCheckSize || ptPoint.y < g_ptStart.y - nCheckSize);
}

//
GraphicsPath* GetCapsule(Rect rtBaseRect)
{
	float fDiameter = 0.0;
	GraphicsPath *pGraphicsPath = new GraphicsPath();
	if (rtBaseRect.Width > rtBaseRect.Height)
	{
		// return horizontal capsule
		fDiameter = (float)rtBaseRect.Height;
		RectF rtArc((REAL)rtBaseRect.GetLeft(), (REAL)rtBaseRect.GetTop(), fDiameter, fDiameter);
		pGraphicsPath->AddArc(rtArc, 90, 180);
		rtArc.X = rtBaseRect.GetRight() - fDiameter;
		pGraphicsPath->AddArc(rtArc, 270, 180);
	}
	else if (rtBaseRect.Width < rtBaseRect.Height)
	{
		// return vertical capsule
		fDiameter = (float)rtBaseRect.Width;
		RectF rtArc((REAL)rtBaseRect.GetLeft(), (REAL)rtBaseRect.GetTop(), fDiameter, fDiameter);
		pGraphicsPath->AddArc(rtArc, 180, 180);
		rtArc.Y = rtBaseRect.GetBottom() - fDiameter;
		pGraphicsPath->AddArc(rtArc, 0, 180);
	}
	else
	{
		// return circle
		pGraphicsPath->AddEllipse(rtBaseRect);
	}

	return pGraphicsPath;
}

//
GraphicsPath* GetRoundedRectangle(Rect rtBaseRect, float fRadius)
{
	GraphicsPath *pGraphicsPath = NULL;
	if (fRadius <= 0.0f)
	{
		pGraphicsPath = new GraphicsPath();
		if(!pGraphicsPath)
			return NULL;
		pGraphicsPath->AddRectangle(rtBaseRect);
		pGraphicsPath->CloseFigure();
		return pGraphicsPath;
	}

	// if the corner radius is greater than or equal to
	// half the width, or height (whichever is shorter)
	// then return a capsule instead of a lozenge
	if (fRadius >= (min(rtBaseRect.Width, rtBaseRect.Height) / 2.0f))
		return GetCapsule(rtBaseRect);

	// create the arc for the rectangle sides and declare
	// a graphics path object for the drawing
	float fDiameter = fRadius * 2.0F;
	RectF rtArc((REAL)rtBaseRect.GetLeft(), (REAL)rtBaseRect.GetTop(), fDiameter, fDiameter);
	pGraphicsPath = new GraphicsPath();
	if(!pGraphicsPath)
		return NULL;

	// top left arc
	pGraphicsPath->AddArc(rtArc, 180, 90);

	// top right arc
	rtArc.X = rtBaseRect.GetRight() - fDiameter;
	pGraphicsPath->AddArc(rtArc, 270, 90);

	// bottom right arc
	rtArc.Y = rtBaseRect.GetBottom() - fDiameter;
	pGraphicsPath->AddArc(rtArc, 0, 90);

	// bottom left arc
	rtArc.X = (REAL)rtBaseRect.GetLeft();
	pGraphicsPath->AddArc(rtArc, 90, 90);
	pGraphicsPath->CloseFigure();

	return pGraphicsPath;
}

//
void FixScreenCoordinates(RECT rtRect, RECT *lprtFixRect)
{
	if(lprtFixRect)
	{
		int screenX = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
		int screenY = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
		lprtFixRect->left = rtRect.left - screenX;
		lprtFixRect->top = rtRect.top - screenY;
		lprtFixRect->right = lprtFixRect->left + rtRect.right - rtRect.left;
		lprtFixRect->bottom = lprtFixRect->top + rtRect.bottom - rtRect.top;
	}
}

//
void DrawTooltip(HWND hWnd, HDC hDC, Point ptOffset, LPCWSTR lpszText)
{
	RECT rtClient = {0};
	HWND hScreen = GetDesktopWindow();
	if(!hScreen)
		return;
	::GetClientRect(hScreen, &rtClient);
	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
	POINT ptMouse = {0, 0};
	if(g_nSnapShotMode==SNAPSHOT_FULLSCREEN)
		ptMouse = g_ptMouse;
	Rect rtLabel(ptMouse.x + ptOffset.X, ptMouse.y + ptOffset.Y, SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT);
	if (rtLabel.GetRight() > rtClient.right - 5)
		rtLabel.X = ptMouse.x - ptOffset.X - rtLabel.Width;
	if (rtLabel.GetBottom() > rtClient.bottom - 5)
		rtLabel.Y = ptMouse.y - ptOffset.Y - rtLabel.Height;
	Rect rtPath(rtLabel.X, rtLabel.Y, (INT)(rtLabel.Width-g_fBorderWidth*2), (INT)(rtLabel.Height-g_fBorderWidth*2));
	GraphicsPath *pGraphicsPath = GetRoundedRectangle(rtPath, (float)g_nSnapShotRounder);
	LinearGradientBrush lgbBrush(Point(rtLabel.X, rtLabel.Y), Point(rtLabel.X + rtLabel.Width, rtLabel.Y), Color(200, 0, 0, 0), Color(100, 0, 0, 0));

	int nScale = g_nSnapShotScale;
	int nSrcLeft = 0, nSrcTop = 0;
	if(g_ptMouse.x + (SNAPSHOT_WIDTH/(nScale*2)) > rtClient.right)
		nSrcLeft = rtClient.right - (SNAPSHOT_WIDTH/(nScale));
	else
		nSrcLeft = g_ptMouse.x - (SNAPSHOT_WIDTH/(nScale*2));
	if(g_ptMouse.y + (PREVIEW_HEIGHT/(nScale*2)) > rtClient.bottom)
		nSrcTop = rtClient.bottom - (PREVIEW_HEIGHT/(nScale));
	else
		nSrcTop = g_ptMouse.y - (PREVIEW_HEIGHT/(nScale*2));
	if(nSrcLeft<0)
		nSrcLeft = 0;
	if(nSrcTop<0)
		nSrcTop = 0;
	if(g_nSnapShotMode==SNAPSHOT_MAGNIFIER)
	{
		HDC hScreenHDC = GetDC(NULL);
		StretchBlt(hDC, rtLabel.X, rtLabel.Y, SNAPSHOT_WIDTH, PREVIEW_HEIGHT, 
			hScreenHDC, nSrcLeft, nSrcTop, SNAPSHOT_WIDTH/nScale, PREVIEW_HEIGHT/nScale, SRCCOPY);
		ReleaseDC(NULL, hScreenHDC);
		RECT rtSnapShot;
		GetClientRect(hWnd, &rtSnapShot);
		rtSnapShot.top += PREVIEW_HEIGHT;
		HBRUSH hTextBrush = CreateSolidBrush(RGB(80, 100, 120));
		FillRect(hDC, &rtSnapShot, hTextBrush);
		DeleteObject(hTextBrush);
	}
	else
	{
		//StretchBlt(hDC, rtLabel.X, rtLabel.Y, SNAPSHOT_WIDTH-g_fBorderWidth*2, PREVIEW_HEIGHT, 
		//	hDC, nSrcLeft, nSrcTop, SNAPSHOT_WIDTH/nScale, PREVIEW_HEIGHT/nScale, SRCCOPY);
	}
	Graphics *pGraphics = Graphics::FromHDC(hDC);
	pGraphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
	pGraphics->SetSmoothingMode(SmoothingModeHighSpeed);
	pGraphics->FillRectangle(&lgbBrush, rtLabel.X, rtLabel.Y + PREVIEW_HEIGHT, (INT)(rtLabel.Width-g_fBorderWidth*2), (INT)(SNAPSHOT_HEIGHT - PREVIEW_HEIGHT-g_fBorderWidth*2));
	pGraphics->DrawPath(g_pBorderPen, pGraphicsPath);
	Point pt1, pt2;
	pt1.X = rtLabel.X + (rtLabel.Width)/2;
	pt1.Y = rtLabel.Y;
	pt2.X = rtLabel.X + (rtLabel.Width)/2;
	pt2.Y = (INT)(rtLabel.Y + PREVIEW_HEIGHT - g_fBorderWidth);
	pGraphics->DrawLine(g_pCrossPen, pt1, pt2);
	pt1.X = rtLabel.X;
	pt1.Y = rtLabel.Y + 50;
	pt2.X = (INT)(rtLabel.X + rtLabel.Width - g_fBorderWidth*2);
	pt2.Y = rtLabel.Y + 50;
	pGraphics->DrawLine(g_pCrossPen, pt1, pt2);
	SolidBrush textBrush((ARGB)Color::White);
	pGraphics->DrawString(lpszText, -1, &font, PointF((REAL)(rtLabel.X + 5), (REAL)(rtLabel.Y + PREVIEW_HEIGHT + 5)), NULL, &textBrush);
	delete pGraphics;
}

//
void DrawRectangle(HWND hWnd, HDC hDC)
{
	RECT rtClient = {0};
	::GetClientRect(hWnd, &rtClient);
	Graphics *pGraphics = Graphics::FromHDC(hDC);
	pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
	pGraphics->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	Pen pen((ARGB)Color::Red, 2);
	pGraphics->DrawRectangle(&pen, g_rtCropRegion.left, g_rtCropRegion.top, g_rtCropRegion.right-g_rtCropRegion.left, g_rtCropRegion.bottom-g_rtCropRegion.top);
}

//
void CancelSnapShot()
{
	g_rtCropRegion.left = 0;
	g_rtCropRegion.top = 0;
	g_rtCropRegion.right = 0;
	g_rtCropRegion.bottom = 0;
	g_ptEnd.x = 0;
	g_ptEnd.y = 0;

	PostMessage(g_hMainHwnd, WM_CLOSE, 0, 0);
}

// 
void SnapShot()
{
	if(g_pBkBitmap)
	{
		Bitmap *pBkBitmap = g_pBkBitmap->Clone(g_rtCropRegion.left, g_rtCropRegion.top, g_rtCropRegion.right-g_rtCropRegion.left, g_rtCropRegion.bottom-g_rtCropRegion.top, PixelFormat24bppRGB);
		if(!pBkBitmap)
			return;

		char *lpData = NULL;
		bool bPngFormat = true;
		INT64 llSize = BitmapToBuffer(pBkBitmap, L"image/png", &lpData);
		if(llSize>(512 * 1024))
		{
			if(lpData)
			{
				delete lpData;
				lpData = NULL;
			}
			bPngFormat = false;
			llSize = BitmapToBuffer(pBkBitmap, L"image/jpeg", &lpData);
		}
		if(llSize>0 && lpData)
		{
			HBITMAP hBitmap = NULL;
			Color colBack;
			pBkBitmap->GetHBITMAP(colBack, &hBitmap);
			BitmapToClipBoard(hBitmap);

			time_t _time = time(NULL);
			tm t;
			Utility::TimeToTm(_time, &t);
			if(bPngFormat)
				swprintf(g_szSnapShotFile, L"%s\\ss_%04d_%02d_%02d_%02d_%02d_%02d.png", 
					g_szSnapShotPath, t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
			else
				swprintf(g_szSnapShotFile, L"%s\\ss_%04d_%02d_%02d_%02d_%02d_%02d.jpeg", 
					g_szSnapShotPath, t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
			char szPath_a[1024] = {0};
			StrUtil::w2a(g_szSnapShotFile, szPath_a);
		
			FILE *file = fopen(szPath_a, "wb");
			if(file)
			{
				fwrite(lpData, (size_t)llSize, 1, file);
				fclose(file);
			}
			delete lpData;
			lpData = NULL;
		}
	}
	CancelSnapShot();
}

//
LRESULT OnCreate(HWND hWnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)lParam;

	// Vista/Win7，由于UAC的限制，低权限进程不能向高权限进程发送消息
	// 如果我们想容许一个消息可以发送给较高特权等级的进程
	// 我们可以在较高特权等级的进程中调用ChangeWindowMessageFilter(Vista以上的API)函数，以MSGFLT_ADD作为参数将消息添加进消息过滤器的白名单。
	PFNChangeWindowMessageFilter pfnChangeWindowMessageFilter = NULL;
	HMODULE hModule = LoadLibrary(L"user32.dll");
	if(hModule)
	{
		pfnChangeWindowMessageFilter = (PFNChangeWindowMessageFilter)GetProcAddress(hModule, "ChangeWindowMessageFilter");
		if(pfnChangeWindowMessageFilter)
		{
#ifndef MSGFLT_ADD
#define MSGFLT_ADD		1
#define MSGFLT_REMOVE	2
#endif
#ifndef WM_COPYGLOBALDATA
#define WM_COPYGLOBALDATA 0x0049
#endif
			pfnChangeWindowMessageFilter(WM_COPYGLOBALDATA, MSGFLT_ADD);
			pfnChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
			pfnChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
		}
		FreeLibrary(hModule);
	}

	SetupPalette();

	GetCursorPos(&g_ptMouse);

	::InitializeCriticalSection(&g_csWndInfo);

	if(g_nSnapShotMode==SNAPSHOT_MAGNIFIER)
	{
		HRGN hRgn = CreateRoundRectRgn(0, 0, SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT, g_nSnapShotRounder, g_nSnapShotRounder);
		SetWindowRgn(hWnd, hRgn, TRUE);
		DeleteObject(hRgn);

		wchar_t szMoudle[1024] = {0};
		GetModuleFileName(NULL, szMoudle, _countof(szMoudle));
		PathRemoveFileSpec(szMoudle);
		PathAppend(szMoudle, kRDKeyHook);
		if(PathFileExists(szMoudle))
		{
			g_hHookModule = LoadLibraryW(szMoudle);
			if(g_hHookModule)
			{
				g_lpfnAttatchHook = (LPFNAttatchHook)GetProcAddress(g_hHookModule, kAttatchHook);
				g_lpfnDetatchHook = (LPFNDetatchHook)GetProcAddress(g_hHookModule, kDetatchHook);
				if(g_lpfnAttatchHook)
					g_lpfnAttatchHook(hWnd);
			}
		}
	}
	else
	{
		RECT rtDesktop = {0};
		HWND hDeskTop = GetDesktopWindow();
		if(!hDeskTop)
			return 0;
		::GetClientRect(hDeskTop, &rtDesktop);
		EnumWindows(EnumWindowsProc, (LPARAM)&rtDesktop);
		Bitmap *pDesktopBitmap = new Bitmap(rtDesktop.right-rtDesktop.left, rtDesktop.bottom-rtDesktop.top, PixelFormat24bppRGB);
		Graphics *graphics = Graphics::FromImage(pDesktopBitmap);
		HDC hdcSrc = GetWindowDC(hDeskTop);
		HDC hdcDest = graphics->GetHDC();
		::BitBlt(hdcDest, 0, 0, rtDesktop.right-rtDesktop.left, rtDesktop.bottom-rtDesktop.top, hdcSrc, rtDesktop.left, rtDesktop.top, SRCCOPY | CAPTUREBLT);
		ReleaseDC(hDeskTop, hdcSrc);
		graphics->ReleaseHDC(hdcDest);

		g_pCleanBitmap = pDesktopBitmap->Clone(0, 0, pDesktopBitmap->GetWidth(), pDesktopBitmap->GetHeight(), PixelFormat24bppRGB);
		g_pBkBitmap = g_pCleanBitmap->Clone(0, 0, pDesktopBitmap->GetWidth(), pDesktopBitmap->GetHeight(), PixelFormat24bppRGB);
		g_pRegionBitmap = g_pCleanBitmap->Clone(0, 0, pDesktopBitmap->GetWidth(), pDesktopBitmap->GetHeight(), PixelFormat24bppRGB);
		delete pDesktopBitmap;
		delete graphics;

		g_pCleanBrush = new TextureBrush(g_pCleanBitmap);
		g_pBkBrush = new TextureBrush(g_pBkBitmap);
		g_pRegionBrush = new TextureBrush(g_pRegionBitmap);

		Graphics *pGraphicsRegion = Graphics::FromImage(g_pRegionBitmap);
		pGraphicsRegion->SetSmoothingMode(SmoothingModeHighQuality);

		ImageAttributes *pImageAttr = new ImageAttributes();
		float fPerc = (float)15 / 100;
		//const float rw = 0.3086f;
		//const float gw = 0.6094f;
		//const float bw = 0.0820f;
		ColorMatrix cmColormatrix = {
			1, 0, 0, 0, 0,
			0, 1, 0, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, 1, 0,
			fPerc, fPerc, fPerc, 0, 1
		};
		pImageAttr->SetColorMatrix(&cmColormatrix);
		pGraphicsRegion->DrawImage(g_pCleanBitmap, RectF(0.0, 0.0, (REAL)g_pRegionBitmap->GetWidth(), (REAL)g_pRegionBitmap->GetHeight()), 
			0, 0, (REAL)g_pRegionBitmap->GetWidth(), (REAL)g_pRegionBitmap->GetHeight(), UnitPixel, pImageAttr);
		delete pImageAttr;
		delete pGraphicsRegion;
	}
	g_pCrossPen = new Pen(g_BorderColor, g_fBorderWidth);
	g_pBorderPen = new Pen(g_BorderColor);

	SetTimer(hWnd, SNAPSHOT_TIMER, SNAPSHOT_ELAPSE, NULL);

	return 0;
}

LRESULT OnEraseBkgnd(HWND hWnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(g_nSnapShotMode==SNAPSHOT_FULLSCREEN)
		return 1;
	RECT rtClient;
	GetClientRect(hWnd, &rtClient);
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	HDC hScreenDC = GetDC(NULL);
	int nOffset = g_nSnapShotOffset;
	int nScreenWidth = GetDeviceCaps(hScreenDC, HORZRES);
	int nScreenHeight = GetDeviceCaps(hScreenDC, VERTRES);
	DWORD dwColor = GetPixel(hScreenDC, ptCursor.x, ptCursor.y);
	ReleaseDC(NULL, hScreenDC);

	RECT rtLabel = { g_ptMouse.x + nOffset, g_ptMouse.y + nOffset, g_ptMouse.x + nOffset + SNAPSHOT_WIDTH, g_ptMouse.y + nOffset + SNAPSHOT_HEIGHT };
	if (rtLabel.right > nScreenWidth - 5)
	{
		rtLabel.left = g_ptMouse.x - nOffset - (rtLabel.right - rtLabel.left);
		rtLabel.right += SNAPSHOT_WIDTH;
	}
	if (rtLabel.bottom > nScreenHeight - 5)
	{
		rtLabel.top = g_ptMouse.y - nOffset - (rtLabel.bottom - rtLabel.top);
		rtLabel.bottom += SNAPSHOT_HEIGHT;
	}
	SetWindowPos(hWnd, HWND_TOPMOST, rtLabel.left, rtLabel.top, SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT, SWP_NOACTIVATE);

	HDC hDC = GetDC(hWnd);
	HDC hTempDC = ::CreateCompatibleDC(hDC);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hTempDC, hBitmap);

	wchar_t szText[MAX_PATH];
	wchar_t szColor[MAX_PATH];
	swprintf(szColor, L"RGB(%d,%d,%d)", GetRValue(dwColor), GetGValue(dwColor), GetBValue(dwColor));
	swprintf_s(szText, L"X: %d px, Y: %d px\nW: %d px, H: %d px\n%s", 
		ptCursor.x, ptCursor.y, g_rtCropRegion.right - g_rtCropRegion.left, g_rtCropRegion.bottom - g_rtCropRegion.top, szColor);
	DrawTooltip(hWnd, hTempDC, Point(0, 0), szText);

	/*BOOL bRet = */::BitBlt(hDC, 0, 0, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top, hTempDC, 0, 0, SRCCOPY);
	SelectObject(hTempDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hTempDC);
	ReleaseDC(hWnd, hDC);

	if(g_hInvoker && (((ptCursor.x!=g_ptLastMouse.x || ptCursor.y!=g_ptLastMouse.y) && dwColor != g_dwCurColor) || g_bRecordColor))
	{
		g_ptLastMouse.x = ptCursor.x;
		g_ptLastMouse.y = ptCursor.y;
		g_dwCurColor = dwColor;
		ZOOMIN_INFO info = {0};
		info.dwColor = dwColor;
		info.ptCursor.x = ptCursor.x;
		info.ptCursor.y = ptCursor.y;
		COPYDATASTRUCT data;
		if(g_bRecordColor)
			data.dwData = WM_ZOOMIN_COLOR_MSG;
		else
			data.dwData = WM_ZOOMIN_UPDATE_MSG;
		data.cbData = sizeof(SNAPSHOT_INFO);
		data.lpData = &info;
		g_bRecordColor = FALSE;
		::SendMessage(g_hInvoker, WM_COPYDATA, (WPARAM)(HWND)g_hMainHwnd, (LPARAM)(LPVOID)&data);
	}

	return 1;
}

LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(g_nSnapShotMode==SNAPSHOT_MAGNIFIER)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	PAINTSTRUCT psPaint;
	RECT rtClient;
	HDC hDC = BeginPaint(hWnd, &psPaint);
	GetClientRect(hWnd, &rtClient);
	HDC hTempDC = ::CreateCompatibleDC(hDC);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hTempDC, hBitmap);

	Graphics *pGraphics = Graphics::FromHDC(hTempDC);
	pGraphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
	pGraphics->SetSmoothingMode(SmoothingModeHighSpeed);
	// Draw background
	pGraphics->FillRectangle(g_pBkBrush, 0, 0, g_pBkBitmap->GetWidth(), g_pBkBitmap->GetHeight());
	pGraphics->FillRectangle(g_pRegionBrush, 0, 0, g_rtCropRegion.right - g_rtCropRegion.left, g_rtCropRegion.bottom - g_rtCropRegion.top);
	pGraphics->ReleaseHDC(hTempDC);
	DrawRectangle(hWnd, hTempDC);
	delete pGraphics;

	wchar_t szText[MAX_PATH];
	wchar_t szColor[MAX_PATH];
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	HDC hColorDC = hDC;
	g_dwCurColor = GetPixel(hColorDC, ptCursor.x, ptCursor.y);
	if(hColorDC!=hDC)
		ReleaseDC(NULL, hColorDC);
	swprintf(szColor, L"RGB(%d,%d,%d)", GetRValue(g_dwCurColor), GetGValue(g_dwCurColor), GetBValue(g_dwCurColor));
	swprintf_s(szText, L"X: %d px, Y: %d px\nW: %d px, H: %d px\n%s", 
		g_rtCropRegion.left, g_rtCropRegion.top, g_rtCropRegion.right - g_rtCropRegion.left, g_rtCropRegion.bottom - g_rtCropRegion.top, szColor);
	DrawTooltip(hWnd, hTempDC, Point(g_nSnapShotOffset, g_nSnapShotOffset), szText);

	/*BOOL bRet = */::BitBlt(hDC, 0, 0, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top, hTempDC, 0, 0, SRCCOPY);
	SelectObject(hTempDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hTempDC);
	EndPaint(hWnd, &psPaint);

	return 0;
}

LRESULT OnTimer(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
{
	UINT_PTR nIDEvent = (UINT_PTR)wParam;
	if(nIDEvent!=SNAPSHOT_TIMER)
		return 0;
	GetCursorPos(&g_ptMouse);
	if(g_nSnapShotMode==SNAPSHOT_MAGNIFIER)
	{
		if(IsWindowVisible(g_hMainHwnd))
			RedrawWindow(g_hMainHwnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE);
	}
	else
	{
		if (g_ptLastMouse.x == g_ptMouse.x && g_ptLastMouse.y == g_ptMouse.y)
			return 0;
		bool bIsDraging = IsDragging(g_ptMouse);
		if(bIsDraging)
		{
			//OutputDebugString(L"Mouse Down\n");
			g_ptEnd.x = g_ptMouse.x;
			g_ptEnd.y = g_ptMouse.y;
			if(g_ptStart.x > g_ptEnd.x)
			{
				g_rtCropRegion.left = g_ptEnd.x;
				g_rtCropRegion.right = g_ptStart.x;
				if(g_ptStart.y < g_ptEnd.y)
				{
					g_rtCropRegion.top = g_ptStart.y;
					g_rtCropRegion.bottom = g_ptEnd.y;
				}
				else
				{
					g_rtCropRegion.top = g_ptEnd.y;
					g_rtCropRegion.bottom = g_ptStart.y;
				}
			}
			else
			{
				g_rtCropRegion.left = g_ptStart.x;
				g_rtCropRegion.right = g_ptEnd.x;
				if(g_ptStart.y < g_ptEnd.y)
				{
					g_rtCropRegion.top = g_ptStart.y;
					g_rtCropRegion.bottom = g_ptEnd.y;
				}
				else
				{
					g_rtCropRegion.top = g_ptEnd.y;
					g_rtCropRegion.bottom = g_ptStart.y;
				}
			}
		}
		else if(!g_bIsDrawRegion)
		{
			//HWND hDeskTop = GetDesktopWindow();
			list<WinInfo *>::iterator it;
			WinInfo *pWinInfo = NULL;
			RECT rtWin = { 0,0,0,0 };
			RECT rtFix = { 0,0,0,0 };
			for(it=g_lstWndInfo.begin(); it!=g_lstWndInfo.end(); it++)
			{
				pWinInfo = (*it);
				if(pWinInfo)
				{
					pWinInfo->GetRect(&rtWin);
					if(::PtInRect(&rtWin, g_ptMouse))
					{
						if(pWinInfo->GetHWND()==g_hMainHwnd/* || pWinInfo->GetHWND()==hDeskTop*/)
							continue;
						FixScreenCoordinates(rtWin, &rtFix);
						g_rtCropRegion.left = rtFix.left;
						g_rtCropRegion.top = rtFix.top;
						g_rtCropRegion.right = rtFix.right;
						g_rtCropRegion.bottom = rtFix.bottom;
						break;
					} // end of 'if(::PtInRect(&rtWin, g_ptMouse))'
				} // end of 'if(pWinInfo)'
				rtWin.left = 0;
				rtWin.top = 0;
				rtWin.right = 0;
				rtWin.bottom = 0;
				rtFix.left = 0;
				rtFix.top = 0;
				rtFix.right = 0;
				rtFix.bottom = 0;
			} // end of 'for(it=g_lstWndInfo.begin(); it!=g_lstWndInfo.end(); it++)'
		} // end of 'else if(!g_bIsDrawRegion)'

		InvalidateRect(g_hMainHwnd, NULL, TRUE);
	}
	return 0;
}

LRESULT OnPaletteChanged(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	SetupPalette();
	return 1;
}

LRESULT OnCopyData(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
{
	PCOPYDATASTRUCT lpData = (PCOPYDATASTRUCT)lParam;
	if(lpData->dwData==WM_ZOOMIN_QUIT_MSG)
	{
		PostMessage(g_hMainHwnd, WM_CLOSE, 0, 0);
	}
	else if(lpData->dwData==WM_ZOOMIN_COLOR_MSG)
	{
		g_bRecordColor = TRUE;
	}
	else if(lpData->dwData==WM_ZOOMIN_MOVE_UP)
	{
		POINT pt = {0};
		GetCursorPos(&pt);
		SetCursorPos(pt.x,pt.y-1);
	}
	else if(lpData->dwData==WM_ZOOMIN_MOVE_DOWN)
	{
		POINT pt = {0};
		GetCursorPos(&pt);
		SetCursorPos(pt.x,pt.y+1);
	}
	else if(lpData->dwData==WM_ZOOMIN_MOVE_LEFT)
	{
		POINT pt = {0};
		GetCursorPos(&pt);
		SetCursorPos(pt.x-1,pt.y);
	}
	else if(lpData->dwData==WM_ZOOMIN_MOVE_RIGHT)
	{
		POINT pt = {0};
		GetCursorPos(&pt);
		SetCursorPos(pt.x+1,pt.y);
	}
	else if(lpData->dwData==WM_ZOOMIN_ZOOM_MSG&&g_nSnapShotScale<8)
	{
		g_nSnapShotScale++;
	}
	else if(lpData->dwData==WM_ZOOMIN_REDUCE_MSG&&g_nSnapShotScale>1)
	{
		g_nSnapShotScale--;
	}
	return 0;
}

LRESULT OnKeyUp(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
{
	UINT nChar = (UINT)wParam;
	switch(nChar)
	{
	case VK_ESCAPE:
		CancelSnapShot();
		break;
	case VK_RETURN:
		SnapShot();
		break;
	}
	return 0;
}

LRESULT OnLButtonDown(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
{
	g_bIsMouseDown = true;
	if(g_ptStart.x==0 && g_ptStart.y==0)
	{
		g_ptStart.x = GET_X_LPARAM(lParam);
		g_ptStart.y = GET_Y_LPARAM(lParam);
	}
	return 0;
}

LRESULT OnLButtonUp(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	g_bIsMouseDown = false;
	g_ptStart.x = 0;
	g_ptStart.y = 0;
	g_ptEnd.x = 0;
	g_ptEnd.y = 0;
	g_bIsDrawRegion = true;
	return 0;
}

LRESULT OnRButtonUp(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(g_bIsDrawRegion)
	{
		g_bIsDrawRegion = false;
	}
	else
	{
		g_bIsMouseDown = false;
		CancelSnapShot();
	}
	return 0;
}

LRESULT OnLButtonDblClk(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	SnapShot();
	return 0;
}

LRESULT OnDestroy(HWND hWnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	KillTimer(hWnd, SNAPSHOT_TIMER);
	DeleteCriticalSection(&g_csWndInfo);
	if(g_pCleanBitmap)
		delete g_pCleanBitmap;
	if(g_pBkBitmap)
		delete g_pBkBitmap;
	if(g_pRegionBitmap)
		delete g_pRegionBitmap;
	if(g_pCleanBrush)
		delete g_pCleanBrush;
	if(g_pBkBrush)
		delete g_pBkBrush;
	if(g_pRegionBrush)
		delete g_pRegionBrush;
	if(g_pBorderPen)
		delete g_pBorderPen;
	if(g_pCrossPen)
		delete g_pCrossPen;
	list<WinInfo*>::iterator iter;
	WinInfo* lpWinInfo = NULL;
	if(g_lstWndInfo.size())
		lpWinInfo = (WinInfo*)g_lstWndInfo.front();
	while(lpWinInfo)
	{
		g_lstWndInfo.pop_front();
		delete lpWinInfo;
		lpWinInfo = NULL;
		if(g_lstWndInfo.size())
			lpWinInfo = (WinInfo*)g_lstWndInfo.front();
		else
			break;
	}
	if(g_hHookModule)
	{
		if(g_lpfnDetatchHook)
			g_lpfnDetatchHook();
		FreeLibrary(g_hHookModule);
	}
	PostQuitMessage(0);
	return 0;
}

//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return OnCreate(hWnd, uMsg, wParam, lParam);
		break;
	case WM_ERASEBKGND:
		return OnEraseBkgnd(hWnd, uMsg, wParam, lParam);
		break;
	case WM_PAINT:
		OnPaint(hWnd, uMsg, wParam, lParam);
		break;
	case WM_TIMER:
		OnTimer(hWnd, uMsg, wParam, lParam);
		break;
	case WM_PALETTECHANGED:
		return OnPaletteChanged(hWnd, uMsg, wParam, lParam);
		break;
	case WM_COPYDATA:
		OnCopyData(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYUP:
		OnKeyUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_RBUTTONUP:
		OnRButtonUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_LBUTTONDBLCLK:
		OnLButtonDblClk(hWnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		OnDestroy(hWnd, uMsg, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

//
int OptParse(int nArgc, TCHAR* lpszArgv[])
{
	int nRetCode = E_OK;
	nRetCode = g_OptOptions.addOption(ARGS_HWND, ARGS_LONG_HWND, ARGS_HWND_DESC, OPT_NEEDARG, NULL);
	if(nRetCode!=E_OK)
		return nRetCode;
	nRetCode = g_OptOptions.addOption(ARGS_PATH, ARGS_LONG_PATH, ARGS_PATH_DESC, OPT_NEEDARG, NULL);
	if(nRetCode!=E_OK)
		return nRetCode;
	nRetCode = g_OptOptions.addOption(ARGS_MODE, ARGS_LONG_MODE, ARGS_MODE_DESC, OPT_NEEDARG, NULL);
	if(nRetCode!=E_OK)
		return nRetCode;

	g_OptParser.setFlags(OPT_PARSE_QUOTED_ARGS | OPT_PARSE_UNQUOTE_ARGS | OPT_PARSE_AUTO_HELP | OPT_PARSE_INCLUDE_ARGV0);
	nRetCode = g_OptParser.parse(nArgc, lpszArgv, g_OptOptions);

	return nRetCode;
}

//
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
	// 内存跟踪调试
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(192);
#endif

	// 解析启动参数
	int nCount = 0;
	LPWSTR* lpszArgs = CommandLineToArgvW(GetCommandLine(), &nCount);
	OptParse(nCount, lpszArgs);
	LocalFree(lpszArgs);

	BOOL bWaitForAttach = FALSE;
	if(g_OptOptions.isSet(ARGS_DEBUG) || g_OptOptions.isSet(ARGS_LONG_DEBUG))
		bWaitForAttach = TRUE;
	if(bWaitForAttach)
		MessageBox(NULL, NULL, NULL, MB_OK);

	wchar_t szMoudle[1024] = {0}, szPath[1024] = {0};
	GetModuleFileName(NULL, szMoudle, _countof(szMoudle));
	PathRemoveFileSpec(szMoudle);
	PathRemoveFileSpec(szMoudle);
	wcscpy(szPath, szMoudle);
	PathAppend(szPath, L"img");
	if(g_OptOptions.isSet(ARGS_PATH))
		wcscpy(g_szSnapShotPath, g_OptOptions.asString(ARGS_PATH).c_str());
	else if(g_OptOptions.isSet(ARGS_LONG_PATH))
		wcscpy(g_szSnapShotPath, g_OptOptions.asString(ARGS_LONG_PATH).c_str());
	else
		wcscpy(g_szSnapShotPath, szPath);
	if(!PathFileExists(g_szSnapShotPath))
		SHCreateDirectoryEx(NULL, g_szSnapShotPath, NULL);
	if(g_OptOptions.isSet(ARGS_HWND))
		g_hInvoker = (HWND)_wtol(g_OptOptions.asString(ARGS_HWND).c_str());
	else if(g_OptOptions.isSet(ARGS_LONG_HWND))
		g_hInvoker = (HWND)_wtol(g_OptOptions.asString(ARGS_LONG_HWND).c_str());

	wchar_t szMode[MAX_PATH] = {0};
	if(g_OptOptions.isSet(ARGS_MODE))
		wcscpy(szMode, g_OptOptions.asString(ARGS_MODE).c_str());
	else if(g_OptOptions.isSet(ARGS_LONG_MODE))
		wcscpy(szMode, g_OptOptions.asString(ARGS_LONG_MODE).c_str());
	if(wcslen(szMode) && _wcsicmp(szMode, L"m")==0)
		g_nSnapShotMode = SNAPSHOT_MAGNIFIER;
	else if(wcslen(szMode) && _wcsicmp(szMode, L"f")==0)
		g_nSnapShotMode = SNAPSHOT_FULLSCREEN;
	else
		g_nSnapShotMode = SNAPSHOT_FULLSCREEN;

	g_hInst = hInstance; // 将实例句柄存储在全局变量中

	GdiplusStartupInput gdipluStartupInput;
	GdiplusStartup(&g_ulGdiToken, &gdipluStartupInput, NULL);
	
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAPSHOT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= kSnapShotClass;
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAPSHOT));
	if(RegisterClassEx(&wcex)==0)
		return 0;

	RECT rtDesktop = {0};
	HWND hDeskTop = GetDesktopWindow();
	if(!hDeskTop)
		return 0;
	::GetClientRect(hDeskTop, &rtDesktop);
	if(g_nSnapShotMode==SNAPSHOT_MAGNIFIER)
	{
		POINT pt = {0};
		GetCursorPos(&pt);
		int nOffset = g_nSnapShotOffset;
		RECT rtLabel = { pt.x + nOffset, pt.y + nOffset, pt.x + nOffset + SNAPSHOT_WIDTH, pt.y + nOffset + SNAPSHOT_HEIGHT };
		if (rtLabel.right > rtDesktop.right - 5)
		{
			rtLabel.left = pt.x - nOffset - (rtLabel.right - rtLabel.left);
			rtLabel.right += SNAPSHOT_WIDTH;
		}
		if (rtLabel.bottom > rtDesktop.bottom - 5)
		{
			rtLabel.top = pt.y - nOffset - (rtLabel.bottom - rtLabel.top);
			rtLabel.bottom += SNAPSHOT_HEIGHT;
		}
		g_hMainHwnd = CreateWindowEx(0, kSnapShotClass, L"", WS_POPUP, rtLabel.left, rtLabel.top, SNAPSHOT_WIDTH, SNAPSHOT_HEIGHT, NULL, NULL, g_hInst, NULL);
		if (!g_hMainHwnd)
			return 0;
		::SetWindowLong(g_hMainHwnd, GWL_EXSTYLE, GetWindowLong(g_hMainHwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		::SetWindowPos(g_hMainHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(g_hMainHwnd, SW_SHOWNOACTIVATE);
	}
	else
	{
		g_hMainHwnd = CreateWindowEx(0, kSnapShotClass, L"", WS_POPUP, rtDesktop.left, rtDesktop.top, rtDesktop.right-rtDesktop.left, rtDesktop.bottom-rtDesktop.top, NULL, NULL, hInstance, NULL);
		if (!g_hMainHwnd)
			return 0;
		::SetWindowLong(g_hMainHwnd, GWL_EXSTYLE, GetWindowLong(g_hMainHwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		::SetWindowPos(g_hMainHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(g_hMainHwnd, nCmdShow);
	}
	UpdateWindow(g_hMainHwnd);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(g_ulGdiToken);

	if(IsWindow(g_hInvoker))
	{
		SNAPSHOT_INFO info = {0};
		info.dwColor = g_dwCurColor;
		wcscpy(info.szPath, g_szSnapShotFile);
		COPYDATASTRUCT data;
		data.dwData = WM_SNAPSHOT_MSG;
		data.cbData = sizeof(SNAPSHOT_INFO);
		data.lpData = &info;
		/*LRESULT lRes = */::SendMessage(g_hInvoker, WM_COPYDATA, (WPARAM)(HWND)g_hMainHwnd, (LPARAM)(LPVOID)&data);
	}

	return (int) msg.wParam;
}
#pragma warning(push)




