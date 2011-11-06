#include "StdAfx.h"
class CMainFrame; 

#include "LogEdit.h"
#include "LogWindow.h"
#include "Mytabview.h"
#include "MainFrm.h"

void CMytabview::UpdateLayout()
{
	RECT rect;
	GetClientRect(&rect);

	if(m_tab.IsWindow() && m_tab.IsWindowVisible())
		m_tab.SetWindowPos(NULL, 0, 0, rect.right - rect.left , m_cyTabHeight, SWP_NOZORDER);

	if(m_nActivePage != -1)
		::SetWindowPos(GetPageHWND(m_nActivePage), NULL, 0, m_cyTabHeight, rect.right - rect.left, rect.bottom - rect.top - m_cyTabHeight, SWP_NOZORDER);
}

bool CMytabview::CreateTabControl()
{
	bool bRet = CTabViewImpl<CMytabview>::CreateTabControl();
	
	return bRet;
}

void CMytabview::ShowTabControl( bool bShow )
{
	m_tab.ShowWindow(bShow ? SW_SHOWNOACTIVATE : SW_HIDE);
	
}

void CMytabview::OnContextMenu( int nPage, POINT pt )
{
	CMenu m;
	m.LoadMenu(IDR_TAB);
	ClientToScreen(&pt);
	CMenuHandle hm = m.GetSubMenu(0);
	CBaseView *pView =  (CBaseView *)GetPageData(nPage);
	if(pView->getType() == BV_ROOMLIST)
	{
		int ret = GetFrame()->m_CmdBar.TrackPopupMenu(hm,TPM_RETURNCMD, pt.x, pt.y);
		if(ret == ID_TAB_CLOSE)
		{
			RemovePage(nPage);
			delete pView;
		}
	}
	
}
