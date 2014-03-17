#include "StdAfx.h"
#include "UrlEncoder.h"

const wchar_t* const kUrlEncoderEdit = L"encoder_text";
const wchar_t* const kUrlEncoderEncodedEdit = L"encoded_text";
const wchar_t* const kUrlEncoderEscapeCB = L"escape";		
const wchar_t* const kUrlEncoderGb2312CB = L"gb2312";
const wchar_t* const kUrlEncoderUtf8CB = L"utf8";
const wchar_t* const kUrlEncoderEncodeBtn = L"encode";
const wchar_t* const kUrlEncoderDecodeBtn = L"decode";
const wchar_t* const kUrlEncoderCopyBtn = L"copy";
const wchar_t* const kUrlEncoderClearBtn = L"clear";

CUrlEncoder::CUrlEncoder()
{
	m_pEscapeCB = NULL;
	m_pGb2312CB = NULL;
	m_pUtf8CB = NULL;
	m_pEncodeBtn = NULL;
	m_pDecodeBtn = NULL;
	m_pCopyBtn = NULL;
	m_pClearBtn = NULL;
	m_pEncodingEdit = NULL;
	m_pEncodedEdit = NULL;
}

BOOL CUrlEncoder::OnInit(WPARAM wParam, LPARAM /*lParam*/)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	if(!pManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_pEscapeCB, CCheckBoxUI, pManager, kUrlEncoderEscapeCB)
	FIND_CONTROL_BY_ID(m_pGb2312CB, CCheckBoxUI, pManager, kUrlEncoderGb2312CB)
	FIND_CONTROL_BY_ID(m_pUtf8CB, CCheckBoxUI, pManager, kUrlEncoderUtf8CB)
	FIND_CONTROL_BY_ID(m_pEncodeBtn, CButtonUI, pManager, kUrlEncoderEncodeBtn)
	FIND_CONTROL_BY_ID(m_pDecodeBtn, CButtonUI, pManager, kUrlEncoderDecodeBtn)
	FIND_CONTROL_BY_ID(m_pCopyBtn, CButtonUI, pManager, kUrlEncoderCopyBtn)
	FIND_CONTROL_BY_ID(m_pClearBtn, CButtonUI, pManager, kUrlEncoderClearBtn)
	FIND_CONTROL_BY_ID(m_pEncodingEdit, CRichEditUI, pManager, kUrlEncoderEdit)
	FIND_CONTROL_BY_ID(m_pEncodedEdit, CRichEditUI, pManager, kUrlEncoderEncodedEdit)
	return TRUE;
}

BOOL CUrlEncoder::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang, LPCWSTR lpszLangSection)
{
	if(!pManager)
		return FALSE;
	SET_CONTROL_BEGIN(pManager, lpszLang, lpszLangSection)
		SET_CONTROL_TEXT2(kUrlEncoderEscapeCB)
		SET_CONTROL_TEXT2(kUrlEncoderGb2312CB)
		SET_CONTROL_TEXT2(kUrlEncoderUtf8CB)
		SET_CONTROL_TEXT2(kUrlEncoderEncodeBtn)
		SET_CONTROL_TEXT2(kUrlEncoderDecodeBtn)
		SET_CONTROL_TEXT2(kUrlEncoderCopyBtn)
		SET_CONTROL_TEXT2(kUrlEncoderClearBtn)
	SET_CONTROL_END()
	return TRUE;
}

BOOL CUrlEncoder::IsCanQuit(HWND hWnd, CPaintManagerUI* pManager)
{
	return TRUE;
}

void CUrlEncoder::OnQuit()
{
}

void CUrlEncoder::OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& /*bHandled*/)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kUrlEncoderEncodeBtn&&!m_pEncodingEdit->GetText().IsEmpty())
	{
		CDuiString duiStr = m_pEncodingEdit->GetText();
		LPCTSTR source = duiStr.GetData();
		if(m_pEscapeCB->GetCheck())
		{
			int len = wcslen(source);
			wchar_t* encodedStr = UrlEncode::escape(source);
			m_pEncodedEdit->SetText(encodedStr);
			delete[] encodedStr;
		}
		else if(m_pGb2312CB->GetCheck())
		{
			char* gbk = StrUtil::w2a(source);
			char* encodedStr = UrlEncode::url_encode(gbk);
			wchar_t* uStr = StrUtil::a2w(encodedStr);
			m_pEncodedEdit->SetText(uStr);
			delete[] gbk;
			delete[] encodedStr;
			delete[] uStr;
		}
		else if(m_pUtf8CB->GetCheck())
		{
			char* utf8 = StrUtil::u2u8(source);
			char* encodedStr = UrlEncode::url_encode(utf8);
			wchar_t* uStr = StrUtil::u82u(encodedStr);
			m_pEncodedEdit->SetText(uStr);
			delete[] utf8;
			delete[] encodedStr;
			delete[] uStr;
		}
	}
	else if (sCtrlName==kUrlEncoderDecodeBtn)
	{
		CDuiString duiStr = m_pEncodingEdit->GetText();
		LPCTSTR source = duiStr.GetData();
		if(m_pEscapeCB->GetCheck())
		{
			wchar_t* encodedStr = UrlEncode::unescape(source);
			m_pEncodedEdit->SetText(encodedStr);
			delete[] encodedStr;
		}
		else if(m_pGb2312CB->GetCheck())
		{
			char* aSource = StrUtil::w2a(source);
			char* encodedStr = UrlEncode::url_unencode(aSource);
			wchar_t* uencodedStr = StrUtil::a2w(encodedStr);
			m_pEncodedEdit->SetText(uencodedStr);
			delete[] aSource;
			delete[] encodedStr;
			delete[] uencodedStr;
		}
		else if(m_pUtf8CB->GetCheck())
		{
			char* aSource = StrUtil::w2a(source);
			char* encodedStr = UrlEncode::url_unencode(aSource);
			wchar_t* uencodedStr = StrUtil::u82u(encodedStr);
			m_pEncodedEdit->SetText(uencodedStr);
			delete[] aSource;
			delete[] encodedStr;
			delete[] uencodedStr;
		}
	}
	else if (sCtrlName==kUrlEncoderCopyBtn)
	{
		CDuiString duiStr = m_pEncodedEdit->GetText();
		LPCTSTR source = duiStr.GetData();
		char* aSource = StrUtil::w2a(source);
		Utility::CopyTextToClipBoard(aSource);
		delete[] aSource;
	}
	else if (sCtrlName==kUrlEncoderClearBtn)
	{
		m_pEncodingEdit->SetText(L"");
		m_pEncodedEdit->SetText(L"");
	}
}