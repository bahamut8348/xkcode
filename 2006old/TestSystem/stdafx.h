// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <PSDashLib.h>
using namespace  PSDash;

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

#include <CommCtrl.h>

#include "../Include/Utils/Functions.h"
#include "../Include/Utils/ILog.h"
#include "../Include/Utils/Location.h"
using namespace PSFC::Utils;
#include "../Include/Comm/ServerType.h"
#include "../Include/Comm/PSUID.h"

using namespace PSFC::Common;
#include "../Include/Protocol.h"  
#include "../Include/Errors.h"
using namespace PSFC::Protocol;
using namespace PSFC::Error;
#include "../Include/Utils/infocheck.h" 

#include "../Include/ClientCommon.h"      


#include "ClientHelpClass2.h"


#include <boost/python.hpp>
using namespace boost::python;
using namespace boost;

// TODO: reference additional headers your program requires here
