#pragma once

class CSelUserNameDlg : public CDialogImpl<CSelUserNameDlg>
	, public CMessageFilter
	, public CWinDataExchange<CSelUserNameDlg>
{
public:
	enum
	{
		IDD = IDD_SELUSERNAME,
	};
	
	RegisterResult m_Result;

	CString m_SeledUserName;
	
	CAtlArray<int> m_Radios;

	BEGIN_DDX_MAP(CSelUserNameDlg)
		
	END_DDX_MAP()

	BEGIN_MSG_MAP(CSelUserNameDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_CODE_HANDLER(BN_CLICKED, OnBnClick)
		
		
		COMMAND_HANDLER(IDC_CUSUSERNAME, EN_SETFOCUS, OnEditSetFocus)
		COMMAND_HANDLER(IDC_CUSUSERNAME, EN_CHANGE, OnEditChange)
	END_MSG_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT OnInitDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	LRESULT OnFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	LRESULT OnOK(WORD nCode, WORD nID, HWND hWnd, BOOL & bHandled);
	LRESULT OnCancel(WORD nCode, WORD nID, HWND hWnd, BOOL & bHandled);

public:
	LRESULT OnEditSetFocus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

};
