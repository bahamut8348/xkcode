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
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#include <atlbase.h>
#include <atlapp.h>
#include <Windows.h>
#include <string>
#include <hash_map>
#include <vector>

#include "../udt/src/udt.h"

#pragma  comment( lib , "ws2_32")
#pragma  comment( lib, "Iphlpapi" )
#pragma  comment( lib , "udt" )
#ifdef _DEBUG
#pragma  comment( lib , "libboost_python-vc80-mt-gyd.lib" )
#endif

// TODO: reference additional headers your program requires here
