#pragma once




class CLobbyServerdlg : public CDialogImpl<CLobbyServerdlg>,
	public CDialogResize<CLobbyServerdlg>
{
public:
	CLobbyServerdlg(void);
public:
	~CLobbyServerdlg(void);
public:

	enum
	{
		IDD = IDD_LOBBYSERVERDLG,
	};

	Array<stIP_LobbyNode> m_servers;
	CSortListViewCtrl m_serverlist;

	BEGIN_DLGRESIZE_MAP(CLobbyServerdlg)
		DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC2, DLSZ_SIZE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_LIST1, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()


	BEGIN_MSG_MAP(CLobbyServerdlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		
		CHAIN_MSG_MAP(CDialogResize<CLobbyServerdlg>)
	END_MSG_MAP()
public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK ( UINT uCode, int nID, HWND hwndCtrl, BOOL& bHandled );
};
