#include "StdAfx.h"

#include "CommonContrl.h"
#include "ValidEdit.h"

#include "RegisterDlg.h"




LRESULT CRegisterDlg::OnBnClickedCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	EndDialog(IDCANCEL);
	return 0;
}

LRESULT CRegisterDlg::OnBnClickedOK( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	DoDataExchange(TRUE);
	m_RegInfo.EmailNews = (_byte) m_bNewsLetter;
	m_LocationCb = GetDlgItem(IDC_LOCATION);
	m_RegInfo.Country = m_LocationCb.GetValue();
	m_RegInfo.Sex	= (_byte)m_GenderCb.GetValue();
	Oword md5;
	
	Encryption::MD5Checksum((_void *)m_Password.GetBuffer(1024), 
		m_Password.GetLength()*2, md5);
	m_Password.ReleaseBuffer();
	memcpy(m_RegInfo.Password,&md5, sizeof(md5));
	EndDialog(IDOK);
	return 0;
}
LRESULT CRegisterDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	CMenuHandle menu = GetSystemMenu(FALSE);
	menu.EnableMenuItem(SC_CLOSE, MF_BYCOMMAND|MF_GRAYED);
	m_LocationCb = GetDlgItem(IDC_LOCATION);
	m_LocationCb.Init();
	m_LocationCb.SetValue(m_RegInfo.Country);

	m_GenderCb = GetDlgItem(IDC_GENDER);
	m_GenderCb.InitGender();
	m_GenderCb.SetValue(m_RegInfo.Sex);

	m_bNewsLetter = (bool)m_RegInfo.EmailNews;
	DoDataExchange();
	CenterWindow();
	return TRUE;
}
