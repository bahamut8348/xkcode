#include "StdAfx.h"

#include "resource.h"

#include "ValidEdit.h"
#include "CommonContrl.h"
#include "RegisterDlg.h"
#include "DetailInfoDlg.h"

LRESULT CDetailInfoDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CDetailInfoDlg::OnRefresh( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	GetProcessor()->QueryAccountInfo(GetFramework()->GetLocalPSUID(), 0);
	return 0;
}

LRESULT CDetailInfoDlg::OnModify( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	DoDataExchange(TRUE);
	m_RegInfo.Country = m_LocationCb.GetValue();
	m_RegInfo.Sex = m_GenderCb.GetValue();
	GetProcessor()->ModifyAccountInfo(0, m_RegInfo.AccID, m_RegInfo);

	return 0;
}

LRESULT CDetailInfoDlg::OnBnClose( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CDetailInfoDlg::OnInitDlg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	m_LocationCb = GetDlgItem(IDC_LOCATION);
	m_LocationCb.Init();
	m_GenderCb = GetDlgItem(IDC_GENDER);
	m_GenderCb.InitGender();
	CenterWindow(GetParent());
	SetWindowText(m_RegInfo.UserName);
	return 0;
}

void CDetailInfoDlg::InitInfo()
{
	m_bNewsLetter = m_RegInfo.EmailNews;
	m_LocationCb.SetValue(m_RegInfo.Country);
	m_GenderCb.SetValue(m_RegInfo.Sex);

	DoDataExchange();

}

LRESULT CDetailInfoDlg::OnShowWindow( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	InitInfo();
	return 0;
}