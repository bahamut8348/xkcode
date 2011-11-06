#include "StdAfx.h"
#include "logedit.h"
#include "LogWindow.h"


LRESULT CLogWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	
	const DWORD dwRichEditStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		WS_HSCROLL | WS_VSCROLL | WS_BORDER |
		ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE;	
	//m_logedit.Create(*this, CWindow::rcDefault, NULL,dwRichEditStyle);
	m_hWndClient = m_logedit.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL, WS_EX_CLIENTEDGE);
	m_logedit.SetFont(AtlGetDefaultGuiFont());
	int a= GetLastError();
	SetMsgHandled(FALSE);
	return 0;
}

LRESULT CLogWindow::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	SetMsgHandled(FALSE);
	return 0;
}

LRESULT CLogWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	return 0;
}

BOOL CLogWindow::PreTranslateMessage( MSG* pMsg )
{
	return CFrameWindowImpl<CLogWindow>::PreTranslateMessage(pMsg);
}

BOOL CLogWindow::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}
LRESULT CLogWindow::OnNcCalcSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS *)lParam;
	//params->rgrc[0].top -= 18;
	
	bHandled = FALSE;
	return 0;
}

LRESULT CLogWindow::OnNcPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	//CDC hdc;
	//hdc = GetDCEx((HRGN)wParam, DCX_WINDOW|DCX_INTERSECTRGN);
	//// Paint into this DC
	//hdc.Draw3dRect(0,0,2,100,0,9);
	//ReleaseDC(hdc);	
	bHandled = FALSE;
	return 0;
}

LRESULT CLogWindow::OnOther( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

LRESULT CLogWindow::OnClose( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	ShowWindow(SW_HIDE);
	return 0;
}