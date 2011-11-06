#if !defined(AFX_DLG4_H__AC3AAEAB_066D_491F_8DCE_6F1A4B898C3B__INCLUDED_)
#define AFX_DLG4_H__AC3AAEAB_066D_491F_8DCE_6F1A4B898C3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg4.h : header file
//

#include "HtmlDialog.h"
/////////////////////////////////////////////////////////////////////////////
// This is "Add data" (IDR_HTML4)
//   IDR_HTML3 passed to CHtmlDialog in contructor CDlg4
//

class CDlg4 : public CHtmlDialog
{
// Construction
public:
	int cbSelectedBlockSize;
	CString sSelectedBlock;
	CDlg4(CWnd* pParent = NULL);   // standard constructor

	void _onHtmlCmd(UINT cmd, LPCTSTR params);
	virtual void _onDocumentComplete();
// Dialog Data
	//{{AFX_DATA(CDlg4)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg4)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG4_H__AC3AAEAB_066D_491F_8DCE_6F1A4B898C3B__INCLUDED_)
