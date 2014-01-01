#include "StdAfx.h"
#include "Coder.h"

const wchar_t* const kCoderListText = L"coder_list_text";
const wchar_t* const kCoderList = L"coder_list";
const wchar_t* const kCoderHeaderPath = L"coder_header_path";
const wchar_t* const kCoderHeaderEncoding = L"coder_header_encoding";
const wchar_t* const kCoderHeaderStatus = L"coder_header_status";
const wchar_t* const kCoderStart = L"coder_start";
const wchar_t* const kCoderStartTip = L"coder_start_tip";
const wchar_t* const kCoderStop = L"coder_stop";
const wchar_t* const kCoderStopTip = L"coder_stop_tip";
const wchar_t* const kCoderNewFile = L"coder_newfile";
const wchar_t* const kCoderNewFileTip = L"coder_newfile_tip";
const wchar_t* const kCoderNewFolder = L"coder_newfolder";
const wchar_t* const kCoderNewFolderTip = L"coder_newfolder_tip";
const wchar_t* const kCoderDel = L"coder_del";
const wchar_t* const kCoderDelTip = L"coder_del_tip";
const wchar_t* const kCoderDelAll = L"coder_delall";
const wchar_t* const kCoderDelAllTip = L"coder_delall_tip";
const wchar_t* const kCoderFrom = L"coder_from";
const wchar_t* const kCoderComboFrom = L"coder_combo_from";
const wchar_t* const kCoderTo = L"coder_to";
const wchar_t* const kCoderComboTo = L"coder_combo_to";
const wchar_t* const kCoderBOM = L"coder_bom";
const wchar_t* const kCoderBOMTip = L"coder_bom_tip";
const wchar_t* const kCoderBackup = L"coder_backup";
const wchar_t* const kCoderBackupTip = L"coder_backup_tip";
const wchar_t* const kCoderPathText = L"coder_path_text";
const wchar_t* const kCoderPathEdit = L"coder_path_edit";
const wchar_t* const kCoderPathEditTip = L"coder_path_edit_tip";
const wchar_t* const kCoderPathOpen = L"coder_path_open";
const wchar_t* const kCoderPathOpenTip = L"coder_path_open_tip";

CCoder::CCoder()
{
	pCoderList = NULL;
	pCoderComboFrom = NULL;
	pCoderComboTo = NULL;
	pCoderBackupCheck = NULL;
	pCoderNoBOM = NULL;
	pCoderBackupEdit = NULL;
	bCoderBackup = TRUE;
	bWithBom = FALSE;
	bWorking = FALSE;
	bQuitThread = TRUE;
	memset(szHandleText, 0, sizeof(szHandleText));
	memset(szHandleErrText, 0, sizeof(szHandleErrText));
	memset(szNoHandleText, 0, sizeof(szNoHandleText));
	dwHightLightColor = 0;
	dwGrayColor = 0;
	hConvertThread = NULL;
}

BOOL CCoder::IsCanQuit(HWND hWnd)
{
	if(hConvertThread)
	{
		RDMsgBox(hWnd, MSG_BUSY, MSG_ERR, MB_OK);
		return FALSE;
	}
	return TRUE;
}

void CCoder::OnQuit()
{
	list<LPCHARSET_INFO>::iterator charset_iter;
	LPCHARSET_INFO lpCharSetInfo = NULL;
	for(charset_iter=lstCharSetsInfo.begin(); charset_iter!=lstCharSetsInfo.end(); )
	{
		lpCharSetInfo = (*charset_iter);
		charset_iter++;
		delete lpCharSetInfo;
	}
	lstCharSetsInfo.clear();
	DelAllFiles();
}

BOOL CCoder::OnInit(WPARAM wParam, LPARAM lParam)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	IListCallbackUI* pIListCallback = (IListCallbackUI *)lParam;
	if(!pManager || !pIListCallback)
		return FALSE;
	FIND_CONTROL_BY_ID(pCoderList, CListUI, pManager, kCoderList)
	FIND_CONTROL_BY_ID(pCoderComboFrom, CComboUI, pManager, kCoderComboFrom)
	FIND_CONTROL_BY_ID(pCoderComboTo, CComboUI, pManager, kCoderComboTo)
	FIND_CONTROL_BY_ID(pCoderBackupCheck, CButtonUI, pManager, kCoderBackup)
	if(pCoderBackupCheck)
	{
		pCoderBackupCheck->SetUserData(L"TRUE");
		dwHightLightColor = pCoderBackupCheck->GetBorderColor();
	}
	FIND_CONTROL_BY_ID(pCoderNoBOM, CButtonUI, pManager, kCoderBOM)
	if(pCoderNoBOM)
	{
		pCoderNoBOM->SetUserData(L"FALSE");
		dwGrayColor = pCoderNoBOM->GetBkColor();
	}
	FIND_CONTROL_BY_ID(pCoderBackupEdit, CEditUI, pManager, kCoderPathEdit)
		if(pCoderBackupEdit)
			pCoderBackupEdit->SetText(g_szCoderBackupPath);
	if(pCoderList)
		pCoderList->SetTextCallback(pIListCallback);
	if(!PathFileExists(g_szCoderBackupPath))
		SHCreateDirectoryEx(NULL, g_szCoderBackupPath, NULL);

	LoadCharSet();
	InitCharSetCombo();
	
	return TRUE;
}

BOOL CCoder::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang)
{
	if(!pManager)
		return FALSE;
	memset(szHandleText, 0, sizeof(szHandleText));
	memset(szHandleErrText, 0, sizeof(szHandleErrText));
	memset(szNoHandleText, 0, sizeof(szNoHandleText));
	Utility::GetINIStr(lpszLang, LS_MSG, kHandleText, szHandleText);
	Utility::GetINIStr(lpszLang, LS_MSG, kHandleErrText, szHandleErrText);
	Utility::GetINIStr(lpszLang, LS_MSG, kNoHandleText, szNoHandleText);

SET_CONTROL_BEGIN(pManager, lpszLang, LS_CODERPANEL)
	SET_CONTROL_TEXT2(kCoderListText)
	SET_CONTROL_TEXT2(kCoderHeaderPath)
	SET_CONTROL_TEXT2(kCoderHeaderEncoding)
	SET_CONTROL_TEXT2(kCoderHeaderStatus)
	SET_CONTROL_TEXT_AND_TIP(kCoderStart, kCoderStartTip)
	SET_CONTROL_TEXT_AND_TIP(kCoderStop, kCoderStopTip)
	SET_CONTROL_TEXT_AND_TIP(kCoderNewFile, kCoderNewFileTip)
	SET_CONTROL_TEXT_AND_TIP(kCoderNewFolder, kCoderNewFolderTip)
	SET_CONTROL_TEXT_AND_TIP(kCoderDel, kCoderDelTip)
	SET_CONTROL_TEXT_AND_TIP(kCoderDelAll, kCoderDelAllTip)
	SET_CONTROL_TEXT2(kCoderFrom)
	SET_CONTROL_TIP2(kCoderComboFrom)
	SET_CONTROL_TEXT2(kCoderTo)
	SET_CONTROL_TIP2(kCoderComboTo)
	SET_CONTROL_TEXT_AND_TIP(kCoderBOM, kCoderBOMTip)
	SET_CONTROL_TEXT_AND_TIP(kCoderBackup, kCoderBackupTip)
	SET_CONTROL_TEXT2(kCoderPathText)
	SET_CONTROL_TIP2(kCoderPathEditTip)
	SET_CONTROL_TEXT_AND_TIP(kCoderPathOpen, kCoderPathOpenTip)
SET_CONTROL_END()

	return TRUE;
}

void CCoder::OnClick(HWND hWnd, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kCoderStart)
	{
		StartConvert(hWnd);
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderStop)
	{
		StopConvert();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderNewFile)
	{
		AddNewFile(hWnd);
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderNewFolder)
	{
		AddNewFolder(hWnd);
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderDel)
	{
		DelSelFile();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderDelAll)
	{
		DelAllFiles();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderBOM)
	{
		if(pCoderNoBOM)
		{
			CDuiString strData = pCoderNoBOM->GetUserData();
			if(strData==L"FALSE")
			{
				pCoderNoBOM->SetBkColor(dwHightLightColor);
				pCoderNoBOM->SetUserData(L"TRUE");
				bWithBom = TRUE;
			}
			else
			{
				pCoderNoBOM->SetBkColor(dwGrayColor);
				pCoderNoBOM->SetUserData(L"FALSE");
				bWithBom = FALSE;
			}
		}
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderBackup)
	{
		if(pCoderBackupCheck)
		{
			CDuiString strData = pCoderBackupCheck->GetUserData();
			if(strData==L"FALSE")
			{
				if(dwGrayColor==0)
					dwGrayColor = pCoderBackupCheck->GetBorderColor();
				pCoderBackupCheck->SetBorderColor(dwHightLightColor);
				pCoderBackupCheck->SetUserData(L"TRUE");
				bCoderBackup = TRUE;
			}
			else
			{
				if(dwHightLightColor==0)
					dwHightLightColor = pCoderBackupCheck->GetBorderColor();
				pCoderBackupCheck->SetBorderColor(dwGrayColor);
				pCoderBackupCheck->SetUserData(L"FALSE");
				bCoderBackup = FALSE;
			}
		}
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderPathOpen)
	{
		SHHelper::OpenFolder(hWnd, g_szCoderBackupPath);
		bHandled = TRUE;
	}
}

void CCoder::OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(!pParent)
		return;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return;
	if(pList->GetName()!=kCoderList)
		return;
	CDuiString sCtrlName = msg.pSender->GetClass();
	if(sCtrlName==L"ListLabelElementUI")
	{

	}
}

LPCWSTR CCoder::GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* pControl, int iIndex, int iSubItem)
{
	LPCWSTR lpszData = NULL;
	CListBodyUI *pParent = (CListBodyUI *)pControl->GetParent();
	if(!pParent)
		return L"";
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return L"";
	if(pList->GetName()==kCoderList)
	{
		lpszData = GetCoderItemText(pControl, iIndex, iSubItem);
	}
	return lpszData;
}

LPCWSTR CCoder::GetCoderItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	TCHAR szText[1024] = {0};
	LPCODER_INFO pCoderInfo = lstCoderInfo[iIndex];
	if(!pCoderInfo)
		return NULL;
	switch (iSubItem)
	{
	case 0:
		pControl->SetAttribute(L"align", L"left");
		wcscpy(szText, pCoderInfo->szPath);
		break;
	case 1:
		wchar_t szFrom_w[64];
		StrUtil::a2w(pCoderInfo->szFrom, szFrom_w);
		wcscpy(szText, szFrom_w);
		break;
	case 2:
		if(pCoderInfo->bHandle)
			wcscpy(szText, szHandleText);
		else
			wcscpy(szText, szNoHandleText);
		break;
	}
	pControl->SetUserData(szText);
	return pControl->GetUserData();
}

BOOL CCoder::InitCharSet()
{
	BOOL bRet = FALSE;
	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCEW(IDC_CHARSET_SQL), L"SQL");
	if(hRes == NULL)
		return bRet;
	DWORD dwSize = 0;
	HGLOBAL hGlobal = ::LoadResource(NULL, hRes);
	if(hGlobal == NULL)
	{
		::FreeResource(hRes);
		return bRet; 
	}
	dwSize = ::SizeofResource(NULL, hRes);
	if(dwSize == 0)
	{
		::FreeResource(hRes);
		return bRet; 
	}
	LPBYTE lpData = (LPBYTE)::LockResource(hGlobal);
	char *pszSQL = new char[dwSize+1];
	if(!pszSQL)
	{
		::FreeResource(hRes);
		return bRet; 
	}
	memcpy(pszSQL, lpData, dwSize);
	pszSQL[dwSize] = '\0';
	g_SQLite.ExecDML(pszSQL);
	delete pszSQL;
	::FreeResource(hRes);
	bRet = TRUE;
	return bRet;
}

BOOL CCoder::LoadCharSet()
{
	BOOL bRet = FALSE;
	CCharSetTableDB charset(&g_SQLite);
	if(!charset.Init())
		bRet = InitCharSet();
	if(!bRet)
		return bRet;
	if(!charset.Query(NULL))
		return bRet;

	LPCHARSET_TABLE lpCharSetTable = charset.GetResults();
	LPCHARSET_INFO lpCharSetInfo = NULL;
	for(int i=0; i<charset.GetRows(); i++)
	{
		lpCharSetInfo = new CHARSET_INFO();
		if(!lpCharSetInfo)
			return bRet;
		lpCharSetInfo->nType = lpCharSetTable[i].nType;
		wcscpy(lpCharSetInfo->szName, lpCharSetTable[i].szName);
		wcscpy(lpCharSetInfo->szDesc, lpCharSetTable[i].szDesc);
		lstCharSetsInfo.push_back(lpCharSetInfo);
	}

	bRet = TRUE;
	return bRet;
}

BOOL CCoder::InitCharSetCombo()
{
	BOOL bRet = FALSE;
	if(!pCoderComboFrom || !pCoderComboTo)
		return bRet;
	LPCHARSET_INFO lpCharSetInfo = NULL;
	list<LPCHARSET_INFO>::iterator iter;
	CListLabelElementUI *pItem = NULL;
	pCoderComboFrom->RemoveAll();
	pCoderComboTo->RemoveAll();
	for(iter=lstCharSetsInfo.begin(); iter!=lstCharSetsInfo.end(); iter++)
	{
		lpCharSetInfo = (*iter);
		if(lpCharSetInfo->nType==CHARSET_INPUT)
		{
			pItem = new CListLabelElementUI();
			pItem->SetText(lpCharSetInfo->szName);
			pCoderComboFrom->AddAt(pItem, pCoderComboFrom->GetCount());  
		}
		else if(lpCharSetInfo->nType==CHARSET_OUTPUT)
		{
			pItem = new CListLabelElementUI();
			pItem->SetText(lpCharSetInfo->szName);
			pCoderComboTo->AddAt(pItem, pCoderComboTo->GetCount());
		}
		else if(lpCharSetInfo->nType==CHARSET_BOTH)
		{
			pItem = new CListLabelElementUI();
			pItem->SetText(lpCharSetInfo->szName);
			pCoderComboFrom->AddAt(pItem, pCoderComboFrom->GetCount());
			pItem = new CListLabelElementUI();
			pItem->SetText(lpCharSetInfo->szName);
			pCoderComboTo->AddAt(pItem, pCoderComboTo->GetCount());
		}
	}

	bRet = TRUE;
	return bRet;
}

BOOL CCoder::StartConvert(HWND hWnd)
{
	BOOL bRet = FALSE;
	if(!bQuitThread)
	{
		RDMsgBox(hWnd, MSG_STILL_WORKING, MSG_ERR, MB_OK);
		return bRet;
	}
	if(!pCoderComboTo)
		return bRet;
	CDuiString strCoderCharSet = pCoderComboTo->GetText();
	if(strCoderCharSet.IsEmpty())
	{
		RDMsgBox(hWnd, MSG_CHARSET_ERR, MSG_ERR, MB_OK);
		return bRet;
	}
	DWORD dwThread = 0;
	hConvertThread = CreateThread(NULL, 0, ConvertThread, (LPVOID)this, 0, &dwThread);
	if(!hConvertThread)
		return bRet;
	LPCODER_INFO lpCoderInfo = NULL;
	vector<LPCODER_INFO>::iterator iter;
	for(iter=lstCoderInfo.begin(); iter!=lstCoderInfo.end(); iter++)
	{
		lpCoderInfo = (*iter);
		if(lpCoderInfo)
			StrUtil::w2a(strCoderCharSet.GetData(), lpCoderInfo->szTo);
	}
	bWorking = TRUE;
	return TRUE;
}

BOOL CCoder::StopConvert()
{
	if(bWorking)
		bWorking = FALSE;
	return TRUE;
}

BOOL CCoder::DelSelFile()
{
	BOOL bRet = FALSE;
	if(!pCoderList)
		return bRet;
	int nSel = pCoderList->GetCurSel();
	if(nSel<0)
		return bRet;
	LPCODER_INFO lpCoderInfo = NULL;
	vector<LPCODER_INFO>::iterator iter;
	int nCur = 0;
	for(iter=lstCoderInfo.begin(); iter!=lstCoderInfo.end(); iter++, nCur++)
	{
		if(nCur==nSel)
		{
			lpCoderInfo = (*iter);
			lstCoderInfo.erase(iter);
			delete lpCoderInfo;
			bRet = TRUE;
			break;
		}
	}
	if(bRet && pCoderList->RemoveAt(nSel))
		return TRUE;
	return FALSE;
}

BOOL CCoder::DelAllFiles()
{
	BOOL bRet = FALSE;
	if(!pCoderList)
		return bRet;
	LPCODER_INFO lpCoderInfo = NULL;
	if(lstCoderInfo.size())
		lpCoderInfo = (LPCODER_INFO)lstCoderInfo.back();
	while(lpCoderInfo)
	{
		lstCoderInfo.pop_back();
		delete lpCoderInfo;
		lpCoderInfo = NULL;
		if(lstCoderInfo.size())
			lpCoderInfo = (LPCODER_INFO)lstCoderInfo.back();
	}
	pCoderList->RemoveAll();
	return TRUE;
}

BOOL CCoder::AddNewFile(HWND hWnd)
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024];
	wchar_t szFilter[] = L"All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";
	bRet = SHHelper::SelectFile(hWnd, szPath, szFilter);
	if(bRet && pCoderList)
	{
		CListTextElementUI* pElement = new CListTextElementUI();
		if(!pElement)
			return FALSE;
		LPCODER_INFO pCoderInfo = new CODER_INFO();
		if(!pCoderInfo)
			return bRet;
		wcscpy(pCoderInfo->szPath, szPath);
		pCoderInfo->bHandle = FALSE;
		pCoderInfo->bValid = TRUE;
		char szFrom[64] = {0};
		wchar_t szFrom_w[64] = {0};
		if(GetFileCharSet(szPath, szFrom))
			strcpy(pCoderInfo->szFrom, szFrom);
		else
			strcpy(pCoderInfo->szFrom, "UnKnown");
		//pElement->SetTag(lstCoderInfo.size());
		lstCoderInfo.push_back(pCoderInfo);
		pCoderList->Add(pElement);
		StrUtil::a2w(szFrom, szFrom_w);
		SelFromCharSet(szFrom_w);
	}
	return bRet;
}

BOOL CCoder::AddNewFolder(HWND hWnd)
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024];
	wchar_t szTitle[1024];
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kFolderTitle, szTitle);
	bRet = SHHelper::SelectFolder(hWnd, szPath, szTitle);
	if(bRet && pCoderList)
		bRet = AddCoderFolder(szPath);

	return bRet;
}

BOOL CCoder::AddCoderFolder(LPCWSTR lpszFolder)
{
	BOOL bRet = FALSE;
	wchar_t szFolder[1024] = {0};
	wcscpy_s(szFolder, lpszFolder);
	PathAppendW(szFolder, L"*.*");
	WIN32_FIND_DATAW FindFileData = {0};
	HANDLE hFind = ::FindFirstFileW(szFolder, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return bRet;
	BOOL bFolder = FALSE;
	wchar_t szCurPath[1024];
	BOOL bFirst = TRUE;
	while(TRUE)
	{
		bRet = TRUE;
		memset(szCurPath, 0, sizeof(szCurPath));
		wcscpy_s(szCurPath, lpszFolder);
		PathAppendW(szCurPath, FindFileData.cFileName);
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) // 忽略硬连接文件
		{
			bFolder = TRUE;
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(wcsicmp(FindFileData.cFileName, L".")==0 || wcsicmp(FindFileData.cFileName, L"..")==0)
			{
				if(!FindNextFile(hFind, &FindFileData))
					break;
				continue;
			}
			bRet = AddCoderFolder(szCurPath);
			bFolder = TRUE;
		}
		if(!bFolder)
		{
			if(!PathIsExe(szCurPath))
			{
				CListTextElementUI* pElement = new CListTextElementUI();
				if(!pElement)
					return FALSE;
				LPCODER_INFO pCoderInfo = new CODER_INFO();
				if(!pCoderInfo)
					return bRet;
				wcscpy(pCoderInfo->szPath, szCurPath);
				pCoderInfo->bHandle = FALSE;
				pCoderInfo->bValid = TRUE;
				char szFrom[64] = {0};
				if(GetFileCharSet(szCurPath, szFrom))
					strcpy(pCoderInfo->szFrom, szFrom);
				else
					strcpy(pCoderInfo->szFrom, "UnKnown");
				strcpy(pCoderInfo->szFrom, szFrom);
				//pElement->SetTag(lstCoderInfo.size());
				lstCoderInfo.push_back(pCoderInfo);
				pCoderList->Add(pElement);
				if(bFirst)
				{
					wchar_t szFrom_w[64] = {0};
					StrUtil::a2w(szFrom, szFrom_w);
					SelFromCharSet(szFrom_w);
					bFirst = FALSE;
				}
			}
		}
		if(!FindNextFileW(hFind, &FindFileData))
			break;
	}
	return bRet;
}

BOOL CCoder::SelFromCharSet(LPCWSTR lpszCharSet)
{
	BOOL bRet = FALSE;
	if(!pCoderComboFrom || !lpszCharSet)
		return bRet;
	CListLabelElementUI *pItem = NULL;
	for(int i=0; i<pCoderComboFrom->GetCount(); i++)
	{
		pItem = (CListLabelElementUI *)pCoderComboFrom->GetItemAt(i);
		if(pItem && wcsicmp(pItem->GetText().GetData(), lpszCharSet)==0)
			pCoderComboFrom->SelectItem(i);
	}

	return bRet;
}

BOOL CCoder::GetFileCharSet(LPCWSTR lpszPath, LPSTR lpszSet)
{
	BOOL bRet = FALSE;
#define CSD_DATA_SIZE 4096
	if(!PathFileExists(lpszPath))
		return bRet;
	char szPath[1024], szData[CSD_DATA_SIZE] = {0};
	int nSize = CSD_DATA_SIZE;
	StrUtil::w2a(lpszPath, szPath);
	FILE *pFile = fopen(szPath, "rb");
	if(!pFile)
		return bRet;
	fread(szData, (size_t)nSize, 1, pFile);
	fclose(pFile);
	csd_t pCSD = csd_open();
	if(!pCSD)
		return bRet;
	int nCSD = csd_consider(pCSD, szData, nSize);
	if(nCSD<0)
		bRet = FALSE;
	const char *pCharSet = csd_close(pCSD);
	bRet = FALSE;
	if(nCSD>=0 && pCharSet)
	{
		strcpy(lpszSet, pCharSet);
		bRet = TRUE;
	}
	return bRet;
}

BOOL CCoder::ConvertFile(HWND /*hWnd*/, LPCWSTR lpszPath, LPCSTR lpszFrom, LPCSTR lpszTo)
{
	BOOL bRet = FALSE;
	if(!lpszPath)
		return bRet;
	wchar_t szNewPath_w[1024], szBackUp[1024];
	LPCWSTR lpszFileName = PathFindFileName(lpszPath);
	swprintf(szNewPath_w, L"%s\\%s", g_szCoderPath, lpszFileName);
	swprintf(szBackUp, L"%s\\%s", g_szCoderBackupPath, lpszFileName);
	if(bCoderBackup)
		SHHelper::CopyFile(NULL, lpszPath, szBackUp);

	char szOldPath[1024], szNewPath[1024];
	char *lpOldData = NULL, *lpNewData = NULL;
	DWORD dwOldData = 0, dwNewData = 0;
	size_t nRead = 0, nLeftData = 0;
	StrUtil::w2a(lpszPath, szOldPath);
	StrUtil::w2a(szNewPath_w, szNewPath);
	FILE *pOldFile = fopen(szOldPath, "rb");
	if(!pOldFile)
		return bRet;
	FILE *pNewFile = fopen(szNewPath, "wb");
	if(!pNewFile)
	{
		fclose(pOldFile);
		return bRet;
	}
	fseek(pOldFile, 0L, SEEK_END);
	dwOldData = ftell(pOldFile);
	dwNewData = dwOldData * 8;
	fseek(pOldFile, 0L, SEEK_SET);
	lpOldData = (char *)malloc(dwOldData);
	if(!lpOldData)
	{
		fclose(pOldFile);
		fclose(pNewFile);
		return bRet;
	}
	lpNewData = (char *)malloc(dwNewData);
	if(!lpNewData)
	{
		free(lpOldData);
		fclose(pOldFile);
		fclose(pNewFile);
		return bRet;
	}
	memset(lpNewData, 0, dwNewData);
	char* lpSrc = lpOldData;
	char* lpDst = lpNewData;
	iconv_t cd;
	cd = iconv_open(lpszTo, lpszFrom);
	if ((iconv_t)-1 == cd)
	{
		free(lpOldData);
		free(lpNewData);
		fclose(pOldFile);
		fclose(pNewFile);
		return bRet;
	}
	nRead = fread(lpOldData, 1, (size_t)dwOldData, pOldFile);
	nLeftData = dwNewData;
	fclose(pOldFile);
	pOldFile = NULL;
	if(nRead==dwOldData && bWorking)
	{
		size_t ret = iconv(cd, (const char**)&lpOldData, &nRead, (char**)&lpNewData, &nLeftData);
		if(ret==0)
		{
			ret = fwrite(lpDst, 1, dwNewData-nLeftData, pNewFile);
			if(ret==dwNewData-nLeftData)
				bRet = TRUE;
		}
	}
	//iconv's arg
	iconv_close(cd);
	free(lpSrc);
	free(lpDst);
	fclose(pNewFile);

	return bRet;
}

DWORD WINAPI CCoder::ConvertThread(LPVOID lpVoid)
{
	CCoder *pCoder = (CCoder *)lpVoid;
	if(!pCoder)
		return 0;
	vector<LPCODER_INFO>::iterator iter;
	LPCODER_INFO lpCoderInfo = NULL;
	CListTextElementUI *pElement = NULL;
	int i = 0;
	BOOL bRet = FALSE;
	pCoder->bQuitThread = FALSE;
	while(pCoder && pCoder->bWorking)
	{
		lpCoderInfo=pCoder->lstCoderInfo[i];
		if(lpCoderInfo)
		{
			bRet = pCoder->ConvertFile(NULL, lpCoderInfo->szPath, lpCoderInfo->szFrom, lpCoderInfo->szTo);
			lpCoderInfo->bHandle = TRUE;
			pElement = (CListTextElementUI *)pCoder->pCoderList->GetItemAt(i);
			if(pElement)
			{
				if(bRet)
					pElement->SetText(2, pCoder->szHandleText);
				else
					pElement->SetText(2, pCoder->szHandleErrText);
			}
		}
		i++;
		if(i>=pCoder->lstCoderInfo.size())
			break;
	}
	if(pCoder)
	{
		pCoder->bQuitThread = TRUE;
		CloseHandle(pCoder->hConvertThread);
		pCoder->hConvertThread = NULL;
	}
	return 0;
}
