#pragma once

class CAnotherwindow :  public CDialogImpl<CAnotherwindow>
{
public:
	enum { IDD = IDD_DIALOG1 };

	BEGIN_MSG_MAP(CAboutDlg)
		COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()
	CAnotherwindow(void);
public:
	~CAnotherwindow(void);
public:
	LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
