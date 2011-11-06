#include "StdAfx.h"
#include "ConnEvent.h"
#include "common.h"


STDMETHODIMP CConnEvent::raw_InfoMessage( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_InfoMessage", pError, *adStatus);
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_ConnectComplete( struct Error * pError, EventStatusEnum * adStatus, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_ConnectComplete", pError, *adStatus);
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_WillConnect( BSTR * ConnectionString, BSTR * UserID, BSTR * Password, long * Options, EventStatusEnum * adStatus, struct _Connection *pConnection )
{
	asyncAdo::Common::info(L"raw_WillConnect", NULL, *adStatus);
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_BeginTransComplete( LONG TransactionLevel, struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_BeginTransComplete", pError, *adStatus);
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_CommitTransComplete( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_CommitTransComplete", pError, *adStatus);
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_RollbackTransComplete( struct Error * pError, EventStatusEnum * adStatus, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_RollbackTransComplete", pError, *adStatus);
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_WillExecute( BSTR * Source, CursorTypeEnum * CursorType, LockTypeEnum * LockType, long * Options, EventStatusEnum * adStatus, struct _Command * pCommand, struct _Recordset * pRecordset, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_WillExecute", NULL, *adStatus);
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_ExecuteComplete( LONG RecordsAffected, struct Error * pError, EventStatusEnum * adStatus, struct _Command * pCommand, struct _Recordset * pRecordset, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_ExecuteComplete", NULL, *adStatus);

	return S_OK;
}

STDMETHODIMP CConnEvent::raw_Disconnect( EventStatusEnum * adStatus, struct _Connection * pConnection )
{
	asyncAdo::Common::info(L"raw_Disconnect", NULL, *adStatus);
	return S_OK;
}