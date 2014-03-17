#include "StdAfx.h"
#include "Finder.h"

const wchar_t* const kFinderNameText = L"finder_name_text";
const wchar_t* const kFinderNameEdit = L"finder_name_edit";
const wchar_t* const kFinderContentText = L"finder_content_text";
const wchar_t* const kFinderContentEdit = L"finder_content_edit";
const wchar_t* const kFinderPathText = L"finder_path_text";
const wchar_t* const kFinderPathEdit = L"finder_path_edit";
const wchar_t* const kFinderPathSelect = L"finder_path_select";
const wchar_t* const kFinderPathSelectTip = L"finder_path_select_tip";
const wchar_t* const kFinderList = L"finder_list";
const wchar_t* const kFinderHeaderFile = L"finder_header_file";
const wchar_t* const kFinderHeaderContent = L"finder_header_content";
const wchar_t* const kFinderHeaderSize = L"finder_header_size";
const wchar_t* const kFinderReset = L"finder_reset";
const wchar_t* const kFinderResetTip = L"finder_reset_tip";
const wchar_t* const kFinderSearch = L"finder_search";
const wchar_t* const kFinderSearchTip = L"finder_search_tip";

CFinder::CFinder()
{
	pFinderList = NULL;
	pFinderNameEdit = NULL;
	pFinderContentEdit = NULL;
	pFinderPathEdit = NULL;
}

BOOL CFinder::IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/)
{
	return TRUE;
}

void CFinder::OnQuit()
{
	ResetFinder();
}

BOOL CFinder::OnInit(WPARAM wParam, LPARAM lParam)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	IListCallbackUI* pIListCallback = (IListCallbackUI *)lParam;
	if(!pManager || !pIListCallback)
		return FALSE;
	FIND_CONTROL_BY_ID(pFinderList, CListUI, pManager, kFinderList)
	FIND_CONTROL_BY_ID(pFinderNameEdit, CEditUI, pManager, kFinderNameEdit)
	FIND_CONTROL_BY_ID(pFinderContentEdit, CEditUI, pManager, kFinderContentEdit)
	FIND_CONTROL_BY_ID(pFinderPathEdit, CEditUI, pManager, kFinderPathEdit)
	if(pFinderList)
		pFinderList->SetTextCallback(pIListCallback);
	return TRUE;
}

BOOL CFinder::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang, LPCWSTR lpszLangSection)
{
	if(!pManager)
		return FALSE;
SET_CONTROL_BEGIN(pManager, lpszLang, lpszLangSection)
	SET_CONTROL_TEXT2(kFinderNameText)
	SET_CONTROL_TEXT2(kFinderContentText)
	SET_CONTROL_TEXT2(kFinderPathText)
	SET_CONTROL_TEXT_AND_TIP(kFinderPathSelect, kFinderPathSelectTip)
	SET_CONTROL_TEXT2(kFinderHeaderFile)
	SET_CONTROL_TEXT2(kFinderHeaderContent)
	SET_CONTROL_TEXT2(kFinderHeaderSize)
	SET_CONTROL_TEXT_AND_TIP(kFinderReset, kFinderResetTip)
	SET_CONTROL_TEXT_AND_TIP(kFinderSearch, kFinderSearchTip)
SET_CONTROL_END()

	return TRUE;
}

void CFinder::OnClick(HWND hWnd, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kFinderPathSelect)
	{
		SelectFolder(hWnd);
		bHandled = TRUE;
	}
	else if(sCtrlName==kFinderReset)
	{
		ResetFinder();
		bHandled = TRUE;
	}
	else if(sCtrlName==kFinderSearch)
	{
		StartFinder(hWnd);
		bHandled = TRUE;
	}
}

void CFinder::OnItemActive(HWND hWnd, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(!pParent)
		return;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return;
	if(pList->GetName()!=kFinderList)
		return;
	CDuiString sCtrlName = msg.pSender->GetClass();
	CListTextElementUI *pElement = (CListTextElementUI *)msg.pSender;
	if(!pElement)
		return;
	if(sCtrlName!=L"ListTextElementUI")
		return;
	int nIndex = pElement->GetIndex();
	if(nIndex>=(int)lstFinderInfo.size())
		return;
	LPFINDER_INFO pFinderInfo = (LPFINDER_INFO)lstFinderInfo[nIndex];
	if(pFinderInfo)
		SHHelper::OpenFile(hWnd, pFinderInfo->szPath);
}

LPCWSTR CFinder::GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* pControl, int iIndex, int iSubItem)
{
	LPCWSTR lpszData = L"";
	CListBodyUI *pParent = (CListBodyUI *)pControl->GetParent();
	if(!pParent)
		return lpszData;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return lpszData;
	if(pList->GetName()==kFinderList)
	{
		lpszData = GetFinderItemText(pControl, iIndex, iSubItem);
	}
	return lpszData;
}

LPCWSTR CFinder::GetFinderItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	TCHAR szText[1024] = {0};
	LPFINDER_INFO pFinderInfo = lstFinderInfo[iIndex];
	if(!pFinderInfo)
		return NULL;
	switch (iSubItem)
	{
	case 0:
		pControl->SetAttribute(L"align", L"left");
		wcscpy(szText, pFinderInfo->szPath);
		break;
	case 1:
		break;
	case 2:
		swprintf(szText, L"%lld", pFinderInfo->llSize);
		break;
	}
	pControl->SetUserData(szText);
	return pControl->GetUserData();
}

BOOL CFinder::SelectFolder(HWND hWnd)
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024];
	wchar_t szTitle[1024];
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kFolderTitle, szTitle);
	bRet = SHHelper::SelectFolder(hWnd, szPath, szTitle);
	if(bRet && pFinderPathEdit)
	{
		pFinderPathEdit->SetText(szPath);
		bRet = TRUE;
	}

	return bRet;
}

BOOL CFinder::ResetFinder()
{
	BOOL bRet = FALSE;
	if(!pFinderList)
		return bRet;
	if(pFinderPathEdit)
		pFinderPathEdit->SetText(L"");
	if(pFinderContentEdit)
		pFinderContentEdit->SetText(L"");
	if(pFinderNameEdit)
		pFinderNameEdit->SetText(L"");
	finder.Reset();
	LPFINDER_INFO lpFinderInfo = NULL;
	if(lstFinderInfo.size())
		lpFinderInfo = (LPFINDER_INFO)lstFinderInfo.back();
	while(lpFinderInfo)
	{
		lstFinderInfo.pop_back();
		delete lpFinderInfo;
		lpFinderInfo = NULL;
		if(lstFinderInfo.size())
			lpFinderInfo = (LPFINDER_INFO)lstFinderInfo.back();
	}
	pFinderList->RemoveAll();
	bRet = TRUE;
	return bRet;
}

BOOL CFinder::StartFinder(HWND hWnd)
{
	BOOL bRet = FALSE;
	if(!pFinderPathEdit || !pFinderContentEdit || !pFinderNameEdit)
		return bRet;
	CFinderOpts opts;
	CDuiString strPath = pFinderPathEdit->GetText();
	CDuiString strContent = pFinderContentEdit->GetText();
	CDuiString strName = pFinderNameEdit->GetText();
	if(strPath.IsEmpty())
	{
		RDMsgBox(hWnd, MSG_INVALID_PATH, MSG_ERR, MB_OK);
		pFinderPathEdit->SetFocus();
		return bRet;
	}
	if(strContent.IsEmpty() && strName.IsEmpty())
	{
		RDMsgBox(hWnd, MSG_FINDER_ERR, MSG_ERR, MB_OK);
		if(strName.IsEmpty())
			pFinderNameEdit->SetFocus();
		if(strContent.IsEmpty())
			pFinderContentEdit->SetFocus();
		return bRet;
	}
	opts.SetFolder(strPath.GetData());
	if(!strName.IsEmpty())
		opts.SetName(strName);
	if(!strContent.IsEmpty())
		opts.SetText(strContent);
	opts.FindAllFiles();
	opts.RecusiveSearch();
	finder.SetCallback(FinderCallBack, (LPVOID)this);
	finder.Find(opts);
	return TRUE;
}

LRESULT CFinder::FinderCallBack(LPFINDER_INFO lpInfo, WPARAM /*wParam*/, LPARAM lParam)
{
	LRESULT lRet = 0;
	CFinder *pFinder = (CFinder *)lParam;
	if(pFinder && pFinder->pFinderList)
	{
		CListTextElementUI* pElement = new CListTextElementUI();
		if(!pElement)
			return lRet;
		LPFINDER_INFO pFinderInfo = new FINDER_INFO();
		if(!pFinderInfo)
			return lRet;
		wcscpy(pFinderInfo->szPath, lpInfo->szPath);
		pFinderInfo->llSize = lpInfo->llSize;

		pFinder->lstFinderInfo.push_back(pFinderInfo);
		pFinder->pFinderList->Add(pElement);
	}
	return 0;
}