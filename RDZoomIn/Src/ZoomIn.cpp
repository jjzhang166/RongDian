// ZoomIn.cpp : 定义应用程序的入口点。
//
#define STRICT
#include <windows.h>
#include <Windowsx.h>

#pragma	comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define USE_PALETTE		1

#include <OptParse.h>
#include <KeyHook.h>

typedef LONG (__cdecl *LPAttatchHook)(HWND hInvoker);
typedef LONG (__cdecl *LPDetatchHook)();

#include "resource.h"
#include "RongDian.h"

#ifdef _DEBUG
const wchar_t*	const	kRDKeyHook = L"RDKeyHookd.dll";
#else
const wchar_t*	const	kRDKeyHook = L"RDKeyHook.dll";
#endif
const char*		const	kAttatchHook = "AttatchHook";
const char*		const	kDetatchHook = "DetatchHook";
const wchar_t*	const	ARGS_SHORT_HWND = L"-h";
const wchar_t*	const	ARGS_LONG_HWND = L"--hwnd";
const wchar_t*	const	ARGS_HWND_DESC = L"调用取色模块的窗口句柄";
const int				ZOOMIN_TIMER = 1000;
const int				ZOOMIN_ELAPSE = 10;
int						ZOOMIN_WIDTH = 120;
int						ZOOMIN_HEIGHT = 120;

Options					g_OptOptions;
Parser					g_OptParser;
HINSTANCE				g_hInst = NULL;
HWND					g_hZoomHwnd = NULL;
HWND					g_hInvoker = NULL;
HHOOK					g_hHook = NULL;
HPALETTE				g_hZoomPal = NULL;
HPEN					g_hPen = NULL;
HBRUSH					g_hBorder = NULL;
COLORREF				g_clrBorder = 0xEE6662; // BGR
int						g_nOffset = 20;
int						g_nZoomScale = 4;
int						g_nBorderWidth = 1;
int						g_nZoomRounder = 2;
POINT					g_lastPoint = {0};
COLORREF				g_lastColor = 0;
BOOL					g_bRecordColor = FALSE;
HMODULE					g_hHookModule = NULL;
LPAttatchHook			g_lpAttatchHook = NULL;
LPDetatchHook			g_lpDetatchHook = NULL;

void SetupPalette()
{
#if USE_PALETTE
	if(g_hZoomPal)
		DeleteObject(g_hZoomPal);
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
		g_hZoomPal = CreatePalette(lpLogPal);
		free(lpLogPal);
	}
	else
	{
		g_hZoomPal = NULL;
	}
	DeleteDC(hDC);
#endif
}

BOOL UpdateZoomWindow()
{
	HDC hDC = GetDC(NULL);
	if(!hDC)
		return FALSE;
	POINT pt = {0};
	GetCursorPos(&pt);
	DWORD dwColor = GetPixel(hDC, pt.x, pt.y);
	int nOffset = g_nOffset;
	int nScreenWidth = GetDeviceCaps(hDC, HORZRES);
	int nScreenHeight = GetDeviceCaps(hDC, VERTRES);
	RECT rtLabel = { pt.x + nOffset, pt.y + nOffset, pt.x + nOffset + ZOOMIN_WIDTH, pt.y + nOffset + ZOOMIN_HEIGHT };
	if (rtLabel.right > nScreenWidth - 5)
	{
		rtLabel.left = pt.x - nOffset - (rtLabel.right - rtLabel.left);
		rtLabel.right += ZOOMIN_WIDTH;
	}
	if (rtLabel.bottom > nScreenHeight - 5)
	{
		rtLabel.top = pt.y - nOffset - (rtLabel.bottom - rtLabel.top);
		rtLabel.bottom += ZOOMIN_HEIGHT;
	}
	SetWindowPos(g_hZoomHwnd, HWND_TOPMOST, rtLabel.left, rtLabel.top, ZOOMIN_WIDTH, ZOOMIN_HEIGHT, SWP_NOACTIVATE);
	
	HDC hZoomDC = GetDC(g_hZoomHwnd);
	HDC hMemDC = CreateCompatibleDC(hZoomDC);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(hZoomDC, ZOOMIN_WIDTH, ZOOMIN_HEIGHT);
	HPALETTE hOldZoomPal = NULL, hOldScrPal = NULL, hOldMemPal = NULL;
	HBITMAP hOldMemBitmap = NULL;
	HPEN hOldPen = NULL;
	if(g_hZoomPal)
	{
		hOldScrPal = SelectPalette(hDC, g_hZoomPal, FALSE);
		hOldZoomPal = SelectPalette(hZoomDC, g_hZoomPal, FALSE);
		hOldMemPal = SelectPalette(hMemDC, g_hZoomPal, FALSE);
		RealizePalette(hDC);
	}
	int nScale = g_nZoomScale;
	int nSrcLeft = 0, nSrcTop = 0;
	if(pt.x + (ZOOMIN_WIDTH/(nScale*2)) > nScreenWidth)
		nSrcLeft = nScreenWidth - (ZOOMIN_WIDTH/(nScale));
	else
		nSrcLeft = pt.x - (ZOOMIN_WIDTH/(nScale*2));
	if(pt.y + (ZOOMIN_HEIGHT/(nScale*2)) > nScreenHeight)
		nSrcTop = nScreenHeight - (ZOOMIN_HEIGHT/(nScale));
	else
		nSrcTop = pt.y - (ZOOMIN_HEIGHT/(nScale*2));
	if(nSrcLeft<0)
		nSrcLeft = 0;
	if(nSrcTop<0)
		nSrcTop = 0;
#if SHOW_DEBUG
	wchar_t szMsg[MAX_PATH] = {0};
	swprintf(szMsg, L"Left:%d\nTop:%d\nRight:%d\nBottom:%d\n", nSrcLeft, nSrcTop, ZOOMIN_WIDTH/nScale, ZOOMIN_HEIGHT/nScale);
	OutputDebugStringW(szMsg);
#endif

	hOldMemBitmap = (HBITMAP)SelectObject(hMemDC, hMemBitmap);
	hOldPen = (HPEN)SelectObject(hMemDC, g_hPen);
	RECT rtZoom = { 0, 0, ZOOMIN_WIDTH, ZOOMIN_HEIGHT };
	FillRect(hMemDC, &rtZoom, g_hBorder);
	//DrawEdge(hMemDC, &rtZoom, EDGE_ETCHED, BF_RECT); // Zoom Border

	StretchBlt(hMemDC, g_nBorderWidth, g_nBorderWidth, ZOOMIN_WIDTH-(g_nBorderWidth*2)-g_nZoomRounder/2, ZOOMIN_HEIGHT-(g_nBorderWidth*2)-g_nZoomRounder/2, 
		hDC, nSrcLeft, nSrcTop, ZOOMIN_WIDTH/nScale, ZOOMIN_HEIGHT/nScale, SRCCOPY);
	//StretchBlt(hMemDC,ZOOMIN_WIDTH-16,ZOOMIN_HEIGHT-16,16,16,hDC,pt.x,pt.y,1,1,SRCCOPY);
	POINT pt1, pt2;
	pt1.x = g_nBorderWidth + ZOOMIN_WIDTH/2;
	pt1.y = g_nBorderWidth;
	pt2.x = g_nBorderWidth + ZOOMIN_WIDTH/2;
	pt2.y = ZOOMIN_HEIGHT - g_nBorderWidth;
	MoveToEx(hMemDC, pt1.x, pt1.y, NULL);
	LineTo(hMemDC, pt2.x, pt2.y);
	pt1.x = g_nBorderWidth;
	pt1.y = ZOOMIN_HEIGHT/2;
	pt2.x = ZOOMIN_WIDTH - g_nBorderWidth;
	pt2.y = ZOOMIN_HEIGHT/2;
	MoveToEx(hMemDC, pt1.x, pt1.y, NULL);
	LineTo(hMemDC, pt2.x, pt2.y);

	BitBlt(hZoomDC, 0, 0, ZOOMIN_WIDTH, ZOOMIN_HEIGHT, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldMemBitmap);
	SelectObject(hMemDC, hOldPen);
	SelectPalette(hDC, hOldScrPal, FALSE);
	SelectPalette(hZoomDC, hOldZoomPal, FALSE);
	SelectPalette(hMemDC, hOldMemPal, FALSE);
	ReleaseDC(g_hZoomHwnd, hZoomDC);
	ReleaseDC(NULL, hDC);
	DeleteDC(hMemDC);
	DeleteObject(hOldMemBitmap);

	if(g_hInvoker && (((pt.x!=g_lastPoint.x || pt.y!=g_lastPoint.y) && dwColor != g_lastColor) || g_bRecordColor))
	{
		g_lastPoint.x = pt.x;
		g_lastPoint.y = pt.y;
		g_lastColor = dwColor;
		ZOOMIN_INFO info = {0};
		info.dwColor = dwColor;
		info.ptCursor.x = pt.x;
		info.ptCursor.y = pt.y;
		COPYDATASTRUCT data;
		if(g_bRecordColor)
			data.dwData = WM_ZOOMIN_COLOR_MSG;
		else
			data.dwData = WM_ZOOMIN_UPDATE_MSG;
		data.cbData = sizeof(SNAPSHOT_INFO);
		data.lpData = &info;
		g_bRecordColor = FALSE;
		::SendMessage(g_hInvoker, WM_COPYDATA, (WPARAM)(HWND)g_hZoomHwnd, (LPARAM)(LPVOID)&data);
	}

	return TRUE;
}

LRESULT OnCreate(HWND hWnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Vista/Win7，由于UAC的限制，低权限进程不能向高权限进程发送消息
	// 如果我们想容许一个消息可以发送给较高特权等级的进程
	// 我们可以在较高特权等级的进程中调用ChangeWindowMessageFilter(Vista以上的API)函数，以MSGFLT_ADD作为参数将消息添加进消息过滤器的白名单。
	PFNChangeWindowMessageFilter pfnChangeWindowMessageFilter = NULL;
	HMODULE hModule = LoadLibrary(_T("user32.dll"));
	if(hModule)
	{
		pfnChangeWindowMessageFilter = (PFNChangeWindowMessageFilter)GetProcAddress(hModule, "ChangeWindowMessageFilter");
		if(pfnChangeWindowMessageFilter)
		{
#ifndef MSGFLT_ADD
#define MSGFLT_ADD		1
#define MSGFLT_REMOVE	2
#endif
#define WM_COPYGLOBALDATA 0x0049
			pfnChangeWindowMessageFilter(WM_COPYGLOBALDATA, MSGFLT_ADD);
			pfnChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
			pfnChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
#undef WM_COPYGLOBALDATA
		}
		FreeLibrary(hModule);
	}
	
	wchar_t szMoudle[1024] = {0};
	GetModuleFileName(NULL, szMoudle, _countof(szMoudle));
	PathRemoveFileSpec(szMoudle);
	PathAppend(szMoudle, kRDKeyHook);
	if(PathFileExists(szMoudle))
	{
		g_hHookModule = LoadLibraryW(szMoudle);
		if(g_hHookModule)
		{
			g_lpAttatchHook = (LPAttatchHook)GetProcAddress(g_hHookModule, kAttatchHook);
			g_lpDetatchHook = (LPDetatchHook)GetProcAddress(g_hHookModule, kDetatchHook);
			if(g_lpAttatchHook)
				g_lpAttatchHook(hWnd);
		}
	}

	SetTimer(hWnd, ZOOMIN_TIMER, ZOOMIN_ELAPSE, NULL);
	SetupPalette();
	g_hPen = CreatePen(PS_SOLID, g_nBorderWidth, g_clrBorder);
	g_hBorder = CreateSolidBrush(g_clrBorder);
	HRGN hRgn = CreateRoundRectRgn(0, 0, ZOOMIN_WIDTH, ZOOMIN_HEIGHT, g_nZoomRounder, g_nZoomRounder);
	SetWindowRgn(hWnd, hRgn, TRUE);
	DeleteObject(hRgn);
	return 0;
}

LRESULT OnKeyUp(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
{
	switch(wParam)
	{
	case VK_ESCAPE:
	case VK_RETURN:
		PostMessage(g_hZoomHwnd, WM_CLOSE, 0, 0);
		break;
	}
	return 0;
}

LRESULT OnEraseBkgnd(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	UpdateZoomWindow();
	return TRUE;
}

LRESULT OnTimer(HWND hWnd, UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
{
	if(wParam==ZOOMIN_TIMER)
	{
		if(IsWindowVisible(hWnd))
			//UpdateZoomWindow();
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE);
	}
	return 0;
}

LRESULT OnPaletteChanged(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	SetupPalette();
	return 0;
}

LRESULT OnCopyData(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
{
	PCOPYDATASTRUCT lpData = (PCOPYDATASTRUCT)lParam;
	if(lpData->dwData==WM_ZOOMIN_QUIT_MSG)
	{
		PostMessage(g_hZoomHwnd, WM_CLOSE, 0, 0);
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
	else if(lpData->dwData==WM_ZOOMIN_ZOOM_MSG&&g_nZoomScale<8)
	{
		g_nZoomScale++;
	}
	else if(lpData->dwData==WM_ZOOMIN_REDUCE_MSG&&g_nZoomScale>1)
	{
		g_nZoomScale--;
	}
	return 0;
}

LRESULT OnDestroy(HWND hWnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	KillTimer(hWnd, ZOOMIN_TIMER);
	if(g_hZoomPal)
		DeleteObject(g_hZoomPal);
	if(g_hPen)
		DeleteObject(g_hPen);
	if(g_hBorder)
		DeleteObject(g_hBorder);
	if(g_hHookModule)
	{
		if(g_lpDetatchHook)
			g_lpDetatchHook();
		FreeLibrary(g_hHookModule);
	}
	PostQuitMessage(0);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		OnCreate(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYUP:
		OnKeyUp(hWnd, uMsg, uMsg, lParam);
		break;
	case WM_TIMER:
		OnTimer(hWnd, uMsg, wParam, lParam);
		break;
	case WM_ERASEBKGND:
		return OnEraseBkgnd(hWnd, uMsg, wParam, lParam);
	case WM_PALETTECHANGED:
		OnPaletteChanged(hWnd, uMsg, wParam, lParam);
		return TRUE;
	case WM_COPYDATA:
		OnCopyData(hWnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		OnDestroy(hWnd, uMsg, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int OptParse(int nArgc, TCHAR* lpszArgv[])
{
	int nRetCode = E_OK;
	nRetCode = g_OptOptions.addOption(ARGS_SHORT_HWND, ARGS_LONG_HWND, ARGS_HWND_DESC, OPT_NEEDARG, NULL);
	if(nRetCode!=E_OK)
		return nRetCode;

	g_OptParser.setFlags(OPT_PARSE_QUOTED_ARGS | OPT_PARSE_UNQUOTE_ARGS | OPT_PARSE_AUTO_HELP | OPT_PARSE_INCLUDE_ARGV0);
	nRetCode = g_OptParser.parse(nArgc, lpszArgv, g_OptOptions);

	return nRetCode;
}

//
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	// 内存跟踪调试
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(192);
#endif	
	g_hInst = hInstance;
	if(::FindWindow(kZoomInClass, NULL))
		return 0;
	// 解析启动参数
	int nCount = 0;
	LPWSTR* lpszArgs = CommandLineToArgvW(GetCommandLine(), &nCount);
	OptParse(nCount, lpszArgs);
	LocalFree(lpszArgs);

	if(g_OptOptions.isSet(ARGS_SHORT_HWND))
		g_hInvoker = (HWND)_wtol(g_OptOptions.asString(ARGS_SHORT_HWND).c_str());

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInst;
	wcex.hIcon			= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ZOOMIN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= kZoomInClass;
	wcex.hIconSm		= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ZOOMIN));
	if(RegisterClassEx(&wcex)==0)
		return 0;
	POINT pt = {0};
	GetCursorPos(&pt);
	RECT rtDesktop = {0};
	HWND hDeskTop = GetDesktopWindow();
	if(!hDeskTop)
		return FALSE;
	::GetClientRect(hDeskTop, &rtDesktop);
	int nOffset = g_nOffset;
	RECT rtLabel = { pt.x + nOffset, pt.y + nOffset, pt.x + nOffset + ZOOMIN_WIDTH, pt.y + nOffset + ZOOMIN_HEIGHT };
	if (rtLabel.right > rtDesktop.right - 5)
	{
		rtLabel.left = pt.x - nOffset - (rtLabel.right - rtLabel.left);
		rtLabel.right += ZOOMIN_WIDTH;
	}
	if (rtLabel.bottom > rtDesktop.bottom - 5)
	{
		rtLabel.top = pt.y - nOffset - (rtLabel.bottom - rtLabel.top);
		rtLabel.bottom += ZOOMIN_HEIGHT;
	}
	g_hZoomHwnd = CreateWindowEx(0, kZoomInClass, L"", WS_POPUP, rtLabel.left, rtLabel.top, ZOOMIN_WIDTH, ZOOMIN_HEIGHT, NULL, NULL, g_hInst, NULL);
	if (!g_hZoomHwnd)
		return 0;
	::SetWindowLong(g_hZoomHwnd, GWL_EXSTYLE, GetWindowLong(g_hZoomHwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
	::SetWindowPos(g_hZoomHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(g_hZoomHwnd, SW_SHOWNOACTIVATE);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}