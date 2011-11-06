// ShortCutExt.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "ShortCutExt.h"


class CShortCutExtModule : public CAtlDllModuleT< CShortCutExtModule >
{
public :
	DECLARE_LIBID(LIBID_ShortCutExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SHORTCUTEXT, "{8BB88B5A-EFD5-46B6-9357-C4CF76066BE1}")
};

CShortCutExtModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib

	if ( 0 == (GetVersion() & 0x80000000UL) )
	{
		CRegKey reg;
		LONG    lRet;

		lRet = reg.Open ( HKEY_LOCAL_MACHINE,
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
			KEY_SET_VALUE );

		if ( ERROR_SUCCESS != lRet )
			return E_ACCESSDENIED;

		lRet = reg.SetStringValue(_T("{A81EBE8C-831C-4D06-AAC7-CA605822026E}") , 
			_T("常用功能扩展") );

		if ( ERROR_SUCCESS != lRet )
			return E_ACCESSDENIED;
	}


    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	if ( 0 == (GetVersion() & 0x80000000UL) )
	{
		CRegKey reg;
		LONG    lRet;

		lRet = reg.Open ( HKEY_LOCAL_MACHINE,
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
			KEY_SET_VALUE );

		if ( ERROR_SUCCESS == lRet )
		{
			lRet = reg.DeleteValue( _T("{A81EBE8C-831C-4D06-AAC7-CA605822026E}") );
		}
	}
	
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

