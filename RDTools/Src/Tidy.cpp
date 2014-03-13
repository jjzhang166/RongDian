#include "StdAfx.h"
#include "Tidy.h"
#include "TidyHelper.h"

const wchar_t* const kTidyList = L"tidy_list";
// 
const wchar_t* const kTidyPaste = L"tidy_paste";
const wchar_t* const kTidyCut = L"tidy_cut";
const wchar_t* const kTidyCopy = L"tidy_copy";
const wchar_t* const kTidySelAll = L"tidy_selall";
const wchar_t* const kTidyClipbrdText = L"tidy_clipbrd_text";
//
const wchar_t* const kTidyFormat = L"tidy_format";
const wchar_t* const kTidyFormatSel = L"tidy_format_sel";
const wchar_t* const kTidyFormatBatch = L"tidy_format_batch";
const wchar_t* const kTidyOperatorText = L"tidy_operator_text";
//
const wchar_t* const kTidyLanguage = L"tidy_language";
const wchar_t* const kTidyIndentMore = L"tidy_indent_more";
const wchar_t* const kTidyIndentLess = L"tidy_indent_less";
const wchar_t* const kTidyComment = L"tidy_comment";
const wchar_t* const kTidyUnComment = L"tidy_uncomment";
const wchar_t* const kTidyStyleText = L"tidy_style_text";

// Panel Contents
const wchar_t* const kTidyTab = L"tidy_tab";
const wchar_t* const kTidyPanels = L"tidy_panels";
const wchar_t* const kTidyTabButton = L"TidyTabButton";
const wchar_t* const kTidyHotTabButton = L"TidyHotTabButton";
const wchar_t* const kTidyNew = L"tidy_new";

//
const wchar_t* const kTidyLanguges = L"tidy_languges";
const wchar_t* const kTidyLangsCombo = L"tidy_languges_combo";
const wchar_t* const kTidyEditor = L"tidy_editor";

CTidy::CTidy()
{
	m_pLangsCombo = NULL;
	m_pEditor = NULL;
}

BOOL CTidy::IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/)
{
	return TRUE;
}

void CTidy::OnQuit()
{

}

BOOL CTidy::OnInit(WPARAM wParam, LPARAM /*lParam*/)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	if(!pManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_pLangsCombo, CComboUI, pManager, kTidyLangsCombo)
	FIND_CONTROL_BY_ID(m_pEditor, CRichEditUI, pManager, kTidyEditor)

	InitLangs();
	return TRUE;
}

BOOL CTidy::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang, LPCWSTR lpszLangSection)
{
	SET_CONTROL_BEGIN(pManager, lpszLang, lpszLangSection)
		// Topic
		SET_CONTROL_TEXT2(kTidyLanguges)
		SET_CONTROL_TEXT2(kTidyFormat)
	SET_CONTROL_END()
	return TRUE;
}

void CTidy::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kTidyFormat)
	{
		bHandled = OnTidyFormat();
	}
}

void CTidy::OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(!pParent)
		return;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return;
	if(pList->GetName()!=kTidyList)
		return;
	CDuiString sCtrlName = msg.pSender->GetClass();
	if(sCtrlName==L"ListLabelElementUI")
	{

	}
}

BOOL CTidy::InitLangs()
{
	for (int i=0;i<MAX_TIDY_LANG-1;i++)
	{
		char aIndex[3];
		wchar_t wIndex[3];
		wchar_t* name = g_TidyInfo[i].szName;
		itoa(g_TidyInfo[i].uID,aIndex,10);
		StrUtil::a2w(aIndex,wIndex);
		CListLabelElementUI* pItem = new CListLabelElementUI();
		pItem->SetText(name);
		pItem->SetUserData(wIndex);
		m_pLangsCombo->Add(pItem);
	}
	if (m_pLangsCombo->GetCount()>0)
	{
		m_pLangsCombo->SelectItem(0);
	}
	return TRUE;
}

BOOL CTidy::OnTidyFormat()
{
	int curSel = m_pLangsCombo->GetCurSel();
	if (curSel<0)
	{
		return FALSE;
	}
	char aUID[4];
	CListLabelElementUI* pItem = (CListLabelElementUI*)m_pLangsCombo->GetItemAt(curSel);
	LPCTSTR wUID = pItem->GetUserData().GetData();
	StrUtil::w2a(wUID,aUID);
	int uid = atoi(aUID);
	CDuiString sContent = m_pEditor->GetText();
	if (sContent.IsEmpty())
	{
		return TRUE;
	}
	LPCSTR psContentA = StrUtil::w2a(sContent.GetData());
	LPWSTR pszTextOut = NULL, pszMsgOut = NULL;
	CTidyHelper tidy;
	tidy.DoTidy(uid, psContentA, &pszTextOut, &pszMsgOut);
	if(pszTextOut && wcslen(pszTextOut)>0)
	{
		m_pEditor->SetText(pszTextOut);
		delete pszTextOut;
	}
	else
	{
		m_pEditor->SetText(pszMsgOut);
	}
	if(pszMsgOut)
		delete pszMsgOut;
	return TRUE;
}
