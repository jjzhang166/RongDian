// HookTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "HookTest.h"

#pragma data_seg("Shared")
HHOOK mHook=NULL;	//钩子全局变量
HINSTANCE hInstance=NULL;
#pragma data_seg()

/************************************************************************/
/* 钩子回调函数  
/************************************************************************/
LRESULT WINAPI KeyProc(int code,WPARAM wParam,LPARAM lParam)
{
	if(code==HC_ACTION && (lParam & 0xc000ffff) ==1)
	{
		char *sName;
		BOOL b_sft=::GetAsyncKeyState (VK_SHIFT)>>((sizeof(short)*8)-1);
		if(b_sft)
		{
			switch(wParam)
			{
			case '1':sName="!"; break;
			case '2':sName="@"; break;
			case '3':sName = "#";break;
			case '4':sName = "$";break;
			case '5':sName = "%";break;
			case '6':sName = "^";break;
			case '7':sName = "&";break;
			case '8':sName = "*";break;
			case '9':sName = "(";break;
			case '0':sName = ")";break;
			case 'A':sName = "A";break;
			case 'B':sName = "B";break;
			case 'C':sName = "C";break;
			case 'D':sName = "D";break;
			case 'E':sName = "E";break;
			case 'F':sName = "F";break;
			case 'G':sName = "G";break;
			case 'H':sName = "H";break;
			case 'I':sName = "I";break;
			case 'J':sName = "J";break;
			case 'K':sName = "K";break;
			case 'L':sName = "L";break;
			case 'M':sName = "M";break;
			case 'N':sName = "N";break;
			case 'O':sName = "O";break;
			case 'P':sName = "P";break;
			case 'Q':sName = "Q";break;
			case 'R':sName = "R";break;
			case 'S':sName = "S";break;
			case 'T':sName = "T";break;
			case 'U':sName = "U";break;
			case 'V':sName = "V";break;
			case 'W':sName = "W";break;
			case 'X':sName = "X";break;
			case 'Y':sName = "Y";break;
			case 'Z':sName = "Z";break;
			}
		}
		else
		{
			switch(wParam)
			{
				/*0~9*/

			case '1': sName="1"; break;

			case '2':sName = "2";break;

			case '3':sName = "3";break;

			case '4':sName = "4";break;

			case '5':sName = "5";break;

			case '6':sName = "6";break;

			case '7':sName = "7";break;

			case '8':sName = "8";break;

			case '9':sName = "9";break;

			case '0':sName = "0";break;

				/*A~Z*/

			case 'A':sName = "a";break;

			case 'B':sName = "b";break;

			case 'C':sName = "c";break;

			case 'D':sName = "d";break;

			case 'E':sName = "e";break;

			case 'F':sName = "f";break;

			case 'G':sName = "g";break;

			case 'H':sName = "h";break;

			case 'I':sName = "i";break;

			case 'J':sName = "j";break;

			case 'K':sName = "k";break;

			case 'L':sName = "l";break;

			case 'M':sName = "m";break;

			case 'N':sName = "n";break;

			case 'O':sName = "o";break;

			case 'P':sName = "p";break;

			case 'Q':sName = "q";break;

			case 'R':sName = "r";break;

			case 'S':sName = "s";break;

			case 'T':sName = "t";break;

			case 'U':sName = "u";break;

			case 'V':sName = "v";break;

			case 'W':sName = "w";break;

			case 'X':sName = "x";break;

			case 'Y':sName = "y";break;

			case 'Z':sName = "z";break;

			}

		}

		switch(wParam) //数字键
		{
		case VK_BACK:sName="~"; break;
		case VK_NUMPAD1:sName="1";break;
		case VK_NUMPAD3:sName = "3";break;
		case VK_NUMPAD4:sName = "4";break;
		case VK_NUMPAD5:sName = "5";break;
		case VK_NUMPAD6:sName = "6";break;
		case VK_NUMPAD7:sName = "7";break;
		case VK_NUMPAD8:sName = "8";break;
		case VK_NUMPAD9:sName = "9";break;
		case VK_NUMPAD0:sName = "0";break;
		case VK_MULTIPLY:sName = "*";break;
		case VK_ADD:
			sName="+";
			break;
		case VK_SUBTRACT:
			sName="-";
			break;
		case VK_DECIMAL:
			sName=".";
			break;
		case VK_DIVIDE:
			sName="/";
			break;
		}
		HWND H_wnd=::GetForegroundWindow();
		//自定义消息发送
		::SendMessage (H_wnd,WM_Hook,wParam, (LPARAM)(LPCSTR)sName);
		MessageBox(H_wnd,sName,"键盘提示",MB_OK);
	}
	return CallNextHookEx( mHook, code, wParam, lParam );
}

BOOL APIENTRY DllMain(HANDLE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInstance=(HINSTANCE)hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL WINAPI Start()
{
	if(mHook!=NULL)   return FALSE;
	//WH_KEYBOARD值为2,键盘消息钩子
	//KeyProc 为回调函数
	//hInstance:实例
	//0:表示全局钩子
	mHook=::SetWindowsHookEx (WH_KEYBOARD,KeyProc,hInstance,0);
	return mHook!=NULL;
}

extern "C" __declspec(dllexport) void WINAPI Stop()
{
	//要卸载的钩子
	::UnhookWindowsHookEx (mHook);
}