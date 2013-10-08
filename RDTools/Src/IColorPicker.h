#pragma once

class IColorPicker
{
public:
	IColorPicker();
	virtual ~IColorPicker();

public:
	LPCWSTR			GetPickerListName();
	void			SetPickerOwner(HWND hWnd, CPaintManagerUI *pManager) { hPickerOwner = hWnd; pPickerManager = pManager; };
	BOOL			InitPicker();
	BOOL			SetPickerLang(LPCWSTR lpszLang);

	BOOL			SetColorInfo(POINT ptCursor);
	BOOL			SetColorPicker(DWORD dwColor, BOOL bRecord = TRUE, BOOL bAuto = TRUE);
	BOOL			OpenColorDlg();
	BOOL			OpenZoomIn();
	BOOL			OpenSnapShot();

	void			OnPickerClick(TNotifyUI& msg, BOOL& bHandled);
	void			OnPickerItemActive(TNotifyUI& msg);
	void			OnPickerItemClick(TNotifyUI& msg);
	void			OnPickerTextChanged(TNotifyUI& msg, BOOL& bHandled);
	LRESULT			OnPickerLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			OnPickerMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT			OnPickerLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	void			OnSnapShotUpdate(COLORREF clrColor, LPCWSTR lpszPath = NULL);
	void			OnZoomInUpdate(COLORREF clrColor, POINT ptCursor, BOOL bRecord);

public:
	HWND hPickerOwner;
	CPaintManagerUI *pPickerManager;
	COLORREF colCurColor;
	CControlUI *pPickerPreview;
	CControlUI *pPickerArrow;
	CControlUI *pPickerSnapShot;
	CControlUI *pPickerPos;
	CControlUI *pPickerClass;
	CControlUI *pPickerText;
	CControlUI *pPickerColor;
	CControlUI *pPickerHtmlColor;
	CControlUI *pPickerRGBColor;
	CControlUI *pPickerHexColor;
	CControlUI *pPickerHSLColor;
	CControlUI *pPickerDelphiColor;
	CControlUI *pPickerHSVColor;
	CListUI *pColorList;
	BOOL bIsCapturing;
};
