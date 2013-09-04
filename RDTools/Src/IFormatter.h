#pragma once
#include "Formatter.h"

typedef enum _tagFORMAT_TYPE
{
	FORMAT_UNDEFINE = -1,
	FORMAT_XML = 1,
	FORMAT_JSON = 2
} FORMAT_TYPE;

class IFormatter
{
public:
	IFormatter();
	virtual ~IFormatter();

public:
	void			SetFormatterOwner(HWND hWnd, CPaintManagerUI *pManager) { m_hFormatterOwner = hWnd; m_pFormatterManager = pManager; };
	BOOL			InitFormatter();
	BOOL			SetFormatterLang(LPCWSTR lpszLang);

	void			OnFormatterClick(TNotifyUI& msg, BOOL& bHandled);

public:
	HWND m_hFormatterOwner;
	CPaintManagerUI *m_pFormatterManager;
	CEditUI *m_pFormatterContent;
	wchar_t m_szHandleText[1024];
	wchar_t m_szNoHandleText[1024];
	FORMAT_TYPE m_nFormatterType; // 当前选择的文本类型
	CFormatter *m_pFormatter;
};
