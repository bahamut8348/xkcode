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
#include <assert.h>

#include <winsock2.h>
#include <Windows.h>
#include <atlbase.h>
#include <atlapp.h>


#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <Python.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/shared_ptr.hpp>

#include <ws2tcpip.h>

#include <hash_map>

#pragma  comment( lib, "ws2_32" )

// TODO: reference additional headers your program requires here
