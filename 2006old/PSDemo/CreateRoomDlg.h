#pragma once

class CCreateRoomDlg : public CDialogImpl<CCreateRoomDlg>
			,public CWinDataExchange<CCreateRoomDlg>
{
public:
	enum
	{
		IDD = IDD_CREATEROOM,
	};


	CString m_RoomName;
	CString m_NickName;
	CString m_Password;
	BOOL m_bUsePass;
	_dword m_MaxUser;
	int m_RoomType;


	void SetParam(CString NickName, CString RoomName, CString Password, BOOL bUsePass,
		_dword MaxUser, int Roomtype)
	{
		m_NickName = NickName;
		m_RoomType = Roomtype;
		m_RoomName = RoomName;
		m_Password = Password;
		m_bUsePass = bUsePass;
		m_MaxUser  = MaxUser;
	}
	

	BEGIN_DDX_MAP(CCreateRoomDlg)
		DDX_TEXT_LEN(IDC_NICKNAME, m_NickName, 40)
		DDX_UINT(IDC_MAXUSER, m_MaxUser)
		DDX_TEXT_LEN(IDC_ROOMNAME, m_RoomName, 40)
		DDX_TEXT_LEN(IDC_PASSWORD, m_Password, 40)
		DDX_CHECK(IDC_BUSEPASS ,	m_bUsePass)
		DDX_RADIO(IDC_CHATTYPE, m_RoomType)
	END_DDX_MAP() 

	BEGIN_MSG_MAP(CCreateRoomDlg)
		
//		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()
public:
//	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
