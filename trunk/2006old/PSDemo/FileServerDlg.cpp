#include "StdAfx.h"
#include "FileServerDlg.h"



LRESULT CFileServerDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	DlgResize_Init(FALSE);
	// TODO: Add your message handler code here and/or call default
	CListViewCtrl list = GetDlgItem(IDC_LIST1);
	list.InsertColumn(0, L"PSUID");
	list.InsertColumn(1, L"Server Address");
	

	

	int n = 0;
	for(_dword a=0; a<m_FsList.Number(); a++)
	{
		stIP_FSNode & node = m_FsList[a];
		
		for(_dword b=0; b<IP_MaxAddrNum; b++)
		{
			if(node.Addr[b].mIP ==0) continue;
			list.InsertItem(n, ClientCommon::PSUID2Str(node.ServerID));
			list.SetItemText(n, 1, ClientCommon::Address2Str(node.Addr[b]) );
			n++;
		}
		
	}

	gResizeList(list);
	
	

	return TRUE;
}

LRESULT CFileServerDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(0);
	return 0;
}

LRESULT CFileServerDlg::OnClose2( UINT uCode, int nID, HWND hwndCtrl, BOOL& bHandled )
{
	EndDialog(0);
	return 0;
}