// TestShellWindows.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Exdisp.h"

void NavigateWindow(HWND hwnd, const TCHAR * url)
{
	CComPtr<IShellWindows> psw;
	psw.CoCreateInstance(CLSID_ShellWindows);
	long lShellWindowCount=0;
	psw->get_Count(&lShellWindowCount);
	for(int a= 0; a< lShellWindowCount; a++)
	{	
		CComPtr<IDispatch> pdispShellWindow;
		psw->Item(CComVariant(a),&pdispShellWindow);
		CComQIPtr<IWebBrowser2> pIE(pdispShellWindow);
		if(pIE == NULL) continue;
		HWND iehwnd;
		pIE->get_HWND((SHANDLE_PTR *)&iehwnd);
		if(iehwnd == hwnd)
		{
			HRESULT ret = pIE->Navigate2(&CComVariant(url), &CComVariant(0), 
				&CComVariant(L"_self"), &CComVariant(0), &CComVariant(0));
		}
		
	}
	
	return;
}


int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);
	NavigateWindow(0, L"e:\\");
	
	return 0;
}

