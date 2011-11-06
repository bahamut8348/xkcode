#include "StdAfx.h"
#include "resource.h"
#include "MainDlg.h"

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	End();
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	this->DestroyWindow();
	::PostQuitMessage(0);
	
	return 0;
}

LRESULT CMainDlg::OnSysCommand( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL&  bHandled )
{
	UINT id = 0xFFF0 & wParam;
	if(id == ID_APP_ABOUT)
	{
		BOOL k=FALSE;
		OnAppAbout(0,0,0,k);
	}
	bHandled = FALSE;
	return 0;
}

void CMainDlg::Render()
{
	if(m_pD3DDevice == NULL) return;

	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	GetDevice()->BeginScene();

	// 1 draw triangle
	//DrawTriangle();

	// 2
	DrawRetangle();

	GetDevice()->EndScene();
	GetDevice()->Present(NULL,NULL, NULL, NULL);
}

void CMainDlg::InitialseVertexBuffer()
{
	void * pVertices = NULL;
	XVertex vers[] = {
		{
			250.0f,10.0f,0.5f, 1.0f,D3DCOLOR_XRGB(255,0,0),
		},
		{
			350.0f,200.0f,0.5f,1.0f,D3DCOLOR_XRGB(0,255,0),
		},
		{
			10.0f,200.0f,0.5f,1.0f,D3DCOLOR_XRGB(0,0,255),
		},
	};

	GetDevice()->CreateVertexBuffer(3*sizeof(XVertex) , 0, D3DFVF_XYZRHW|D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT, &m_pVertexBuf, NULL);
	GetVertexBuffer()->Lock(0, sizeof(vers), &pVertices, 0);
	memcpy(pVertices, vers, sizeof(vers));
	GetVertexBuffer()->Unlock();

}

void CMainDlg::DrawTriangle()
{
	GetDevice()->SetStreamSource(0, GetVertexBuffer(),0,sizeof(XVertex));
	GetDevice()->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
	GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
}

void CMainDlg::Init()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	ATLASSERT(m_pD3D);
	D3DDISPLAYMODE d3ddm;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = d3ddm.Format;

	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, & m_pD3DDevice);

	GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	InitialseVertexBuffer2();  // draw a trigger
}

void CMainDlg::InitialseVertexBuffer2()
{
	void *pVertices;
	CUSTOMVERTEX cvVertice[] = 
	{
		// Top Face
		{ -5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(0,0,255), },	//vertex 0 - blue
		{ -5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(255,0,0), },		//vertex 1 - Red
		{ 5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(255,0,0), },		//vertex 2 - Red
		{ 5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(0,255,0), },		//vertex 3 - Red

		// face 1
		{ -5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(255,0,0), },	//vertex 4 - Red
		{ -5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(0,0,255), },	//vertex 5 - Blue
		{ 5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(0,255,0), },	//vertex 6 - Green
		{ 5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(255,0,0), },		//vertex 7 - Green
		
		// face2
		{ 5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0,0,255), },		//vertex 8 - Blue
		{ 5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(0,255,0), },		//vertex 9 - Green

		// face3
		{ -5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0,255,0), },	//vertex 10 - Green
		{ -5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(255,0,0), },		//vertex 11 - Red
		
		// face4
		{ -5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0,255,0), },	//vertex 12 - Red
		{ -5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(0,0,255), },		//vertex 13 - Blue

		// Bottom Face
		{ 5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(0,255,0), },	//vertex 14 - Green
		{ 5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0,0,255), },		//vertex 15 - Blue
		{ -5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(255,0,0), },	//vertex 16 - Red
		{ -5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0,255,0), },	//vertex 17 - Green
	};

	if( FAILED(GetDevice()->CreateVertexBuffer(18*sizeof(CUSTOMVERTEX),0 , D3DFVF_XYZ|D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT, &m_pVertexBuf, NULL) ) )
	{
		return;
	}

	GetVertexBuffer()->Lock(0 , sizeof(cvVertice), &pVertices, 0 );
	
	memcpy(pVertices, cvVertice, sizeof(cvVertice));

	GetVertexBuffer()->Unlock();
}

void CMainDlg::SetupRotation()
{
	D3DXMATRIX matWorld, matWorldX, matWorldZ, matWorldY;

	// create the transformation matrices
	D3DXMatrixRotationX(&matWorldX, timeGetTime() / 400.0f);
	D3DXMatrixRotationY(&matWorldY, timeGetTime() / 400.0f);
	D3DXMatrixRotationZ(&matWorldZ, timeGetTime() / 400.0f);

	// Combine the transformations by multiplying them together
	D3DXMatrixMultiply(&matWorld, &matWorldX, &matWorldY);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldZ);

	// apply the transformation
	GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);


}

void CMainDlg::SetupCamera()
{
	// here we will setup the camera.
	// the camera has three settings: "Camera Position", "Look at Position" and
	// "Up Direction"
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0.0f, 0.0f, -30.0f),
				&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	GetDevice()->SetTransform(D3DTS_VIEW, &matView);

	

}

void CMainDlg::SetupPerspective()
{
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 500.0f);
	GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CMainDlg::DrawRetangle()
{
	SetupRotation();
	SetupCamera();
	SetupPerspective();
	// Rendering our objects
	GetDevice()->SetStreamSource(0, GetVertexBuffer(), 0,  sizeof(CUSTOMVERTEX));
	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // top
	GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 8); // sides
	GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 14, 2); // bottom

}