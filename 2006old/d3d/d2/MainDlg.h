// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

_dword g_ctrl = 0;

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		IRenderDevice* dev = GetRenderDeviceSingleton();
		if( dev == NULL ) return FALSE;
		dev->ClearBuffer();
		
		INewGUIManager* mgr = GetNewGUIManagerSingleton();
		if( mgr )
		{
			mgr->SetControlPosition( g_ctrl, Point( mgr->GetControlPosition(g_ctrl).x+1,  mgr->GetControlPosition(g_ctrl).y + 1 ) );
			if ( mgr->GetControlPosition(g_ctrl).y > 5 ) mgr->SetControlPosition( g_ctrl, Point( mgr->GetControlPosition(g_ctrl).x+1,  mgr->GetControlPosition(g_ctrl).y -5 ) );
			if ( mgr->GetControlPosition(g_ctrl).x > 5 ) mgr->SetControlPosition( g_ctrl, Point( 0,  mgr->GetControlPosition(g_ctrl).y -5 ) );

			AtlTrace(L"Position: %d, %d\r\n", mgr->GetControlPosition(g_ctrl).y,mgr->GetControlPosition(g_ctrl).x );
			mgr->Update( 4 );
			mgr->Render();
		}

		GetNewGUIManagerSingleton()->Update( 4 );
		GetNewGUIManagerSingleton()->Render();

		dev->Present();
		
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		

		CreateRenderDevice( m_hWnd, 1 );
		CreateNewGUIManager(m_hWnd);

		GetNewGUIManagerSingleton()->CreateControlGroup( L"test" );

		GUIFontRender fr;
		fr.mFontName=L"Arial";
		fr.mFontColor = 10000;
		fr.mSize = 50;
		fr.mAlign = 0x20;
		GUIRectRender rr;
		rr.mFillColor = 0xFFeeeeee;
		rr.mLineColor = 0xFFeeeeee;
		rr.mLineWidth = 2;
		g_ctrl = GetNewGUIManagerSingleton()->CreateControl(L"sdf", INewGUIManager::_TYPE_MULTILINEEDIT, Point(0,0), Point(600,30),L"test");
		GetNewGUIManagerSingleton()->SetControlRender(g_ctrl, 0 ,rr );
		GetNewGUIManagerSingleton( )->SetControlGroupCurrentCamera( GetNewGUIManagerSingleton()->GetCurrentControlGroup(), GetRenderDeviceSingleton()->GetCamera( ) );
		GetNewGUIManagerSingleton()->BringControlToTop( g_ctrl );

		GetNewGUIManagerSingleton()->Update( 1000 );

		return FALSE;
	}
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL bHandled)
	{
		bHandled = TRUE;
		OnIdle();
		return 1;
	}
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};
