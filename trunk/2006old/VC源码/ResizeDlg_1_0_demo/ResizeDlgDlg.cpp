// ResizeDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResizeDlgApp.h"
#include "ResizeDlgDlg.h"

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


// CResizeDlgDlg dialog



CResizeDlgDlg::CResizeDlgDlg(CWnd* pParent /*=NULL*/)
	: CResizeDlg(CResizeDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CResizeDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizeDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CResizeDlgDlg, CResizeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CResizeDlgDlg message handlers

BOOL CResizeDlgDlg::OnInitDialog()
{
	CResizeDlg::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	GetDlgItem(IDC_EDIT_RESIZE)->SetWindowText(_T("resize x-left and x-right"));
	GetDlgItem(IDC_EDIT_LEFT)->SetWindowText(_T("resize x-left only"));
	GetDlgItem(IDC_EDIT_RIGHT)->SetWindowText(_T("resize x-right only"));
	GetDlgItem(IDC_EDIT_LEFT2)->SetWindowText(_T("resize 50%"));
	GetDlgItem(IDC_EDIT_RIGHT2)->SetWindowText(_T("repos and resize 50%"));

	AddControl(IDC_EDIT_RESIZE, CST_RESIZE, CST_RESIZE, CST_NONE, CST_NONE, 1);

	AddControl(IDOK, CST_REPOS, CST_REPOS, CST_NONE, CST_NONE, 1);

	AddControl(IDCANCEL, CST_RELATIVE, CST_RELATIVE, CST_REPOS, CST_REPOS, 1, 1.0);

	AddControl(IDC_EDIT_LEFT, CST_RESIZE, CST_NONE, CST_NONE, CST_NONE, CST_NONE, 1);
	AddControl(IDC_EDIT_RIGHT, CST_REPOS, CST_RESIZE, CST_NONE, CST_NONE, 1);

	// two edit controls as example 4. make them size share the same of dialog's changed-width. In another words, if dialog's width increase 10 pixels, then each of the edit-control increase its width 5 pixels respectively. 
	AddControl(IDC_EDIT_LEFT2, CST_DELTA_ZOOM, CST_DELTA_ZOOM, CST_NONE, CST_NONE, 0, 0.0, 0.5);
	AddControl(IDC_EDIT_RIGHT2, CST_DELTA_ZOOM, CST_DELTA_ZOOM, CST_NONE, CST_NONE, 0, 0.5, 0.5);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CResizeDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CResizeDlg::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CResizeDlgDlg::OnPaint() 
{
	if (IsIconic())
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
		CResizeDlg::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CResizeDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
