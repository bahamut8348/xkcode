// Odbctest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
using namespace std;


#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <time.h>



#include "Odbc.h"



void mytest()
{


	wstring dsn = L"psplayer";
	wstring user = L"psplayer";
	wstring pass = L"psplayer";

	int a = 0;

	SqlEnv  evn;
	SqlError err;
	SqlDbc dbc;
	ULONG n;
	
	//dbc.Connect(dsn.c_str(), user.c_str(), pass.c_str());
	dbc.Connect(L"DRIVER={SQL Server};SERVER=192.168.1.13;UID=psplayer;PWD=psplayer;APP=Generic32;DATABASE=psplayer");
	SqlStmt stmt(dbc);
	stmt.SetCursorType(SQL_CURSOR_DYNAMIC);
	stmt.GetError(err);

	TCHAR data[16];
	SQLINTEGER len;
	n = 32;

	stmt.Execute(L"select loginname ,id, password, nickname, email, country, city, sex, birth, sendemail, rightid, [money] from account where loginname = N'ps_ma.rui'");
	stmt.FetchNext();
	bool ret = stmt.GetData(L"password", data, n, &len);
	stmt.GetError(err);
	
	stmt.Execute(L"update account set email=N'kk@gmai.com' where loginname = N'ps_ma.ruixxxxxxxxxxxxxxxxxxxxx'");
	stmt.GetError(err);
	ret = stmt.Fetch();
	stmt.GetError(err);

	
	//stmt.GetData(3, data, 40, &len);
	stmt.GetData(L"loginname", &n, &len);
	//stmt.GetData(1, data, 40, &len);

	SqlColDescribe col;
	SQLDescribeCol(stmt, 3, col.ColName, 10, (SQLSMALLINT*)&len,NULL,NULL,NULL,NULL);
	
	stmt.GetData(2, &n, &len);
	SQLFetchScroll(stmt, SQL_FETCH_RELATIVE,0);
	stmt.GetData(7, data, 40, &len);
	stmt.GetData(2, &n, &len);
	SYSTEMTIME st;
	GetLocalTime(&st);
	wprintf(L"%d %s %d.%d\n",a, data, st.wSecond, st.wMilliseconds);
	stmt.Close();
	
	stmt.GetError(err);
	wprintf(L"%s\n", err.GetDesc());

}


void mytest2()
{
	int a = 0;
	SqlEnv evn;
	SQLSetEnvAttr(evn, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_HENV, 0);
	SqlDbc dbc;
	//dbc.Connect(L"DRIVER={SQL Server};SERVER=192.168.1.65;UID=psplayer;PWD=psplayer;APP=Generic32;DATABASE=psplayer");
	//dbc.Connect(L"psplayer",L"psplayer",L"psplayer");

	dbc.Connect(L"DSN=psplayer;UID=psplayer;PWD=psplayer");
	
	SqlStmt stmt(dbc);
//	SQLSetStmtAttr(stmt, SQL_ATTR_CURSOR_SCROLLABLE, (SQLPOINTER)SQL_SCROLLABLE,0);
	while (1)
	{
		stmt.Close();
		stmt.Close();
		bool ret = stmt.Execute(L"select loginname ,id, password, nickname, email, country, city, sex, birth, sendemail, rightid, [money] from account where loginname = N'ps_ma.rui'");
		stmt.Close();
		stmt.Close();
		//ret = stmt.Execute(L"select id from InvalidWord where (charindex(InvalidContent, lower('ps_ma.rui')) > 0) and RegType = 1");
		stmt.Close();
		stmt.Close();
		//ret = stmt.Execute(L"select id from InvalidWord where (charindex(InvalidContent, lower('ps_ma.rui')) > 0) and RegType = 1");
		stmt.Close();
		stmt.Close();
		TCHAR tempbuf[2048] = {0};
	//	swprintf(tempbuf, L"insert into account( loginname, password, nickname, email, country, city, sex, sendemail, rightid, type, RegProductID ) values ( N'ps_testtest%d', N'09e31d1ab4de57a06dac4834a7cb1432', N'sdfsdf'", a);
		swprintf(tempbuf, L"%s", L"insert into account( loginname, password, nickname, email, country, city, sex, sendemail, rightid, type, RegProductID ) values ( N'testt0', N'09e31d1ab4de57a06dac4834a7cb1432', N'', N'', 0, N'', 0, 0, 0, 0, 0 )");
		swprintf(tempbuf, L"%s", L"insert into account( loginname, password, nickname, email, country, city, sex, birth, sendemail, rightid, type, RegProductID ) values ( N'ps_10test1', N'09e31d1ab4de57a06dac4834a7cb1432', N'sdfdf', N'', 1, N'2sd', 1, '1986-10-2', 1, 3, 1, 2 )");

		ret = stmt.Execute(tempbuf);
		//if(ret == false)
		//{
		//	SqlError err;
		//	stmt.GetError(err);
			
		//	break;
		//}
		a++;
		
		stmt.Fetch();

	
		if(a%2000 == 0)
		{
			SYSTEMTIME st;
			GetSystemTime(&st);
			_tprintf(L"%d %d.%d\n", a, st.wSecond, st.wMilliseconds);
		}
	}

	return;
}


void mytest3()
{
	int a = 0;
	
	SqlEnv evn;

	{
		SqlDbc dbc;
		dbc.Disconnect();
		SqlStmt stmt(dbc);
	}
	
	SQLSetEnvAttr(evn, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_HENV, 0);
//	SqlDbc dbc;
//	dbc.Connect(L"DRIVER={SQL Server};SERVER=192.168.1.65;UID=psplayer;PWD=psplayer;APP=Generic32;DATABASE=psplayer");
	while (1)
	{
		SqlDbc dbc;
		//dbc.Connect(L"DRIVER={SQL Server};SERVER=192.168.1.65;UID=psplayer;PWD=psplayer;APP=Generic32;DATABASE=psplayer");
		dbc.Connect(L"psplayer",L"psplayer",L"psplayer");	
		SqlStmt stmt(dbc);
		

// 		bool ret = stmt.Execute(L"select loginname ,id, password, nickname, email, country, city, sex, birth, sendemail, rightid, [money] from account where loginname = N'ps_ma.rui'");
// 		if(ret == false)
// 		{
// 			SqlError err;
// 			stmt.GetError(err);
// 
// 			break;
// 		}
// 		a++;
// 
// 		stmt.Fetch();


// 		if(a%1000 == 0)
// 		{
// 			SYSTEMTIME st;
// 			GetSystemTime(&st);
// 			_tprintf(L"%d %d.%d\n", a, st.wSecond, st.wMilliseconds);
// 		}
		dbc.Disconnect();
	}
}


void mytest4()
{
	wstring dsn = L"psplayer";
	wstring user = L"psplayer";
	wstring pass = L"psplayer";

	int a = 0;

	SqlEnv  evn;
	SqlError err;
	SqlDbc dbc;


	//dbc.Connect(dsn.c_str(), user.c_str(), pass.c_str());
	dbc.Connect(L"DRIVER={SQL Server};SERVER=127.0.0.1,7788;UID=sa;PWD=admin;APP=Generic32;DATABASE=xk");
	SqlStmt stmt(dbc);
	BYTE * pdata;
	SQLINTEGER len;


	stmt.Execute(L"select image, newstitle from news where id= 331");
	stmt.FetchNext();
	bool ret = stmt.GetData((SQLUSMALLINT)1,SQL_C_BINARY, NULL, 0, &len);
	stmt.GetError(err);

	if(ret)
	{
		pdata = new BYTE[len];
	}

	ret = stmt.GetData((SQLUSMALLINT)1,SQL_C_BINARY, pdata, 6411, &len);
	stmt.GetError(err);
	
	ofstream of;
	of.open(L"c:/k.jpg", ios_base::out | _IOSbinary);
	of.write((const char *)pdata, len);
	of.flush();
	of.close();
	delete pdata;
	dbc.Disconnect();
}

void mytest5()
{
	SqlEnv  evn;
	SqlError err;
	SqlDbc dbc;




	//dbc.Connect(dsn.c_str(), user.c_str(), pass.c_str());
	dbc.Connect(L"DRIVER={SQL Server};SERVER=127.0.0.1,7788;UID=sa;PWD=admin;APP=Generic32;DATABASE=xk");
	SqlStmt stmt(dbc);
//	SQLSetStmtAttr(stmt, SQL_ATTR_CONCURRENCY, 	(SQLPOINTER) SQL_CONCUR_ROWVER, 0);

//	SQLSetStmtAttr(stmt, SQL_ATTR_CURSOR_TYPE  , (SQLPOINTER)SQL_CURSOR_DYNAMIC,0);
	//SQLSetStmtAttr(stmt, SQL_ATTR_USE_BOOKMARKS, (SQLPOINTER)SQL_UB_VARIABLE, 0);
//	SQLSetStmtAttr(stmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER) 30, 0);	
//	SQLSetStmtAttr(stmt, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)1, 0);
//	SQLSetStmtAttr(stmt, SQL_ATTR_ROW_STATUS_PTR, RowStatusArray, 0);


//	bool ret = stmt.Execute(L"select top 1 * from table1");
	bool ret = stmt.Prepare(L"insert into table1 (col1, col2) values( 1,? ) ");
	
	stmt.GetError( err );
	SQLINTEGER outlen;
	//stmt.GetData(1, &n, &outlen);

	char *cptr = "teststestestetet";
	
	long b = 0;
	long a = 10;
	
	outlen = 10;
	//::SQLBindParameter( stmt,  2, 1 , SQL_C_BINARY, SQL_BINARY, a,0,(SQLPOINTER)cptr, a, &outlen );
	//stmt.BindParam( 0, &a);
	ret =stmt.BindParam( 0, (SQLPOINTER)cptr, &outlen );
	//::SQLBindParameter( stmt,  2, 1 , SQL_C_BINARY, SQL_BINARY, a,0,(SQLPOINTER)cptr, a, &outlen );	
	//stmt.GetError( err );
	
	ret =stmt.Execute();
	stmt.GetError( err );
	dbc.GetError( err );
	
	ret = stmt.BindCol( 0, &a );

	ret = stmt.BindCol( 1, &a );
	
	//ret = stmt.FetchNext();

	//ret = stmt.AddNew();
	stmt.GetError( err );
	dbc.Disconnect();
}

inline bool BindParam3(SQLHANDLE h,  SQLPOINTER value)
{
	SQLLEN outlen =  5;
	//long a = ::SQLBindParameter( h,  2, SQL_PARAM_INPUT , SQL_C_BINARY, SQL_BINARY, 2,0,(SQLPOINTER)value, 2, &outlen );
	bool ret;// = SQL_SUCCEEDED( ::SQLBindParameter( h,  2, SQL_PARAM_INPUT , SQL_C_BINARY, SQL_VARBINARY, 1000,0,(SQLPOINTER)value, 0, &outlen ) );
	ret = SQL_SUCCEEDED( ::SQLBindParameter(      h,  1, SQL_PARAM_INPUT , SQL_C_BINARY, SQL_BINARY, 0,0,(SQLPOINTER)value, 5, &outlen ) );
	return ret;
}

bool BindParam2( SQLHANDLE h, long* value )
{
	
	long outlen = 0;
	return  SQL_SUCCEEDED( ::SQLBindParameter(h, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, value, 0, &outlen) );
}

void mytest6()
{

	SQLHANDLE h_env ;
	::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &h_env);
	::SQLSetEnvAttr(h_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0); 

	SQLHANDLE h_dbc;
	::SQLAllocHandle(SQL_HANDLE_DBC, h_env, &h_dbc);
	::SQLSetConnectAttr(h_dbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(10), 0);

	SQLTCHAR StrOut[1024];
	SQLSMALLINT StrOutLen = 0;
	SQLTCHAR  sql_str[] = L"DRIVER={SQL Server};SERVER=192.168.1.13;UID=psplayer;PWD=psplayer;DATABASE=psplayer";
	::SQLDriverConnect( h_dbc, NULL,  sql_str, SQL_NTS, StrOut, 1024, &StrOutLen, SQL_DRIVER_NOPROMPT);

	SQLHANDLE h_stmt;
	SQLAllocHandle(SQL_HANDLE_STMT, h_dbc, &h_stmt);

	SQLCHAR cptr[1024];
	long a = 0;
	
	::SQLPrepare(h_stmt, L"insert into subitem (subitembuffer) values( ? ) ;", SQL_NTS);

	bool ret;
	ret = BindParam3(h_stmt, (SQLPOINTER)cptr );

	long outlen = 12;
	//ret = SQL_SUCCEEDED( ::SQLBindParameter( h_stmt,  1, SQL_PARAM_INPUT , SQL_C_BINARY, SQL_BINARY, 1000,0,(SQLPOINTER)cptr, 12, &outlen) );	
	
	//ret = BindParam2( h_stmt, &a );

//	::SQLPrepare(h_stmt, L"insert into subitem (itemid, subitembuffer) values( ?,? ) ", SQL_NTS);
	
	strcpy( (char *)cptr,"teststestestetet");
	a = 12;
	long r = ::SQLExecute(h_stmt);
	SQL_NEED_DATA;
	ret = SQL_SUCCEEDED( r );
	

	SqlError err;
	SQLSMALLINT ErrorLen = sizeof(err.ErrorMsg)/sizeof(SQLTCHAR);
	SQLINTEGER StateLen = sizeof(err.State)/sizeof(SQLTCHAR);
	SQL_SUCCEEDED(SQLGetDiagRec(SQL_HANDLE_STMT, h_stmt, 1, err.State, &StateLen, err.ErrorMsg, ErrorLen, &ErrorLen ));

	return;
}


void mytest7()
{
	SqlEnv  evn;
	SqlError err;
	SqlDbc dbc;

	dbc.Connect(L"DRIVER={SQL Server};SERVER=127.0.0.1,7788;UID=sa;PWD=admin;APP=Generic32;DATABASE=xk");
	SqlStmt stmt(dbc);
	bool ret = stmt.Prepare(L"insert into table1 (col1, col2) values( ?,? ) ");
	SQLINTEGER outlen;

	char *cptr = "teststestestetet";

	long b = 0;
	long a = 10;

	outlen = 10;
	//::SQLBindParameter( stmt,  2, 1 , SQL_C_BINARY, SQL_BINARY, a,0,(SQLPOINTER)cptr, a, &outlen );
	ret =stmt.BindParam( 1, (SQLPOINTER)cptr, &outlen );
	stmt.BindParam( 0, &a);
	ret =stmt.Execute();
	stmt.GetError( err );
	dbc.GetError( err );

	dbc.Disconnect();

}

void mytest8()
{
	SqlEnv  evn;
	SqlError err;
	SqlDbc dbc;

	dbc.Connect(L"DRIVER={SQL Server};SERVER=127.0.0.1,7788;UID=sa;PWD=admin;APP=Generic32;DATABASE=xk");
	SqlStmt stmt(dbc);
	stmt.Prepare(L"insert into table1 (col1, col2) values( 1,? ) ");
	char *cptr = "teststestestetet";
	//stmt.BindParam( 0, (SQLPOINTER)cptr, 10);
	// /stmt.BindBinary( 0 );
	
	SQLLEN outlen = SQL_LEN_DATA_AT_EXEC(0);
	bool ret =  SQL_SUCCEEDED( ::SQLBindParameter( stmt,1 , SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, 0, 0, (SQLPOINTER)1, 0, &outlen) );
	
	stmt.GetError( err );

	ret = stmt.Execute();

	stmt.PutData( 0, cptr, 5);

	stmt.GetError( err );

	return;

};

int _tmain(int argc, _TCHAR* argv[])
{
	
	mytest7();
	
	
	return 0;
}

