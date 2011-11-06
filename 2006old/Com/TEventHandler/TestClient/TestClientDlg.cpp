// TestClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestClient.h"
#include "TestClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestClientDlg dialog

CTestClientDlg::CTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestClientDlg, CDialog)
	//{{AFX_MSG_MAP(CTestClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CALL_TEST_FUNCTION, OnButtonCallTestFunction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestClientDlg message handlers

BOOL CTestClientDlg::OnInitDialog()
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

	// ***** Create an instance of an object which implements IEventFiringObject. *****
	m_spIEventFiringObject.CreateInstance(__uuidof(EventFiringObject));

	// ***** Instantiate an IEventFiringObjectEventHandler object. *****
	m_pIEventFiringObjectEventHandler = new IEventFiringObjectEventHandler(*this, m_spIEventFiringObject, &CTestClientDlg::OnEventFiringObjectInvoke);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}





// ***** OnEventFiringObjectInvoke() is inoked by the TEventHandler based class object *****
// ***** when an event is fired from the COM object that implements IEventFiringObject. *****
HRESULT CTestClientDlg::OnEventFiringObjectInvoke
(
  IEventFiringObjectEventHandler* pEventHandler,
  DISPID dispidMember, 
  REFIID riid,
  LCID lcid, 
  WORD wFlags, 
  DISPPARAMS* pdispparams, 
  VARIANT* pvarResult,
  EXCEPINFO* pexcepinfo, 
  UINT* puArgErr
)
{
  if (dispidMember == 0x01)  // Event1 event.
  {
	// 1st param : [in] long lValue.
	VARIANT	varlValue;
	long	lValue = 0;

	VariantInit(&varlValue);
	VariantClear(&varlValue);

	varlValue = (pdispparams -> rgvarg)[0];

	lValue = V_I4(&varlValue);

	TCHAR szMessage[256];

	sprintf (szMessage, "Event 1 is fired with value : %d.", lValue);

	::MessageBox (NULL, szMessage, "Event", MB_OK);
  }

  return S_OK;
}

void CTestClientDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	// ***** When the program is terminating, make sure that we instruct our *****
	// ***** Event Handler to disconnect from the connection point of the *****
	// ***** object which implemented the IEventFiringObject interface. *****
	// ***** We also needs to Release() it (instead of deleting it). *****
	if (m_pIEventFiringObjectEventHandler)
	{
	  m_pIEventFiringObjectEventHandler -> ShutdownConnectionPoint();
	  m_pIEventFiringObjectEventHandler -> Release();
	  m_pIEventFiringObjectEventHandler = NULL;
	}
}

void CTestClientDlg::OnButtonCallTestFunction() 
{
	// TODO: Add your control notification handler code here
	// ***** Call the IEventFiringObject.TestFunction(). *****
	// ***** This will cause the object which implements *****
	// ***** IEventFiringObject to fire Event1. *****
	m_spIEventFiringObject -> TestFunction(456);
}
