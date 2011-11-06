#include "StdAfx.h"
#include "resource.h"

#include "Config.h"
#include "ValidEdit.h"
#include "LogEdit.h"
#include "LogWindow.h"
#include "LobbyServerdlg.h"
#include "FileServerDlg.h"
#include "CommonContrl.h"
#include "SelUserNameDlg.h"
#include "RegisterDlg.h"
#include "CMenuButton.h"
#include "LoginDlg.h"




LRESULT CLoginDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	

	DestroyWindow();

	::PostQuitMessage(0);
	return 0;
}

LRESULT CLoginDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	DoDataExchange();

	::ZeroMemory(&m_RegInfo, sizeof(m_RegInfo));
	
	// TODO: Add your message handler code here and/or call default
	HICON hIcon= ::LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, FALSE);
	DeleteObject(hIcon);
	
	SetTimer(1,100,NULL);

	CenterWindow();
	m_cfg.m_FileName = System::GetProcessPathName() +  L"\\PSDemo.ini";

	CString ISIP = m_cfg.Get(L"Config", L"ISIP",L"Auto");
	CString UserName = m_cfg.Get(L"Config", L"UserName", L"ps_test");
	CString Password = m_cfg.Get(L"Config", L"Password", L"123123");
	int   AppID		= m_cfg.Get(L"Config", L"AppID",1)
		, SubID		= m_cfg.Get(L"Config", L"SubID",1)
		, MajorVer	= m_cfg.Get(L"Config", L"MajorVer",1)
		, SubVer	= m_cfg.Get(L"Config", L"SubVer",1);
	
	CComboBox box = GetDlgItem(IDC_COMBO1);
	struct ISServer{ wchar_t * server; } Servers[] = {
		L"tcp://60.28.222.89:7788",
		L"tcp://60.28.222.89:7789",
		L"tcp://127.0.0.1:7788",
		L"auto",
		L"tcp://192.168.1.252:7788",
		L"tcp://192.168.1.13:7788",
		NULL,
	};
	for(u_int a=0; a<sizeof(Servers)/sizeof(ISServer); a++)
	{
		box.InsertString(0,Servers[a].server);
	}
	box.SetWindowText(ISIP);

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);


	box = GetDlgItem(IDC_APPID);
	for(int a=0; a<10; a++)
	{
		CString s;
		s.Format(L"%d", a);
		int nItem = box.InsertString(a, s);
		box.SetItemData(nItem, a);
	}
	box.SetCurSel(AppID);
	box = GetDlgItem(IDC_SUBID);
	for(int a=0; a<10; a++)
	{
		CString s;
		s.Format(L"%d", a);
		int nItem = box.InsertString(a, s);
		box.SetItemData(nItem, a);
	}
	box.SetCurSel(SubID);
	
	GetFramework()->SetUIClient(this);
	
	SetDlgItemText(IDC_USERNAME, UserName);
	SetDlgItemText(IDC_PASSWORD, Password);



	CString s;
	s.Format(L"PS - %s %s", _T(__DATE__), _T(__TIME__) );
	SetWindowText(s);

	m_mbtn.SubclassWindow(GetDlgItem(IDC_Game));
	m_mbtn.SpecailMenu(IDR_LOGINOPT);

	UIAddChildWindowContainer(m_hWnd);
//	logwin.Create(NULL,CWindow::rcDefault, L"LogWin");
	return TRUE;
}

LRESULT CLoginDlg::OnSelectServer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}

LRESULT CLoginDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	BSTR IP=0;
	GetDlgItemText(IDC_COMBO1, IP);
	if(IP == 0) IP=L"auto";
	m_cfg.Set(L"Config", L"ISIP",IP);
	CComboBox box;
	box = GetDlgItem(IDC_APPID);
	m_cfg.Set(L"Config", L"AppID", box.GetCurSel() );
	box = GetDlgItem(IDC_SUBID);
	m_cfg.Set(L"Config", L"SubID",box.GetCurSel());
	m_cfg.Set(L"Config", L"MajorVer",1);
	m_cfg.Set(L"Config", L"SubVer",1);


	return 0;
}

LRESULT CLoginDlg::OnBnConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	if(GetFramework()->IsConnected())
	{
		GetFramework()->Disconnect();
		GetDlgItem(IDC_BUTTON4).SetWindowText(L"Connect");
		GetDlgItem(IDC_APPID).EnableWindow(TRUE);
		GetDlgItem(IDC_SUBID).EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO1).EnableWindow(TRUE);
	}
	else
	{
		CComboBox box = GetDlgItem(IDC_COMBO1);
		BSTR text = NULL;
		box.GetWindowText(text);
		if(text == NULL)
		{
			AtlMessageBox(m_hWnd, L"Please select a interface server!");
			return 0;
		}
		box = GetDlgItem(IDC_APPID);
		int AppID = box.GetCurSel();
		box = GetDlgItem(IDC_SUBID);
		int SubID = box.GetCurSel();
		GetFramework()->SetAppInfo(AppID, SubID,1,1);
		if(CString(L"auto") != text)
		{
			GetFramework()->SetISAddress(ClientCommon::StrToAddress(text));
		}
		GetProcessor()->ConnectToIS();
		GetDlgItem(IDC_BUTTON4).EnableWindow(FALSE);
	}
	

	return 0;
}

PSDash::_void CLoginDlg::OnConnectIS( _bool bConnect, const PSUID is_id, _dword Reason, const _char *desp /*= _null*/ )
{
	GetDlgItem(IDC_BUTTON4).EnableWindow(TRUE);
	GetDlgItem(IDC_USERNAME).SetFocus();
	if(bConnect)
	{
		GetDlgItem(IDC_BUTTON4).SetWindowText(L"Disconnect");
		
		if(StringPtr(desp).Length()>0)
		AtlMessageBox(m_hWnd, desp);
		GetDlgItem(IDC_APPID).EnableWindow(FALSE);
		GetDlgItem(IDC_SUBID).EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1).EnableWindow(FALSE);

	}
	else
	{
		GetDlgItem(IDC_BUTTON4).SetWindowText(L"Connect");
		AtlMessageBox(m_hWnd, (LPCTSTR)GetFramework()->GetError(Reason));
	}
}

void CLoginDlg::MoveLogDlg()
{
	CRect rect;
	GetWindowRect(&rect);
	
	CRect logrect;
	GetLogWindow().GetWindowRect(&logrect);

	logrect.MoveToX(rect.right);
	GetLogWindow().MoveWindow(&logrect);

}

LRESULT CLoginDlg::OnBnShowLog( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	
	if(GetLogWindow().IsWindowVisible())
	{
		GetLogWindow().ShowWindow(SW_HIDE);
	}
	else
	{
		CRect rect;
		GetWindowRect(&rect);
		GetLogWindow().MoveWindow(&rect);
		MoveLogDlg();
		GetLogWindow().ShowWindow(SW_SHOWNA);
		
	}
	return 0;
}

LRESULT CLoginDlg::OnMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( (HWND)GetLogWindow())
	MoveLogDlg();
	return 0;
}

LRESULT CLoginDlg::OnBnLobbyServer( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	CLobbyServerdlg dlg;
	dlg.m_servers = GetFramework()->GetLobbyServers();
	dlg.DoModal(m_hWnd);

	return 0;
}

LRESULT CLoginDlg::OnBnFileServer( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	GetFramework()->GetProcessor()->QueryFSList(0);
	return 0;
}

PSDash::_void CLoginDlg::OnFSListRecv( _dword Callback,const Array<stIP_FSNode> & FsList )
{
	CFileServerDlg dlg;
	dlg.m_FsList = FsList;
	dlg.DoModal(m_hWnd);
}

PSDash::_void CLoginDlg::OnValidateResult( _dword byCode )
{
	if(byCode != AP_E_ValidateSucc)
	{
		AtlMessageBox(m_hWnd,(LPCTSTR)GetFramework()->GetError(byCode));
	}
}

PSDash::_void CLoginDlg::OnCreateSessionResult( _dword dwCode )
{
	if(GetFramework()->GetLocalPSUID().IDType() == PSUID_GameServer)
	{
		return;
	}
	
	if(dwCode != E_SP_SUCCESS)
	{
		AtlMessageBox(m_hWnd,(LPCTSTR)GetFramework()->GetError(dwCode));
	}
	else
	{
		
		BSTR Username = NULL, Password = NULL;
		GetDlgItemText(IDC_USERNAME, Username);
		GetDlgItemText(IDC_PASSWORD, Password);
		if(Username == NULL) Username = L"";
		if(Password == NULL) Password = L"";
		
		m_cfg.Set(L"Config", L"UserName",Username);
		m_cfg.Set(L"Config", L"Password",Password);
		GetLogWindow().SetParent(NULL);
		DestroyWindow();
		AtlTrace(L"[LOGINDLG] Post Quit MSG 1\n");
		::PostQuitMessage(1);
	}
}

LRESULT CLoginDlg::OnBnLogin( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	BSTR Username = NULL, Password = NULL;
	GetDlgItemText(IDC_USERNAME, Username);
	GetDlgItemText(IDC_PASSWORD, Password);
	if(Username == NULL) Username = L"";
	if(Password == NULL) Password = L"";
	GetProcessor()->Login(Username, Password);

	return 0;
}

BOOL CLoginDlg::OnIdle()
{
	
	return FALSE;
}

LRESULT CLoginDlg::OnTimer( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	if(wParam == 1)
	{
		GetFramework()->Update();
		UISetDefault(ID_LOGIN, GetFramework()->IsConnected());
		UISetDefault(IDC_BUTTON4, !GetFramework()->IsConnected());
		UIEnable(ID_LOGIN, GetFramework()->IsConnected());
		UIEnable(IDC_REGISTER, GetFramework()->IsConnected());
		UIEnable(IDC_LOBBYS, GetFramework()->IsConnected());
		UIEnable(IDC_FILESERVER, GetFramework()->IsConnected());
		UIEnable(IDC_APPID , !GetFramework()->IsConnected());
		UIEnable(IDC_SUBID , !GetFramework()->IsConnected());
		UIEnable(IDC_COMBO1 , !GetFramework()->IsConnected());
		UIUpdateChildWindows();
	}

	return 0;
}

LRESULT CLoginDlg::OnCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	return SendMessage(WM_CLOSE);
}
LRESULT CLoginDlg::OnBnClickedRegister(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CRegisterDlg dlg;
	dlg.m_RegInfo = m_RegInfo;
	int ret = dlg.DoModal(m_hWnd);
	if(ret == IDOK)
	{
		m_RegInfo = dlg.m_RegInfo;
		GetProcessor()->Register(0, dlg.m_RegInfo);

	}
	

	return 0;
}

PSDash::_void CLoginDlg::OnRegisterResult( _dword Callback, const RegisterResult & Result )
{
	
	BOOL bShowSelectUserName = FALSE, bCanceled = FALSE;
	if(Result.ErrorNum == 0)
	{
		MessageBox(GetFramework()->GetError(AP_E_Info_Succ), L"", MB_OK|MB_ICONEXCLAMATION);
		GetDlgItem(IDC_USERNAME).SetWindowText(m_RegInfo.UserName);
		GetDlgItem(IDC_PASSWORD).SetFocus();
	}
	else
	{
		CString OutMsg;
		for(_dword a=0; a<Result.ErrorNum; a++)
		{
			CString tmp;
			tmp.Format(L"%d. %s\r\n", a+1, 
				GetFramework()->GetError(Result.ErrorCode[a]));
			OutMsg +=tmp;
			if(Result.ErrorCode[a] == AP_E_Info_UserNameOccupied)
			{
				bShowSelectUserName = TRUE;
			}

		}

		if(!OutMsg.IsEmpty())
		{
			MessageBox(OutMsg, L"Error", MB_OK | MB_ICONEXCLAMATION);
		}

		if(bShowSelectUserName)
		{
			CSelUserNameDlg dlg;
			dlg.m_Result = Result;
			int ret = dlg.DoModal(m_hWnd);
			if(ret == IDOK)
			{
				STRCPYS(m_RegInfo.UserName, (LPCTSTR)dlg.m_SeledUserName);
				

			}
			else if(ret == IDCANCEL)
			{
				bCanceled = TRUE;
			}
		}
		if(!bCanceled)
			this->SendMessage(WM_COMMAND,
				MAKEWPARAM(IDC_REGISTER, BN_CLICKED), 
				(LPARAM)GetDlgItem(IDC_REGISTER).m_hWnd);
		

		
	}
}

LRESULT CLoginDlg::OnGame( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	GetProcessor()->LoginToMMS(10);
	return 0;
}

PSDash::_void CLoginDlg::OnDispatchGame( _dword GameID, _word MaxUserNum )
{
	SessionAddress saddr;
	saddr.mAddress = GetFramework()->GetPublicIP();
	saddr.mSessionID = 10;
	GetProcessor()->HandleDispatchGame(E_MP_DISPATCH_SUCCESS, GameID, MaxUserNum, (_dword *)this, saddr );
}