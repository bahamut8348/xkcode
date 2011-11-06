// TestClient.h : main header file for the TESTCLIENT application
//

#if !defined(AFX_TESTCLIENT_H__3DED3F5C_22B7_443A_9561_7B54B917ACA4__INCLUDED_)
#define AFX_TESTCLIENT_H__3DED3F5C_22B7_443A_9561_7B54B917ACA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestClientApp:
// See TestClient.cpp for the implementation of this class
//

class CTestClientApp : public CWinApp
{
public:
	CTestClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestClientApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCLIENT_H__3DED3F5C_22B7_443A_9561_7B54B917ACA4__INCLUDED_)
