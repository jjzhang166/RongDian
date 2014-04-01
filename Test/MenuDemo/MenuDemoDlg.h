
// MenuDemoDlg.h : header file
//

#pragma once


// CMenuDemoDlg dialog
class CMenuDemoDlg : public CDialog, public CHookMgr<CMenuDemoDlg>
{
	friend class CHookMgr<CMenuDemoDlg>;
// Construction
public:
	CMenuDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MENUDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClick();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	virtual void OnCallWnd(const MSG& msg);
	virtual BOOL OnCbt(int nCode, WPARAM wParam, LPARAM lParam);
};
