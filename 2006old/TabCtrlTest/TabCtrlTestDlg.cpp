// TabCtrlTestDlg.cpp : implementation file
//

#include "stdafx.h"

#include "Page1.h"
#include "Page2.h"
#include "Page3.h"

#include "TabCtrlTest.h"
#include "TabCtrlTestDlg.h"

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


// CTabCtrlTestDlg dialog




CTabCtrlTestDlg::CTabCtrlTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabCtrlTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTabCtrlTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CTabCtrlTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabCtrlTestDlg::OnTabSel)
END_MESSAGE_MAP()


// CTabCtrlTestDlg message handlers

BOOL CTabCtrlTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// TODO: Add extra initialization here

	m_tab.InsertItem(0, L"title1");
	m_tab.InsertItem(1, L"title2");
	m_tab.InsertItem(2, L"title3");
	//m_tab.InsertItem(3, L"title4");

	CRect rcPage;
	GetDlgItem(IDC_POS)->GetWindowRect(&rcPage);
	m_tab.ScreenToClient(&rcPage);
		
	GetDlgItem(IDC_POS)->ShowWindow(SW_HIDE);
	CAutoPtr<CDialog> pPage1(new CPage1(&m_tab));
	pPage1->Create(IDD_Page1, &m_tab);
	m_pages.Add(pPage1);
	CAutoPtr<CDialog> pPage2(new CPage2(&m_tab));
	pPage2->Create(IDD_Page2, &m_tab);
	m_pages.Add(pPage2);
	CAutoPtr<CDialog> pPage3(new CPage3(&m_tab));
	pPage3->Create(IDD_Page3, &m_tab);
	m_pages.Add(pPage3);

	for(DWORD a=0; a<m_pages.GetCount(); a++)
	{
		CDialog *pdlg = m_pages[a];
		pdlg->MoveWindow(&rcPage);
	}


	SelPage(0);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTabCtrlTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
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

void CTabCtrlTestDlg::OnPaint()
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
		CDialog::OnPaint();
		
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTabCtrlTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTabCtrlTestDlg::OnTabSel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	SelPage(m_tab.GetCurSel());
	*pResult = 0;
}

void CTabCtrlTestDlg::SelPage( int idx )
{
	for(DWORD a=0; a<m_pages.GetCount(); a++)
	{
		CDialog *pdlg = m_pages[a];
		pdlg->ShowWindow(SW_HIDE);
	}
	m_pages[idx]->ShowWindow(SW_SHOWNA);
}