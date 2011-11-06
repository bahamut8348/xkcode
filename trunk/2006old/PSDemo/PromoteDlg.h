#pragma once

class CPromoteDlg : public CDialogImpl<CPromoteDlg>
	, public CWinDataExchange<CPromoteDlg>
{
public:
	enum
	{
		IDD = IDD_PROMOTE,
	};

	CString m_Cmd1;
	CString m_Cmd2;
	CString m_Desc;
	CString m_Title;
	CString m_Msg;

	 void Set(LPCTSTR cmd1, LPCTSTR cmd2, LPCTSTR desc, LPCTSTR title)
	 {
		m_Cmd1 = cmd1;
		m_Cmd2 = cmd2;
		m_Desc = desc;
		m_Title = title;
	 }

	CButton m_BnCmd1;
	CButton m_BnCmd2;


	BEGIN_MSG_MAP(CPromoteDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_BNLEFT, OnLeftCmd)
		COMMAND_ID_HANDLER(ID_BNRIGHT, OnRightCmd)
	
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPromoteDlg)
		DDX_TEXT(IDC_MSG, m_Msg)
		DDX_CONTROL_HANDLE(ID_BNLEFT, m_BnCmd1)
		DDX_CONTROL_HANDLE(ID_BNRIGHT, m_BnCmd2)
		DDX_TEXT(IDC_DLGDESC, m_Desc)
		DDX_TEXT(IDD_PROMOTE, m_Title)
	END_DDX_MAP()
public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLeftCmd(WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	LRESULT OnRightCmd(WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	

};
