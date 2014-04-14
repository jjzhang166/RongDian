#include "StdAfx.h"
#include "Setting.h"

const wchar_t* const kSettingsLangText = L"settings_lang_text";
const wchar_t* const kSettingsVersionText = L"settings_version_text";
const wchar_t* const kSettingsLangs = L"settings_langs";
const wchar_t* const kSettingsApply = L"settings_apply";
const wchar_t* const kSettingsCheckVersion = L"settings_check_version";
const wchar_t* const kSettingsCheckVersionCB = L"settings_check_version_cb";
const wchar_t* const kSettingsBgText = L"settings_bg_text";
const wchar_t* const kSettingsBgEdit = L"settings_bg";
const wchar_t* const kSettingsBgApplyBtn = L"settings_bg_apply";
const wchar_t* const kSettingsBgApplyBtnTip = L"settings_bg_apply_tip";
const wchar_t* const kSettingsBgDefaultBtn = L"settings_bg_default";
const wchar_t* const kSettingsBgDefaultBtnTip = L"settings_bg_default_tip";

CSetting::CSetting()
{
	m_pLangsCombo = NULL;
	m_pCheckVersionCheckbox = NULL;
	m_pCheckVersionLabel = NULL;
	m_pBackgroundEdit = NULL;
	m_pBackgroundApplyBtn = NULL;
	m_pBackgroundDefaultBtn = NULL;
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
	FIND_CONTROL_BY_ID(m_pCheckVersionCheckbox, CCheckBoxUI, pManager, kSettingsCheckVersionCB)
	FIND_CONTROL_BY_ID(m_pCheckVersionLabel, CLabelUI, pManager, kSettingsCheckVersion)
	FIND_CONTROL_BY_ID(m_pBackgroundEdit, CEditUI, pManager, kSettingsBgEdit)
	FIND_CONTROL_BY_ID(m_pBackgroundApplyBtn, CButtonUI, pManager, kSettingsBgApplyBtn)
	FIND_CONTROL_BY_ID(m_pBackgroundDefaultBtn, CButtonUI, pManager, kSettingsBgDefaultBtn)

	InitLangs();
	InitCheckVersion();
	InitBackground();
	return TRUE;
}

LRESULT CSetting::HandleCustomMessage(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	BOOL bRet = FALSE;
	switch(uMsg)
	{
	case WM_UPDATE_BACKGROUND:
		{
			wchar_t path[512]={0};
			wsprintf(path,L"%s\\%s",g_szResPath,g_szBackground);
			m_pBackgroundEdit->SetText(path);
			bRet = TRUE;
			bHandled = TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CSetting::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang, LPCWSTR lpszLangSection)
{
	if(!pManager)
		return FALSE;
	wcscpy(m_pLangSection, lpszLangSection);
SET_CONTROL_BEGIN(pManager, lpszLang, m_pLangSection)
	SET_CONTROL_TEXT2(kSettingsLangText)
	SET_CONTROL_TEXT2(kSettingsVersionText)
	SET_CONTROL_TEXT2(kSettingsCheckVersion)
	SET_CONTROL_TEXT2(kSettingsBgText)
	SET_CONTROL_TEXT_AND_TIP(kSettingsBgApplyBtn,kSettingsBgApplyBtnTip)
	SET_CONTROL_TEXT_AND_TIP(kSettingsBgDefaultBtn,kSettingsBgDefaultBtnTip)
SET_CONTROL_END()
	return TRUE;
}

BOOL CSetting::InitLangs()
{
	WIN32_FIND_DATAW findFileData = {0};
	HANDLE hFind = ::FindFirstFileW(kLangFile, &findFileData);
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
		wcscpy(config.szName, tConfigLang);
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

BOOL CSetting::InitCheckVersion()
{
	BOOL bExist = TRUE;
	CONFIG_TABLE stTable;
	wcscpy(stTable.szName, tConfigCheckVersion);
	CConfigTableDB table(&g_SQLite);
	table.Query(&stTable);
	if(wcslen(table.GetResults()->szValue)==0)
	{
		bExist = FALSE;
		CConfigTableDB table(&g_SQLite);
		CONFIG_TABLE config;
		wcscpy(config.szName, tConfigCheckVersion);
		wcscpy(config.szValue, L"1");
		table.Insert(&config);
	}
	
	if(!bExist)
	{
		table.Query(&stTable);
	}
	if(wcsicmp(table.GetResults()->szValue,L"0")==0)
	{
		m_pCheckVersionCheckbox->Selected(false);
	}
	else
	{
		m_pCheckVersionCheckbox->Selected(true);
	}
	return TRUE;
}

BOOL CSetting::InitBackground()
{
	wchar_t szBkImage[1024];
	swprintf(szBkImage, L"%s\\%s", g_szResPath, g_szBackground);
	if(PathFileExists(szBkImage))
	{
		m_pBackgroundEdit->SetText(szBkImage);
		return TRUE;
	}
	else
		m_pBackgroundEdit->SetText(g_szBackground);
	return TRUE;
}

void CSetting::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName==kSettingsApply)
	{
		bHandled = Apply(hWnd);
	}
	else if (sCtrlName==kSettingsBgApplyBtn)
	{
		wchar_t szDBVal[512]={0};
		LPCWSTR szFileNameOrColor = NULL;
		LPCTSTR szBg = m_pBackgroundEdit->GetText().GetData();
		if(PathFileExists(szBg) && IsImageFile(szBg))
		{
			g_pSkinManager->UpdateBkImage(szBg);
			szFileNameOrColor = PathFindFileNameW(szBg);
			swprintf(szDBVal, L"custom\\%s", szFileNameOrColor);
		}else if (PCREUtil::IsHTMLColor(szBg))
		{
			wchar_t *pszStopStr = NULL;
			DWORD dwColor = wcstoul(szBg, &pszStopStr, 16);
			g_pSkinManager->UpdateBkColor(dwColor);
			swprintf(szDBVal, L"%s", szBg);
		}
		else
		{
			RDMsgBox(hWnd,MSG_BACKGROUND_ERR,MSG_ERR,MB_OK);
			return;
		}
		
		//update database
		CConfigTableDB table(&g_SQLite);
		CONFIG_TABLE config;
		wcscpy(config.szName, kBackGround);
		wcscpy(config.szValue, szDBVal);
		table.Update(&config);
	}
	else if (sCtrlName==kSettingsBgDefaultBtn)
	{
		wchar_t *pszStopStr = NULL;
		DWORD dwColor = wcstoul(g_szDefaultBackground, &pszStopStr, 16);
		g_pSkinManager->UpdateBkColor(dwColor);
		wcscpy(g_szBackground,g_szDefaultBackground);
		m_pBackgroundEdit->SetText(g_szBackground);
		//update database
		CConfigTableDB table(&g_SQLite);
		CONFIG_TABLE config;
		wcscpy(config.szName, kBackGround);
		wcscpy(config.szValue, g_szDefaultBackground);
		table.Update(&config);
	}
}

BOOL CSetting::Apply(HWND hWnd)
{
	BOOL ret = TRUE;
	CConfigTableDB table(&g_SQLite);
	CONFIG_TABLE config;
		
	//set language
	if (m_pLangsCombo->GetCount()>0)
	{
		int curSel = m_pLangsCombo->GetCurSel();
		CListLabelElementUI* item = (CListLabelElementUI*)m_pLangsCombo->GetItemAt(curSel);
		LPCTSTR lang = item->GetUserData().GetData();
		wcscpy(config.szName, tConfigLang);
		wcscpy(config.szValue, lang);
		if(!table.Update(&config))
		{
			ret = FALSE;
		}
	}
	if(m_pCheckVersionCheckbox->IsSelected())
	{
		wcscpy(config.szName, tConfigCheckVersion);
		wcscpy(config.szValue, L"1");
		if(!table.Update(&config))
		{
			ret = FALSE;
		}
	}
	else
	{
		wcscpy(config.szName, tConfigCheckVersion);
		wcscpy(config.szValue, L"0");
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
