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

#ifndef _DEBUG
	#pragma comment( lib, "libboost_system-vc80-s-1_36.lib" )
#else
	#pragma comment( lib, "libboost_system-vc80-mt-sgd-1_36.lib" )
#endif

// TODO: reference additional headers your program requires here
