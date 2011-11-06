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

#include "pcap.h"

#include <boost/python.hpp>
using namespace  boost::python;
#include <WinSock2.h>
#include <Iphlpapi.h>

#include <string>
#include <vector>

#include <atlstr.h>

#pragma  comment( lib, "pcap/lib/wpcap.lib" )
#pragma  comment( lib, "pcap/lib/Packet.lib" )
#pragma  comment( lib, "Iphlpapi.lib" )
// #ifdef _DEBUG
// #pragma  comment( lib, "libboost_python-vc80-mt-gyd.lib" )
// #else
// #pragma  comment( lib, "libboost_python-vc80-mt.lib" )
// #endif

#pragma  comment( lib, "ws2_32.lib" )


// TODO: reference additional headers your program requires here
