#include "StdAfx.h"
#include "LogEdit.h"



void  ShowLineNum( HWND hEdit )
{
	/*
	功能：显示文本的总行数
	参数：        
	hEdit：要显示行号的文本框，普通的Edit控件没有测试过，这里只用RichEdit

	返回值：没有。
	--------------------------------------------------------------------------------
	*/
	                    //RichEdit的客户区大小
	int                        CharHeight;                        //字符的高度
	int                        chHeight;                        //字符的高度，常量        
	int                        FirstLine;                        //文本框中的第一个可见行的行号。
	int                        ClientHeight;                         //RichEdit的客户区高度        
	int                        LineCount;                        //文本的总行数        
	TCHAR                        countBuf[10];                        //显示行号的缓冲区
	CHARFORMAT                CharFmt;                        //RichEdit中的一个结构，用于获取字符的一系列信息，这里只用它来获取字符高度

	//获取RichEdit的Dc
	CDC dcEdit = ::GetDC(hEdit);
	CRect ClientRect;
	::GetClientRect(hEdit, &ClientRect);
	//获取RichEdit的客户区高度
	ClientHeight = ClientRect.Height();
	//创建与RichEdit兼容的Dc

	CDC dcCpb;
	dcCpb.CreateCompatibleDC( dcEdit );
	//创建与RichEdit兼容的位图Dc，我们用来显示行号用的。
	CBitmap bmp;
		bmp.CreateCompatibleBitmap( dcEdit, 40, ClientHeight );
	//将位图dc选入RichEdit环境中
	dcCpb.SelectBitmap(bmp);
	dcCpb.SelectFont(AtlGetDefaultGuiFont());
	//填充显示行号dc的背景颜色。大家可以试试其它颜色
	dcCpb.FillRect(&ClientRect, CreateSolidBrush(RGB(255,255,255)) );
	dcCpb.SetBkMode(TRANSPARENT );
	//获取当前RichEdit文本中的总行数
	LineCount = SendMessage( hEdit, EM_GETLINECOUNT, 0, 0 );

	RtlZeroMemory( &CharFmt, sizeof(CharFmt) );
	CharFmt.cbSize = sizeof( CharFmt );
	SendMessage( hEdit, EM_GETCHARFORMAT, TRUE, (long)&CharFmt );
	//获取字符高度，以英寸为单位，需转化为磅，只要除以20就得到磅尺寸。
	CharHeight = CharFmt.yHeight / 20;  
	chHeight = CharHeight;
	CharHeight = 1;
	//设置显示行号的前景色
	dcCpb.SetTextColor(0x000000 );
	//获取文本框中第一个可见的行的行号，没有这个行号，显示不会跟着文本的滚动而滚动。
	FirstLine = ::SendMessage( hEdit, EM_GETFIRSTVISIBLELINE, 0, 0 );
	FirstLine++;
	//在位图dc中循环输出行号
	
	int line = 0;
	while( FirstLine <= LineCount )
	{
		CRect rcText(0, CharHeight , 20,CharHeight + chHeight);
		//dcCpb.DrawText(0, CharHeight, countBuf, wsprintf( countBuf, L"%2u", FirstLine++ ));
		//dcCpb.DrawText()
		CharHeight += chHeight + 4;
		rcText.MoveToY(CharHeight);
		dcCpb.DrawEdge(&rcText,BDR_SUNKENINNER,BF_BOTTOM );
		FirstLine++;
		if( CharHeight > ClientHeight ) break;
	}
	//将已"画好"的位图真正"贴"到RichEdit中
	dcEdit.BitBlt(0, 0, 20, ClientHeight, dcCpb, 0, 0, SRCCOPY );
	

}



PSDash::_void CLogEdit::log( const _char* Format, ... )
{

	_char buffer[ 2048 ];
	SYSTEMTIME st;
	GetLocalTime(&st);
	//_dword len = wsprintf( buffer,L"[%02d-%02d-%02d %02d:%02d:%02d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	int len = 0;
	_vsnwprintf( buffer + len, 2048, Format, (_chara*)( &Format ) + sizeof( Format ) );
	CString s(buffer);
	if(_tcsstr( buffer,_T("SEND")) != NULL)
	{
		s.Replace(L"[SEND] ",L"");
		s.Replace(L"SEND ",L"");

		AddLog(GetColor(buffer),_T("[SEND] %s\r\n") ,s );
	}
	else if(_tcsstr(buffer,_T("RECV")) != NULL)
	{
		s.Replace(L"[RECV] ",L"");
		s.Replace(L"RECV ",L"");
		AddLog(GetColor(buffer),_T("[RECV] %s\r\n") ,s );
	}
	else
	{
		s.Replace(L"[INFO] ",L"");
		s.Replace(L"INFO ",L"");
		AddLog(GetColor(buffer),_T("[INFO] %s\r\n") ,s );
	}



}

_dword CLogEdit::GetColor( const _char *c )
{
	for(int i=0; i<ColorNum; i++)
	{
		if(pcmap[i].test(c))
			return pcmap[i].Color;
	}
	m_F.Open(L"Client.Log", File::_CREATE_ALWAYS, File::_OPERATION_WRITE);
	return 0;
}

CLogEdit::CLogEdit()
{
	ColorMap cmap[] =  {
		L"RECV",RGB(0,128,0),
		L"SEND",RGB(0,0,255),
		L"INFO",RGB(0,0,0),
		L"ERROR",RGB(255,0,0)
	};
	ColorNum = sizeof(cmap)/sizeof(ColorMap);
	pcmap = new ColorMap[ColorNum];
	Memory::MemCpy(pcmap, cmap, sizeof(cmap));
}

CLogEdit::~CLogEdit()
{

	delete pcmap;
}
_void CLogEdit::AddLog( _dword type, const _char* Format, ... )
{

	
	_char buffer[ 2048 ];
	SYSTEMTIME st;
	GetLocalTime(&st);
	_dword len = wsprintf( buffer,L"[%02d-%02d-%02d %02d:%02d:%02d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	_vsnwprintf( buffer + len, 2048, Format, (_chara*)( &Format ) + sizeof( Format ) );

	AtlTrace(L"[CLIENT] %s", buffer);
	
	m_F.WriteString(buffer);
	m_F.Flush();


	len = GetTextLength();
	int Count = GetLineCount()-1;
	SetSel(len, len);
	ReplaceSel(buffer);
	CHARFORMAT2 cf;
	GetSelectionCharFormat(cf);
	_tcscpy(cf.szFaceName,L"Fixedsys");
	SetSel(len-Count, GetTextLength());
	//SetSel(1,5);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = type;
	cf.dwEffects = 0;
	SetSelectionCharFormat(cf);
	SetSel(0, 0);
	SendMessage(WM_VSCROLL,SB_BOTTOM,0);
}
LRESULT CLogEdit::OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	u_int x = GET_X_LPARAM(lParam);
	u_int y = GET_Y_LPARAM(lParam);

	CMenu m1 = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_LOGMENU));
	CMenuHandle m = m1.GetSubMenu(0);
	if(m)
	{
		int ret = TrackPopupMenu(m,TPM_RETURNCMD,x,y, 0, m_hWnd,NULL );
		switch(ret)
		{
		case ID_LOGPOP_COPY:
			Copy();
			break;
		case ID_LOGPOP_COPYALL:
			SetSel(0,-1);
			Copy();
			SetSel(0,0);
			break;
		case ID_LOGPOP_CLEAR:
			SetWindowText(L"");
			break;
		}
	}
	::DeleteObject(m1);
	return 0;
}

LRESULT CLogEdit::OnPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	PAINTSTRUCT                ps;
	bHandled = FALSE;
	ShowLineNum(m_hWnd);
	return 0;
	
	::BeginPaint( m_hWnd, &ps);
	return ::EndPaint( m_hWnd, &ps);
}

LRESULT CLogEdit::OnInitDlg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	SendMessage(EM_SETMARGINS,EC_LEFTMARGIN,MAKEWPARAM(20,0));
	bHandled = FALSE;
	return TRUE;
}