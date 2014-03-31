
// MenuDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MenuDemo.h"
#include "MenuDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMenuDemoDlg dialog




CMenuDemoDlg::CMenuDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMenuDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMenuDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMenuDemoDlg::OnClick)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CMenuDemoDlg message handlers

BOOL CMenuDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if(!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitHooks(HM_CALLWNDPROC | HM_CBT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMenuDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMenuDemoDlg::OnPaint()
{
	if(IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();

		CPaintDC dc(this);
		RECT rect;
		GetClientRect(&rect);
		//::DrawEdge(dc.GetSafeHdc(), &rect, TRUE ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);

		HBRUSH brush = ::CreateSolidBrush(GetSysColor(COLOR_3DHILIGHT));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(dc.GetSafeHdc(), brush);
		::PatBlt(dc.GetSafeHdc(), rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
		SelectObject(dc.GetSafeHdc(), hOldBrush);
		DeleteObject(brush);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMenuDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMenuDemoDlg::OnClick()
{
	HWND hBtn = ::GetDlgItem(m_hWnd, IDC_BUTTON1);
	RECT rtBtn;
	::GetWindowRect(hBtn, &rtBtn);
	POINT ptBtn = { rtBtn.left, rtBtn.bottom };
	CMenuEx	*pMenu = new CMenuEx();
	pMenu->CreatePopupMenu();
	pMenu->SetSelectedBarColor(RGB(0x66, 0x99, 0xff));
	HBITMAP hBackBitmap = ::LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BACK), RT_BITMAP), MAKEINTRESOURCE(IDB_BACK));
	pMenu->SetBackBitmap(hBackBitmap);
	//pMenu->SetBackColor(RGB(250, 250, 250));
	HBITMAP hSepBitmap = ::LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_SEPARATOR), RT_BITMAP), MAKEINTRESOURCE(IDB_SEPARATOR));
	pMenu->SetSepBitmap(hSepBitmap);

	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 18;
	lf.lfWeight = FW_LIGHT;
	wcscpy(lf.lfFaceName, L"微软雅黑");
	pMenu->SetFont(lf);
	pMenu->SetTextColor(RGB(15, 57, 110));
	pMenu->SetWidth(216);
	pMenu->SetHeight(24);

	//pMenu->AddSideBar(new CMenuExSideBar(34, L"MenuXP"));
	//pMenu->AddTitle(new CMenuExTitle(24, L"MenuXP"));
	ACCEL stAccel;
	stAccel.fVirt = FCONTROL | FVIRTKEY;
	stAccel.key = 0x4E;
	pMenu->AddMenu(0, new CMenuExText(10, L"写新邮件", AfxGetApp()->LoadIcon(IDI_ICON1)), &stAccel);
	stAccel.fVirt = FVIRTKEY;
	stAccel.key = VK_F4;
	pMenu->AddMenu(0, new CMenuExText(11, L"收取所有邮件", AfxGetApp()->LoadIcon(IDI_ICON2)), &stAccel);
	pMenu->AddSeparator();
	pMenu->AddMenu(0, new CMenuExText(12, L"显示主窗口", AfxGetApp()->LoadIcon(IDI_ICON3)));
	pMenu->AddSeparator();

	CMenuEx *pPopup = new CMenuEx();
	pPopup->CreatePopupMenu();

	pPopup->SetSelectedBarColor(RGB(0x66, 0x99, 0xff));
	//pPopup->SetMenuStyle(CMenuXP::STYLE_XP);

	//pPopup->AddSideBar(new CMenuExSideBar(34, L"Buttons"));
	//pPopup->SetSideBarStartColor(RGB(125, 230, 192));
	//pPopup->SetSideBarEndColor(RGB(0, 0, 0));
	pPopup->SetBackColor(RGB(255, 255, 255));
	pPopup->SetFont(lf);
	pPopup->SetTextColor(RGB(15, 57, 110));
	pPopup->SetWidth(216);
	pPopup->SetHeight(24);
	pPopup->AddMenu(0, new CMenuExText(21, L"abc@163.com", AfxGetApp()->LoadIcon(IDI_ICON4)));

	pMenu->AddPopup(0, pPopup, new CMenuExText(13, L"一键登录", AfxGetApp()->LoadIcon(IDI_ICON1)));

	pMenu->AddMenu(0, new CMenuExButton(14, AfxGetApp()->LoadIcon(IDI_ICON5)));
	pMenu->AddSeparator();
	stAccel.fVirt = FVIRTKEY | FCONTROL | FSHIFT;
	stAccel.key = 0x4C;
	pMenu->AddMenu(MF_DISABLED, new CMenuExButton(15, AfxGetApp()->LoadIcon(IDI_ICON1)), &stAccel);
	stAccel.fVirt = FVIRTKEY;
	stAccel.key = VK_F3;
	pMenu->AddMenu(MF_CHECKED, new CMenuExText(16, L"系统设置", NULL), &stAccel);
	pMenu->AddMenu(0, new CMenuExText(17, L"退出", NULL));

	pMenu->TrackPopupMenu(TPM_LEFTBUTTON, ptBtn.x, ptBtn.y, m_hWnd);
	delete pMenu;
}

void CMenuDemoDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if(lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		HMENU hMenu = CMenuEx::GetLastMenu();
		if(hMenu)
		{
			wchar_t szDebug[256];
			swprintf(szDebug, L"CMenuDemoDlg::OnMeasureItem - %p\n", hMenu);
			OutputDebugStringW(szDebug);
			CMenuEx *pMenu = CMenuEx::FromHandle(hMenu);
			pMenu->MeasureItem(lpMeasureItemStruct);
		}
	}
	else
	{
		CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	}
}

void CMenuDemoDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->CtlType == ODT_MENU)
	{
		CMenuEx *pMenu = CMenuEx::FromHandle((HMENU)lpDrawItemStruct->hwndItem);
		CMenuEx *pMenu2 = CMenuEx::GetMenuEx(lpDrawItemStruct->hwndItem);
		pMenu->DrawItem(lpDrawItemStruct);
	}
	else
	{
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}

BOOL CMenuDemoDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	wchar_t szDebug[1024];
	if(HIWORD(wParam)==0)
	{
		swprintf(szDebug, L"CMenuDemoDlg::OnCommand - Menu ID:%d\n", LOWORD(wParam));
		OutputDebugStringW(szDebug);
		if(LOWORD(wParam)==17)
			PostMessage(WM_CLOSE);
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CMenuDemoDlg::OnCallWnd(const MSG& msg)
{
	static BOOL bSkinning = FALSE;
	if(!bSkinning)
	{
		bSkinning = TRUE;
		switch(msg.message)
		{
		case WM_CREATE:
			{
				if(CMenuEx::IsMenuHwnd(msg.hwnd))
					CMenuEx::InitHook(msg.hwnd);
			}
			break;
		case WM_SHARED_MENU:
			{
				if(CMenuEx::IsMenuHwnd(msg.hwnd) && msg.wParam)
					CMenuEx::InitHook(msg.hwnd);
			}
			break;
		case WM_WINDOWPOSCHANGING: 
			{
				if(CMenuEx::IsMenuHwnd(msg.hwnd) && msg.lParam)
				{
					WINDOWPOS* pWPos = (WINDOWPOS*)msg.lParam;
					if(pWPos->flags & SWP_SHOWWINDOW)
						CMenuEx::InitHook(msg.hwnd);
					else if(pWPos->flags & SWP_HIDEWINDOW)
						CMenuEx::UninitHook(msg.hwnd);
				}
			}
			break;
		case WM_SHOWWINDOW:
			{
				if(CMenuEx::IsMenuHwnd(msg.hwnd))
				{
					if(msg.wParam)
						CMenuEx::InitHook(msg.hwnd);
					else // if(!msg.wParam)
						CMenuEx::UninitHook(msg.hwnd);
				}
			}
			break;
		case WM_DESTROY:
		case WM_NCDESTROY:
			{
				if(CMenuEx::IsMenuHwnd(msg.hwnd))
					CMenuEx::UninitHook(msg.hwnd);
			}
			break;
		case WM_CONTEXTMENU:
			{

			}
			break;
		case WM_INITMENUPOPUP:
			{

			}
			break;
		}
		bSkinning = FALSE;
	}
}

BOOL CMenuDemoDlg::OnCbt(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HCBT_SYSCOMMAND)
	{
		switch(wParam)
		{
		case SC_MOUSEMENU:
			{
				if(!lParam)
					break;
				if(CMenuEx::OnMouseMenu(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
					return TRUE;
			}
			break;
		}
	}
	return FALSE;
}