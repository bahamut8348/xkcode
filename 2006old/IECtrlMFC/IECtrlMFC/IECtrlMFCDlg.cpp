// IECtrlMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IECtrlMFC.h"
#include "IECtrlMFCDlg.h"

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


// CIECtrlMFCDlg dialog




CIECtrlMFCDlg::CIECtrlMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIECtrlMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIECtrlMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_ie);
}

BEGIN_MESSAGE_MAP(CIECtrlMFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CIECtrlMFCDlg::OnBnHtmlClick)
	ON_BN_CLICKED(IDC_BUTTON2, &CIECtrlMFCDlg::OnBnTo163Click)
END_MESSAGE_MAP()


// CIECtrlMFCDlg message handlers

BOOL CIECtrlMFCDlg::OnInitDialog()
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
	m_ie.Navigate(L"about:blank", NULL, NULL, NULL, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIECtrlMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIECtrlMFCDlg::OnPaint()
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
HCURSOR CIECtrlMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIECtrlMFCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CIECtrlMFCDlg::OnBnHtmlClick()
{
	// TODO: Add your control notification handler code here
	IPersistStreamInitPtr iestream;
	IStreamPtr s;
	CString  str=L"test";
	TCHAR *Ptr = (TCHAR *)GlobalAlloc(GPTR, (str.GetLength()+1) * sizeof(TCHAR));
	lstrcpy(Ptr, str);
	
	::CreateStreamOnHGlobal(Ptr, true, &s);
	
	iestream = m_ie.get_Document();
	iestream->InitNew();
	iestream->Load(s);
	

	
}

void CIECtrlMFCDlg::OnBnTo163Click()
{
	// TODO: Add your control notification handler code here
	IWebBrowser2Ptr web;

	web = m_ie.get_Application();

	web->Navigate(L"http://www.163.com", NULL, NULL, NULL, NULL);
}


BEGIN_EVENTSINK_MAP(CIECtrlMFCDlg, CDialog)
	ON_EVENT(CIECtrlMFCDlg, IDC_EXPLORER1, 259, CIECtrlMFCDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CIECtrlMFCDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: Add your message handler code here
}
