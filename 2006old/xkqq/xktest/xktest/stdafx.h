// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						
#pragma warning( disable :  4819 4511 4512 4663 4245 4018)

#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace boost;
using boost::asio::ip::udp;

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <hash_map>
#include <vector>

#include <windows.h>
#include "utils.h"



// TODO: reference additional headers your program requires here
