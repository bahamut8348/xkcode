// ResizeDlg.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CResizeDlgApp:
// See ResizeDlg.cpp for the implementation of this class
//

class CResizeDlgApp : public CWinApp
{
public:
	CResizeDlgApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CResizeDlgApp theApp;