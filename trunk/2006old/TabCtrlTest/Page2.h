#pragma once


// CPage2 dialog

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPage2();

// Dialog Data
	enum { IDD = IDD_Page2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
