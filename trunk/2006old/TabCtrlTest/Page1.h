#pragma once


// CPage1 dialog

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPage1();

// Dialog Data
	enum { IDD = IDD_Page1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
