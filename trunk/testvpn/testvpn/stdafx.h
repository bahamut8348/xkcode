// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>

#pragma warning( disable :  4819 4511 4512 4663 4245 4018 4251 )



#include <Winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <hash_map>
#include <list>
#include <assert.h>

#include <tchar.h>

#include <atlbase.h>
#include <atlapp.h>


#include "Common.h"
#include "..\udt\src\udt.h"
#include "IOCP.h"

#ifdef _DEBUG
#define  LOG 1
#endif



#pragma  comment( lib , "ws2_32")
#pragma  comment( lib, "Iphlpapi" )
#pragma  comment( lib , "udt" )
// TODO: reference additional headers your program requires here
