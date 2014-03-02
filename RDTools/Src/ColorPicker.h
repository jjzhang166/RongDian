#pragma once

//////////////////////////////////////////////////////////////////////////
// CColorPicker
class CColorPicker : public CBaseTool
{
public:
	CColorPicker();
	virtual ~CColorPicker() { };
public:
	BOOL IsCanQuit(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/);
	void OnQuit();
	BOOL OnInit(WPARAM /*wParam*/, LPARAM /*lParam*/);
	BOOL SetLang(CPaintManagerUI* /*pManager*/, LPCWSTR /*lpszLang*/);

	void OnClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemActive(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemClick(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnTextChanged(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	void OnItemSelected(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	LPCWSTR GetItemText(HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);
	LRESULT OnCopyData(WPARAM /*wParam*/, LPARAM /*lParam*/);

	BOOL SetColorInfo(POINT ptCursor);
	BOOL SetColorPicker(DWORD dwColor, BOOL bRecord = TRUE, BOOL bAuto = TRUE);
	BOOL OpenColorDlg(HWND hWnd);
	BOOL OpenZoomIn(HWND hWnd);
	BOOL OpenSnapShot(HWND hWnd);

	void OnSnapShotUpdate(COLORREF clrColor, LPCWSTR lpszPath = NULL);
	void OnZoomInUpdate(COLORREF clrColor, POINT ptCursor, BOOL bRecord);

public:
	COLORREF colCurColor;
	CControlUI * pPickerPreview;
	CControlUI * pPickerArrow;
	CControlUI * pPickerSnapShot;
	CControlUI * pPickerPos;
	CControlUI * pPickerClass;
	CControlUI * pPickerText;
	CControlUI * pPickerColor;
	CControlUI * pPickerHtmlColor;
	CControlUI * pPickerRGBColor;
	CControlUI * pPickerHexColor;
	CControlUI * pPickerHSLColor;
	CControlUI * pPickerDelphiColor;
	CControlUI * pPickerHSVColor;
	CListUI * pColorList;
	BOOL * bIsCapturing;
};
// CColorPicker
//////////////////////////////////////////////////////////////////////////
