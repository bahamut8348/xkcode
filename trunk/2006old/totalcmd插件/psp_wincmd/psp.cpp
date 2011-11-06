#include "StdAfx.h"
#include "psp.h"


static tChangeVolProc ChangeVolProc;
static tProcessDataProc ProcessDataProc;

#define EXPORT_API  extern"C" __declspec( dllexport )

static int Message( WCHAR * a )
{
	return MessageBox( 0, a, a, 0 );
}

static int Message( CHAR * a )
{
	return MessageBoxA( 0, a, a, 0 );
}


PackInfo g_ArchList[MAX_NUM];

char g_logbuf[1024];

EXPORT_API  HANDLE __stdcall  OpenArchive( tOpenArchiveData* ArchiveData )
{
	sprintf( g_logbuf, "OpenArchive %s", ArchiveData->ArcName );
	Message( g_logbuf );
	USES_CONVERSION;
	int n = 0;
	IResourceManager * mgr = GetResourceManagerSingleton( );
	char name[32] = {0};
	strcpy( name, strrchr( ArchiveData->ArcName, '\\') + 1 );
	char *p = strrchr( name, '.' );
	if( p !=0 ) p[0]= 0;
	_bool ret = mgr->OpenPackArchive( A2W( ArchiveData->ArcName  ) , A2W(name) );
	if( ret )
	{
		for( _dword a = 1; a < MAX_NUM ; a++ )
		{
			if( g_ArchList[a].mOpened == 0 )
			{
				n = a;
				break;
			}
		}
	}
	else
	{
		PackInfo & info = g_ArchList[n];
		info.mOpenResult = E_UNKNOWN_FORMAT;
	}
	if( n!= 0)
	{
		PackInfo & info = g_ArchList[n];
		strcpy( info.mPackName, ArchiveData->ArcName);
		info.mOpenResult = E_EOPEN;
		info.mOpened = 1;
		FileSystem::GetFileSize( A2W(info.mPackName),  info.mPackSize);
		sprintf( g_logbuf, "return %d", n );
		Message(g_logbuf);
		return (HANDLE)n;
	}

	return (HANDLE) 0;
}

EXPORT_API HANDLE __stdcall  CloseArchive( int )
{
		Message(L"Close");
		return (HANDLE)1;
}


EXPORT_API int __stdcall ProcessFile( HANDLE hArcData, int Operation, CHAR* DestPath, CHAR* DestName )
{
	sprintf( g_logbuf,"%s, %d, %d, %s,%s","ProcessFile", hArcData, Operation, DestPath, DestName );
	Message( g_logbuf );


	if( (int)hArcData < 1 )
	{
		return E_BAD_DATA;
	}

	if( Operation == PK_SKIP )
	{
		return 0;
	}

	PK_EXTRACT;
	return E_BAD_DATA;
}


EXPORT_API int __stdcall ReadHeader( HANDLE hArcData, tHeaderData* HeaderData )
{
	ZeroMemory(HeaderData, sizeof(*HeaderData));
	sprintf( g_logbuf,"%s, %d","ReadHeader", (int)hArcData );
	Message( g_logbuf );

	if( hArcData == 0 )
	{
		return E_EREAD;
	}

	PackInfo & info = g_ArchList[(int)hArcData];
	
	char name[32] = {0};
	strcpy( name, strrchr( info.mPackName, '\\') + 1 );
	char *p = strrchr( name, '.' );
	if( p !=0 ) p[0]= 0;

	strcpy(HeaderData->ArcName , info.mPackName);
	strcpy( HeaderData->FileName , name );
	HeaderData->PackSize = info.mPackSize;
	HeaderData->UnpSize = info.mPackSize;
	HeaderData->FileAttr = FA_ARCHIVE | 2;

	sprintf( g_logbuf,"Read header%s, %s",HeaderData->ArcName, HeaderData->FileName  );
	Message( g_logbuf );

	HeaderData->FileTime = System::GetElapsedSecondSinceUTC( );
	return 0;

	return E_END_ARCHIVE;
}

EXPORT_API void __stdcall SetChangeVolProc( HANDLE /*hArcData*/, tChangeVolProc pChangeVolProc )
{
	Message(L"SetChangeVolProc");
	ChangeVolProc = pChangeVolProc;
}

EXPORT_API void __stdcall SetProcessDataProc( HANDLE /*hArcData*/, tProcessDataProc pProcessDataProc )
{
	Message(L"SetProcessDataProc");
	ProcessDataProc = pProcessDataProc;
}

EXPORT_API BOOL __stdcall  CanYouHandleThisFile( CHAR* FileName )
{
	Message(L"CanYouHandleThisFileW");
	if( !FileName )
		return FALSE;
	USES_CONVERSION;
	
	IResourceManager * mgr = GetResourceManagerSingleton( );
	return mgr->OpenPackArchive( A2W( FileName ) , L"File" );
}


EXPORT_API int __stdcall  GetPackerCaps()
{
	Message(L"GetPackerCaps");
	return PK_CAPS_BY_CONTENT | PK_CAPS_SEARCHTEXT;
}


