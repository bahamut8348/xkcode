//  CHtmlDialog class
//
//
//
// 

/////////////////////////////////////////////////////////////////
//             Written by Alexander Shilonosov                      
//                     November,2003                                
//                   shilo@dekart.com                               
//               Last Update: November, 2003                        
/////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "dhtml2.h"
#include "HtmlDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlDialog dialog

// CHtmlDialog child will transmit HTML_ID and STATIC_ID here
//  
CHtmlDialog::CHtmlDialog( UINT nID_Dlg, CWnd* pParent,UINT nID_HTML, UINT n_ID_static): 
	m_ID_HTML(nID_HTML), m_ID_Static(n_ID_static),
	CDialog(nID_Dlg, pParent)	,
	CHtml_Host_Handlers()
{
	//{{AFX_DATA_INIT(CHtmlDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}

void CHtmlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHtmlDialog, CDialog)
	//{{AFX_MSG_MAP(CHtmlDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CHtmlDialog message handlers

BOOL CHtmlDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	VERIFY(m_HtmlCtrl.CreateFromStatic(m_ID_Static, this, (CHtml_Host_Handlers*)this ) );
	m_HtmlCtrl.LoadFromResource( m_ID_HTML);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// process default messages from Script routines
//  
//
void CHtmlDialog::_onHtmlCmd(UINT cmd, LPCTSTR params){
	switch (cmd) {

		// close dialog
	case 1000:
		EndDialog(1);
		break;

		// Dialog was resized Horizontaly 
	case 1001: // + , params =  "cx@cy"
	case 1002: // - , params =  "cx@cy"
		{
			UINT cx, cy, x;
			CString csCmd(params); // "20@40"
			
			x=0;
				cx = atoi(csCmd.Mid(x+1) );
			if (cx<0)
				return;

			x=0;
			x = csCmd.Find(CMD_DELI, x);
			if (x>=0) 
				cy = atoi(csCmd.Mid(x+1) );
			
			CRect rect;
			GetWindowRect(&rect);
			if (cmd == 1001) {
				rect.left+=cx;
				rect.bottom+=cy;
			} else{
				rect.left-=cx;
				rect.bottom-=cy;
			} 
			
			SetWindowPos(NULL, 0,0, rect.Width(), rect.Height(), SWP_NOMOVE);
			m_HtmlCtrl.SetWindowPos(NULL, 0,0, rect.Width(), rect.Height(), 0);
		}
		break;
	}
	
}

// HTML page loaded to WebBrowser controll 
//   now we should set Dialog Title and Size.
//
void CHtmlDialog::_onDocumentComplete(){

	// CHtmlCtrl allready process "onDocumentComplete" event 
	//   and extract Size and Title from HTML Page.
	//

	SetWindowPos(NULL, 0, 0, m_HtmlCtrl.m_html_width+1, m_HtmlCtrl.m_html_height+1, SWP_NOMOVE);
	CRect r;
	GetClientRect( &r);
	m_HtmlCtrl.SetWindowPos(NULL, 0,0, r.Width(), r.Height(), 0);

	SetWindowText(m_HtmlCtrl.m_html_Title);
}
