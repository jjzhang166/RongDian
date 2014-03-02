#pragma once

//////////////////////////////////////////////////////////////////////////
// CUrlEncoder
class CUrlEncoder : public CBaseTool
{
public:
	CUrlEncoder();
	virtual ~CUrlEncoder() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/);

	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

public:
	CCheckBoxUI * m_pEscapeCB;
	CCheckBoxUI * m_pGb2312CB;
	CCheckBoxUI * m_pUtf8CB;
	CButtonUI * m_pEncodeBtn;
	CButtonUI * m_pDecodeBtn;
	CButtonUI * m_pCopyBtn;
	CButtonUI * m_pClearBtn;
	CRichEditUI * m_pEncodingEdit;
	CRichEditUI * m_pEncodedEdit;
};
// CUrlEnCoder
//////////////////////////////////////////////////////////////////////////