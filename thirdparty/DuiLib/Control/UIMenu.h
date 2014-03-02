#ifndef __H_CMENUUI_20111113
#define __H_CMENUUI_20111113

#pragma once

namespace DuiLib { 

enum MenuAlignment
{
	eMenuAlignment_Left = 1 << 1,
	eMenuAlignment_Top = 1 << 2,
	eMenuAlignment_Right = 1 << 3,
	eMenuAlignment_Bottom = 1 << 4,
};

class /*UILIB_API*/ CMenuUI : public WindowImplBase
{
public:
	CMenuUI(HWND hParent = NULL);
	virtual ~CMenuUI();

	LPCWSTR				GetWindowClassName() const;
	void				InitWindow();
	CDuiString			GetSkinFile();
	CDuiString			GetSkinFolder();
	void				OnFinalMessage(HWND hWnd);

	void				Notify(TNotifyUI& msg);
	virtual LRESULT		HandleMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	virtual LRESULT		HandleCustomMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT		OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnMouseHover(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnActivateApp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	virtual LRESULT		OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void			Init(CControlUI* pOwner, LPCTSTR xml, LPCTSTR pSkinType, POINT pt);
	void			Init(CControlUI* pOwner, LPCTSTR xml, LPCTSTR pSkinType, POINT pt, SIZE st);
	void			AdjustPostion();

public:
	CDuiString	builder_xml_;
	CDuiString	resource_dir_;
	CControlUI	*owner_control_;
	DWORD		popup_alignment;
	HWND		parent_hwnd;
	POINT		base_position;
	SIZE		base_size;
};

} // namespace DuiLib
#endif __H_CMENUUI_20111113
