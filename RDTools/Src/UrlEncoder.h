#pragma once

//////////////////////////////////////////////////////////////////////////
// CUrlEnCoder
RD_DECLARE_BEGIN(UrlEncoder)
RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);

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
RD_DECLARE_END()
// CUrlEnCoder
//////////////////////////////////////////////////////////////////////////