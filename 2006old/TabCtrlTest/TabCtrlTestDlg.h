// TabCtrlTestDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CTabCtrlTestDlg dialog
class CTabCtrlTestDlg : public CDialog
{
// Construction
public:
	CTabCtrlTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TABCTRLTEST_DIALOG };

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
	CTabCtrl m_tab;
	CAutoPtrArray<CDialog> m_pages;


	void SelPage(int idx);

	afx_msg void OnTabSel(NMHDR *pNMHDR, LRESULT *pResult);
};
