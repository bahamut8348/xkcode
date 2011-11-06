#include "StdAfx.h"
#include "SelUserNameDlg.h"



LRESULT CSelUserNameDlg::OnInitDlg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	
	m_Radios.Add(IDC_NAME1);
	m_Radios.Add(IDC_NAME2);
	m_Radios.Add(IDC_NAME3);
	m_Radios.Add(IDC_NAME4);
	m_Radios.Add(IDC_NAME5);
	m_Radios.Add(IDC_CUSRADIO);
	
	for(UINT a=0; a<m_Radios.GetCount(); a++)
	{
		SetDlgItemText(m_Radios[a], m_Result.RecommandUserName[a]);
	}
	
	CButton bn = GetDlgItem(IDC_NAME1);
	bn.SetCheck(BST_CHECKED);

	
	return TRUE;
}

LRESULT CSelUserNameDlg::OnFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return 0;
}

BOOL CSelUserNameDlg::PreTranslateMessage( MSG* pMsg )
{
	if(IsDialogMessage(pMsg)) return TRUE;
	if(pMsg->message == WM_SETFOCUS)
	{
		return FALSE;
	}
	return FALSE;
}

LRESULT CSelUserNameDlg::OnOK( WORD nCode, WORD nID, HWND hWnd, BOOL & bHandled )
{

	for(UINT a=0; a<m_Radios.GetCount(); a++)
	{
		CButton bn = GetDlgItem(m_Radios[a]);
		if(bn.GetCheck() == BST_CHECKED)
		{
			bn.GetWindowText(m_SeledUserName.GetBuffer(1024), 1024);
			m_SeledUserName.ReleaseBuffer();
			break;
		}
	}
	
	EndDialog(IDOK);
	return FALSE;
}

LRESULT CSelUserNameDlg::OnCancel( WORD nCode, WORD nID, HWND hWnd, BOOL & bHandled )
{
	EndDialog(IDCANCEL);
	return FALSE;
}

LRESULT CSelUserNameDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	EndDialog(SC_CLOSE);
	return 0;
}
LRESULT CSelUserNameDlg::OnEditSetFocus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CButton bn = GetDlgItem(IDC_CUSRADIO);
	if(!bn.GetCheck())
		bn.SendMessage(WM_LBUTTONDOWN);
	
	return 0;
}

LRESULT CSelUserNameDlg::OnBnClick( WORD nCode/*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	if(wID == IDC_CUSRADIO)
	{
		GetDlgItem(IDC_CUSUSERNAME).SetFocus();
	}
	return FALSE;
}

LRESULT CSelUserNameDlg::OnEditChange( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	BSTR text = 0;
	GetDlgItem(IDC_CUSUSERNAME).GetWindowText(text);
	GetDlgItem(IDC_CUSRADIO).SetWindowText(text);
	return 0;
}