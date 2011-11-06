#include "StdAfx.h"
#include "ServerStatus.h"


BOOL CServerStatus::OnIdle()
{
	return FALSE;
}

BOOL CServerStatus::PreTranslateMessage(MSG* pMsg)
{
	if (CFrameWindowImpl<CServerStatus>::PreTranslateMessage(pMsg))
		return TRUE;

	return FALSE;
}

LRESULT CServerStatus::OnCreate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	
	m_LastUpdate = 0.0;

	m_hwndProc = NULL;

	CRect rc;
	GetParent().GetClientRect(&rc);
	
	DWORD CmdLen = 1024;
	Utils::GetProcessCommandLine(m_ProcInfo.PID, m_CmdLine.GetBuffer(1024),&CmdLen);
	m_CmdLine.ReleaseBuffer();

	ServiceChecker::Instance()->GetServerNameByCommandLine(m_CmdLine, m_ServiceName.GetBuffer(1024));
	m_ServiceName.ReleaseBuffer();

	m_hwndProc = Utils::GetProcessHWND(m_ProcInfo.PID);

	::GetWindowText(m_hwndProc, m_Title.GetBuffer(1024),1024);
	m_Title.ReleaseBuffer();

	m_pCpu = new CPUsage(m_ProcInfo.PID);

	m_vSplit.Create(m_hWnd, rc,NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_vSplit.m_cxyMin = 100;
	m_vSplit.SetSplitterPos(150);
	m_hWndClient = m_vSplit;


	// right
	m_hSplit.Create(m_vSplit, rcDefault,NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_vSplit.SetSplitterPane(1, m_hSplit);
	m_hSplit.SetSplitterPos(150);
	m_vSplit.SetSinglePaneMode(SPLIT_PANE_RIGHT);
	
	

	m_PanelListen.Create(m_hSplit,L"Opened Port");

	m_PanelConnection.Create(m_hSplit,L"Connections");

	m_hSplit.SetSplitterPanes(m_PanelListen, m_PanelConnection);


	// Listen list
	m_ListenList.Create(m_PanelListen, rcDefault, NULL,WS_HSCROLL| WS_VISIBLE | WS_CHILDWINDOW 
		| LVS_REPORT |LVS_SINGLESEL |WS_CLIPCHILDREN);
	m_ListenList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES );
	m_PanelListen.SetClient(m_ListenList);
	
	struct ColumnHeader
	{
		LPTSTR name;
		DWORD width;
	} headers[] = {L"Type",100, L"Local Address", 200, L"Connection Count", 200};

	for(DWORD a=0; a<(sizeof(headers)/sizeof(headers[0])); a++)
	{
		m_ListenList.InsertColumn(a, headers[a].name, LVCFMT_LEFT, headers[a].width);
	}

	m_ConnectionList.Create(m_PanelConnection, 0, NULL,WS_HSCROLL| WS_VISIBLE | WS_CHILDWINDOW 
		| LVS_REPORT |LVS_SINGLESEL |WS_CLIPCHILDREN);
	m_ConnectionList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES );
	m_PanelConnection.SetClient(m_ConnectionList);
	
	
	
	ColumnHeader headers2[] = {L"Type",40, L"Direction", 80, L"Local Address", 150, L"Remote Address", 150,L"Status", 100};

	for(DWORD a=0; a<(sizeof(headers2)/sizeof(headers2[0])); a++)
	{
		m_ConnectionList.InsertColumn(a, headers2[a].name, LVCFMT_LEFT, headers2[a].width);
	}
	
	ListenInfo *pi = new ListenInfo;
	pi->ePortType = (PORTTYPE)4;
	m_Listens.push_back(pi);

	SendMessage(WM_TIMER,1);
	SetTimer(1,300,NULL);

	

	return 0;
}

LRESULT CServerStatus::OnForwardMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	return this->PreTranslateMessage((MSG*)lParam);

}

void CServerStatus::RefreshUdp( const vector<MIB_UDPROW_OWNER_PID> & intable )
{
	return ;
	vector<MIB_UDPROW_OWNER_PID> table = intable;
	for(DWORD a=0; a<m_Listens.size(); a++ )
	{
		
		ListenInfo  & info = *(m_Listens[a]);
		if( info.ePortType == PORTTYPE_UDP )
		{
			BOOL bFound = FALSE;
			for(DWORD b=0; b<table.size(); b++)
			{
				const MIB_UDPROW_OWNER_PID & row = table[b];
				if( (row.dwOwningPid == m_ProcInfo.PID) && (row.dwLocalAddr==info.dwLocalAddr)  && (row.dwLocalPort == info.dwLocalPort) )
				{
					table.erase(table.begin()+b);
					bFound = TRUE;
				}
			}
			if(bFound == FALSE)
			{
				// delete
				DeleteItem(info);
				delete &info;
				m_Listens.erase( m_Listens.begin() + a);
				--a;
			}
		}
		
	}

	for(DWORD b=0; b<table.size(); b++)
	{
		const MIB_UDPROW_OWNER_PID & row = table[b];
		if( row.dwOwningPid == m_ProcInfo.PID )
		{
			ListenInfo *pinfo = new ListenInfo;
			pinfo->ePortType = PORTTYPE_UDP;
			pinfo->dwLocalAddr = row.dwLocalAddr;
			pinfo->dwLocalPort = row.dwLocalPort;
			m_Listens.push_back(pinfo);
			AddItem(*pinfo);
		}
	}



		
	
}

void CServerStatus::RefreshTcp( const vector<MIB_TCPROW_OWNER_PID> & intable )
{
	
	time_t t;
	m_LastUpdate = (DOUBLE) time(&t);
	vector<MIB_TCPROW_OWNER_PID> table = intable;

	// make map
	stdext::hash_map<int,int> portmap;
	std::vector<MIB_TCPROW_OWNER_PID> listenlist;
	CONNMAP & connmap = m_ConnMap;
	m_ConnMap.clear();
	

#define  MAKEKEY() swprintf(addrbuf,L"%d:%d-%d:%d",row.dwLocalAddr, row.dwLocalPort,row.dwRemoteAddr, row.dwRemotePort);

	
	

	
	for( vector<MIB_TCPROW_OWNER_PID>::const_iterator Iter = intable.begin(); Iter!=intable.end(); Iter++ )
	{
		const MIB_TCPROW_OWNER_PID & row = *Iter;
		if(row.dwOwningPid == m_ProcInfo.PID)
		{
			if(row.dwState == 5)
			{
				stdext::hash_map<int,int>::iterator it =  portmap.find(row.dwLocalPort);
				if(it == portmap.end())
				{
					// cant find
					portmap[row.dwLocalPort] = 1;
				}
				else
				{
					++portmap[row.dwLocalPort];
				}
			}
			
			if(row.dwState == 2)
			{
				listenlist.push_back(row);
			}
			else
			{
				connmap.insert(&row);
				MIB_TCPROW_OWNER_PID r = row;
				r.dwLocalAddr = 9;
				CONNMAP::iterator IT = connmap.find(&r);
				int i = 0;
				i++;
			}
		}
	}
	
	


	for(DWORD a=0; a<m_Listens.size();a++)
	{
		
		ListenInfo & info = *(m_Listens[a]);
		if( info.ePortType == PORTTYPE_TCP )
		{
			BOOL bFound = FALSE;
			for(DWORD b=0; b<listenlist.size(); b++)
			{
				const MIB_TCPROW_OWNER_PID & row = listenlist[b];
				if( (row.dwLocalAddr == info.dwLocalAddr)
					&&(row.dwLocalPort == info.dwLocalPort)
					)
				{
					listenlist.erase(listenlist.begin()+b);
					bFound = TRUE;

					// Count
					DWORD Count = portmap[info.dwLocalPort];
					if(Count != info.dwCount)
					{
						info.dwCount = Count;
						UpdateItem(info);
					}

				}
			}

			if(bFound == FALSE)
			{
				DeleteItem(info);
				m_Listens.erase( m_Listens.begin() + a);
				delete &info;
				--a;
			}
		}
		

	}


	for(std::list<ConnectionInfo *>::iterator Iter=m_Connections.begin(); Iter!=m_Connections.end(); )
	{
		ConnectionInfo  & info = *(*Iter);
		if( info.ePortType == PORTTYPE_TCP )
		{
			CONNMAP::iterator it = connmap.find((PMIB_TCPROW_OWNER_PID)&info);
			if(it == connmap.end())
			{
				DeleteItem(info);
				Iter = m_Connections.erase( Iter);
				delete &info;
				continue;
			}

			else
			{
				if( info.dwState != (*it)->dwState)
					info.dwState = (*it)->dwState;
				connmap.erase(it);
			}
		}

		Iter++;
		
	}


	for(DWORD a=0; a<listenlist.size(); a++)
	{
		const MIB_TCPROW_OWNER_PID & row = listenlist[a];

		ListenInfo *pInfo = new ListenInfo;
		pInfo->dwCount = 0;
		pInfo->ePortType = PORTTYPE_TCP;
		pInfo->dwLocalAddr = row.dwLocalAddr;
		pInfo->dwLocalPort = row.dwLocalPort;
		m_Listens.push_back(pInfo);
		AddItem(*pInfo);
		
	}
	
	for (CONNMAP::iterator Iter = connmap.begin(); Iter!= connmap.end(); Iter++)
	{
		const MIB_TCPROW_OWNER_PID & row = *(*(Iter));
		ConnectionInfo *pInfo = new ConnectionInfo;
		pInfo->ePortType = PORTTYPE_TCP;
		pInfo->eConnDirect = CONNDIRECT_OUT;
		if(pInfo->dwLocalAddr == 0)
		{
			pInfo->eConnDirect = CONNDIRECT_IN;
		}
		pInfo->dwLocalAddr = row.dwLocalAddr;
		pInfo->dwLocalPort = row.dwLocalPort;
		pInfo->dwRemoteAddr = row.dwRemoteAddr;
		pInfo->dwRemotePort = row.dwRemotePort;
		pInfo->dwState = row.dwState;
		pInfo->Desp = 0;
		m_Connections.push_back(pInfo);
		AddItem(*pInfo);		

	}
}

void CServerStatus::DeleteItem( ConnectionInfo & Info )
{
	static LVFINDINFO fi;
	ZeroMemory(&fi,sizeof fi);
	fi.flags = LVFI_PARAM | LVFI_PARTIAL;
	fi.lParam = (LPARAM) &Info;
	
	int n= m_ConnectionList.FindItem( &fi ,-1 );
	m_ConnectionList.DeleteItem(n);
}

void CServerStatus::DeleteItem( ListenInfo & Info )
{
	static LVFINDINFO fi;
	ZeroMemory(&fi,sizeof fi);
	fi.flags = LVFI_PARAM | LVFI_PARTIAL;
	fi.lParam = (LPARAM) &Info;

	int n= m_ListenList.FindItem( &fi ,-1 );
	m_ListenList.DeleteItem(n);
}

void CServerStatus::AddItem( ConnectionInfo & Info )
{
	LV_ITEM it;
	ZeroMemory(&it, sizeof(it));
	it.mask = LVIF_IMAGE|LVIF_PARAM|LVIF_TEXT;
	it.pszText = LPSTR_TEXTCALLBACK;
	it.lParam =(LPARAM) &Info;
	it.iImage = I_IMAGECALLBACK;
	it.iItem = m_ConnectionList.GetItemCount();
	m_ConnectionList.InsertItem(&it);
}

void CServerStatus::AddItem( ListenInfo & Info )
{
	LV_ITEM it = {0};
	it.mask = LVIF_IMAGE|LVIF_PARAM|LVIF_TEXT;
	it.cchTextMax = MAX_PATH;
	it.pszText = LPSTR_TEXTCALLBACK;
	it.lParam =(LPARAM) &Info;
	it.iImage = I_IMAGECALLBACK;
	it.iItem = m_ConnectionList.GetItemCount();
	m_ListenList.InsertItem(&it);
}



LRESULT CServerStatus::OnLVGetDispInfo( int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	int a = ::GetCurrentThreadId();
	
	NMLVDISPINFO *pdi = (NMLVDISPINFO*)pnmh;
	LVITEM & item = pdi->item;
	static CString s;
	if(pdi->hdr.hwndFrom == m_ListenList.m_hWnd)
	{
		ListenInfo *pInfo =(ListenInfo *) item.lParam;
		switch (item.iSubItem)
		{
		case 0:
			{
				if(pInfo->ePortType == PORTTYPE_TCP)
				{
					item.pszText = L"TCP";
				}
				else if(pInfo->ePortType == PORTTYPE_UDP)
				{
					item.pszText = L"UDP";
				}
				break;
			}
		case 1:
			{
				wcsncpy_s(item.pszText,item.cchTextMax,(LPCTSTR)Utils::MakeAddr(pInfo->dwLocalAddr, pInfo->dwLocalPort),item.cchTextMax);
				break;
			}
		case 2:
			{
				static CString s;
				s.Format(L"%d", pInfo->dwCount);
				wcsncpy_s(item.pszText,item.cchTextMax,(LPCTSTR)s, item.cchTextMax);
				break;
			}
		}
	}
	else if(pdi->hdr.hwndFrom == m_ConnectionList.m_hWnd)
	{
		ConnectionInfo *pInfo =(ConnectionInfo *) item.lParam;
		switch (item.iSubItem)
		{
		case 0:
			{
				if(pInfo->ePortType == PORTTYPE_TCP)
				{
					item.pszText = L"TCP";
				}
				else if(pInfo->ePortType == PORTTYPE_UDP)
				{
					item.pszText = L"UDP";
				}
				break;
			}
		case 1:
			{
				break;
			}
		case 2:
			{
				
				wcsncpy_s(item.pszText,item.cchTextMax,(LPCTSTR)Utils::MakeAddr(pInfo->dwLocalAddr, pInfo->dwLocalPort),item.cchTextMax);
				break;
			}
		case 3:
			{
				wcsncpy_s(item.pszText,item.cchTextMax,(LPCTSTR)Utils::MakeAddr(pInfo->dwRemoteAddr, pInfo->dwRemotePort),item.cchTextMax);
				break;
			}
		case 4:
			{
				wcsncpy_s(item.pszText,item.cchTextMax, (LPCTSTR)Utils::ConnStatus(pInfo->dwState) ,item.cchTextMax);
			}

		}
	}

	return 0;
}

void CServerStatus::UpdateItem( ListenInfo & Info )
{
	LV_ITEM it = {0};
	it.lParam = (LPARAM)&Info;
	it.mask = LVIF_PARAM;
	m_ListenList.GetItem(&it);
	m_ListenList.Update(it.iItem);
}

LRESULT CServerStatus::OnTimer( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	// get cup
	m_CPUUsage = m_pCpu->GetUsage();
	m_MemUsage = m_pCpu->GetMemUsage();
	m_VMemUsage = m_pCpu->GetVMemUsage();
	return 0;	
}