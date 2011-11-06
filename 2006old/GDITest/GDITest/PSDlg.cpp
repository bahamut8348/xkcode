// PSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GDITest.h"
#include "PSDlg.h"


// CPSDlg dialog

IMPLEMENT_DYNAMIC(CPSDlg, CDialog)

CPSDlg::CPSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPSDlg::IDD, pParent)
{
	bNCLButtonDown = FALSE;
	bMouseInClose  = FALSE;
}

CPSDlg::CPSDlg( DWORD DlgID, CWnd* pParent /*= NULL*/ ) : CDialog(DlgID, pParent)
{
	bNCLButtonDown = FALSE;
	bMouseInClose  = FALSE;
	bMouseInMin		= FALSE;
	bBtnClosePressed = FALSE;
	bBtnMinPressed = FALSE;


}


CPSDlg::~CPSDlg()
{
}

void CPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPSDlg, CDialog)
	ON_WM_NCPAINT()
	//ON_WM_MOVE()
	ON_WM_NCHITTEST()
	ON_WM_NCACTIVATE()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	ON_WM_SIZING()
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_INITMENU()
END_MESSAGE_MAP()


// CPSDlg message handlers

void CPSDlg::OnNcPaint()
{
	RenderFrame(1);
}

int CPSDlg::RenderFrame(unsigned int _rgn)
{
	
	CDC* pDC = GetWindowDC(); // window temp dc
	if(pDC==NULL)
		return 1;

	


	CRect rcWindow; // window screen rect

	//取得窗口大小
	CRect rcClient;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	rcWindow.SetRect(0, 0, rcWindow.Width(), rcWindow.Height());

	CRgn winrgn;
	winrgn.CreateRectRgn(0,0,rcWindow.Width(), rcWindow.Height());
	CRgn clientrgn;
	clientrgn.CreateRectRgn(cFrameWidth,m_ImgLeftTop.GetHeight(),
		rcWindow.Width()-cFrameWidth, rcWindow.Height()-cFrameWidth);

	::CombineRgn(winrgn, winrgn, clientrgn, RGN_DIFF);
	pDC->SelectObject(winrgn);
	

	


	pDC->SetBkMode( TRANSPARENT );


	CDC WinMemDC;
	WinMemDC.CreateCompatibleDC(pDC);
	CBitmap WinBmp;
	WinBmp.CreateCompatibleBitmap(pDC, rcWindow.Width(), rcWindow.Height());
	WinMemDC.SelectObject(&WinBmp);

	
	
	// left top
	WinMemDC.BitBlt(0,0,m_ImgLeftTop.GetWidth(), m_ImgLeftTop.GetHeight(),
		&m_LeftTopInGameMemDC,0,0,SRCCOPY);
	// right top
	WinMemDC.BitBlt(rcWindow.Width()-m_ImgRightTop.GetWidth(), 0, m_ImgRightTop.GetWidth(),
		m_ImgRightTop.GetHeight(), &m_RightTopInGameMemDC, 0, 0, SRCCOPY);
	// title bar
	WinMemDC.StretchBlt(m_ImgLeftTop.GetWidth(), 0, rcWindow.Width()-m_ImgLeftTop.GetWidth()-m_ImgRightTop.GetWidth(),
		m_ImgLeftTop.GetHeight(), &m_TitleBarMemDC, 0, 0, 1, m_ImgLeftTop.GetHeight(), SRCCOPY);

	
	// left frame
	WinMemDC.StretchBlt(0, 0, cFrameWidth, rcWindow.Height(), &m_LeftBottomMemDC, 0,0,1,1, SRCCOPY);
	// right frame
	WinMemDC.StretchBlt(rcWindow.Width()-cFrameWidth, 0, cFrameWidth, rcWindow.Height(),&m_LeftBottomMemDC,0,0,1,1,SRCCOPY);
	// bottom frame
	WinMemDC.StretchBlt(0, rcWindow.Height()-cFrameWidth, rcWindow.Width(), cFrameWidth, &m_LeftBottomMemDC,0,0,1,1,SRCCOPY);
	
	
	
	// 系统按钮
	DrawCloseBtn(&WinMemDC);
	DrawMinBtn(&WinMemDC);
	DrawTitle(&WinMemDC);

	pDC->BitBlt(0,0,rcWindow.Width(), rcWindow.Height(), &WinMemDC, 0,0,SRCCOPY);

	ReleaseDC(pDC);



	return 0;
}

void CPSDlg::UpdateRegion()
{
	CRect Rect;
	GetWindowRect(&Rect);

	CRgn rgn;
	rgn.CreateRectRgn(0,0, Rect.Width(), Rect.Height());
	::CombineRgn(rgn, rgn, m_CornerRgnLT, RGN_DIFF);

	m_CornerRgnRT.OffsetRgn(Rect.Width()- m_ImgRightTop.GetWidth(), 0);
	::CombineRgn(rgn, rgn, m_CornerRgnRT, RGN_DIFF);
	SetWindowRgn(rgn, 1);
	m_CornerRgnRT.OffsetRgn(-(Rect.Width()-m_ImgRightTop.GetWidth()), 0);

}



BOOL CPSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CDC *pDC = GetDC();
	
	m_ImgLeftTop.LoadFromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_MASKLEFTTOP));
	m_ImgRightTop.LoadFromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_TOPRIGHT));
	
	
	
	int cornerLH = m_ImgLeftTop.GetHeight();
	int cornerLW = m_ImgLeftTop.GetWidth();
	m_CornerRgnLT.CreateRectRgn(0,0,cornerLW, cornerLH);
	HRGN rgn2 = BitmapRegion((HBITMAP)m_ImgLeftTop, 0x00FF00FF, 1);
	::CombineRgn(m_CornerRgnLT,m_CornerRgnLT, rgn2, RGN_DIFF);
	::DeleteObject(rgn2);

	int cornerRH = m_ImgRightTop.GetHeight();
	int cornerRW = m_ImgRightTop.GetWidth();
	m_CornerRgnRT.CreateRectRgn(0,0,cornerRW, cornerRH);
	rgn2 = BitmapRegion((HBITMAP)m_ImgRightTop, 0x00FF00FF,1);
	::CombineRgn(m_CornerRgnRT, m_CornerRgnRT, rgn2, RGN_XOR);
	::DeleteObject(rgn2);
	

	CImage Img;
	Img.LoadFromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_TOPRIGHT));

	m_TitleBarMemDC.CreateCompatibleDC(pDC);
	::SelectObject(m_TitleBarMemDC, (HBITMAP)Img);

	m_LeftTopInGameMemDC.CreateCompatibleDC(pDC);
	::SelectObject(m_LeftTopInGameMemDC.m_hDC, (HBITMAP)m_ImgLeftTop);
	
	m_RightTopInGameMemDC.CreateCompatibleDC(pDC);
	::SelectObject(m_RightTopInGameMemDC.m_hDC, (HBITMAP)m_ImgRightTop);

	m_ImgLBCorner.LoadFromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LBCORNER));
	m_LeftBottomMemDC.CreateCompatibleDC(pDC);
	::SelectObject(m_LeftBottomMemDC.m_hDC, (HBITMAP)m_ImgLBCorner);

	m_ImgBtnClose.LoadFromResource(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BTNCLOSE));
	m_BtnCloseMemDC.CreateCompatibleDC(pDC);
	::SelectObject(m_BtnCloseMemDC.m_hDC, (HBITMAP)m_ImgBtnClose);
	
	m_ImgBtnMin.LoadFromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BTNMIN));
	m_BtnMinMemDC.CreateCompatibleDC(pDC);
	::SelectObject(m_BtnMinMemDC.m_hDC, (HBITMAP)m_ImgBtnMin);


	UpdateRegion();	
	
	
	ReleaseDC(pDC);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



LRESULT CPSDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	RenderFrame(1);
	

	

	LRESULT n =  CDialog::OnNcHitTest(point);
	
	switch(n)
	{
	case HTCLOSE:
	case HTMAXBUTTON:
	case HTSYSMENU:
	case HTMINBUTTON:
	//case HTREDUCE:
	//case HTZOOM:
		n = HTCAPTION;
		break;

	}
	
	
	return n;
}

BOOL CPSDlg::OnNcActivate(BOOL bActive)
{
	// TODO: Add your message handler code here and/or call default
	
	RenderFrame(1);
	
	//CDialog::OnNcActivate(bActive);
	

	
	
	return 1;
}

void CPSDlg::OnPaint()
{
	// CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	PAINTSTRUCT ps;
	CDC *pDc  =BeginPaint(&ps);
	HBRUSH hBrush = CreateSolidBrush(0x0026201F);
	::FillRect(pDc->m_hDC, &ps.rcPaint,hBrush);
	DeleteObject(hBrush);
	EndPaint(&ps);
	
	//CDialog::OnPaint();
}

void CPSDlg::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp )
{
	int mSystemCaptionHeight	= GetSystemMetrics(SM_CYCAPTION);
	int mSystemFrameSizeX		= GetSystemMetrics(SM_CXSIZEFRAME);
	int mSystemFrameSizeY		= GetSystemMetrics(SM_CYSIZEFRAME);
	
	lpncsp->rgrc[0].top -= mSystemCaptionHeight + mSystemFrameSizeY - cCornerHeight-1;
	lpncsp->rgrc[0].bottom	+= mSystemFrameSizeY - cFrameWidth-1;
	lpncsp->rgrc[0].left	-= mSystemFrameSizeX - cFrameWidth-1;
	lpncsp->rgrc[0].right	+= mSystemFrameSizeX - cFrameWidth-1;
	
	
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}

BOOL CPSDlg::HaveMinBtn()
{
	BOOL ret = FALSE;
	CMenu *pm = GetSystemMenu(FALSE);
	MENUITEMINFO Info;
	for(u_int a=0; a<pm->GetMenuItemCount(); a++)
	{
		Info.cbSize = sizeof Info;
		Info.fMask = MIIM_ID;
		ret = pm->GetMenuItemInfo(a, &Info, 1);
		if(Info.wID == SC_MINIMIZE) return 1;
		
	}

	return FALSE;
}

void CPSDlg::DrawCloseBtn(CDC *pDC)
{

	GetBtnCloseRect();
	
	if(bMouseInClose)
	{
		if(bNCLButtonDown)
		{
			pDC->BitBlt(m_rcCloseBtn.left, m_rcCloseBtn.top, m_rcCloseBtn.Width(),
				m_rcCloseBtn.Height(), &m_BtnCloseMemDC, 0,48,SRCCOPY);
		}
		else
		{
			pDC->BitBlt(m_rcCloseBtn.left, m_rcCloseBtn.top, m_rcCloseBtn.Width(),
				m_rcCloseBtn.Height(), &m_BtnCloseMemDC, 0,24,SRCCOPY);
		}
	}
	else
	{
		pDC->BitBlt(m_rcCloseBtn.left, m_rcCloseBtn.top, m_rcCloseBtn.Width(),
			m_rcCloseBtn.Height(), &m_BtnCloseMemDC, 0,0,SRCCOPY);
	}
	
	
}

void CPSDlg::DrawMinBtn(CDC *pDC)
{
	if(!HaveMinBtn()) return;	
	GetBtnMinRect();

	if(bMouseInMin)
	{
		if(bNCLButtonDown)
		{
			pDC->BitBlt(m_rcMinBtn.left, m_rcMinBtn.top, m_rcMinBtn.Width(),
				m_rcMinBtn.Height(), &m_BtnMinMemDC, 0,48,SRCCOPY);
		}
		else
		{
			pDC->BitBlt(m_rcMinBtn.left, m_rcMinBtn.top, m_rcMinBtn.Width(),
				m_rcMinBtn.Height(), &m_BtnMinMemDC, 0,24,SRCCOPY);
		}
	}
	else
	{
		pDC->BitBlt(m_rcMinBtn.left, m_rcMinBtn.top, m_rcMinBtn.Width(),
			m_rcMinBtn.Height(), &m_BtnMinMemDC, 0,0,SRCCOPY);
	}


}

void CPSDlg::OnNcLButtonDown( UINT nHitTest,CPoint point )
{
	bNCLButtonDown = TRUE;
	RenderFrame(0);
	if(IsMouseInClose(point))
	{
		SetCapture();
		bBtnClosePressed = TRUE;
		
	}
	else if(IsMouseInMin(point))
	{
		SetCapture();
		bBtnMinPressed = TRUE;
		
	}
	else
	{
		CDialog::OnNcLButtonDown(nHitTest, point);
	}
	
	
	
	IsMouseInMin(point);
	
	bNCLButtonDown = FALSE;
	

}

void CPSDlg::OnNcLButtonUp( UINT nHitTest,CPoint point )
{
	bNCLButtonDown = FALSE;
	
	CDialog::OnNcLButtonUp(nHitTest, point);
}



void CPSDlg::OnNcMouseMove( UINT nHitTest,CPoint point )
{
	CPoint pt = point;
	ScreenToClient(&pt);
	pt.x = pt.x+cFrameWidth;
	pt.y += m_ImgLeftTop.GetHeight();
	if(m_rcCloseBtn.PtInRect(pt))
	{
		bMouseInClose = TRUE;
	}
	else
	{
		bMouseInClose = FALSE;
	}

	if(IsMouseInMin(point))
	{
		bMouseInMin = TRUE;
	}
	else
	{
		bMouseInMin = FALSE;
	}

	
	CDialog::OnNcMouseMove(nHitTest, point);
}

CRect CPSDlg::GetBtnCloseRect()
{
	CRect rcWin;
	GetWindowRect(&rcWin);

	m_rcCloseBtn.top = cBtnTopOffset;
	m_rcCloseBtn.left = rcWin.Width()-cFrameWidth*2-cBtnWidth-cBtnRightOffset;
	m_rcCloseBtn.bottom = cBtnTopOffset + cBtnHeight;
	m_rcCloseBtn.right = m_rcCloseBtn.left + cBtnWidth;
	m_rcMinBtn = m_rcCloseBtn;
	m_rcMinBtn.MoveToX(m_rcCloseBtn.left-cBtnWidth-cBtnRightOffset);
	return m_rcCloseBtn;
}

CRect CPSDlg::GetBtnMinRect()
{

	GetBtnCloseRect();
	return m_rcMinBtn;

}


BOOL CPSDlg::IsMouseInClose( POINT pt )
{
	ScreenToClient(&pt);
	pt.x = pt.x+cFrameWidth;
	pt.y += m_ImgLeftTop.GetHeight();
	return m_rcCloseBtn.PtInRect(pt);

}
BOOL CPSDlg::IsMouseInMin( POINT pt )
{
	ScreenToClient(&pt);
	pt.x = pt.x+cFrameWidth;
	pt.y += m_ImgLeftTop.GetHeight();
	return m_rcMinBtn.PtInRect(pt);
	
}

void CPSDlg::OnSizing( UINT nSide, LPRECT lpRect )
{
	CDialog::OnSizing(nSide, lpRect);
	UpdateRegion();
}

void CPSDlg::OnMove( int x, int y )
{
	
	CDialog::OnMove(x,y);
}

void CPSDlg::OnMoving( UINT nSide,LPRECT lpRect )
{	
	POINT pt;
	pt.x = lpRect->left;
	pt.y = lpRect->top;
	RenderFrame(0);
	CDialog::OnMoving(nSide, lpRect);
	
}

void CPSDlg::OnLButtonUp( UINT nFlags,CPoint point )
{
	CDialog::OnLButtonUp(nFlags, point);
	
	ClientToScreen(&point);
	if(IsMouseInClose(point) && bBtnClosePressed)
	{
		EndDialog(0);
	}
	if(IsMouseInMin(point) && bBtnMinPressed)
	{
	
		ShowWindow(SW_MINIMIZE);
		
	}
	if(bBtnMinPressed || bBtnClosePressed)
	{
		ReleaseCapture();
		
	}
	bBtnClosePressed = FALSE;
	bBtnMinPressed = FALSE;
	

	

}

BOOL CPSDlg::OnEraseBkgnd( CDC* pDC )
{
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CPSDlg::WindowProc( UINT message,WPARAM wParam, LPARAM lParam )
{

	
	if (message   == 0x02A2)   
		return   0; 
	else if(message  == WM_SETCURSOR || message  == WM_NCLBUTTONDOWN ||
		message   ==   WM_NCLBUTTONDBLCLK)   
	{   
		ModifyStyle(WS_SYSMENU, 0);   
		LRESULT   lRet   =   CDialog::WindowProc(message,   wParam,   lParam);   
		ModifyStyle(0,   WS_SYSMENU);   
		return   lRet;   
	}   
	return   CDialog::WindowProc(message,   wParam,   lParam);   


}

void CPSDlg::OnInitMenu( CMenu* pMenu )
{
	CDialog::OnInitMenu(pMenu);   
	SendMessage(WM_NCPAINT);   

}

void CPSDlg::DrawTitle( CDC *pDC )
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(0x00E59A92);
	CFont font;
	//font.CreatePointFont(120, L"Verdana");
	font.CreateFont(22,10,0,0,800,0,0,0,0,0,0,0,0,L"黑体");
	pDC->SelectObject(&font);
	pDC->TextOut(cTitleTextTopOffset, cTitleTextLeftOffset, L"PSClientDemo");
	
}