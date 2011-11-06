#pragma once



struct SqlColDescribe
{
	SQLTCHAR  ColName[128];
	SQLSMALLINT DataType;
	SQLUINTEGER ColSize;
	SQLSMALLINT DecimalDigits;
	SQLSMALLINT Nullable;
};


class SqlError 
{
public:
	SQLTCHAR *GetDesc();
	SQLTCHAR *GetState();

	void SetDesc(const SQLTCHAR *Msg);
	void SetState(const SQLTCHAR *State);


	SQLTCHAR ErrorMsg[1024];
	SQLTCHAR State[16];

};

class SqlHandle
{
public:
	enum HANDLE_TYPE
	{
		TYPE_ENV	=      1,
		TYPE_DBC	=      2,
		TYPE_STMT	=      3,
		TYPE_DESC	=      4,
	};


public:
	SqlHandle(HANDLE_TYPE type);
	virtual ~SqlHandle();


	bool GetError(SqlError & err);

	operator SQLHANDLE () const
	{
		return m_Handle;
	}

	bool operator !() const
	{
		return m_Handle == (SQLHANDLE)SQL_INVALID_HANDLE;
	}

	

protected:
	SQLHANDLE Handle();
	
	SQLSMALLINT m_HandleType;
	SQLHANDLE m_Handle;
};




class SqlEnv : public SqlHandle
{
public:
	SqlEnv();
	static SQLHANDLE Instance();
	static SQLHANDLE s_instance;
};

class SqlDbc : public SqlHandle
{
public:
	SqlDbc();
	bool Connect(const TCHAR *DsnName, const TCHAR *User, const TCHAR *Password);
	bool Connect( const TCHAR *ConnectString );
	bool Disconnect();
};

class SqlStmt : virtual public SqlHandle
{
public:
	SqlStmt();
	SqlStmt(SQLHANDLE odbc);
	bool Create(SQLHANDLE odbc );
	SQLINTEGER ColsNum();
	bool ColDescribe(SQLSMALLINT Idx,SqlColDescribe *pColDescribe);
	bool GetColName(SQLSMALLINT Idx, SQLTCHAR *Name, SQLSMALLINT MaxNameLen);
	SQLSMALLINT GetColIndex(SQLTCHAR *ColName);
	bool Close();

	bool AddNew( );

	bool Prepare(SQLTCHAR *SqlStr);

	bool EnableAsync(bool bEnable = true) ;

	bool ExtendedFetch(SQLUSMALLINT Orientation, SQLINTEGER Offset, SQLUINTEGER *RowCount, SQLUSMALLINT *StateArray  );
	bool GetRecordCount(SQLUINTEGER *RowCount);
	bool SetPos( SQLSMALLINT RowNumber, SQLSMALLINT Operation, SQLSMALLINT LockType );


	bool SetCursorType( ULONG CursorType);
	bool SetCursorConcurrency(ULONG ConcurrencyType);
	bool FetchScroll(SQLSMALLINT Orientation, SQLINTEGER OffSet);
	bool FetchFirst();
	bool FetchNext();
	bool FetchPrior();
	bool FetchLast();
	bool FetchAbsolute(SQLINTEGER Offset);
	bool FetchRelative(SQLINTEGER Offset);

	bool Execute();
	bool Execute(const SQLTCHAR *SqlString);
	bool Fetch();

	bool BindCol( SQLSMALLINT idx, SQLINTEGER *value, SQLLEN *out = NULL  );
	bool BindCol( SQLSMALLINT idx, SQLPOINTER* value, SQLLEN length, SQLLEN *out = NULL  );
	bool BindCol( SQLUSMALLINT ColumnNumber, SQLSMALLINT     TargetType, SQLPOINTER  TargetValuePtr, SQLLEN BufferLength, SQLLEN * StrLen_or_Ind = NULL );

	bool BindParam( SQLSMALLINT paramnum, SQLSMALLINT  valueType, SQLSMALLINT    SqlType, SQLULEN   ColumnSize, SQLPOINTER     ParameterValuePtr, SQLINTEGER     BufferLength, SQLSMALLINT     InputOutputType = SQL_PARAM_INPUT,   SQLLEN * StrLen_or_IndPtr = 0, SQLSMALLINT     DecimalDigits = 0 );
	bool BindParam( SQLSMALLINT paramnum, SQLINTEGER *value, SQLSMALLINT paramType = SQL_PARAM_INPUT, SQLLEN *out = NULL );
	bool BindParam( SQLSMALLINT paramnum, SQLPOINTER value, SQLINTEGER* length, SQLSMALLINT paramType =SQL_PARAM_INPUT, SQLLEN *out = NULL );
	bool BindBinary( SQLSMALLINT paramnum );

	bool PutData( SQLSMALLINT paramnum, void* data_buffer, SQLINTEGER datalength );


	bool MoreResult();
	
	bool GetData(SQLTCHAR * ColName, SQLSMALLINT  TargetType, SQLPOINTER TargetPtr, SQLINTEGER TargetLength, SQLINTEGER *OutLength);
	bool GetData(SQLUSMALLINT idx, SQLSMALLINT  TargetType, SQLPOINTER TargetPtr, SQLINTEGER TargetLength, SQLINTEGER *OutLength);

	bool GetData(SQLUSMALLINT idx, SQLWCHAR *StrPtr, SQLINTEGER TargetLen, SQLINTEGER *OutLen );
	bool GetData(SQLTCHAR * ColName, SQLWCHAR *StrPtr, SQLINTEGER TargetLen, SQLINTEGER *OutLen );


	bool GetData(SQLTCHAR * ColName, LONG *lOutNum, SQLINTEGER *OutLen);
	bool GetData(SQLUSMALLINT idx, LONG *lOutNum, SQLINTEGER *OutLen);

	bool GetData(SQLTCHAR * ColName, ULONG *lOutNum, SQLINTEGER *OutLen);
	bool GetData(SQLUSMALLINT idx, ULONG *lOutNum, SQLINTEGER *OutLen);

	bool GetData(SQLTCHAR * ColName, SQL_TIMESTAMP_STRUCT *TimeStamp, SQLINTEGER *OutLen);
	bool GetData(SQLUSMALLINT idx, SQL_TIMESTAMP_STRUCT *TimeStamp, SQLINTEGER *OutLen);
};


