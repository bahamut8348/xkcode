#include "StdAfx.h"

#include "ServerStatus.h"
#include "ServerSummary.h"
#include "resource.h"

#define CPUAGESTR L"%2.06f"

BOOL CServerSummary::PreTranslateMessage( MSG* pMsg )
{
	if (CFrameWindowImpl<CServerSummary>::PreTranslateMessage(pMsg))
		return TRUE;

	return FALSE;
}

BOOL CServerSummary::OnIdle()
{
	return FALSE;
}

LRESULT CServerSummary::OnCreate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	

	CImageList il;
	il.Create(16,16, ILC_COLOR32, 5,5);
	il.AddIcon(::AtlLoadIcon(IDI_RUNAPP));
	il.AddIcon(::AtlLoadIcon(IDI_UP));
	il.AddIcon(::AtlLoadIcon(IDI_DOWN));

	
	m_ServerList.Create(m_hWnd, rcDefault, NULL,WS_HSCROLL| WS_VISIBLE | WS_CHILDWINDOW 
		| LVS_REPORT |LVS_SINGLESEL |WS_CLIPCHILDREN|LVS_SHOWSELALWAYS );
	m_hWndClient = m_ServerList;
	m_ServerList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES );



	m_ServerList.SetImageList(il.m_hImageList, LVSIL_SMALL);
	
	struct ColumnHeader
	{
		LPTSTR name;
		DWORD width;
		DWORD pos;
	} headers[] = {	L"Name",150,LVCFMT_LEFT,
					L"PID", 80, LVCFMT_RIGHT,
					L"CPU", 80, LVCFMT_RIGHT,
					L"Memory", 100, LVCFMT_RIGHT,
					L"Virtual Memory", 100, LVCFMT_RIGHT,
					L"Version", 100, LVCFMT_LEFT,
					L"Command Line", 300, LVCFMT_LEFT };

	for(DWORD a=0; a<(sizeof(headers)/sizeof(headers[0])); a++)
	{
		m_ServerList.InsertColumn(a, headers[a].name, headers[a].pos, headers[a].width);
	}
	
	SetTimer(1,500,NULL);

	return 0;
}

void CServerSummary::AddServer( const CServerStatus *pServer )
{
	LV_ITEM item;
	ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE|LVIF_IMAGE;
	item.lParam = (LPARAM)pServer;
	item.iImage = I_IMAGECALLBACK;
	item.pszText= LPSTR_TEXTCALLBACK;
	item.cchTextMax = MAX_PATH;
	m_ServerList.InsertItem(&item);
	//int n=m_ServerList.InsertItem(0,L"sdfdf",0);
	//m_ServerList.SetItemData(n, (LPARAM)pServer);
}

LRESULT CServerSummary::OnLVGetDispInfo( int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	

	NMLVDISPINFO *pdi = (NMLVDISPINFO*)pnmh;
	LVITEM & item = pdi->item;
	static CString s;
	static CServerStatus *pServer;
	pServer = (CServerStatus *)item.lParam;
	static TCHAR tstr[24];
	if(pdi->hdr.hwndFrom == m_ServerList.m_hWnd)
	{
		switch (item.iSubItem)
		{
		case 0:
			{
				//item.mask = LVIF_TEXT | LVIF_IMAGE;
				item.iImage = 0;
				wcsncpy_s(item.pszText,item.cchTextMax, pServer->m_ProcInfo.Name ,item.cchTextMax);
				break;
			}
		case 1:
			{
				_itow_s(pServer->m_ProcInfo.PID, item.pszText, item.cchTextMax, 10);
				break;
			}
		case 2:
			{
				// CPU Usage
				break;
			}
		case 3:
			{
				// Memory Usage
				break;
			}
		case 4:
			{
				// virtual usage
				_itow_s(pServer->m_VMemUsage, item.pszText, item.cchTextMax, 10);
				break;
			}
		
		case 5:
			{
				wcsncpy_s(item.pszText,item.cchTextMax, pServer->m_ProcInfo.FileVersion ,item.cchTextMax);
				break;
			}

		case 6:
			{
				item.pszText =(LPTSTR) (LPCTSTR)pServer->m_CmdLine;
				//wcsncpy_s(item.pszText,item.cchTextMax, pServer->m_CmdLine ,item.cchTextMax);
				break;
			}

		}
	}
	
	return 1;
}

void CServerSummary::DelServer( const CServerStatus *pServer )
{
	static LVFINDINFO fi;
	ZeroMemory(&fi,sizeof fi);
	fi.flags = LVFI_PARAM | LVFI_PARTIAL;
	fi.lParam = (LPARAM) pServer;

	int n= m_ServerList.FindItem( &fi ,-1 );
	m_ServerList.DeleteItem(n);
}

LRESULT CServerSummary::OnTimer( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{

	for(int a=0; a<m_ServerList.GetItemCount(); a++)
	{
		CServerStatus *pServer = (CServerStatus *) m_ServerList.GetItemData(a);
		static TCHAR memstr[24];
		static TCHAR tstr[24];
		
		bool need = false;
		int UseImage = -1;
	
		LVITEM litem={0};
		litem.mask = LVIF_IMAGE;
		litem.iSubItem =3;
		litem.iItem = a;
		m_ServerList.GetItem(&litem);
		m_ServerList.GetItemText(a, litem.iSubItem, memstr,24);
		int memusage = _wtoi(memstr);
		int cha = pServer->m_MemUsage - memusage;
		litem.mask = 0;
		// = 0 
		if( cha != 0 )
		{
			need = true;
			_itow_s(pServer->m_MemUsage, memstr, 24, 10);
			litem.mask |= LVIF_TEXT;
			litem.cchTextMax = 24;
			litem.pszText = memstr;
			if(cha > 0)
			{
				UseImage = 1;
			}
			else
			{
				UseImage = 2;
			}
		}
		
		if(litem.iImage != UseImage)
		{
			litem.iImage = UseImage;
			litem.mask |= LVIF_IMAGE;
			need = TRUE;
		}
		

		if(need)
		{
			m_ServerList.SetItem(&litem);
		}
		

		



		static TCHAR cpustr[24];
		m_ServerList.GetItemText(a, 2, cpustr, 24);
		swprintf_s(tstr, 24, CPUAGESTR, pServer->m_CPUUsage);
		if( _tcscmp(tstr, cpustr ) != 0)
		{
			m_ServerList.SetItemText(a,2, tstr);
			need = true;
		}


	}
	return 0;
}

LRESULT CServerSummary::OnDbClick( int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	if(pnmh->hwndFrom == m_ServerList.m_hWnd)
	{
		int idx = m_ServerList.GetSelectedIndex();
		if(idx>-1)
		{
			ViewType *pView = (ViewType *) (CServerStatus *)m_ServerList.GetItemData(idx);
			SendMessage(GetParent().GetParent(), WM_TOFRAME, WF_SETACTIVEPAGE,(LPARAM)pView);
			
		}
	}
	return 1;
}
 
LRESULT CServerSummary::OnContextMenu( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
	int idx = m_ServerList.GetSelectedIndex();
	if(idx == -1) return 1;
	DWORD xPos = GET_X_LPARAM(lParam); 
	DWORD yPos = GET_Y_LPARAM(lParam); 


	CServerStatus *pServer =  (CServerStatus *)m_ServerList.GetItemData(idx);

	CMenu m;
	m.LoadMenu(IDR_MENU1);
	int ret = m.GetSubMenu(0).TrackPopupMenu(TPM_NONOTIFY | TPM_RETURNCMD ,xPos, yPos, m_hWnd );
	if(ret == ID_FORSERVERITEM_ACTIVE)
	{
		::ShowWindow(pServer->m_hwndProc, SW_SHOW);
		::SetForegroundWindow(pServer->m_hwndProc);
		::SetActiveWindow(pServer->m_hwndProc);
		
	}
	else if(ret == ID_FORSERVERITEM_HIDE)
	{
		::ShowWindow(pServer->m_hwndProc, SW_HIDE);
	}
	else if(ret == ID_FORSERVERITEM_SHOW)
	{
		::ShowWindow(pServer->m_hwndProc, SW_SHOW);
	}
	else if(ret == ID_FORSERVERITEM_HIDEALL)
	{
		for(int a =0; a < m_ServerList.GetItemCount(); a++)
		{
			::ShowWindow(((CServerStatus *)m_ServerList.GetItemData(a))->m_hwndProc, SW_HIDE);
		}
	}else if(ret == ID_FORSERVERITEM_STOP )
	{
		Utils::KillProcess(pServer->m_ProcInfo.PID);
	}
	else if(ret == ID_FORSERVERITEM_RESTART)
	{
		Utils::KillProcess(pServer->m_ProcInfo.PID);
		if(pServer->m_ServiceName.IsEmpty())
		{
			Utils::StartProcess(pServer->m_CmdLine,pServer->m_Title);
		}
		else
		{
			Utils::StartService(pServer->m_ServiceName);
		}
	}
	

	return 0;
}