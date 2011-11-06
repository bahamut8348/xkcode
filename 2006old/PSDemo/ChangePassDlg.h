#pragma once

class CChangePassDlg : public CDialogImpl<CChangePassDlg>
	, public CEditValid<CChangePassDlg, RGB(255,0,0)>
	, public CWinDataExchange<CChangePassDlg>
{
	
public:
	enum
	{
		IDD=IDD_CHANGEPASS,
	};

	CString m_Password;
	CString m_Password1;
	CString m_OldPassword;

	typedef CEditValid<CChangePassDlg, RGB(255,0,0)>  ValidClass;

	BEGIN_DDX_MAP(CChangePassDlg)
		DDX_TEXT(IDC_OLDPASSWORD, m_OldPassword)
		DDX_TEXT(IDC_PASSWORD, m_Password)
		DDX_TEXT(IDC_PASSWORD1, m_Password1)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CChangePassDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(ValidClass)
	END_MSG_MAP()


	LRESULT OnInitDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnCancel (WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	LRESULT OnOK (WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);




};
