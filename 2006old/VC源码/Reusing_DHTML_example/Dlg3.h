#if !defined(AFX_DLG3_H__F3E46BD0_64C8_4345_857C_7183A8EC2862__INCLUDED_)
#define AFX_DLG3_H__F3E46BD0_64C8_4345_857C_7183A8EC2862__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg3.h : header file
//


#include "Dlg2.h"
#include "Dlg4.h"
/////////////////////////////////////////////////////////////////////////////
// CMainDlgdialog

class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor

	CDlg2 dlg2;	
	CDlg4 dlg4;	
// Dialog Data
	//{{AFX_DATA(CDlg3)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg3)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG3_H__F3E46BD0_64C8_4345_857C_7183A8EC2862__INCLUDED_)
