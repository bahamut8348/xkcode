// Skin.h : Haupt-Header-Datei f�r die Anwendung SKIN
//

#if !defined(AFX_SKIN_H__8FB95337_6960_409B_BA3C_D19A12F79751__INCLUDED_)
#define AFX_SKIN_H__8FB95337_6960_409B_BA3C_D19A12F79751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CSkinApp:
// Siehe Skin.cpp f�r die Implementierung dieser Klasse
//

class CSkinApp : public CWinApp
{
public:
	CSkinApp();

// �berladungen
	// Vom Klassenassistenten generierte �berladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CSkinApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CSkinApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingef�gt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VER�NDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // !defined(AFX_SKIN_H__8FB95337_6960_409B_BA3C_D19A12F79751__INCLUDED_)
