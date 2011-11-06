#pragma once

class CSendMsgDlg : public CDialogImpl<CSendMsgDlg>
	,public CUpdateUI<CSendMsgDlg>
	,public CMessageFilter
	,public CIdleHandler
	,public CDialogResize<CSendMsgDlg>

{
public:
	enum { IDD = IDD_SENDMSGDLG };
	
	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_UPDATE_UI_MAP(CSendMsgDlg)
	END_UPDATE_UI_MAP()

	BEGIN_DLGRESIZE_MAP(CSendMsgDlg)
		DLGRESIZE_CONTROL(IDC_BNSEND, DLSZ_MOVE_X)
		// DLGRESIZE_CONTROL(IDC_USERS, DLSZ_SIZE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_MSG,  DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CSendMsgDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		
		CHAIN_MSG_MAP(CDialogResize<CSendMsgDlg>)
	END_MSG_MAP()

	virtual BOOL OnIdle();
	
public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnForwardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};
