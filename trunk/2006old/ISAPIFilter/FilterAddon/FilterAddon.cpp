// FilterAddon.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#define  PIPE_NAME L"PY_ISAPI_PIPE"

static void  DebugPut( const char *s)
{
	FILE *F = fopen("c:\\filter.log","a+b");
	fwrite(s, strlen(s), 1, F);
	fclose(F);
}




struct App
{
	enum
	{
		_BUFSIZE = 4096,
	};
	
	static App * s_Instance;
	static App * Instance()
	{
		if( App::s_Instance == NULL)
		{
			return new App;
		}
		return s_Instance;
	}
		
	struct ThreadInfo
	{
		int mThreadID;
		HANDLE mPipe;
		HANDLE mFile;
		
		bool ConnectServer()
		{
			bool bret = false;
			mFile = CreateFile( L"\\\\.\\pipe\\" PIPE_NAME , GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if( mFile == INVALID_HANDLE_VALUE )
			{
				bret = true;
				DebugPut( "Create File Error\r\n" );
			}
			return bret;
		}


		DWORD Process( HTTP_FILTER_CONTEXT *context, DWORD notifyType, void * pvData )
		{
			DWORD dwRet = SF_STATUS_REQ_NEXT_NOTIFICATION ;
			return dwRet;
		}
		
	};

	stdext::hash_map<int, ThreadInfo> mThreadMap;
	


};




App * App::s_Instance = NULL;



BOOL WINAPI GetFilterVersion(
							 HTTP_FILTER_VERSION * pVer
							 )
{


	pVer->dwFilterVersion = HTTP_FILTER_REVISION;
	pVer->dwFlags = SF_NOTIFY_LOG | SF_NOTIFY_ORDER_MEDIUM  | SF_NOTIFY_URL_MAP ;

	strcpy_s( pVer->lpszFilterDesc, SF_MAX_FILTER_DESC_LEN, "FilterAddon" );

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
	
	return dwRet;
}

