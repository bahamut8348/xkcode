// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


struct XVertex
{
	float x,y,z,rhw;
	DWORD color;
};


struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD colour;
};



class CMainDlg : public CDialogImpl<CMainDlg>,
			public CIdleHandler, public CMessageFilter
{
public:
	enum { IDD = IDD_MAINDLG };



	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG,	OnInitDialog)
		MESSAGE_HANDLER(WM_SYSCOMMAND,	OnSysCommand)
		COMMAND_ID_HANDLER(ID_APP_ABOUT,OnAppAbout)
		MESSAGE_HANDLER(WM_DESTROY,		OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE,		OnClose)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	
	
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}


	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuf;


	LPDIRECT3DDEVICE9 GetDevice() const
	{
		return m_pD3DDevice;
	}

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() const
	{
		return m_pVertexBuf;
	}

	void Init();

	void InitialseVertexBuffer();
	void InitialseVertexBuffer2(); //draw a rectangle
	void Render();
	void DrawTriangle();
	void DrawRetangle();


	void SetupRotation();
	void SetupCamera();
	void SetupPerspective();

	void End()
	{
		if(GetDevice())
		{
			GetDevice()->Release();
			m_pD3DDevice = NULL;
		}
		m_pD3D->Release();
		if(GetVertexBuffer())
			GetVertexBuffer()->Release();
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();
		Init();
		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);


		CMenu m =  GetSystemMenu(FALSE);
		m.InsertMenu(-1, MF_SEPARATOR);
		m.InsertMenu(-1, MF_STRING ,ID_APP_ABOUT,L"¹ØÓÚ(&A)...");

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}



	virtual BOOL OnIdle()
	{
		
		Render();
		return FALSE;
	}
public:
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
