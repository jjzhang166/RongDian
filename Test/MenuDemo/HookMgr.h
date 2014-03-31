#pragma once

#pragma warning(push)
#pragma warning(disable:4996)

#ifdef _DEBUG
#define HOOK_MGR_DEBUG() \
	{ \
		char szDebug[1024]; \
		sprintf(szDebug, "%s - %d\n", __FUNCTION__, GetTickCount()); \
		OutputDebugStringA(szDebug); \
	}
#else
#define HOOK_MGR_DEBUG()
#endif

enum _tagHOOK_FLAG
{
	HM_CALLWNDPROC		= 0x0001,
	HM_CALLWNDPROCRET	= 0x0002,
	HM_CBT				= 0x0004,
	HM_FOREGROUNDIDLE	= 0x0008,
	HM_GETMESSAGE		= 0x0010,
	HM_KEYBOARD			= 0x0020,
	HM_MOUSE			= 0x0040,
	HM_MSGFILTER		= 0x0080,
	HM_SHELL			= 0x0100,
	HM_SYSMSGFILTER		= 0x0200,
} HOOK_FLAG;

#ifndef HSHELL_APPCOMMAND
#define HSHELL_APPCOMMAND           12
#endif

static inline BOOL MatchClass(HWND hWnd, LPCWSTR pszClass)
{
	if(!pszClass || wcslen(pszClass)==0)
		return TRUE; // no filter class
	static wchar_t szName[256] = {0};
	::GetClassName(hWnd, szName, _countof(szName));
	return (wcsicmp(pszClass, szName)==0);
}

template <class MGR_TYPE>
class CHookMgr
{
public:
	CHookMgr()
	{
		m_hCallWndHook = NULL;
		m_hCallWndRetHook = NULL;
		m_hCbtHook = NULL;
		m_hForegroundIdleHook = NULL;
		m_hGetMessageHook = NULL;
		m_hKeyboardHook = NULL;
		m_hMouseHook = NULL;
		m_hMsgFilterHook = NULL;
		m_hShellHook = NULL;
		m_hSysMsgFilterHook = NULL;
		memset(m_szFilter, 0, sizeof(m_szFilter));
	};
	virtual ~CHookMgr() { UninitHooks(); };

protected:
	BOOL InitHooks(DWORD dwOptions = HM_CALLWNDPROC, LPCWSTR pszFilter = NULL)
	{
		UninitHooks();

#define INITHOOK(hook, option, flag, type, lpfn) \
		{ \
			if(option & flag) \
				hook = SetWindowsHookEx(type, lpfn, NULL, GetCurrentThreadId()); \
		}

		INITHOOK(m_hCallWndHook, dwOptions, HM_CALLWNDPROC, WH_CALLWNDPROC, CallWndProc);
		INITHOOK(m_hCallWndRetHook, dwOptions, HM_CALLWNDPROCRET, WH_CALLWNDPROCRET, CallWndRetProc);
		INITHOOK(m_hCbtHook, dwOptions, HM_CBT, WH_CBT, CbtProc);
		INITHOOK(m_hForegroundIdleHook, dwOptions, HM_FOREGROUNDIDLE, WH_FOREGROUNDIDLE, ForegroundIdleProc);
		INITHOOK(m_hGetMessageHook, dwOptions, HM_GETMESSAGE, WH_GETMESSAGE, GetMessageProc);
		INITHOOK(m_hKeyboardHook, dwOptions, HM_KEYBOARD, WH_KEYBOARD, KeyboardProc);
		INITHOOK(m_hMouseHook, dwOptions, HM_MOUSE, WH_MOUSE, MouseProc);
		INITHOOK(m_hMsgFilterHook, dwOptions, HM_MSGFILTER, WH_MSGFILTER, MsgFilterProc);
		INITHOOK(m_hShellHook, dwOptions, HM_SHELL, WH_SHELL, ShellProc);
		INITHOOK(m_hSysMsgFilterHook, dwOptions, HM_SYSMSGFILTER, WH_SYSMSGFILTER, SysMsgFilterProc);
		if(pszFilter)
		{
			memset(m_szFilter, 0, sizeof(m_szFilter));
			wcsncpy(m_szFilter, pszFilter, _countof(m_szFilter));
		}

#undef INITHOOK
		return TRUE;
	}

	void UninitHooks()
	{
#define UNINITHOOK(hook) \
		{ \
			if(hook) \
				UnhookWindowsHookEx(hook); \
			hook = NULL; \
		}

		UNINITHOOK(m_hCallWndHook);
		UNINITHOOK(m_hCallWndRetHook);
		UNINITHOOK(m_hCbtHook);
		UNINITHOOK(m_hForegroundIdleHook);
		UNINITHOOK(m_hGetMessageHook);
		UNINITHOOK(m_hKeyboardHook);
		UNINITHOOK(m_hMouseHook);
		UNINITHOOK(m_hMsgFilterHook);
		UNINITHOOK(m_hShellHook);
		UNINITHOOK(m_hSysMsgFilterHook);
#undef UNINITHOOK
	}

protected:
	inline BOOL MatchClass(HWND hWnd)
	{
		return ::MatchClass(hWnd, m_szFilter);
	};

	static MGR_TYPE& GetInstance()
	{
		static MGR_TYPE manager;
		return manager;
	};

	virtual BOOL OnCbt(int nCode, WPARAM wParam, LPARAM lParam) { HOOK_MGR_DEBUG(); return FALSE; };
	virtual BOOL OnShell(int nCode, WPARAM wParam, LPARAM lParam) { HOOK_MGR_DEBUG(); return FALSE; };
	virtual void OnCallWnd(const MSG &tMsg) { HOOK_MGR_DEBUG(); };
	virtual void OnCallWndRet(const MSG &tMsg, LRESULT nRet) { HOOK_MGR_DEBUG(); };
	virtual void OnForegroundIdle() { HOOK_MGR_DEBUG(); };
	virtual void OnGetMessage(const MSG &tMsg) { HOOK_MGR_DEBUG(); };
	virtual void OnKeyboard(UINT uKey, UINT uFlags) { HOOK_MGR_DEBUG(); };
	virtual void OnMouse(UINT uMsg, const POINT &ptMouse) { HOOK_MGR_DEBUG(); };
	virtual void OnMsgFilter(const MSG &tMsg, int nEvent) { HOOK_MGR_DEBUG(); };
	virtual void OnSysMsgFilter(const MSG &tMsg, int nEvent) { HOOK_MGR_DEBUG(); };

protected:
	static LRESULT CALLBACK CbtProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
		{
			if(GetInstance().OnCbt(nCode, wParam, lParam))
				return TRUE;
		}
		return CallNextHookEx(GetInstance().m_hCbtHook, nCode, wParam, lParam);
	};

	static LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(GetInstance().OnShell(nCode, wParam, lParam))
		{
			if(nCode==HSHELL_APPCOMMAND)
				return TRUE;
		}
		return FALSE; // CallNextHookEx(GetInstance().m_hShellHook, nCode, wParam, lParam);
	};

	static HRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
		{
			CWPSTRUCT *pStruct = (CWPSTRUCT*)lParam;
			if(GetInstance().MatchClass(pStruct->hwnd))
			{
				MSG tMsg = { pStruct->hwnd, pStruct->message, pStruct->wParam, pStruct->lParam, 0, { 0, 0 } };
				GetInstance().OnCallWnd(tMsg);
			}
		}
		return CallNextHookEx(GetInstance().m_hCallWndHook, nCode, wParam, lParam);
	};

	static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
		{
			CWPRETSTRUCT *pStruct = (CWPRETSTRUCT *)lParam;
			if(GetInstance().MatchClass(pStruct->hwnd))
			{
				MSG tMsg = { pStruct->hwnd, pStruct->message, pStruct->wParam, pStruct->lParam, 0, {0, 0} };
				GetInstance().OnCallWndRet(tMsg, pStruct->lResult);
			}
		}
		return CallNextHookEx(GetInstance().m_hCallWndRetHook, nCode, wParam, lParam);
	};

	static LRESULT CALLBACK ForegroundIdleProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
			GetInstance().OnForegroundIdle();
		return CallNextHookEx(GetInstance().m_hForegroundIdleHook, nCode, wParam, lParam);
	};
	
	static LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
		{
			MSG *pMsg = (MSG *)lParam;
			if(GetInstance().MatchClass(pMsg->hwnd))
				GetInstance().OnGetMessage(*pMsg);
		}
		return CallNextHookEx(GetInstance().m_hGetMessageHook, nCode, wParam, lParam);
	};

	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
			GetInstance().OnKeyboard(wParam, lParam);
		return CallNextHookEx(GetInstance().m_hKeyboardHook, nCode, wParam, lParam);
	};

	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
		{
			POINT *ptCursor = (POINT *)lParam;
			GetInstance().OnMouse(wParam, *ptCursor);
		}
		return CallNextHookEx(GetInstance().m_hMouseHook, nCode, wParam, lParam);
	};

	static LRESULT CALLBACK MsgFilterProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
		{
			MSG* pMsg = (MSG *)lParam;
			if(GetInstance().MatchClass(pMsg->hwnd))
				GetInstance().OnMsgFilter(*pMsg, nCode);
		}
		return CallNextHookEx(GetInstance().m_hMsgFilterHook, nCode, wParam, lParam);
	};

	static LRESULT CALLBACK SysMsgFilterProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if(nCode==HC_ACTION)
		{
			MSG *pMsg = (MSG *)lParam;
			if(GetInstance().MatchClass(pMsg->hwnd))
				GetInstance().OnSysMsgFilter(*pMsg, nCode);
		}
		return CallNextHookEx(GetInstance().m_hSysMsgFilterHook, nCode, wParam, lParam);
	};

protected:
	HHOOK m_hCallWndHook;
	HHOOK m_hCallWndRetHook;
	HHOOK m_hCbtHook;
	HHOOK m_hForegroundIdleHook;
	HHOOK m_hGetMessageHook;
	HHOOK m_hKeyboardHook;
	HHOOK m_hMouseHook;
	HHOOK m_hMsgFilterHook;
	HHOOK m_hShellHook;
	HHOOK m_hSysMsgFilterHook;
	wchar_t m_szFilter[256];
};
#pragma warning(pop)
