#pragma once


// CPage3 dialog

class CPage3 : public CDialog
{
	DECLARE_DYNAMIC(CPage3)

public:
	CPage3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPage3();

// Dialog Data
	enum { IDD = IDD_Page3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
