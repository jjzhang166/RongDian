#include <Windows.h>

#define KEYHOOK_DLL
#include <KeyHook.h>
#include <RongDian.h>

HINSTANCE	g_hInst = NULL;
HHOOK		g_hKerboardHook = NULL;
HHOOK		g_hMouseHook = NULL;
HWND		g_hInvoker = NULL;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/*
	在win32中，wParam是普通键(字母、数字之类的)
	lParam是用来判断是否按下了alt、ctrl等键,具体可看msdn文档中的WM_KEYDOWN

	在钩子里面，有所不同，相关信息都存在KBDLLHOOKSTRUCT这个结构提里面
	钩子消息处理函数的wParam就是消息值(WM_KEYDOWN)
	vkCode相当于wParam
	flags相当于lParam
	*/
	if(nCode == HC_ACTION)
	{
		PKBDLLHOOKSTRUCT pKeyStruct = (PKBDLLHOOKSTRUCT)lParam;
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			if(pKeyStruct->vkCode==VK_RETURN || pKeyStruct->vkCode==VK_ESCAPE && g_hInvoker)
			{
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_QUIT_MSG;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}else if (pKeyStruct->vkCode==VK_UP)
			{
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_MOVE_UP;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}else if (pKeyStruct->vkCode==VK_DOWN)
			{
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_MOVE_DOWN;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}else if (pKeyStruct->vkCode==VK_LEFT)
			{
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_MOVE_LEFT;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}else if (pKeyStruct->vkCode==VK_RIGHT)
			{
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_MOVE_RIGHT;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}else if (pKeyStruct->vkCode==VK_OEM_4)
			{
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_ZOOM_MSG;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}
			else if (pKeyStruct->vkCode==VK_OEM_6)
			{
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_REDUCE_MSG;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}
			//alt+c 取色
			else if((pKeyStruct->flags&LLKHF_ALTDOWN)&&(pKeyStruct->vkCode=='c'||pKeyStruct->vkCode=='C')){
				COPYDATASTRUCT data;
				data.dwData = WM_ZOOMIN_COLOR_MSG;
				data.cbData = 0;
				data.lpData = NULL;
				::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
			}
		}
	} // end of 'if(nCode == HC_ACTION)'
	return CallNextHookEx(g_hKerboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		//PMSLLHOOKSTRUCT pMouseStruct = (PMSLLHOOKSTRUCT)lParam;
		if(wParam==WM_RBUTTONUP && g_hInvoker)
		{
			COPYDATASTRUCT data;
			data.dwData = WM_ZOOMIN_QUIT_MSG;
			data.cbData = 0;
			data.lpData = NULL;
			::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
		}
		/*else if(wParam==WM_LBUTTONUP && g_hInvoker)
		{
			COPYDATASTRUCT data;
			data.dwData = WM_ZOOMIN_COLOR_MSG;
			data.cbData = 0;
			data.lpData = NULL;
			::SendMessage(g_hInvoker, WM_COPYDATA, 0, (LPARAM)(LPVOID)&data);
		}*/
	}
	return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

LONG __cdecl AttatchHook(HWND hInvoker)
{
	if(g_hKerboardHook)
		return 0;
	g_hKerboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, g_hInst, 0);
	g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, g_hInst, 0);
	if(!g_hKerboardHook)
		return -1;
	g_hInvoker = hInvoker;
	return 0;
}

LONG __cdecl DetatchHook()
{
	g_hInvoker = NULL;
	if(g_hKerboardHook)
	{
		UnhookWindowsHookEx(g_hKerboardHook);
		g_hKerboardHook = NULL;
	}
	if(g_hMouseHook)
	{
		UnhookWindowsHookEx(g_hMouseHook);
		g_hMouseHook = NULL;
	}
	return 0;
}

BOOL WINAPI DllMain(HANDLE hModule, ULONG dwReason, LPVOID /*lpReserved*/)
{
	if(dwReason == DLL_PROCESS_ATTACH)
		g_hInst = (HINSTANCE)hModule;
	return TRUE;
}
