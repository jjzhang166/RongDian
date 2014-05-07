#include "StdAfx.h"
#include "ColorPicker.h"

#define PICKERCOLOR_MOUSEMOVE 0

// Picker's Frame first column
const wchar_t* const kPickerDesc = L"picker_desc";
const wchar_t* const kPickerTopic1 = L"picker_topic1";
const wchar_t* const kPickerPreview = L"picker_preview";
const wchar_t* const kPickerColorText = L"picker_color_text";
const wchar_t* const kPickerColor = L"picker_color";
const wchar_t* const kPickerPosText = L"picker_pos_text";
const wchar_t* const kPickerPos = L"picker_pos";
const wchar_t* const kPickerClassText = L"picker_class_text";
const wchar_t* const kPickerClass = L"picker_class";
const wchar_t* const kPickerTextText = L"picker_text_text";
const wchar_t* const kPickerText = L"picker_text";
const wchar_t* const kPickerArrow = L"picker_arrow";
const wchar_t* const kPickerSnapShot = L"picker_snapshot";
const wchar_t* const kPickerPaletteTip = L"picker_palette_tip";
const wchar_t* const kPickerPalette = L"picker_palette";
// Picker's Frame mid column
const wchar_t* const kPickerTopic2 = L"picker_topic2";
const wchar_t* const kPickerHtmlColorText = L"color_html_text";
const wchar_t* const kPickerHtmlColor = L"color_html";
const wchar_t* const kPickerRGBColorText = L"color_rgb_text";;
const wchar_t* const kPickerRGBColor = L"color_rgb";
const wchar_t* const kPickerHEXColorText = L"color_hex_text";
const wchar_t* const kPickerHEXColor = L"color_hex";
const wchar_t* const kPickerHSLColorText = L"color_hsl_text";
const wchar_t* const kPickerHSLColor = L"color_hsl";
const wchar_t* const kPickerDelphiColorText = L"color_delphi_text";
const wchar_t* const kPickerDelphiColor = L"color_delphi";
const wchar_t* const kPickerHSVColorText = L"color_hsv_text";
const wchar_t* const kPickerHSVColor = L"color_hsv";
// Picker's Frame last column
const wchar_t* const kPickerColorHistory = L"color_history";
const wchar_t* const kPickerColorList = L"color_list";

CColorPicker::CColorPicker()
{
	pPickerPreview = NULL;
	pPickerColor = NULL;
	pPickerArrow = NULL;
	pPickerSnapShot = NULL;
	pPickerPos = NULL;
	pPickerClass = NULL;
	pPickerText = NULL;
	pPickerHtmlColor = NULL;
	pPickerRGBColor = NULL;
	pPickerHexColor = NULL;
	pPickerHSLColor = NULL;
	pPickerDelphiColor = NULL;
	pPickerHSVColor = NULL;
	pColorList = NULL;
	bIsCapturing = FALSE;
}

BOOL CColorPicker::IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/)
{
	return TRUE;
}

void CColorPicker::OnQuit()
{

}

BOOL CColorPicker::OnInit(WPARAM wParam, LPARAM /*lParam*/)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	if(!pManager)
		return FALSE;
	FIND_CONTROL_BY_ID(pPickerPreview, CControlUI, pManager, kPickerPreview)
	FIND_CONTROL_BY_ID(pPickerColor, CControlUI, pManager, kPickerColor)
	FIND_CONTROL_BY_ID(pPickerPos, CControlUI, pManager, kPickerPos)
	FIND_CONTROL_BY_ID(pPickerClass, CControlUI, pManager, kPickerClass)
	FIND_CONTROL_BY_ID(pPickerText, CControlUI, pManager, kPickerText)
	FIND_CONTROL_BY_ID(pPickerArrow, CControlUI, pManager, kPickerArrow)
	FIND_CONTROL_BY_ID(pPickerSnapShot, CControlUI, pManager, kPickerSnapShot)
	FIND_CONTROL_BY_ID(pPickerHtmlColor, CControlUI, pManager, kPickerHtmlColor)
	FIND_CONTROL_BY_ID(pPickerRGBColor, CControlUI, pManager, kPickerRGBColor)
	FIND_CONTROL_BY_ID(pPickerHexColor, CControlUI, pManager, kPickerHEXColor)
	FIND_CONTROL_BY_ID(pPickerHSLColor, CControlUI, pManager, kPickerHSLColor)
	FIND_CONTROL_BY_ID(pPickerDelphiColor, CControlUI, pManager, kPickerDelphiColor)
	FIND_CONTROL_BY_ID(pPickerHSVColor, CControlUI, pManager, kPickerHSVColor)
	FIND_CONTROL_BY_ID(pColorList, CListUI, pManager, kPickerColorList)

	return TRUE;
}

BOOL CColorPicker::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang, LPCWSTR lpszLangSection)
{
	if(!pManager || !lpszLang)
		return FALSE;
SET_CONTROL_BEGIN(pManager, lpszLang, lpszLangSection)
	// Topic
	SET_CONTROL_TEXT2(kPickerTopic1)
	// Picker Color
	SET_CONTROL_TEXT2(kPickerColorText)
	SET_CONTROL_TIP2(kPickerColor)
	// Picker Position
	SET_CONTROL_TEXT2(kPickerPosText)
	SET_CONTROL_TIP2(kPickerPos)
	// Picker Class
	SET_CONTROL_TEXT2(kPickerClassText)
	SET_CONTROL_TIP2(kPickerClass)
	// Picker Title
	SET_CONTROL_TEXT2(kPickerTextText)
	SET_CONTROL_TIP2(kPickerText)
	// Arrow
	SET_CONTROL_TIP2(kPickerArrow)
	// SnapShot
	SET_CONTROL_TIP2(kPickerSnapShot)
	// Palette
	SET_CONTROL_TEXT_AND_TIP(kPickerPalette, kPickerPaletteTip)

	// Topic2
	SET_CONTROL_TEXT2(kPickerTopic2)
	// Picker HTML
	SET_CONTROL_TEXT2(kPickerHtmlColorText)
	SET_CONTROL_TIP2(kPickerHtmlColor)
	// Picker RGB
	SET_CONTROL_TEXT2(kPickerRGBColorText)
	SET_CONTROL_TIP2(kPickerRGBColor)
	// Picker HEX
	SET_CONTROL_TEXT2(kPickerHEXColorText)
	SET_CONTROL_TIP2(kPickerHEXColor)
	// Picker HSL
	SET_CONTROL_TEXT2(kPickerHSLColorText)
	SET_CONTROL_TIP2(kPickerHSLColor)
	// Picker Delphi
	SET_CONTROL_TEXT2(kPickerDelphiColorText)
	SET_CONTROL_TIP2(kPickerDelphiColor)
	// Picker HSV
	SET_CONTROL_TEXT2(kPickerHSVColorText)
	SET_CONTROL_TIP2(kPickerHSVColor)

	// Topic3
	SET_CONTROL_TEXT2(kPickerColorHistory)
SET_CONTROL_END()

	return TRUE;
}

void CColorPicker::OnClick(HWND hWnd, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kPickerPalette)
	{
		bHandled = TRUE;
		OpenColorDlg(hWnd);
	}
	else if(sCtrlName == kPickerSnapShot)
	{
		bHandled = TRUE;
		OpenSnapShot(hWnd);
	}
	else if(sCtrlName == kPickerArrow)
	{
		bHandled = TRUE;
		OpenZoomIn(hWnd);
	}
}

void CColorPicker::OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(!pParent)
		return;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return;
	if(pList->GetName()!=kPickerColorList)
		return;
	CDuiString sCtrlName = msg.pSender->GetClass();
	if(sCtrlName==L"ListLabelElementUI")
	{
		/*CDuiString strData = msg.pSender->GetUserData();
		wchar_t *pszStopStr = NULL;
		int nBase = 16;
		DWORD dwColor = wcstoul(strData, &pszStopStr, nBase);
		ColorUtil::AdjustColor(dwColor, m_colCurColor);
		SetColorPicker(m_colCurColor, FALSE, FALSE);*/
	}
}

void CColorPicker::OnItemClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(!pParent)
		return;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return;
	if(pList->GetName()!=kPickerColorList)
		return;
	CDuiString sCtrlName = msg.pSender->GetClass();
	if(sCtrlName==L"ListLabelElementUI")
	{
		CDuiString strData = msg.pSender->GetUserData();
		wchar_t *pszStopStr = NULL;
		int nBase = 16;
		DWORD dwColor = wcstoul(strData, &pszStopStr, nBase);
		ColorUtil::AdjustColor(dwColor, colCurColor);
		if(pPickerPreview)
		{
			DWORD dwBackColor = 0;
			ColorUtil::AdjustColor(colCurColor, dwBackColor);
			pPickerPreview->SetBkColor(dwBackColor);
		}
		SetColorPicker(colCurColor, FALSE);
		bHandled = TRUE;
	}
}

void CColorPicker::OnTextChanged(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	if(pPickerColor && msg.pSender==pPickerColor)
	{
		bHandled = TRUE;
		DWORD dwColor = 0;
		CDuiString strColor = msg.pSender->GetText();
		if(ColorUtil::StrToRGB(strColor, dwColor))
		{
			ColorUtil::AdjustColor(dwColor, colCurColor);
			SetColorPicker(colCurColor, TRUE, FALSE);
			if(pPickerPreview)
				pPickerPreview->SetBkColor(dwColor);
		}
	}
}

void CColorPicker::OnItemSelected(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	if(pPickerColor && msg.pSender==pPickerColor)
	{
		bHandled = TRUE;
		DWORD dwColor = 0;
		CDuiString strColor = msg.pSender->GetText();
		if(ColorUtil::StrToRGB(strColor, dwColor))
		{
			ColorUtil::AdjustColor(dwColor, colCurColor);
			SetColorPicker(colCurColor, TRUE, FALSE);
			if(pPickerPreview)
				pPickerPreview->SetBkColor(dwColor);
		}
		bHandled = TRUE;
	}
}

LPCWSTR CColorPicker::GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* pControl, int /*iIndex*/, int /*iSubItem*/)
{
	CListBodyUI *pParent = (CListBodyUI *)pControl->GetParent();
	if(!pParent)
		return L"";
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return L"";
	return L"";
}

LRESULT CColorPicker::OnCopyData(WPARAM /*wParam*/, LPARAM lParam)
{
	PCOPYDATASTRUCT lpData = (PCOPYDATASTRUCT)lParam;
	if(lpData->dwData==WM_SNAPSHOT_MSG)
	{
		PSNAPSHOT_INFO lpInfo = (PSNAPSHOT_INFO)lpData->lpData;
		if(lpInfo)
			OnSnapShotUpdate(lpInfo->dwColor, lpInfo->szPath);
		return TRUE;
	}
	else if(lpData->dwData==WM_ZOOMIN_UPDATE_MSG || lpData->dwData==WM_ZOOMIN_COLOR_MSG)
	{
		PZOOMIN_INFO lpInfo = (PZOOMIN_INFO)lpData->lpData;
		if(lpInfo)
		{
			BOOL bRecord = (lpData->dwData==WM_ZOOMIN_COLOR_MSG);
			OnZoomInUpdate(lpInfo->dwColor, lpInfo->ptCursor, bRecord);
		}
		return TRUE;
	}
	return 0;
}

BOOL CColorPicker::SetColorInfo(POINT ptCursor)
{
	wchar_t szPos[1024];
	swprintf(szPos, L"%d, %d", ptCursor.x, ptCursor.y);
	if(pPickerPos)
		pPickerPos->SetText(szPos);
	HWND hWnd = ::WindowFromPoint(ptCursor);
	if(hWnd)
	{
		wchar_t szClassName[1024], szWindowText[1024];
		::GetClassNameW(hWnd, szClassName, _countof(szClassName));
		::GetWindowTextW(hWnd, szWindowText, _countof(szWindowText));	
		if(pPickerClass)
			pPickerClass->SetText(szClassName);
		if(pPickerText)
			pPickerText->SetText(szWindowText);
	}
	return TRUE;
}

BOOL CColorPicker::SetColorPicker(DWORD dwColor, BOOL bRecord/* = TRUE*/, BOOL bAuto /*= TRUE*/)
{
	BOOL bRet = FALSE;
	wchar_t szRGB[MAX_PATH];
	BYTE bRed = GetRValue(dwColor);
	BYTE bGreen = GetGValue(dwColor);
	BYTE bBlue = GetBValue(dwColor);
	swprintf(szRGB, L"%d,%d,%d", bRed, bGreen, bBlue);
	if(!bAuto && pPickerPreview)
		pPickerPreview->SetBkColor(dwColor);
	if(pPickerColor && bAuto)
		pPickerColor->SetText(szRGB);
	if(pPickerRGBColor)
		pPickerRGBColor->SetText(szRGB);
	if(pPickerHtmlColor)
	{
		wchar_t szColor[MAX_PATH];
		ColorUtil::RGBToHTML(dwColor, szColor);
		pPickerHtmlColor->SetText(szColor);
	}
	if(pPickerHexColor)
	{
		wchar_t szColor[MAX_PATH];
		ColorUtil::RGBToHex(dwColor, szColor);
		pPickerHexColor->SetText(szColor);
	}
	if(pPickerHSLColor)
	{
		double h, s, l;
		wchar_t szColor[MAX_PATH];
		ColorUtil::RGBToHSL(dwColor, h, s, l);
		swprintf(szColor, L"%.2f,%.2f,%.2f", h, s, l);
		pPickerHSLColor->SetText(szColor);
	}
	if(pPickerDelphiColor)
	{
		wchar_t szColor[MAX_PATH];
		ColorUtil::RGBToDelphi(dwColor, szColor);
		pPickerDelphiColor->SetText(szColor);
	}
	if(pPickerHSVColor)
	{
		double h, s, v;
		wchar_t szColor[MAX_PATH];
		ColorUtil::RGBToHSV(dwColor, h, s, v);
		swprintf(szColor, L"%.2f,%.2f,%.2f", h, s, v);
		pPickerHSVColor->SetText(szColor);
	}
	wchar_t szHexColor[256] = {0};
	char szHexColor_a[256] = {0};
	ColorUtil::RGBToHex(dwColor, szHexColor);
	StrUtil::w2a(szHexColor, szHexColor_a);
	Utility::CopyTextToClipBoard(szHexColor_a);
	if(bRecord && pColorList)
	{
		CListLabelElementUI* pListElement = new CListLabelElementUI();
		if(pListElement)
		{
			wchar_t szColor[1024] = {0};
			ColorUtil::RGBToStr(dwColor, szColor);
			DWORD dwBackColor = 0;
			ColorUtil::AdjustColor(dwColor, dwBackColor);
			if(abs(bRed-bBlue)<=30)
			{
				BYTE bAlpha = 0xff;
				bRed += 60;
				bBlue -= 60;
				dwColor = (COLORREF)(((BYTE)(bBlue)|((WORD)((BYTE)(bGreen))<<8))|(((DWORD)(BYTE)(bRed))<<16)|(((DWORD)(BYTE)(bAlpha))<<24));
			}
			pListElement->SetTextColor(dwColor);
			pListElement->SetBkColor(dwBackColor);
			pListElement->SetText(szColor);
			pListElement->SetAttribute(L"align", L"center");
			pListElement->SetUserData(szHexColor);
			pColorList->AddAt(pListElement, 0);
		}
	}
	return bRet;
}

BOOL CColorPicker::OpenColorDlg(HWND hWnd)
{
	BOOL bRet = FALSE;
	CHOOSECOLORW cc;
	static COLORREF crColors[16];
	memset(&cc, 0, sizeof(CHOOSECOLORW));
	cc.lStructSize = sizeof(CHOOSECOLORW);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD)crColors;
	cc.rgbResult = 0xFFFFFF;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.lCustData = NULL;
	cc.lpfnHook = NULL;
	cc.hInstance = NULL;
	cc.lpTemplateName = NULL;
	if(ChooseColorW(&cc))
	{
		//ColorUtil::AdjustColor(cc.rgbResult, colCurColor);
		if(pPickerPreview)
		{
			DWORD dwBackColor = 0;
			ColorUtil::AdjustColor(cc.rgbResult, dwBackColor);
			pPickerPreview->SetBkColor(dwBackColor);
		}
		SetColorPicker(cc.rgbResult, TRUE);
	}

	return bRet;
}

BOOL CColorPicker::OpenZoomIn(HWND hWnd)
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024], szArgs[1024];
	swprintf(szPath, L"%s\\%s", g_szModule, kSnapShotExec);
	if(!PathFileExists(szPath))
	{
		wchar_t szErr[1024], szNoFound[1024], szTitle[1024];
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kMsgErr, szTitle);
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kNoFoundErr, szNoFound);
		swprintf(szErr, szNoFound, kSnapShotExec);
		DuiMsgBox(hWnd, szErr, szTitle, MB_OK);
		return bRet;
	}
	swprintf(szArgs, _T("-h %ld -m m"), hWnd);
	//swprintf(szArgs, _T("-h %ld -m m -d"), hWnd);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	GetStartupInfo(&si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	bRet = CreateProcess(szPath, szArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(!bRet)
		return bRet;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bRet;
}

BOOL CColorPicker::OpenSnapShot(HWND hWnd)
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024], szArgs[1024];
	swprintf(szPath, L"%s\\%s", g_szModule, kSnapShotExec);
	if(!PathFileExists(szPath))
	{
		wchar_t szErr[1024], szNoFound[1024], szTitle[1024];
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kMsgErr, szTitle);
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kNoFoundErr, szNoFound);
		swprintf(szErr, szNoFound, kSnapShotExec);
		DuiMsgBox(hWnd, szErr, szTitle, MB_OK);
		return bRet;
	}
	swprintf(szArgs, _T("-p %s -h %ld -m f"), g_szSnapShot, hWnd);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	GetStartupInfo(&si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	bRet = CreateProcess(szPath, szArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(!bRet)
		return bRet;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bRet;
}

void CColorPicker::OnSnapShotUpdate(COLORREF clrColor, LPCWSTR lpszPath /*= NULL*/)
{
	wchar_t szSnapShotPath[1024] = {0};
	wcscpy(szSnapShotPath, lpszPath);
	colCurColor = clrColor;
	//ColorUtil::AdjustColor(clrColor, colCurColor);
	//SetColorPicker(colCurColor, TRUE, FALSE);
	SetColorPicker(clrColor, TRUE, FALSE);
	if(PathFileExists(szSnapShotPath))
		SHHelper::OpenFolder(NULL, szSnapShotPath);
}

void CColorPicker::OnZoomInUpdate(COLORREF clrColor, POINT ptCursor, BOOL bRecord)
{
	colCurColor = clrColor;
	//ColorUtil::AdjustColor(clrColor, colCurColor);
	//SetColorPicker(colCurColor, bRecord);
	if(pPickerPreview)
	{
		DWORD dwBackColor = 0;
		ColorUtil::AdjustColor(clrColor, dwBackColor);
		pPickerPreview->SetBkColor(dwBackColor);
	}
	SetColorPicker(clrColor, bRecord);
	SetColorInfo(ptCursor);
}