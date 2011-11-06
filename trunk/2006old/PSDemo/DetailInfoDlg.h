#pragma once

class CDetailInfoDlg :public CDialogImpl<CDetailInfoDlg>
	,public CWinDataExchange<CDetailInfoDlg>
	,public CEditValid<CDetailInfoDlg, RGB(255,0,0)>
{
public:
	enum n
	{
		IDD = IDD_DETAILINFO,
	};

	typedef CEditValid<CDetailInfoDlg, RGB(255,0,0)> EditValidBase;

	stAP_CLIENT_INFO m_RegInfo;

	CLocationCb  m_LocationCb;
	CGenderCb    m_GenderCb;

	bool m_bNewsLetter;

	BEGIN_DDX_MAP(CDetailInfoDlg)
		DDX_TEXT_LEN(IDC_USERNAME, m_RegInfo.UserName, 40)
		DDX_TEXT(IDC_NICKNAME, m_RegInfo.NickName)
		DDX_TEXT(IDC_EMAIL, m_RegInfo.Email)
		//DDX_INT(IDC_LOCATION, m_RegInfo.Country)
		DDX_TEXT(IDC_CITY, m_RegInfo.City)
		//DDX_INT(IDC_GENDER, m_RegInfo.Sex)
		DDX_INT(IDC_YEAR, m_RegInfo.Birthday.Year)
		DDX_INT(IDC_MONTH,m_RegInfo.Birthday.Month)
		DDX_INT(IDC_DAY, m_RegInfo.Birthday.Day)
		DDX_CHECK(IDC_NEWSLETTER, m_bNewsLetter)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CDetailInfoDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		COMMAND_ID_HANDLER(ID_REFRESH, OnRefresh)
		COMMAND_ID_HANDLER(ID_MODIFY, OnModify)
		COMMAND_ID_HANDLER(IDOK, OnBnClose)
		CHAIN_MSG_MAP(EditValidBase)
	END_MSG_MAP()
	
	
	void InitInfo();


	LRESULT OnInitDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	LRESULT OnRefresh(WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	LRESULT OnModify(WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	LRESULT OnBnClose(WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);

	




};
