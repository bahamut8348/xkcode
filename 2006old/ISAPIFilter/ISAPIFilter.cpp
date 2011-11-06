// ISAPIFilter.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

static PyObject *g_FilterProc = NULL;

HANDLE g_Event = NULL;

CRITICAL_SECTION g_cs = {0};

#define  EVENT_NAME  L"EVENT_LOCK"

static void  DebugPut( const char *s)
{
	FILE *F = fopen("c:\\filter.log","a+b");
	fwrite(s, strlen(s), 1, F);
	fclose(F);
}


struct PyHTTP_FILTER_CONTEXT : HTTP_FILTER_CONTEXT
{
	str ServerVariable(const std::string & key)
	{
		
		CHAR inkey[1024];
		strcpy_s(inkey, key.c_str());
		BYTE outbuffer[1024];
		DWORD outlength = 1024;
		BOOL bRet = this->GetServerVariable( this, inkey, outbuffer, &outlength);
		if(bRet)
		{
			return str((const char *)outbuffer, outlength);
		}
		else
		{
			bRet = ::GetLastError();
			if(bRet == ERROR_INVALID_INDEX)
			{
				PyErr_SetString(PyExc_KeyError,"invalid index");
				throw error_already_set();
			}
			else if(bRet == ERROR_NO_DATA)
			{
				PyErr_SetString(PyExc_KeyError,"data error");
				throw error_already_set();
			}
			else if (bRet == ERROR_INSUFFICIENT_BUFFER)
			{
				BYTE *newbuf = new BYTE[outlength];
				
				GetServerVariable(this, inkey, newbuf, &outlength);
				str ret((const char *)newbuf, outlength);
				delete [] newbuf;
				return ret;
			}

		}
		
		
	}


	bool AddHeaders( const std::string & s)
	{
		CHAR header[1024];
		strcpy_s(header, s.c_str());
		return AddResponseHeaders(this, header, 0);
	}


	bool Write( const str & s)
	{
		char *p = extract<char *>(s);
		DWORD writelen = len(s);
		return WriteClient(this,(LPVOID) p, &writelen, 0);
	}

	bool Function( DWORD seq_type, const std::string & s , const std::string & data1, const std::string & data2)
	{
		return ServerSupportFunction(this, (SF_REQ_TYPE)seq_type, (PVOID) s.c_str(), (ULONG_PTR)data1.c_str() , (ULONG_PTR)data2.c_str() );
	}


	

};



BOOL WINAPI GetFilterVersion(
				 HTTP_FILTER_VERSION * pVer
				 )
{
	
	if( g_cs.OwningThread == NULL )
	{
		
	}
	
	pVer->dwFilterVersion = HTTP_FILTER_REVISION;
	pVer->dwFlags = SF_NOTIFY_LOG | SF_NOTIFY_ORDER_MEDIUM  | SF_NOTIFY_URL_MAP ;

	strcpy_s( pVer->lpszFilterDesc, SF_MAX_FILTER_DESC_LEN, "ISAPIFilter" );

	DebugPut("GetFilterVersion\r\n");
	return TRUE;
}

DWORD  WINAPI HttpFilterProc(
			   HTTP_FILTER_CONTEXT *      pfc,
			   DWORD                      NotificationType,
			   VOID *                     pvData )
{
	DWORD dwRet = SF_STATUS_REQ_NEXT_NOTIFICATION ;
	DebugPut("HttpFilterProc\r\n");
	PyHTTP_FILTER_CONTEXT *pContext = (PyHTTP_FILTER_CONTEXT *)pfc;
	
	::EnterCriticalSection(&g_cs);
	PyImport_ReloadModule(  g_FilterProc );
	dwRet= call_method<DWORD>(g_FilterProc, "FilterProc", *pContext, NotificationType, 1);
	::LeaveCriticalSection(&g_cs);
	return dwRet;
}




BOOL WINAPI DllMain(
					HINSTANCE hinstDLL,
					DWORD fdwReason,
					LPVOID lpvReserved
					)

{


	

	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		
		DebugPut("Attach\r\n");
		Py_Initialize();
		PyEval_InitThreads();
		::InitializeCriticalSection(&g_cs);
		g_FilterProc  = PyImport_ImportModule("filter");
		if(g_FilterProc == 0)
		{
			DebugPut("Null Module Pointer\r\n");
		}
		class_<PyHTTP_FILTER_CONTEXT>("Context")
			.def_readwrite("Revision",		&PyHTTP_FILTER_CONTEXT::Revision)
			.def_readwrite("ServerContext", &PyHTTP_FILTER_CONTEXT::ServerContext)
			.def_readwrite("FilterContext", &PyHTTP_FILTER_CONTEXT::pFilterContext)
			.def_readwrite("IsSecurePort",	&PyHTTP_FILTER_CONTEXT::fIsSecurePort)
			.def("ServerVariable",			&PyHTTP_FILTER_CONTEXT::ServerVariable)
			.def("AddHeaders",				&PyHTTP_FILTER_CONTEXT::AddHeaders)
			.def("Write",					&PyHTTP_FILTER_CONTEXT::Write)
			.def("Function",				&PyHTTP_FILTER_CONTEXT::Function)
			;
	}
	else {

		DebugPut("Dettach\r\n");
	}


	return TRUE;


}

