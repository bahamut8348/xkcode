#pragma once

class CRegisterDlg : public CDialogImpl<CRegisterDlg>
	,public CWinDataExchange<CRegisterDlg>
	,public CEditValid<CRegisterDlg, RGB(255,0,0)>
{
public:

	enum n
	{
		IDD = IDD_REGISTER,
	};

	typedef CEditValid<CRegisterDlg, RGB(255,0,0)> EditValidBase;

	stAP_CLIENT_INFO m_RegInfo;
	CString m_Password;

	CLocationCb  m_LocationCb;
	CGenderCb    m_GenderCb;
	//CComboBox box;
	

	bool m_bNewsLetter;

	
	BEGIN_DDX_MAP(CRegisterDlg)
		DDX_TEXT_LEN(IDC_USERNAME, m_RegInfo.UserName, 40)
		DDX_TEXT(IDC_NICKNAME, m_RegInfo.NickName)
		DDX_TEXT(IDC_PASSWORD, m_Password)
		DDX_TEXT(IDC_EMAIL, m_RegInfo.Email)
		//DDX_INT(IDC_LOCATION, m_RegInfo.Country)
		DDX_TEXT(IDC_CITY, m_RegInfo.City)
		//DDX_INT(IDC_GENDER, m_RegInfo.Sex)
		DDX_INT(IDC_YEAR, m_RegInfo.Birthday.Year)
		DDX_INT(IDC_MONTH,m_RegInfo.Birthday.Month)
		DDX_INT(IDC_DAY, m_RegInfo.Birthday.Day)
		DDX_CHECK(IDC_NEWSLETTER, m_bNewsLetter)
	END_DDX_MAP()
	
	

	BEGIN_MSG_MAP(CRegisterDlg)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOK)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP( EditValidBase )
	END_MSG_MAP()


	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);




public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
