// OleDBTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"




int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL);

/*
	AdoConnection conn;

	TCHAR str[] = _T("Driver=SQL Server;Server=192.168.1.15;Database=psweb;UID=psweb;PWD=webserver");
	
	if(false == conn.Open(str,L"",L""))
	{
		_tprintf(L"%s\n", conn.GetLastError());
	}

*/

	_ConnectionPtr pconn;
	_bstr_t str = _T("Driver=SQL Server;Server=192.168.1.65;Database=psplayer;UID=psplayer;PWD=psplayer;");
	pconn.CreateInstance(L"Adodb.Connection");
	pconn->put_ConnectionTimeout(12);
	pconn->CommandTimeout = 20;

	try
	{

		
		if( ! FAILED(pconn->Open(str, L"",L"", adConnectUnspecified)))
		{
			_RecordsetPtr rs;
			rs.CreateInstance(L"Adodb.RecordSet");
			
			int a = 0;

			while (1)
			{
				if(!FAILED(rs->Open(L"select loginname ,id, password, nickname, email, country, city, sex, birth, sendemail, rightid, [money] from account where loginname = N'ps_ma.rui'", 
					_variant_t( (IDispatch *)pconn ,true),adOpenForwardOnly ,adLockReadOnly,adCmdText)))
				{
					
					rs->Close();
					a++;
					if(a % 1000 == 0)
					{
						SYSTEMTIME st;
						::GetLocalTime(&st);
						_tprintf(L"%d %d.%d\n",a, st.wSecond, st.wMilliseconds );
					}
					continue;
					while(! rs->adoEOF)
					{
						_bstr_t name = rs->Fields->Item[L"Loginname"]->Value;
						_bstr_t newletter = rs->Fields->Item[L"Sendemail"]->Value;
						_bstr_t data	= rs->Fields->Item[L"LastLoginTime"]->Value;

						_variant_t t;
						try
						{
							t = rs->GetCollect(L"LastLoginTimedd");
						}
						catch (_com_error   & e)
						{
							CComBSTR s;
							s.AppendBSTR(e.Description());
							TCHAR ss[1024];
							lstrcpy(ss, s);
							WriteConsole(GetStdHandle(STD_ERROR_HANDLE), ss, s.Length(),NULL,0);
							_tprintf(L"%s", ss);
							return 1;
						}

						COleDateTime date(t);
						int year = date.GetYear();
						_tprintf(L"%s\t\t-- %s\t\t-- %s\n", (TCHAR *)name, (TCHAR*)newletter, date.Format(L"%y/%m/%d %H:%M:%S"));


						rs->MoveNext();
					}
					rs->Close();
				}
				else
				{
					_tprintf(L"[-] Recordset Open Error\n" );
				}
			}

		
			rs.Release();

			pconn->Close();
		}

	}catch (_com_error   & e) 
	{
		CComBSTR s;
		s.AppendBSTR(e.Description());
		TCHAR ss[1024];
		lstrcpy(ss, s);
		WriteConsole(GetStdHandle(STD_ERROR_HANDLE), ss, s.Length(),NULL,0);
		_tprintf(L"%s", ss);
	}

	
	pconn.Release();

	return 0;
}

