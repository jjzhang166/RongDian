#include "StdAfx.h"
#include "Setting.h"

const wchar_t* const kSettingsLangs = L"settings_langs";
const wchar_t* const kSettingsApply = L"settings_apply";

CSetting::CSetting()
{
	m_pLangsCombo = NULL;
}

BOOL CSetting::IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/)
{
	return TRUE;
}

void CSetting::OnQuit()
{

}

BOOL CSetting::OnInit(WPARAM wParam, LPARAM /*lParam*/)
{

	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	if(!pManager)
		return FALSE;

	FIND_CONTROL_BY_ID(m_pLangsCombo, CComboUI, pManager, kSettingsLangs)
	
	initLangs();
	return TRUE;
}

BOOL CSetting::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang, LPCWSTR lpszLangSection)
{
	return TRUE;
}

BOOL CSetting::initLangs()
{
	wchar_t szFolder[1024] = L"..\\RongDian\\lang\\lang_*.ini";
	WIN32_FIND_DATAW findFileData = {0};
	HANDLE hFind = ::FindFirstFileW(szFolder, &findFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return FALSE;
	BOOL bFolder = FALSE;
	wchar_t szCurPath[1024];
	while(TRUE)
	{
		if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) // 忽略硬连接文件
		{
			bFolder = TRUE;
		}
		if(!bFolder&&!PathIsExe(szCurPath))
		{
			CListLabelElementUI* pItem = new CListLabelElementUI();
			if(!pItem)
				return FALSE;
			wchar_t itemText[256];
			wchar_t* aIndex = wcsstr(findFileData.cFileName,L"_");
			if (aIndex!=NULL)
			{
				wcsncpy_s(itemText,256,aIndex,wcslen(aIndex)-4);
				itemText[0] = ' ';
			}
			pItem->SetText(itemText);
			pItem->SetUserData(findFileData.cFileName);
			m_pLangsCombo->Add(pItem);
		}
		if(!FindNextFileW(hFind, &findFileData))
			break;
	}
	if (m_pLangsCombo->GetCount()>0)
	{
		wchar_t lang[256];
		CConfigTableDB table(&g_SQLite);
		CONFIG_TABLE config;
		wcscpy(config.szName, L"Lang");
		if(table.Query(&config) && table.GetRows())
		{
			LPCONFIG_TABLE lpConfigTable = table.GetResults();
			wcscpy_s(lang,256,lpConfigTable->szValue);
		}

		int count = m_pLangsCombo->GetCount();
		for (int i=0;i<count;i++)
		{
			CListLabelElementUI* pItem = (CListLabelElementUI*)m_pLangsCombo->GetItemAt(i);
			if (wcsicmp(pItem->GetUserData().GetData(),lang)==0)
			{
				m_pLangsCombo->SelectItem(i);
				break;
			}
		}
	}
	return TRUE;
}

void CSetting::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kSettingsApply)
	{
		Apply(hWnd);
	}
}

BOOL CSetting::Apply(HWND hWnd)
{
	BOOL ret = TRUE;
	//set language
	if (m_pLangsCombo->GetCount()>0)
	{
		int curSel = m_pLangsCombo->GetCurSel();
		CListLabelElementUI* item = (CListLabelElementUI*)m_pLangsCombo->GetItemAt(curSel);
		LPCTSTR lang = item->GetUserData().GetData();
		CConfigTableDB table(&g_SQLite);
		CONFIG_TABLE config;
		wcscpy(config.szName, L"Lang");
		wcscpy(config.szValue, lang);
		if(!table.Update(&config))
		{
			ret = FALSE;
		}
	}
	if (ret)
	{
		RDMsgBox(hWnd, MSG_DONE, MSG_SUCCESS, MB_OK);
	}
	return ret;
}
