// TestClientDlg.h : header file
//

#if !defined(AFX_TESTCLIENTDLG_H__69917168_D633_44C9_B326_066B01FC7071__INCLUDED_)
#define AFX_TESTCLIENTDLG_H__69917168_D633_44C9_B326_066B01FC7071__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestClientDlg dialog

// ***** Make a forward declaration so that our TEventHandler template class can use it. *****
class CTestClientDlg;

// ***** Declare an event handling class using the TEventHandler template. *****
typedef TEventHandler<CTestClientDlg, IEventFiringObject, _IEventFiringObjectEvents> IEventFiringObjectEventHandler;

class CTestClientDlg : public CDialog
{
// Construction
public:
	CTestClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestClientDlg)
	enum { IDD = IDD_TESTCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON							m_hIcon;

	// ***** Declare an instance of a IEventFiringObject smart pointer. *****
	IEventFiringObjectPtr			m_spIEventFiringObject;  

	// ***** Declare a pointer to a TEventHandler class which is specially tailored *****
	// ***** to receiving events from the _IEventFiringObjectEvents events of an *****
	// ***** IEventFiringObject object. *****
	IEventFiringObjectEventHandler* m_pIEventFiringObjectEventHandler;

	// Generated message map functions
	//{{AFX_MSG(CTestClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnButtonCallTestFunction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HRESULT CTestClientDlg::OnEventFiringObjectInvoke
	(
	  IEventFiringObjectEventHandler* pEventHandler,
	  DISPID dispidMember, 
	  REFIID riid,
	  LCID lcid, 
	  WORD wFlags, 
	  DISPPARAMS* pdispparams, 
	  VARIANT* pvarResult,
	  EXCEPINFO* pexcepinfo, 
	  UINT* puArgErr
	);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCLIENTDLG_H__69917168_D633_44C9_B326_066B01FC7071__INCLUDED_)
