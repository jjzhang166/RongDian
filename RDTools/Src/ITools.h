#pragma once

class CBaseTool
{
public:
	CBaseTool() {};
	virtual ~CBaseTool() {};

public:
	virtual BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/) = 0;
	virtual BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/)  = 0;
	virtual CControlUI* OnCreateControl(LPCWSTR /*lpszClass*/, CControlUI* /*pParent*/) { return NULL; };
	virtual void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnItemClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnItemSelected(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual void OnTextChanged(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& bHandled) { bHandled = FALSE; };
	virtual LPCWSTR GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/) { return L""; };
	virtual BOOL IsCanQuit(HWND /*hWnd*/) = 0;
	virtual void OnQuit() = 0;

	virtual LRESULT HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { return 0; };
	virtual LRESULT OnCopyData(WPARAM /*wParam*/, LPARAM /*lParam*/) { return 0; };
};

typedef struct _tagTOOLS_INFO
{
	int nIndex;
	wchar_t szName[1024];
	CBaseTool* lpClass;
} TOOLS_INFO, *PTOOLS_INFO, *LPTOOLS_INFO;

#define RD_DECLARE_BEGIN(tool) \
	class C##tool : public CBaseTool\
	{ \
	public: \
		C##tool(); \
		virtual ~C##tool() { }; \
	public: \
		BOOL IsCanQuit(HWND hWnd); \
		void OnQuit(); \
		BOOL OnInit(WPARAM wParam, LPARAM lParam); \
		BOOL SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang); \

#define RD_DECLARE_MEMBER(type, method) \
		type method

#define RD_DECLARE_END(tool) \
	}; \
	__declspec(selectany) C##tool the##tool##Class;

#define RD_DECLARE_TOOLS() \
	private: \
		static const TOOLS_INFO _toolsEntries[]; 

#define RD_BEGIN_TOOLS_MAP(cls) \
	UILIB_COMDAT const TOOLS_INFO cls::_toolsEntries[] = \
	{

#define RD_TOOL(name, x) \
		{ 0, name, &the##x##Class }, \

#define RD_END_TOOLS_MAP() \
		{ 0, NULL, NULL } \
	};

#define RD_ISCAN_QUIT(h, r) \
	{ \
		r = TRUE; \
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			r = _toolsEntries[i].lpClass->IsCanQuit(h); \
			if(!r) \
				break; \
		} \
	}

#define RD_ON_INIT_MSG(w, l) \
	{ \
		BOOL bRet = FALSE;\
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			bRet = _toolsEntries[i].lpClass->OnInit(w, l); \
			if(!bRet) \
				return bRet; \
		} \
	}

#define RD_ON_LANG_MSG(manager, lang) \
	{ \
		BOOL bRet = FALSE;\
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			if(!_toolsEntries[i].lpClass) \
				return FALSE; \
			bRet = _toolsEntries[i].lpClass->SetLang(manager, lang); \
			if(!bRet) \
				return bRet; \
		} \
	}

#define RD_ON_CREATE_CONTROL_MSG(c, p) \
	{ \
		CControlUI *pControl = NULL;\
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			if(!_toolsEntries[i].lpClass) \
				return NULL; \
			pControl = _toolsEntries[i].lpClass->OnCreateControl(c, p); \
			if(pControl) \
				return pControl;\
		} \
	}

#define RD_ON_COMMON_MSG(h, p, m, b, fn) \
	{ \
		b = FALSE; \
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			if(!_toolsEntries[i].lpClass) \
				return ; \
			_toolsEntries[i].lpClass->fn(h, p, m, b); \
			if(b) \
				break; \
		} \
	} 

#define RD_ON_ITEMTEXT_MSG(h, p, c, x, y) \
	{ \
		LPCWSTR lpData = L""; \
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			if(!_toolsEntries[i].lpClass) \
				return FALSE; \
			lpData = _toolsEntries[i].lpClass->GetItemText(h, p, c, x, y); \
			if(wcscmp(lpData, L"")!=0) \
				break; \
		} \
		return lpData; \
	} 

#define RD_ON_QUIT() \
	{ \
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			if(!_toolsEntries[i].lpClass) \
				return; \
			_toolsEntries[i].lpClass->OnQuit(); \
		} \
	} 

#define RD_ON_CUSTOM_MSG(u, w, l) \
	{ \
		HRESULT hRes = 0;\
		BOOL bHandle = FALSE; \
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			if(!_toolsEntries[i].lpClass) \
				return hRes; \
			hRes = _toolsEntries[i].lpClass->HandleCustomMessage(u, w, l, bHandle); \
			if(hRes) \
				break; \
		} \
	} 

#define RD_ON_COPYDATA_MSG(w, l) \
	{ \
		BOOL bRet = FALSE;\
		for(int i=0; i<_countof(_toolsEntries)-1; i++) \
		{ \
			if(!_toolsEntries[i].lpClass) \
				return bRet; \
			bRet = _toolsEntries[i].lpClass->OnCopyData(w, l); \
			if(bRet) \
				break; \
		} \
	} 
