// d1.cpp : main source file for d1.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>



#include "resource.h"


#include "MainDlg.h"

CAppModule _Module;


class CMyMsgLoop : public CMessageLoop
{
public:
	virtual BOOL OnIdle(int nIdleCount)
	{
		CMessageLoop::OnIdle(nIdleCount);
		return TRUE;
	}

};


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window chunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	CMyMsgLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	int nRet = 0;
	// BLOCK: Run application
	CMainDlg dlgMain;
	CWindow win = dlgMain.Create(NULL);
	win.ShowWindow(SW_SHOWNORMAL);
	theLoop.Run();
	_Module.RemoveMessageLoop();

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
