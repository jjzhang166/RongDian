#include "stdafx.h"
#include "IColorPicker.h"

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

IColorPicker::IColorPicker()
{
	pPickerManager = NULL;
	hPickerOwner = NULL;
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

IColorPicker::~IColorPicker()
{

}

BOOL IColorPicker::InitPicker()
{
	if(!pPickerManager)
		return FALSE;

	FIND_CONTROL_BY_ID(pPickerPreview, CControlUI, pPickerManager, kPickerPreview)
	FIND_CONTROL_BY_ID(pPickerColor, CControlUI, pPickerManager, kPickerColor)
	FIND_CONTROL_BY_ID(pPickerPos, CControlUI, pPickerManager, kPickerPos)
	FIND_CONTROL_BY_ID(pPickerClass, CControlUI, pPickerManager, kPickerClass)
	FIND_CONTROL_BY_ID(pPickerText, CControlUI, pPickerManager, kPickerText)
	FIND_CONTROL_BY_ID(pPickerArrow, CControlUI, pPickerManager, kPickerArrow)
	FIND_CONTROL_BY_ID(pPickerSnapShot, CControlUI, pPickerManager, kPickerSnapShot)
	FIND_CONTROL_BY_ID(pPickerHtmlColor, CControlUI, pPickerManager, kPickerHtmlColor)
	FIND_CONTROL_BY_ID(pPickerRGBColor, CControlUI, pPickerManager, kPickerRGBColor)
	FIND_CONTROL_BY_ID(pPickerHexColor, CControlUI, pPickerManager, kPickerHEXColor)
	FIND_CONTROL_BY_ID(pPickerHSLColor, CControlUI, pPickerManager, kPickerHSLColor)
	FIND_CONTROL_BY_ID(pPickerDelphiColor, CControlUI, pPickerManager, kPickerDelphiColor)
	FIND_CONTROL_BY_ID(pPickerHSVColor, CControlUI, pPickerManager, kPickerHSVColor)
	FIND_CONTROL_BY_ID(pColorList, CListUI, pPickerManager, kPickerColorList)

	return TRUE;
}

BOOL IColorPicker::SetPickerLang(LPCWSTR lpszLang)
{
	if(!pPickerManager)
		return FALSE;

SET_CONTROL_BEGIN(pPickerManager, lpszLang, LS_PICKERPANEL)
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

LPCWSTR IColorPicker::GetPickerListName()
{
	return kPickerColorList;
}

BOOL IColorPicker::SetColorInfo(POINT ptCursor)
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

BOOL IColorPicker::SetColorPicker(DWORD dwColor, BOOL bRecord/* = TRUE*/, BOOL bAuto /*= TRUE*/)
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

BOOL IColorPicker::OpenColorDlg()
{
	BOOL bRet = FALSE;
	CHOOSECOLORW cc;
	static COLORREF crColors[16];
	memset(&cc, 0, sizeof(CHOOSECOLORW));
	cc.lStructSize = sizeof(CHOOSECOLORW);
	cc.hwndOwner = hPickerOwner;
	cc.lpCustColors = (LPDWORD)crColors;
	cc.rgbResult = 0xFFFFFF;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.lCustData = NULL;
	cc.lpfnHook = NULL;
	cc.hInstance = NULL;
	cc.lpTemplateName = NULL;
	if(ChooseColorW(&cc))
	{
		ColorUtil::AdjustColor(cc.rgbResult, colCurColor);
		SetColorPicker(colCurColor, TRUE, FALSE);
	}

	return bRet;
}

BOOL IColorPicker::OpenZoomIn()
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024], szArgs[1024];
	swprintf(szPath, L"%s\\bin\\%s", g_szModule, kSnapShotExec);
	if(!PathFileExists(szPath))
	{
		wchar_t szErr[1024], szNoFound[1024], szTitle[1024];
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kMsgErr, szTitle);
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kNoFoundErr, szNoFound);
		swprintf(szErr, szNoFound, kSnapShotExec);
		DuiMsgBox(hPickerOwner, szErr, szTitle, MB_OK);
		return bRet;
	}
	swprintf(szArgs, _T("-h %ld -m m"), hPickerOwner);

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

BOOL IColorPicker::OpenSnapShot()
{
	BOOL bRet = FALSE;
	wchar_t szPath[1024], szArgs[1024];
	swprintf(szPath, L"%s\\bin\\%s", g_szModule, kSnapShotExec);
	if(!PathFileExists(szPath))
	{
		wchar_t szErr[1024], szNoFound[1024], szTitle[1024];
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kMsgErr, szTitle);
		Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kNoFoundErr, szNoFound);
		swprintf(szErr, szNoFound, kSnapShotExec);
		DuiMsgBox(hPickerOwner, szErr, szTitle, MB_OK);
		return bRet;
	}
	swprintf(szArgs, _T("-p %s -h %ld -m f"), g_szSnapShot, hPickerOwner);

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

void IColorPicker::OnPickerClick(TNotifyUI& msg, BOOL& bHandled)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kPickerPalette)
	{
		bHandled = TRUE;
		OpenColorDlg();
	}
	else if(sCtrlName == kPickerSnapShot)
	{
		bHandled = TRUE;
		OpenSnapShot();
	}
	else if(sCtrlName == kPickerArrow)
	{
		bHandled = TRUE;
		OpenZoomIn();
	}
}

void IColorPicker::OnPickerItemActive(TNotifyUI& msg)
{
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

void IColorPicker::OnPickerItemClick(TNotifyUI& msg)
{
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
	}
}

void IColorPicker::OnPickerTextChanged(TNotifyUI& msg, BOOL& bHandled)
{
	if(pPickerColor && msg.pSender==pPickerColor)
	{
		bHandled = TRUE;
		DWORD dwColor = 0;
		CDuiString strColor = msg.pSender->GetText();
		if(ColorUtil::StrToRGB(strColor, dwColor))
		{
			colCurColor = dwColor;
			SetColorPicker(colCurColor, TRUE, FALSE);
		}
	}
}

LRESULT IColorPicker::OnPickerLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
#if PICKERCOLOR_MOUSEMOVE
	if(!pPickerManager)
		return 0;
	POINT pt;
	GetCursorPos(&pt);
	colCurColor = ::GetPixel(::GetDC(NULL), pt.x, pt.y);
	ScreenToClient(hPickerOwner, &pt);
	//CControlUI *pControl = static_cast<CControlUI *>(pPickerManager->FindControl(pt));
	CControlUI *pControl = NULL;
	FIND_CONTROL_BY_PT(pControl, CControlUI, pPickerManager, pt)
	if(pControl && pPickerArrow && pControl == pPickerArrow)
	{
		if(pPickerPreview)
			pPickerPreview->SetBkColor(kDefBkColor);
		wchar_t szPos[1024];
		swprintf(szPos, L"%d, %d", pt.x, pt.y);
		if(pPickerPos)
			pPickerPos->SetText(szPos);
		wchar_t szArrowImage[1024];
		swprintf(szArrowImage, L"%s\\default\\%s", g_szResPath, kImageCross);
		pControl->SetBkImage(szArrowImage);
		SetCapture(hPickerOwner);

		bIsCapturing = TRUE;
	}
#endif
	return 0;
}

#if PICKERCOLOR_MOUSEMOVE
LRESULT IColorPicker::OnPickerMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
{
	if(bIsCapturing)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam); 
		pt.y = GET_Y_LPARAM(lParam); 
		ClientToScreen(hPickerOwner, &pt);
		//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDI_COLOR_ARROW)));
		COLORREF colColor;
		Utility::GetScreenColor(pt.x, pt.y, colColor);
		ColorUtil::AdjustColor(colColor, colCurColor);
		if(pPickerPreview)
			pPickerPreview->SetBkColor(colCurColor);
		SetColorInfo(pt);
		SetColorPicker(colColor, FALSE);
	}
#else
LRESULT IColorPicker::OnPickerMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
#endif
	return 0;
}

LRESULT IColorPicker::OnPickerLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
#if PICKERCOLOR_MOUSEMOVE
	if (bIsCapturing)
	{
		if(pPickerArrow)
		{
			wchar_t szArrowImage[1024];
			swprintf(szArrowImage, L"%s\\default\\%s", g_szResPath, kImageArrow);
			pPickerArrow->SetBkImage(szArrowImage);
		}
		ReleaseCapture();
		bIsCapturing = FALSE; 
		SetColorPicker(colCurColor, TRUE);
	}
#endif
	return 0;
}

void IColorPicker::OnSnapShotUpdate(COLORREF clrColor, LPCWSTR lpszPath /*= NULL*/)
{
	wchar_t szSnapShotPath[1024] = {0};
	wcscpy(szSnapShotPath, lpszPath);
	colCurColor = clrColor;
	//ColorUtil::AdjustColor(clrColor, colCurColor);
	//SetColorPicker(colCurColor, TRUE, FALSE);
	SetColorPicker(clrColor, TRUE, FALSE);
	if(PathFileExists(szSnapShotPath))
		SHHelper::OpenFolder(hPickerOwner, szSnapShotPath);
}

void IColorPicker::OnZoomInUpdate(COLORREF clrColor, POINT ptCursor, BOOL bRecord)
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
