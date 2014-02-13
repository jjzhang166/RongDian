#pragma once

class CBaseTool
{
public:
	CBaseTool() {};
	virtual ~CBaseTool() {};

public:
	virtual BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/) = 0;
	virtual BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/)  = 0;
	virtual CControlUI* OnCreateControl(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszClass*/, CControlUI* /*pParent*/) { return NULL; };
	virtual void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnItemClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnItemSelected(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnTextChanged(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual LPCWSTR GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/) { return L""; };
	virtual BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/) = 0;
	virtual void OnQuit() = 0;

	virtual LRESULT HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { return 0; };
	virtual LRESULT OnCopyData(WPARAM /*wParam*/, LPARAM /*lParam*/) { return 0; };
};

typedef LONG (*LPFNCreateRDTool)(LPWSTR lpszTName, LPVOID *lpClass);
typedef LONG (*LPFNDestroyRDTool)(LPVOID lpClass);

typedef struct _tagTOOLS_INFO
{
	int nIndex;
	wchar_t szName[1024];
	CBaseTool* lpClass;
	LPFNDestroyRDTool lpfnDestroy;
} TOOLS_INFO, *PTOOLS_INFO, *LPTOOLS_INFO;

#define RD_DECLARE_BEGIN(tool) \
	class C##tool : public CBaseTool\
	{ \
	public: \
		C##tool(); \
		virtual ~C##tool() { }; \
	public: \
		BOOL IsCanQuit(HWND hWnd, CPaintManagerUI* pManager); \
		void OnQuit(); \
		BOOL OnInit(WPARAM wParam, LPARAM lParam); \
		BOOL SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang); \

#define RD_DECLARE_MEMBER(type, method) \
		type method

#define RD_DECLARE_END() \
	public: \
		wchar_t _TOOL_NAME[1024]; \
	}; 

#define RD_DECLARE_TOOLS() \
	private: \
		BOOL _InitTools(); \
		void _ReleaseTools(); \
	private: \
		list<LPTOOLS_INFO> _toolsEntries; 

#define RD_BEGIN_TOOLS_MAP(cls) \
	void cls::_ReleaseTools() \
	{ \
		LPTOOLS_INFO tool = NULL; \
		if(_toolsEntries.size()) \
		tool = (LPTOOLS_INFO)_toolsEntries.back(); \
		while(tool) \
		{ \
			_toolsEntries.pop_back(); \
			if(tool->lpClass) \
			{ \
				if(tool->lpfnDestroy) \
					tool->lpfnDestroy(tool->lpClass); \
				else \
					delete tool->lpClass; \
			} \
			delete tool; \
			tool = NULL; \
			if(_toolsEntries.size()) \
				tool = (LPTOOLS_INFO)_toolsEntries.back(); \
		} \
	} \
	BOOL cls::_InitTools() \
	{ 

#define RD_TOOL(x) \
	{ \
		x *entry = new x(); \
		LPTOOLS_INFO tool = new TOOLS_INFO(); \
		if(tool) \
		{ \
			memset(tool, 0, sizeof(TOOLS_INFO)); \
			if(entry) \
			{ \
				tool->nIndex = _toolsEntries.size(); \
				tool->lpClass = entry; \
				_toolsEntries.push_back(tool); \
			} \
		} \
	}

#define RD_END_TOOLS_MAP() \
		return TRUE; \
	} 

#define RD_ISCAN_QUIT(h, manager, r) \
	{ \
		r = TRUE; \
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				r = FALSE; \
				break; \
			} \
			r = tool->lpClass->IsCanQuit(h, manager); \
			if(!r) \
				break; \
		} \
	}

#define RD_ON_INIT_MSG(w, l) \
	{ \
		BOOL bRet = FALSE;\
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				break; \
			} \
			bRet = tool->lpClass->OnInit(w, l); \
			if(!bRet) \
				break; \
		} \
	}

#define RD_ON_LANG_MSG(manager, lang) \
	{ \
		BOOL bRet = FALSE;\
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				break; \
			} \
			bRet = tool->lpClass->SetLang(manager, lang); \
			if(!bRet) \
				break; \
		} \
	}

#define RD_ON_CREATE_CONTROL_MSG(m, c, p) \
	{ \
		CControlUI *pControl = NULL;\
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				pControl = NULL; \
				break; \
			} \
			pControl = tool->lpClass->OnCreateControl(m, c, p); \
			if(pControl) \
				return pControl; \
		} \
	}

#define RD_ON_COMMON_MSG(h, p, m, b, fn) \
	{ \
		b = FALSE; \
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				b = FALSE; \
				break; \
			} \
			tool->lpClass->fn(h, p, m, b); \
			if(b) \
				break; \
		} \
	} 

#define RD_ON_ITEMTEXT_MSG(h, p, c, x, y) \
	{ \
		LPCWSTR lpData = L""; \
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				break; \
			} \
			lpData = tool->lpClass->GetItemText(h, p, c, x, y); \
			if(wcscmp(lpData, L"")!=0) \
				break; \
		} \
		return lpData; \
	} 

#define RD_ON_QUIT() \
	{ \
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				return; \
			} \
			tool->lpClass->OnQuit(); \
		} \
	} 

#define RD_ON_CUSTOM_MSG(u, w, l) \
	{ \
		HRESULT hRes = 0;\
		BOOL bHandle = FALSE; \
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				continue; \
			} \
			hRes = tool->lpClass->HandleCustomMessage(u, w, l, bHandle); \
			if(hRes) \
				break; \
		} \
	} 

#define RD_ON_COPYDATA_MSG(w, l) \
	{ \
		BOOL bRet = FALSE;\
		list<LPTOOLS_INFO>::iterator iter; \
		LPTOOLS_INFO tool = NULL; \
		for(iter=_toolsEntries.begin(); iter!=_toolsEntries.end(); iter++) \
		{ \
			tool = (*iter); \
			if(!tool->lpClass) \
			{ \
				continue; \
			} \
			bRet = tool->lpClass->OnCopyData(w, l); \
			if(bRet) \
				break; \
		} \
	} 

#define OUT_PUT_NUMBER(Number) { \
	wchar_t NumberChar[10]; \
	ZeroMemory(NumberChar, 10 * sizeof(wchar_t)); \
	wsprintfW(NumberChar, L"%d\n", Number); \
	OutputDebugStringW(NumberChar); \
}