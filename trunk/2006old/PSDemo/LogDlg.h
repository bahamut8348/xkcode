#pragma once

class CLogDlg : public CDialogImpl<CLogDlg>
	

{
public:
	enum { IDD = IDD_SHOWLOG };
	

	BEGIN_MSG_MAP(CLogDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()


public :
	CLogEdit m_edit;


public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
