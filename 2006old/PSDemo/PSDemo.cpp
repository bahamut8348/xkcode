// PSDemo.cpp : main source file for PSDemo.exe
//

#include "stdafx.h"

#include "resource.h"
#include "Config.h"
#include "ValidEdit.h"
#include "PSDemoView.h"
#include "Mytabview.h"
#include "CMenuButton.h"
#include "LogEdit.h"
#include "LogWindow.h"
#include "LoginDlg.h"
#include "aboutdlg.h"
#include "MainFrm.h"


CAppModule _Module;
IClientFramework *g_pFramework;
CLogWindow logwin;
CMainFrame *g_Frame;

CMainFrame *GetFrame() 
{
	return g_Frame;
}


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
start:
	int nRet = 1;
	u_int style = 0x94cf004cL | WS_OVERLAPPEDWINDOW;
	style &= ~WS_VISIBLE;
	
	{

		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);
		
		
		g_pFramework = CreateClientFramework();
		
		GetFramework()->Initialize(_null);
		
		

		

		CLoginDlg dlg;
		dlg.Create(NULL);
		dlg.ShowWindow(SW_SHOWDEFAULT);
		
		RECT rect={0,0,100,100};
		logwin.Create(dlg,rect, L"LogWin", style,
			0x10100L);
		logwin.ShowWindow(SW_HIDE);
		
		
		GetFramework()->SetLogHandle(&logwin.m_logedit);

		nRet = theLoop.Run();
		AtlTrace(L"[PSDEMO] Loop Reutn nRet: %d\n", nRet);
		_Module.RemoveMessageLoop();
	}

	if(nRet)
	{
		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);
		
		CMainFrame wndMain;
		g_Frame = &wndMain;
		if(wndMain.CreateEx() == NULL)
		{
			ATLTRACE(_T("Main window creation failed!\n"));
			return 0;
		}

		
		wndMain.ShowWindow(nCmdShow);

		nRet = theLoop.Run();
		
		_Module.RemoveMessageLoop();
	
	}
	GetFramework()->Uninitialize();
	if(nRet == 2) goto start;
	
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
//	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);


	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls
	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));
	
	LoadLibrary ( CRichEditCtrl::GetLibraryName() );



	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

IClientFramework * GetFramework()
{
	return g_pFramework;
}

IUIProcessor  * GetProcessor()
{
	return GetFramework()->GetProcessor();
}

CWindow  GetLogWindow()
{
	return  logwin;
}