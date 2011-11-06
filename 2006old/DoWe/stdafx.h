// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <ace/ace.h>
#include <ace/os_main.h>
#include <ace/log_msg.h>
#include <ace/OS_NS_errno.h>
#include <ace/Event_Handler.h>
#include <ace/NT_Service.h>
#include <ace/Singleton.h>
#include <ace/Mutex.h>
#include <ace/Reactor.h>
#include <ace/Proactor.h>
#include <ace/Asynch_Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include <fstream>


// TODO: reference additional headers your program requires here
