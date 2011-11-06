// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{

	IRenderDevice* renderdevice = GetRenderDeviceSingleton( );

	CRect rcClient;
	GetClientRect(&rcClient);
	renderdevice->ResizeDevice( rcClient.Width( ), rcClient.Height( ) );

	renderdevice->SetClearColor( Color::cDarkBlue );
	renderdevice->ClearBuffer( );

	renderdevice->Render( m_pFontTexture, Vector2( 0.0f, 0.0f ), 0, 
		L"FPS %3.2f Polygon %d Vertex %d Texture Memory %s",
		renderdevice->GetFramePerSecond( ), 
		renderdevice->GetPolygonNumber( ), 
		renderdevice->GetVertexNumber( ),
		StringFormatter::FormatBytesString( renderdevice->GetTextureMemoryUsed( ) ) );


	renderdevice->SetRenderTarget( m_pRenderTarget );

	renderdevice->Render( OverlayRectangleImage( Vector2( 10.0f, 10.0f ), Vector2( 500.0f, 500.0f ), Color( 1.0f, 1.0f, 1.0f, 0.5f ), m_pTexture3 ) );
	renderdevice->Render( PrimitiveAxis( Vector3::cOrigin, 100.0f ) );


	renderdevice->Present( );
	return TRUE;
}


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

	UIAddChildWindowContainer(m_hWnd);



	IRenderDevice* renderdevice = CreateRenderDevice( m_hWnd, IRenderDevice::_DEVICE_HARDWARE_NOVSYNC );
	renderdevice->SetLoadTextureFormat( ISurface::_FORMAT_A8L8 );

	m_pFontTexture = renderdevice->CreateFontTexture( L"Arial", 12, 0, _false, _false, _false );
	m_pFontTexture->SetFontColor( Color::cYellow );

	m_pRenderTarget	= renderdevice->CreateRenderTarget( 1024, 1024, renderdevice->GetRenderTargetFormat( ) );
	
	m_pTexture3 =  renderdevice->CreateTexture( L"Number.png" );

	renderdevice->SetCamera( Camera( Vector3( 100.0f, 100.0f, 100.0f ), Vector3::cOrigin, Vector3::cZAxis ) );
	renderdevice->SetProjectTexture( m_pRenderTarget );

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);


	GetRenderDeviceSingleton( )->ReleaseFontTexture( m_pFontTexture );
	GetRenderDeviceSingleton( )->ReleaseTexture( m_pRenderTarget );

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	//CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
