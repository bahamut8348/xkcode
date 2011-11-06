// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0601
#define _RICHEDIT_VER	0x0302

#define  _WTL_NO_CSTRING
#define _WTL_NO_WTYPES

#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atltypes.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <atlsplit.h>

#include <Tlhelp32.h>
#include <Iphlpapi.h>

#include <string>
#include <vector>
#include <hash_map>
#include <hash_set>
#include <list>
#include "Utils.h"


#define  WM_TOFRAME WM_USER + 1
#define  WF_SETACTIVEPAGE  1



using namespace std;


#pragma  comment(lib, "version")
#pragma  comment(lib, "Iphlpapi")
#pragma  comment(lib, "ws2_32")
#pragma  comment(lib, "Psapi")

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
