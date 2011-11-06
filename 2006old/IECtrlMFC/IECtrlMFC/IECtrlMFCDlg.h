// IECtrlMFCDlg.h : header file
//

#pragma once
#include "explorer1.h"


// CIECtrlMFCDlg dialog
class CIECtrlMFCDlg : public CDialog
{
// Construction
public:
	CIECtrlMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IECTRLMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_ie;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnHtmlClick();
public:
	afx_msg void OnBnTo163Click();
public:
	DECLARE_EVENTSINK_MAP()
public:
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
};
