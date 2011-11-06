#pragma once

class CFileServerDlg : public CDialogImpl<CFileServerDlg>,
	public CDialogResize<CFileServerDlg>
{
public:
	enum
	{
		IDD = IDD_fileserver,
	};

	Array<stIP_FSNode>  m_FsList;

	CSortListViewCtrl m_serverlist;
	
	BEGIN_DLGRESIZE_MAP(CFileServerDlg)
		DLGRESIZE_CONTROL(ID_CLOSE, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_LIST1, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CFileServerDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_CLOSE, OnClose2)
		CHAIN_MSG_MAP(CDialogResize<CFileServerDlg>)
	END_MSG_MAP()


public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose2 (UINT uCode, int nID, HWND hwndCtrl, BOOL& bHandled );

};
