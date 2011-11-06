// Dlg4.cpp : implementation file
//

#include "stdafx.h"
#include "dhtml2.h"
#include "Dlg4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg4 dialog


CDlg4::CDlg4(CWnd* pParent /*=NULL*/)
	:CHtmlDialog(CDlg4::IDD,pParent,  IDR_HTML4, IDC_STATIC1)
{
	//{{AFX_DATA_INIT(CDlg4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg4)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg4, CDialog)
	//{{AFX_MSG_MAP(CDlg4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg4 message handlers


void CDlg4::_onHtmlCmd(UINT cmd, LPCTSTR params){
	CHtmlDialog::_onHtmlCmd(cmd, params);

	switch (cmd) {		
		case 1005: // OK
			{
				CString result;
				m_HtmlCtrl.CallJScript("GetResult", &result);
				AfxMessageBox("Dialog return: \n" + result);
				EndDialog(1);
			}
		break;

	}
	//	MessageBox( params, "1111");

}

BOOL CDlg4::OnInitDialog() 
{
	CHtmlDialog::OnInitDialog();

	// !!!
	// we cant do here any action with Html object ...	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg4::_onDocumentComplete(){

	CHtmlDialog::_onDocumentComplete();

	CStringArray strArray;

	
	strArray.Add("Picture@500");
	strArray.Add("Video Clip@200");
	strArray.Add("Some more@50");

	// first element in array 
	// Name @ size @ free_space
	strArray.Add("File Document@1708@201");

	// Call Html Script, to set parameters 
	m_HtmlCtrl.CallJScript2("SetParameters", strArray);
		

}
