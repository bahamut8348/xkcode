// dhtml2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "dhtml2.h"
#include "dhtml2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDhtml2Dlg dialog

CDhtml2Dlg::CDhtml2Dlg(CWnd* pParent /*=NULL*/)
	: CHtmlDialog(CDhtml2Dlg::IDD, IDR_HTML1, IDC_STATIC1, pParent)
{
	//{{AFX_DATA_INIT(CDhtml2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	
}

void CDhtml2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CHtmlDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDhtml2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDhtml2Dlg, CHtmlDialog)
	//{{AFX_MSG_MAP(CDhtml2Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDhtml2Dlg message handlers

BOOL CDhtml2Dlg::OnInitDialog()
{

	CHtmlDialog::OnInitDialog();	
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


