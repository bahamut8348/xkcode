#include "StdAfx.h"
#include <Python.h>
#include "IOCP_ERROR.h"



namespace pyiocp
{



PyObject *g_iocp_error = 0 ;

PyObject* param_error()
{
	PyErr_SetString( PyExc_Exception, "parameter error." );
	return NULL;
}

PyObject* iocp_error( int last_error  )
{	
	if ( last_error == 0 )
	{
		last_error = GetLastError( );
	}

	LPVOID   lpMsgBuf; 
	::FormatMessageA(  FORMAT_MESSAGE_ALLOCATE_BUFFER   | FORMAT_MESSAGE_FROM_SYSTEM   |  FORMAT_MESSAGE_IGNORE_INSERTS,   
		NULL,   
		last_error,   
		MAKELANGID( LANG_ENGLISH, SUBLANG_DEFAULT),   //   Default   language 
		(LPSTR)   &lpMsgBuf,   
		0,   
		NULL     
		);   

	PyObject* v = Py_BuildValue("(is)", last_error, lpMsgBuf) ;
	PyErr_SetObject( g_iocp_error, v );
	Py_DecRef( v );
	return NULL;
}

}