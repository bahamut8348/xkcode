// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once




// Change these values to use different versions
#define WINVER		0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0601
#define _RICHEDIT_VER	0x0200
#define _WTL_FORWARD_DECLARE_CSTRING
#define _WTL_NO_WTYPES

#include <atlbase.h>
#include <atlapp.h>
#include <atlcoll.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atltypes.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <atlsplit.h>
#include <atlddx.h>
#include <wtl\XKCommandBarCtrl.h>
//#include "atldock.h"
//#include "atldock2.h"

#define WINDOW_MENU_POSITION	3


#define  WM_ROOMID (WM_USER + 100)

#include <PSDash.h>
using namespace  PSDash;

#include <psclienthelper2.h>
using namespace ClientHelpClass;


#pragma comment(lib,"gdiplus.lib")


#include "resource.h"
#include "PSDemo.h"












#include "atlgdix.h"

#include "CustomTabCtrl.h"
#include "DotNetTabCtrl.h"
#include "SimpleTabCtrls.h"
//#include "SimpleDlgTabCtrls.h"
#include "TabbedFrame.h"
//#include "TabbedMDISave.h"
#include "ListViewNoFlicker.h"

#include "BaseView.h"



static void gResizeList(  CListViewCtrl & list)
{
	int nColumnCount = list.GetHeader().GetItemCount();// GetColumnCount();

	for (int i = 0; i < nColumnCount; i++)
	{
		list.SetColumnWidth(i, LVSCW_AUTOSIZE);
		list.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}

	list.Invalidate();
}








#ifdef _DEBUG
	#pragma comment(lib, "clienthelper_d")
#else
	#pragma comment(lib, "clienthelper")

#endif





#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
