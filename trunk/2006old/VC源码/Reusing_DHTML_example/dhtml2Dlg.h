// dhtml2Dlg.h : header file
//

#if !defined(AFX_DHTML2DLG_H__C7C92F26_DE60_41EA_B0E2_F9C47B1DF11D__INCLUDED_)
#define AFX_DHTML2DLG_H__C7C92F26_DE60_41EA_B0E2_F9C47B1DF11D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HtmlDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDhtml2Dlg dialog

class CDhtml2Dlg : public CHtmlDialog
{
// Construction
public:
	CDhtml2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDhtml2Dlg)
	enum { IDD = IDD_DHTML2_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDhtml2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CDhtml2Dlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DHTML2DLG_H__C7C92F26_DE60_41EA_B0E2_F9C47B1DF11D__INCLUDED_)
