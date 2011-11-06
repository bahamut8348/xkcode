from ctypes import *


SQL_FETCH_NEXT = 0x01
SQL_FETCH_FIRST = 0x02
SQL_FETCH_LAST = 0x04

SQL_INVALID_HANDLE = -2
SQL_SUCCESS = 0
SQL_SUCCESS_WITH_INFO = 1
SQL_NO_DATA_FOUND = 100

SQL_NULL_HANDLE = 0
SQL_HANDLE_ENV = 1
SQL_HANDLE_DBC = 2
SQL_HANDLE_DESCR = 4
SQL_HANDLE_STMT = 3
SQL_NTS = -3
SQL_C_BINARY = -2
SQL_BINARY = -2
SQL_ATTR_ODBC_VERSION = 200
SQL_OV_ODBC2 = 3

SQL_TABLE_NAMES = 3
SQL_C_CHAR = 1

env_h = c_int()
dbc_h = c_int()
stmt_h = c_int()

odbc = windll.odbc32

odbc.SQLAllocHandle( SQL_HANDLE_ENV , SQL_NULL_HANDLE, byref(env_h) )
odbc.SQLSetEnvAttr(env_h, SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC2, 0)
odbc.SQLAllocHandle(SQL_HANDLE_DBC, env_h, byref(dbc_h))

conn_str = create_string_buffer(1024)
conn_str.value = "DRIVER={SQL Server};SERVER=127.0.0.1,7788;UID=sa;PWD=admin;APP=Generic32;DATABASE=xk"

out_str = create_string_buffer(1024)
out_len = c_int()

odbc.SQLDriverConnect.restype  = c_short

print odbc.SQLDriverConnect( dbc_h, 0, byref(conn_str), -3, byref(out_str),1024, byref(out_len),0)
print out_str.value

odbc.SQLAllocHandle(SQL_HANDLE_STMT, dbc_h, byref(stmt_h))

odbc.SQLPrepare(stmt_h, "insert into subitem (subitembuffer) values( ? ) ;", SQL_NTS);

odbc.SQLBindParameter( stmt_h,  1, 1 , SQL_C_BINARY, SQL_BINARY, 10,0,byref(out_str), 10, -3)


