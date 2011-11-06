// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3CB2ACFB_975A_4683_8CCF_CE930C295BF0__INCLUDED_)
#define AFX_STDAFX_H__3CB2ACFB_975A_4683_8CCF_CE930C295BF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#pragma warning(disable : 4786)

// ***** import the COM object's type library as usual. *****
#import "..\EventFiringObject\Debug\EventFiringObject.dll"
using namespace EVENTFIRINGOBJECTLib;

// ***** include our TEventHandler.h header class and use the TEventHandlerNamespace namespace. *****
#include "TEventHandler.h"
using namespace TEventHandlerNamespace;

#endif // !defined(AFX_STDAFX_H__3CB2ACFB_975A_4683_8CCF_CE930C295BF0__INCLUDED_)
