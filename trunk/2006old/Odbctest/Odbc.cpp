#include "StdAfx.h"
#include <assert.h>

#include <windows.h>
//#include <sql.h>
#include <sqlext.h>
#include "Odbc.h"



SqlHandle::~SqlHandle()
{
	SQLFreeHandle((SQLSMALLINT)m_HandleType,  Handle());
}

SQLHANDLE SqlHandle::Handle()
{
	return m_Handle;
}

SqlHandle::SqlHandle( HANDLE_TYPE type ) : m_Handle((SQLHANDLE)SQL_INVALID_HANDLE)
{
	m_HandleType = type;
}

bool SqlHandle::GetError(SqlError & err)
{
	SQLSMALLINT ErrorLen = sizeof(err.ErrorMsg)/sizeof(SQLTCHAR);
	SQLINTEGER StateLen = sizeof(err.State)/sizeof(SQLTCHAR);
	return SQL_SUCCEEDED(SQLGetDiagRec(m_HandleType, Handle(), 1, err.State, &StateLen, err.ErrorMsg, ErrorLen, &ErrorLen ));
}

bool SqlStmt::Create( SQLHANDLE odbc )
{
	return SQL_SUCCEEDED(SQLAllocHandle(m_HandleType, odbc, &m_Handle));
}

SqlStmt::SqlStmt() : SqlHandle(SqlHandle::TYPE_STMT)
{
	
}

SqlStmt::SqlStmt( SQLHANDLE dbc ):SqlHandle(SqlHandle::TYPE_STMT)
{
	Create(dbc);
}
bool SqlStmt::Fetch()
{
	return SQL_SUCCEEDED(SQLFetch(Handle()));
}

bool SqlStmt::GetData( SQLUSMALLINT idx, SQLSMALLINT TargetType, SQLPOINTER TargetPtr, SQLINTEGER TargetLength, SQLINTEGER *OutLength )
{
	return SQL_SUCCEEDED( SQLGetData(Handle(), idx, TargetType, TargetPtr, TargetLength,OutLength) );
}

bool SqlStmt::GetData( SQLUSMALLINT idx, SQLWCHAR *StrPtr, SQLINTEGER TargetLen, SQLINTEGER *OutLen )
{
	return GetData(idx, SQL_WCHAR, StrPtr, TargetLen, OutLen);
}

bool SqlStmt::GetData( SQLUSMALLINT idx, LONG *lOutNum, SQLINTEGER *OutLen )
{
	return GetData(idx, SQL_C_LONG, lOutNum, sizeof(LONG),OutLen); 
}

bool SqlStmt::GetData( SQLUSMALLINT idx, ULONG *lOutNum, SQLINTEGER *OutLen )
{
	return GetData(idx, SQL_C_ULONG, lOutNum, sizeof(ULONG),OutLen);
}

bool SqlStmt::GetData( SQLTCHAR * ColName, SQLSMALLINT TargetType, SQLPOINTER TargetPtr, SQLINTEGER TargetLength, SQLINTEGER *OutLength )
{
	return GetData( GetColIndex(ColName), TargetType, TargetPtr, TargetLength, OutLength);
}

bool SqlStmt::GetData( SQLTCHAR * ColName, SQLWCHAR *StrPtr, SQLINTEGER TargetLen, SQLINTEGER *OutLen )
{
	return GetData( GetColIndex(ColName), StrPtr, TargetLen, OutLen);
}

bool SqlStmt::GetData( SQLTCHAR * ColName, LONG *lOutNum, SQLINTEGER *OutLen )
{
	return GetData( GetColIndex(ColName), lOutNum, OutLen);
}

bool SqlStmt::GetData( SQLTCHAR * ColName, ULONG *lOutNum, SQLINTEGER *OutLen )
{
	return GetData( GetColIndex(ColName), lOutNum, OutLen );
}

bool SqlStmt::GetData( SQLUSMALLINT idx, SQL_TIMESTAMP_STRUCT *TimeStamp, SQLINTEGER *OutLen )
{
	return GetData( idx, SQL_C_TIMESTAMP, TimeStamp, sizeof(SQL_TIMESTAMP_STRUCT), OutLen);
}

bool SqlStmt::GetData( SQLTCHAR * ColName, SQL_TIMESTAMP_STRUCT *TimeStamp, SQLINTEGER *OutLen )
{
	return GetData( GetColIndex(ColName), TimeStamp, OutLen);
}

bool SqlStmt::Execute( const TCHAR *SqlString )
{
	return SQL_SUCCEEDED( SQLExecDirect(Handle(), (SQLTCHAR*)SqlString, SQL_NTS) );	
}

bool SqlStmt::Execute()
{
	return SQL_SUCCEEDED( SQLExecute(Handle()));
}
bool SqlStmt::Close()
{
	return SQL_SUCCEEDED(SQLCloseCursor(Handle()));	
}

bool SqlStmt::SetCursorType( ULONG CursorType )
{
	return SQL_SUCCEEDED(SQLSetStmtAttr(Handle(), SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)CursorType, SQL_IS_INTEGER));
}

SQLINTEGER SqlStmt::ColsNum()
{
	SQLSMALLINT i = 0;
	if( SQL_SUCCEEDED( SQLNumResultCols(Handle(), &i)) )
	{
		return i;
	}
	return -1;
}

bool SqlStmt::FetchScroll( SQLSMALLINT Orientation, SQLINTEGER OffSet )
{
	return SQL_SUCCEEDED( SQLFetchScroll(Handle(), Orientation, OffSet) );
}

bool SqlStmt::FetchFirst()
{
	return FetchScroll(SQL_FETCH_FIRST,0);
}

bool SqlStmt::FetchNext()
{
	return FetchScroll(SQL_FETCH_NEXT, 0);
}
bool SqlStmt::FetchPrior()
{
	return FetchScroll(SQL_FETCH_PRIOR, 0);
}

bool SqlStmt::FetchLast()
{
	return FetchScroll(SQL_FETCH_LAST, 0);
}

bool SqlStmt::FetchAbsolute( SQLINTEGER Offset )
{
	return FetchScroll(SQL_FETCH_ABSOLUTE, Offset);
}

bool SqlStmt::FetchRelative( SQLINTEGER Offset )
{
	return FetchScroll(SQL_FETCH_RELATIVE, Offset);
}

bool SqlStmt::ColDescribe( SQLSMALLINT Idx,SqlColDescribe *pColDescribe )
{
	SQLSMALLINT NameLen = 0;
	return SQL_SUCCEEDED( SQLDescribeCol(Handle(), Idx, pColDescribe->ColName, sizeof(pColDescribe->ColName)/sizeof(SQLTCHAR), &NameLen, &pColDescribe->DataType, &pColDescribe->ColSize, &pColDescribe->DecimalDigits, &pColDescribe->Nullable) );
}

bool SqlStmt::GetColName( SQLSMALLINT Idx, SQLTCHAR *Name, SQLSMALLINT MaxNameLen )
{
	SQLSMALLINT NameLen = 0;
	return SQL_SUCCEEDED( SQLDescribeCol(Handle(), Idx, Name, MaxNameLen, &NameLen, NULL, NULL, NULL,NULL ) );

}

SQLSMALLINT SqlStmt::GetColIndex( SQLTCHAR *ColName )
{
	SQLSMALLINT ret= 0;
	SQLINTEGER i = ColsNum();
	SQLTCHAR Name[128];
	for(SQLSMALLINT a=1; a<=i; a++)
	{
		GetColName(a, Name, 128);
		if( _tcscmp(ColName, Name) == 0)
		{
			ret = a;
			break;
		}
	}
	return ret;
}

bool SqlStmt::ExtendedFetch( SQLUSMALLINT Orientation, SQLINTEGER Offset, SQLUINTEGER *RowCount, SQLUSMALLINT *StateArray )
{
	return SQL_SUCCEEDED( SQLExtendedFetch(Handle(), Orientation, Offset, RowCount, StateArray) );
}

bool  SqlStmt::GetRecordCount(SQLUINTEGER *RowCount)
{
	return false;
	//FetchFirst();
	//return SQL_SUCCEEDED(ExtendedFetch(SQL_FETCH_LAST,0, RowCount,NULL));
}

bool SqlStmt::MoreResult()
{
	return SQL_SUCCEEDED(SQLMoreResults(Handle()));
}

bool SqlStmt::Prepare( SQLTCHAR *SqlStr )
{
	return SQL_SUCCEEDED( ::SQLPrepare( Handle(), SqlStr, SQL_NTS ) );
}

bool SqlStmt::SetCursorConcurrency( ULONG ConcurrencyType )
{
	return SQL_SUCCEEDED( SQLSetStmtAttr(Handle(),  SQL_ATTR_CONCURRENCY, (SQLPOINTER) ConcurrencyType, SQL_IS_INTEGER ) );	
}

bool SqlStmt::EnableAsync( bool bEnable /*= true*/ )
{
	if(bEnable)
	{
		return SQL_SUCCEEDED( SQLSetStmtOption(Handle(), SQL_ASYNC_ENABLE, SQL_ASYNC_ENABLE_ON) );
	}
	else
	{
		return SQL_SUCCEEDED( SQLSetStmtOption(Handle(), SQL_ASYNC_ENABLE, SQL_ASYNC_ENABLE_OFF) );
	}
	
}

bool SqlStmt::SetPos( SQLSMALLINT RowNumber, SQLSMALLINT Operation, SQLSMALLINT LockType )
{
	return SQL_SUCCEEDED( SQLSetPos(Handle(), RowNumber, Operation, LockType ) );
}

bool SqlStmt::AddNew()
{
	return SQL_SUCCEEDED( SQLBulkOperations ( Handle(), SQL_ADD) );
}

bool SqlStmt::BindCol( SQLSMALLINT idx, SQLINTEGER *value, SQLLEN * out )
{
	return BindCol( idx ,SQL_C_SLONG, value, sizeof( SQLINTEGER), out ) ;
}

bool SqlStmt::BindCol( SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr, SQLLEN BufferLength, SQLLEN * StrLen_or_Ind /*= NULL */ )
{
	SQLLEN out =  SQL_NTS;
	bool ret= SQL_SUCCEEDED( ::SQLBindCol( Handle() ,ColumnNumber+1 ,TargetType, TargetValuePtr, BufferLength, &out ) );
	if (StrLen_or_Ind)
	{
		*StrLen_or_Ind = out;
	}
	return ret;
}

bool SqlStmt::BindCol( SQLSMALLINT idx, SQLPOINTER* value, SQLLEN length, SQLLEN *out /*= NULL */ )
{
	return  BindCol( idx ,SQL_C_BINARY, value, length, out ) ;
}

bool SqlStmt::BindParam( SQLSMALLINT paramnum, SQLSMALLINT valueType, SQLSMALLINT SqlType, SQLULEN ColumnSize, SQLPOINTER ParameterValuePtr, SQLINTEGER BufferLength, SQLSMALLINT InputOutputType /*= SQL_PARAM_INPUT*/, SQLLEN * StrLen_or_IndPtr /*= 0*/, SQLSMALLINT DecimalDigits /*= 0 */ )
{
	SQLLEN outlen = 0;
	if( StrLen_or_IndPtr == 0 ) StrLen_or_IndPtr = &outlen;
	bool ret= SQL_SUCCEEDED( ::SQLBindParameter( Handle(),  paramnum+1, InputOutputType,valueType, SqlType, ColumnSize,DecimalDigits, ParameterValuePtr, BufferLength, StrLen_or_IndPtr  ) );	
	if( StrLen_or_IndPtr ) *StrLen_or_IndPtr = outlen;
	return ret;
}

bool SqlStmt::BindParam( SQLSMALLINT paramnum, SQLINTEGER *value, SQLSMALLINT paramType, SQLLEN *out /*= NULL */ )
{
	return BindParam( paramnum, SQL_C_SLONG,  SQL_INTEGER, sizeof(SQLINTEGER), value, sizeof(SQLINTEGER), paramType, out );

}



bool SqlStmt::BindParam( SQLSMALLINT paramnum, SQLPOINTER value, SQLINTEGER * length, SQLSMALLINT paramType, SQLLEN *out /*= NULL */ )
{
	return  SQL_SUCCEEDED( ::SQLBindParameter( m_Handle,  paramnum+1, 1 , SQL_C_BINARY, SQL_BINARY, *length,0,(SQLPOINTER)value, 0,length ) );	
}

bool SqlStmt::BindBinary( SQLSMALLINT paramnum )
{
	SQLLEN outlen = SQL_LEN_DATA_AT_EXEC(0);
	return SQL_SUCCEEDED( ::SQLBindParameter( Handle(),paramnum+1 , SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, 0, 0, (SQLPOINTER)paramnum, 0, &outlen) );

}

bool SqlStmt::PutData( SQLSMALLINT paramnum, void* data_buffer, SQLINTEGER datalength )
{
	SQLRETURN retcode;
	SQLPOINTER     pToken;
	retcode = SQLParamData( Handle(), &pToken);
	if ((SQLINTEGER)pToken == paramnum)
	{
		return SQL_SUCCEEDED( ::SQLPutData( Handle(), data_buffer, datalength ) );
	}

	return false;

}


SQLHANDLE SqlEnv::s_instance = SQL_NULL_HANDLE;

SqlEnv::SqlEnv():SqlHandle(SqlHandle::TYPE_ENV)
{
	SQLAllocHandle(m_HandleType, SQL_NULL_HANDLE, &m_Handle);
	SQLSetEnvAttr(Handle(), SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

	s_instance = *this;
}

SQLHANDLE SqlEnv::Instance()
{
	return SqlEnv::s_instance;
}

SqlDbc::SqlDbc():SqlHandle(SqlHandle::TYPE_DBC)
{
	assert(SqlEnv::Instance());
	SQLAllocHandle(m_HandleType, SqlEnv::Instance(), &m_Handle);
}

bool SqlDbc::Connect( const TCHAR *DsnName, const TCHAR *User, const TCHAR *Password )
{
	return SQL_SUCCEEDED(SQLConnect(Handle(), (SQLTCHAR*)DsnName, _tcslen(DsnName), 
		(SQLTCHAR *)User,  _tcslen(User), (SQLTCHAR*)Password, _tcslen(Password)));
}

bool SqlDbc::Connect( const TCHAR *ConnectString )
{
	SQLTCHAR StrOut[1024];
	SQLSMALLINT StrOutLen = 0;
	return SQL_SUCCEEDED( SQLDriverConnect( Handle(), NULL,  (SQLTCHAR *)ConnectString, _tcslen( ConnectString ), StrOut, 1024, &StrOutLen, SQL_DRIVER_NOPROMPT) );
}
bool SqlDbc::Disconnect()
{
	return SQL_SUCCEEDED(SQLDisconnect(Handle()));
}
void SqlError::SetDesc( const SQLTCHAR *Msg )
{
	_tcsncpy_s( ErrorMsg, Msg, 1024 );
}

void SqlError::SetState( const SQLTCHAR *State )
{
	_tcsncpy_s( this->State, State, 16 );
}

SQLTCHAR * SqlError::GetDesc()
{
	return this->ErrorMsg;
}

SQLTCHAR * SqlError::GetState()
{
	return this->State;
}