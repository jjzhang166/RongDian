#include "stdafx.h"
#include "ICoder.h"

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

ICoder::ICoder()
{
	hCoderOwner = NULL;
	pCoderManager = NULL;
	m_CoderList = NULL;
	pCoderFrom = NULL;
	pCoderTo = NULL;
	pCoderBackupCheck = NULL;
	pCoderNoBOM = NULL;
	pCoderBackupEdit = NULL;
	bCoderBackup = TRUE;
	bWithBom = FALSE;
	bWorking = FALSE;
	bQuitThread = TRUE;
	memset(szHandleText, 0, sizeof(szHandleText));
	memset(szNoHandleText, 0, sizeof(szNoHandleText));
	dwHightLightColor = 0;
	dwGrayColor = 0;
}

ICoder::~ICoder()
{

}

LPCWSTR ICoder::GetCoderListName()
{
	return kCoderList;
}

BOOL ICoder::InitCoder(IListCallbackUI* pIListCallback)
{
	if(!pCoderManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_startBtn, CButtonUI, pCoderManager, kCoderStart)
	FIND_CONTROL_BY_ID(m_stopBtn, CButtonUI, pCoderManager, kCoderStop)
	FIND_CONTROL_BY_ID(m_delSelBtn, CButtonUI, pCoderManager, kCoderDel)
	FIND_CONTROL_BY_ID(m_delAllBtn, CButtonUI, pCoderManager, kCoderDelAll)
	FIND_CONTROL_BY_ID(m_CoderList, CListUI, pCoderManager, kCoderList)
	FIND_CONTROL_BY_ID(pCoderFrom, CComboBoxUI, pCoderManager, kCoderFrom)
	FIND_CONTROL_BY_ID(pCoderTo, CComboBoxUI, pCoderManager, kCoderTo)
	FIND_CONTROL_BY_ID(pCoderBackupCheck, CButtonUI, pCoderManager, kCoderBackup)
	if(pCoderBackupCheck)
	{
		pCoderBackupCheck->SetUserData(L"TRUE");
		dwHightLightColor = pCoderBackupCheck->GetBkColor();
	}
	FIND_CONTROL_BY_ID(pCoderNoBOM, CButtonUI, pCoderManager, kCoderBOM)
	if(pCoderNoBOM)
	{
		pCoderNoBOM->SetUserData(L"FALSE");
		dwGrayColor = pCoderNoBOM->GetBkColor();
	}
	FIND_CONTROL_BY_ID(pCoderBackupEdit, CEditUI, pCoderManager, kCoderPathEdit)
	if(pCoderBackupEdit)
		pCoderBackupEdit->SetText(g_szCoderBackupPath);
	if(m_CoderList)
		m_CoderList->SetTextCallback(pIListCallback);
	if(!PathFileExists(g_szCoderBackupPath))
		SHCreateDirectoryEx(NULL, g_szCoderBackupPath, NULL);
	return TRUE;
}

BOOL ICoder::SetCoderLang(LPCWSTR lpszLang)
{
	if(!pCoderManager)
		return FALSE;
	memset(szHandleText, 0, sizeof(szHandleText));
	memset(szNoHandleText, 0, sizeof(szNoHandleText));
	Utility::GetINIStr(lpszLang, LS_MSG, kHandleText, szHandleText);
	Utility::GetINIStr(lpszLang, LS_MSG, kNoHandleText, szNoHandleText);

SET_CONTROL_BEGIN(pCoderManager, lpszLang, LS_CODERPANEL)
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

LPCWSTR ICoder::GetCoderItemText(CControlUI* pControl, int iIndex, int iSubItem)
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

void ICoder::OnCoderClick(TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kCoderStart)
	{
		StartConvert();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderStop)
	{
		StopConvert();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderNewFile)
	{
		AddNewFile();
		ResetBtnStatus();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderNewFolder)
	{
		AddNewFolder();
		ResetBtnStatus();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderDel)
	{
		DelSelFile();
		ResetBtnStatus();
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderDelAll)
	{
		DelAllFiles();
		ResetBtnStatus();
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
					dwGrayColor = pCoderBackupCheck->GetBkColor();
				pCoderBackupCheck->SetBkColor(dwHightLightColor);
				pCoderBackupCheck->SetUserData(L"TRUE");
				bCoderBackup = TRUE;
			}
			else
			{
				if(dwHightLightColor==0)
					dwHightLightColor = pCoderBackupCheck->GetBkColor();
				pCoderBackupCheck->SetBkColor(dwGrayColor);
				pCoderBackupCheck->SetUserData(L"FALSE");
				bCoderBackup = FALSE;
			}
		}
		bHandled = TRUE;
	}
	else if(sCtrlName==kCoderPathOpen)
	{
		SHHelper::OpenFolder(hCoderOwner, g_szCoderBackupPath);
		bHandled = TRUE;
	}
}

void ICoder::OnCoderItemActive(TNotifyUI& /*msg*/)
{

}

BOOL ICoder::StartConvert()
{
	BOOL bRet = FALSE;
	if(!bQuitThread)
	{
		wchar_t szErr[1024], szTitle[1024];
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kMsgErr, szTitle);
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kStillWorkingErr, szErr);
		DuiMsgBox(hCoderOwner, szErr, szTitle, MB_OK);
		return bRet;
	}
	DWORD dwThread = 0;
	HANDLE hThread = CreateThread(NULL, 0, ConvertThread, (LPVOID)this, 0, &dwThread);
	if(!hThread)
		return bRet;
	CloseHandle(hThread);
	return TRUE;
}

BOOL ICoder::StopConvert()
{
	if(bWorking)
		bWorking = FALSE;
	return TRUE;
}

BOOL ICoder::DelSelFile()
{
	BOOL bRet = FALSE;
	if(!m_CoderList)
		return bRet;
	int nSel = m_CoderList->GetCurSel();
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
	if(bRet && m_CoderList->RemoveAt(nSel))
		return TRUE;
	return FALSE;
}

BOOL ICoder::DelAllFiles()
{
	BOOL bRet = FALSE;
	if(!m_CoderList)
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
	m_CoderList->RemoveAll();
	return TRUE;
}

BOOL ICoder::AddNewFile()
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024];
	wchar_t szFilter[] = L"All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";
	bRet = SHHelper::SelectFile(hCoderOwner, szPath, szFilter);
	if(bRet && m_CoderList)
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
		if(GetFileCharSet(szPath, szFrom))
			strcpy(pCoderInfo->szFrom, szFrom);
		else
			strcpy(pCoderInfo->szFrom, "UnKnown");
		//pElement->SetTag(lstCoderInfo.size());
		lstCoderInfo.push_back(pCoderInfo);
		m_CoderList->Add(pElement);
	}
	return bRet;
}

BOOL ICoder::AddNewFolder()
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024];
	wchar_t szTitle[1024];
	Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kFolderTitle, szTitle);
	bRet = SHHelper::SelectFolder(hCoderOwner, szPath, szTitle);
	if(bRet && m_CoderList)
		bRet = AddCoderFolder(szPath);

	return bRet;
}

BOOL ICoder::AddCoderFolder(LPCWSTR lpszFolder)
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
				m_CoderList->Add(pElement);
			}
		}
		if(!FindNextFileW(hFind, &FindFileData))
			break;
	}
	return bRet;
}

BOOL ICoder::GetFileCharSet(LPCWSTR lpszPath, LPSTR lpszSet)
{
	BOOL bRet = FALSE;
	if(!PathFileExists(lpszPath))
		return bRet;
	char szPath[1024], szData[1024];
	int nSize = 1024;
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

BOOL ICoder::ConvertFile(LPCWSTR lpszPath, LPCSTR lpszFrom, LPCSTR lpszTo)
{
	BOOL bRet = FALSE;
	if(!lpszPath)
		return bRet;
	wchar_t szNewPath_w[1024], szBackUp[1024];
	LPCWSTR lpszFileName = PathFindFileName(lpszPath);
	swprintf(szNewPath_w, L"%s\\%s", g_szCoderPath, lpszFileName);
	swprintf(szBackUp, L"%s\\%s", g_szCoderBackupPath, lpszFileName);
	if(bCoderBackup)
		SHHelper::CopyFile(hCoderOwner, lpszPath, szBackUp);

	char szOldPath[1024], szNewPath[1024], szOldData[1024], szNewData[4096];
	int nSize = 1024;
	size_t nRead = 0, nLeftData = 0;
	StrUtil::w2a(lpszPath, szOldPath);
	StrUtil::w2a(szNewPath_w, szNewPath);
	FILE *pOldFile = fopen(szOldPath, "rb");
	if(!pOldFile)
		return bRet;
	FILE *pNewFile = fopen(szNewPath, "ab+");
	if(!pNewFile)
	{
		fclose(pOldFile);
		return bRet;
	}
	const unsigned char* lpSrc = NULL;
	unsigned char* lpDst = NULL;
	iconv_t cd;
	cd = iconv_open(lpszTo, lpszFrom);
	if ((iconv_t)-1 == cd)
	{
		fclose(pOldFile);
		fclose(pNewFile);
		return bRet;
	}
	nLeftData = _countof(szNewData);
	nRead = fread(szOldData, (size_t)nSize, 1, pOldFile);
	while(nRead && bWorking)
	{
		lpSrc = (const unsigned char *)szOldData;
		lpDst = (unsigned char *)szNewData;
		iconv(cd, (const char**)&lpSrc, &nRead, (char**)&lpDst, &nLeftData);
		fwrite(szNewData, _countof(szNewData)-nLeftData, 1, pNewFile);
		memset(szOldData, 0, sizeof(szOldData));
		memset(szNewData, 0, sizeof(szNewData));
		nRead = fread(szOldData, (size_t)nSize, 1, pOldFile);
	}
	//iconv's arg
	iconv_close(cd);
	fclose(pOldFile);
	fclose(pNewFile);

	return TRUE;
}

BOOL ICoder::ResetBtnStatus()
{
	if (m_CoderList->GetCount()>0)
	{
		m_startBtn->SetEnabled(true);
		m_stopBtn->SetEnabled(true);
		m_delSelBtn->SetEnabled(true);
		m_delAllBtn->SetEnabled(true);
	}
	else
	{
		m_startBtn->SetEnabled(false);
		m_stopBtn->SetEnabled(false);
		m_delSelBtn->SetEnabled(false);
		m_delAllBtn->SetEnabled(false);
	}
	return TRUE;
}
DWORD WINAPI ICoder::ConvertThread(LPVOID lpVoid)
{
	ICoder *pICoder = (ICoder *)lpVoid;
	if(!pICoder)
		return 0;
	vector<LPCODER_INFO>::iterator iter;
	LPCODER_INFO lpCoderInfo = NULL;
	CListTextElementUI *pElement = NULL;
	int i = 0;
	BOOL bRet = FALSE;
	pICoder->bQuitThread = FALSE;
	while(pICoder && pICoder->bWorking)
	{
		lpCoderInfo=pICoder->lstCoderInfo[i];
		if(lpCoderInfo)
		{
			bRet = pICoder->ConvertFile(lpCoderInfo->szPath, lpCoderInfo->szFrom, lpCoderInfo->szTo);
			if(bRet)
			{
				lpCoderInfo->bHandle = TRUE;
				pElement = (CListTextElementUI *)pICoder->m_CoderList->GetItemAt(i);
				if(pElement)
					pElement->SetText(2, pICoder->szHandleText);
			}
		}
		i++;
	}
	if(pICoder)
		pICoder->bQuitThread = TRUE;
	return 0;
}
