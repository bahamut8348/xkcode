// dhtml2.h : main header file for the DHTML2 application
//

#if !defined(AFX_DHTML2_H__69D6DCB7_B0EB_4006_B125_6C33DCBCAC79__INCLUDED_)
#define AFX_DHTML2_H__69D6DCB7_B0EB_4006_B125_6C33DCBCAC79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDhtml2App:
// See dhtml2.cpp for the implementation of this class
//

class CDhtml2App : public CWinApp
{
public:
	CDhtml2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDhtml2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDhtml2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DHTML2_H__69D6DCB7_B0EB_4006_B125_6C33DCBCAC79__INCLUDED_)
