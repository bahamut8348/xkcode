// SkinDlg.h : Header-Datei
//

#if !defined(AFX_SKINDLG_H__E19A61D6_2B9D_418D_ACD7_C4C008BA5513__INCLUDED_)
#define AFX_SKINDLG_H__E19A61D6_2B9D_418D_ACD7_C4C008BA5513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSkinDlg Dialogfeld

#include "BtnST.h"

class CSkinDlg : public CDialog
{
// Konstruktion
public:
	CSkinDlg(CWnd* pParent = NULL);	// Standard-Konstruktor
	HBITMAP m_hBmp;
	HRGN m_hClientRgn;
	HRGN m_hWndRgn;
	HRGN DIBToRgn(HBITMAP hBmp,COLORREF BkColor,BOOL Direct); // Handle the Skin

// Dialogfelddaten
	//{{AFX_DATA(CSkinDlg)
	enum { IDD = IDD_SKIN_DIALOG };
	CButtonST	m_btnCancel;
	CButtonST	m_btnMinimize;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CSkinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CSkinDlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMinimize();
	virtual void OnCancel();
	afx_msg HRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_SKINDLG_H__E19A61D6_2B9D_418D_ACD7_C4C008BA5513__INCLUDED_)
