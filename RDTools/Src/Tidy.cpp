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

CTidy::CTidy()
{
	pTidyTab = NULL;
	pTidyPanels = NULL;
	nNewTabIndex = 1;
	strCurTabName.Format(L"%s0", kTidyTab);
}

BOOL CTidy::IsCanQuit(HWND /*hWnd*/)
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
	FIND_CONTROL_BY_ID(pTidyTab, CHorizontalLayoutUI, pManager, kTidyTab)
	FIND_CONTROL_BY_ID(pTidyPanels, CVerticalLayoutUI, pManager, kTidyPanels)
	return TRUE;
}

BOOL CTidy::SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/)
{
	return TRUE;
}

CControlUI* CTidy::OnCreateControl(CPaintManagerUI* pManager, LPCTSTR pstrClass, CControlUI* /*pParent*/)
{
	CControlUI* pControl = NULL;
	if(!pManager)
		return pControl;
	if(wcsicmp(pstrClass, kTidyTabButton)==0)
	{
		pControl = new CButtonUI();
		LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(kTidyTabButton);
		if(pDefaultAttributes && pControl)
			pControl->ApplyAttributeList(pDefaultAttributes);
		if(pControl)
			return pControl;
	}
	else if(wcsicmp(pstrClass, kTidyHotTabButton)==0)
	{
		pControl = new CButtonUI();
		LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(kTidyHotTabButton);
		if(pDefaultAttributes && pControl)
			pControl->ApplyAttributeList(pDefaultAttributes);
		if(pControl)
			return pControl;
	}
	return pControl;
}

void CTidy::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kTidyNew) // 新建标签
	{
		bHandled = TRUE;
		OnNewTab(hWnd, pManager, msg);
	}
	else if(wcsstr(sCtrlName.GetData(), kTidyTab))
	{
		bHandled = TRUE;
		OnSelTab(hWnd, pManager, msg);
	}
	else if(sCtrlName==kTidyFormat)
	{
		bHandled = TRUE;
		OnTidyNormal(hWnd, pManager, msg);
	}
	else if(sCtrlName==kTidyFormatSel)
	{
		bHandled = TRUE;
		OnTidySel(hWnd, pManager, msg);
	}
	else if(sCtrlName==kTidyFormatBatch)
	{
		bHandled = TRUE;
		OnTidyBatch(hWnd, pManager, msg);
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

BOOL CTidy::OnNewTab(HWND /*hWnd*/, CPaintManagerUI* pManager, TNotifyUI& /*msg*/)
{
	if(!pTidyTab || !pTidyPanels)
		return FALSE;
	LPCTSTR pHotAttributes = pManager->GetDefaultAttributeList(kTidyHotTabButton);
	LPCTSTR pNormalAttributes = pManager->GetDefaultAttributeList(kTidyTabButton);
	CDuiString sSelTabName = pTidyTab->GetUserData();
	CButtonUI *pSelTab = NULL;
	FIND_CONTROL_BY_ID(pSelTab, CButtonUI, pManager, sSelTabName.GetData())
	if(pSelTab)
	{
		pSelTab->ApplyAttributeList(pNormalAttributes);
		CDuiString sSelEdit = pSelTab->GetUserData();
		CRichEditUI *pSelEdit = NULL;
		FIND_CONTROL_BY_ID(pSelEdit, CRichEditUI, pManager, sSelEdit.GetData())
		if(pSelEdit)
			pSelEdit->SetVisible(false);
	}
	int nCount = pTidyTab->GetCount();
	CControlUI *pNewTabBlank = new CControlUI();
	pNewTabBlank->SetFixedWidth(1);
	const int nFixControlCount = 3; // 标签兰尾部固定控件个数
	pTidyTab->AddAt(pNewTabBlank, nCount-nFixControlCount - 1); // - 1，控件索引值从0开始
	CButtonUI *pNewTab = new CButtonUI();
	wchar_t szNewTab[1024] = {0}, szNewText[1024] = {0}, szNewEdit[1024] = {0};
	swprintf(szNewTab, L"tidy_tab%d", nNewTabIndex);
	swprintf(szNewText, L"NewFile%d", nNewTabIndex);
	swprintf(szNewEdit, L"tidy_edit%d", nNewTabIndex);
	nNewTabIndex++;
	pNewTab->SetName(szNewTab);
	pNewTab->SetText(szNewText);
	pNewTab->SetUserData(szNewEdit);
	if(pHotAttributes && pNewTab)
		pNewTab->ApplyAttributeList(pHotAttributes);
	pTidyTab->AddAt(pNewTab, nCount-4);
	pTidyTab->SetUserData(szNewTab);
	CRichEditUI *pNewEdit = new CRichEditUI();
	pNewEdit->SetName(szNewEdit);
	pNewEdit->SetText(szNewEdit);
	pTidyPanels->Add(pNewEdit);
	strCurTabName = szNewTab;
	return TRUE;
}

BOOL CTidy::OnSelTab(HWND /*hWnd*/, CPaintManagerUI* pManager, TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName != strCurTabName)
	{
		strCurTabName = sCtrlName;
		LPCTSTR pHotAttributes = pManager->GetDefaultAttributeList(kTidyHotTabButton);
		LPCTSTR pNormalAttributes = pManager->GetDefaultAttributeList(kTidyTabButton);
		CDuiString sSelTabName = pTidyTab->GetUserData();
		CButtonUI *pSelTab = NULL;
		FIND_CONTROL_BY_ID(pSelTab, CButtonUI, pManager, sSelTabName.GetData())
		if(!pSelTab)
			return FALSE;
		pSelTab->ApplyAttributeList(pNormalAttributes);
		CDuiString sSelEdit = pSelTab->GetUserData();
		CRichEditUI *pSelEdit = NULL;
		FIND_CONTROL_BY_ID(pSelEdit, CRichEditUI, pManager, sSelEdit.GetData())
		if(pSelEdit)
			pSelEdit->SetVisible(false);
		pTidyTab->SetUserData(sCtrlName);
		msg.pSender->ApplyAttributeList(pHotAttributes);
		CDuiString sActiveEdit = msg.pSender->GetUserData();
		CRichEditUI *pActiveEdit = NULL;
		FIND_CONTROL_BY_ID(pActiveEdit, CRichEditUI, pManager, sActiveEdit.GetData())
		if(pActiveEdit)
			pActiveEdit->SetVisible(true);
	}
	return TRUE;
}

BOOL CTidy::OnTidyNormal(HWND /*hWnd*/, CPaintManagerUI* pManager, TNotifyUI& msg)
{
	CDuiString sSelTabName = pTidyTab->GetUserData();
	CButtonUI *pSelTab = NULL;
	FIND_CONTROL_BY_ID(pSelTab, CButtonUI, pManager, sSelTabName.GetData())
	if(!pSelTab)
		return FALSE;
	CDuiString sSelEdit = pSelTab->GetUserData();
	CRichEditUI *pSelEdit = NULL;
	FIND_CONTROL_BY_ID(pSelEdit, CRichEditUI, pManager, sSelEdit.GetData())
	if(!pSelEdit)
		return FALSE;
	CDuiString sContent = pSelEdit->GetText();
	LPCSTR psContentA = StrUtil::w2a(sContent.GetData());
	LPWSTR pszTextOut = NULL, pszMsgOut = NULL;
	CTidyHelper tidy;
	tidy.DoTidy(TIDY_CPP, psContentA, &pszTextOut, &pszMsgOut);
	if(pszTextOut)
	{
		pSelEdit->SetText(pszTextOut);
		delete pszTextOut;
	}
	if(pszMsgOut)
		delete pszMsgOut;
	return TRUE;
}

BOOL CTidy::OnTidySel(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/)
{
	return FALSE;
}

BOOL CTidy::OnTidyBatch(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/)
{
	return FALSE;
}
