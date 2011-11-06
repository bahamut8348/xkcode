#include "StdAfx.h"
#include "ConnEvent.h"
#include "TestAdo.h"

asyncAdo::CTestAdo::CTestAdo(void):
	m_pconn(L"Adodb.connection")
{
	
}

asyncAdo::CTestAdo::~CTestAdo(void)
{
}

_ConnectionPtr & asyncAdo::CTestAdo::GetConn()
{
	return m_pconn;
}

void asyncAdo::CTestAdo::Start()
{
	DWORD  gdwCnEvents;
	_bstr_t str = _T("Driver=SQL Server;Server=127.0.0.1;Database=AdventureWorks;UID=s;PWD=sa;");
	IConnectionPointContainerPtr CCP = GetConn();
	IConnectionPointPtr cp;
	CCP->FindConnectionPoint( __uuidof(ConnectionEvents), &cp );
	HRESULT hRet = cp->Advise( new CConnEvent, &gdwCnEvents);
	ATLASSERT(cp);
	
	GetConn()->put_ConnectionTimeout(4);
	try
	{
		hRet = GetConn()->Open(str,L"", L"",adAsyncConnect);
		if(SUCCEEDED(hRet))
		{
			wcout<<L"Conn OK"<<endl;
		}
	}
	catch (_com_error & e)
	{
		_bstr_t str = e.Description();
		WriteConsole(GetStdHandle(STD_ERROR_HANDLE), (LPCTSTR)str, str.length(),NULL,0);
	}
	
}

/*
STDMETHODIMP asyncAdo::CConnEvent::InfoMessage( Error *pError, EventStatusEnum *adStatus, _Connection *pConnection )
{
	TCHAR * desc = 0;
	pError->get_Description(&desc);
	cout<<L"Event Enum:" << (int)*adStatus<<L"Desc:"<< desc<<endl;
	return S_OK;
}
*/