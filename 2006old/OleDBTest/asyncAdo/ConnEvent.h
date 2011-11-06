#pragma once

class CConnEvent : public ConnectionEventsVt
{
public :


	ULONG m_ulRefCount;
	CConnEvent():m_ulRefCount(1){}


	STDMETHOD(QueryInterface)(REFIID iid, LPVOID * ppvObject)
	{
		if (IsEqualIID(__uuidof(IUnknown), iid) ||
			IsEqualIID(__uuidof(ConnectionEventsVt), iid))
		{
			*ppvObject = this;
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}


	STDMETHOD_(ULONG, AddRef)()
	{
		return m_ulRefCount++;
	}


	STDMETHOD_(ULONG, Release)()
	{
		if (--m_ulRefCount == 0)
		{
			delete this;
			return 0;
		}
		else
			return m_ulRefCount;
	}


	STDMETHOD(raw_InfoMessage)(
		struct Error * pError,
		enum EventStatusEnum * adStatus,
		struct _Connection * pConnection);


	STDMETHOD(raw_BeginTransComplete)(
		LONG TransactionLevel,
		struct Error * pError,
		enum EventStatusEnum * adStatus,
		struct _Connection * pConnection);


	STDMETHOD(raw_CommitTransComplete)(
		struct Error * pError,
		enum EventStatusEnum * adStatus,
		struct _Connection * pConnection);


	STDMETHOD(raw_RollbackTransComplete)(
		struct Error * pError,
		EventStatusEnum * adStatus,
		struct _Connection * pConnection);


	STDMETHOD(raw_WillExecute)(
		BSTR * Source,
		CursorTypeEnum * CursorType,
		LockTypeEnum * LockType,
		long * Options,
		EventStatusEnum * adStatus,
		struct _Command * pCommand,
		struct _Recordset * pRecordset,
		struct _Connection * pConnection);


	STDMETHOD(raw_ExecuteComplete)(
		LONG RecordsAffected,
		struct Error * pError,
		EventStatusEnum * adStatus,
		struct _Command * pCommand,
		struct _Recordset * pRecordset,
		struct _Connection * pConnection);


	STDMETHOD(raw_WillConnect)(
		BSTR * ConnectionString,
		BSTR * UserID,
		BSTR * Password,
		long * Options,
		EventStatusEnum * adStatus,
		struct _Connection  *pConnection);


	STDMETHOD(raw_ConnectComplete)(
		struct Error * pError,
		EventStatusEnum * adStatus,
		struct _Connection * pConnection);


	STDMETHOD(raw_Disconnect)(
		EventStatusEnum * adStatus,
		struct _Connection * pConnection);

};
