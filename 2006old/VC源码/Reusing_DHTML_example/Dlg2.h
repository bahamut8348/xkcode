#if !defined(AFX_DLG2_H__88FFE235_8508_4EB3_8574_C34FE51F8DC2__INCLUDED_)
#define AFX_DLG2_H__88FFE235_8508_4EB3_8574_C34FE51F8DC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg2.h : header file
//


#include "HtmlDialog.h"

/////////////////////////////////////////////////////////////////////////////
// This is "Switcher Dialog" (IDR_HTML3)
//   IDR_HTML3 passed to CHtmlDialog in contructor CDlg2
//

class CDlg2 : public CHtmlDialog
{
// Construction
public:
	CDlg2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg2)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG2_H__88FFE235_8508_4EB3_8574_C34FE51F8DC2__INCLUDED_)
