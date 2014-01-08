#pragma once

//////////////////////////////////////////////////////////////////////////
// CColorPicker
RD_DECLARE_BEGIN(ColorPicker)
	RD_DECLARE_MEMBER(void, OnClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemActive) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemClick) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnTextChanged) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(void, OnItemSelected) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, TNotifyUI& /*msg*/, BOOL& /*bHandled*/);
	RD_DECLARE_MEMBER(LPCWSTR, GetItemText) (HWND /*hWnd*/, CPaintManagerUI* /*pManager*/, CControlUI* /*pControl*/, int /*iIndex*/, int /*iSubItem*/);
	RD_DECLARE_MEMBER(LRESULT, OnCopyData) (WPARAM /*wParam*/, LPARAM /*lParam*/);

	RD_DECLARE_MEMBER(BOOL, SetColorInfo) (POINT ptCursor);
	RD_DECLARE_MEMBER(BOOL, SetColorPicker) (DWORD dwColor, BOOL bRecord = TRUE, BOOL bAuto = TRUE);
	RD_DECLARE_MEMBER(BOOL, OpenColorDlg) (HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, OpenZoomIn) (HWND hWnd);
	RD_DECLARE_MEMBER(BOOL, OpenSnapShot) (HWND hWnd);

	RD_DECLARE_MEMBER(void, OnSnapShotUpdate) (COLORREF clrColor, LPCWSTR lpszPath = NULL);
	RD_DECLARE_MEMBER(void, OnZoomInUpdate) (COLORREF clrColor, POINT ptCursor, BOOL bRecord);

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
RD_DECLARE_END()
// CColorPicker
//////////////////////////////////////////////////////////////////////////
