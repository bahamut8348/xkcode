// ShellExt.h : Declaration of the CShellExt

#pragma once
#include "resource.h"       // main symbols
#include "ShorCutExt.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CShellExt



class ATL_NO_VTABLE CShellExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShellExt, &CLSID_ShellExt>,
	public IDispatchImpl<IShellExt, &IID_IShellExt, &LIBID_ShorCutExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
	,public IContextMenu
	,public IShellExtInit
{
public:
	CShellExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SHELLEXT)

BEGIN_COM_MAP(CShellExt)
	COM_INTERFACE_ENTRY(IShellExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}


	TCHAR m_szFile [MAX_PATH];

public:
	
	STDMETHODIMP Initialize( 
		/* [in] */ LPCITEMIDLIST pidlFolder,
		/* [in] */ IDataObject *pdtobj,
		/* [in] */ HKEY hkeyProgID);
	

	STDMETHODIMP QueryContextMenu(
		HMENU hmenu,
		UINT indexMenu,
		UINT idCmdFirst,
		UINT idCmdLast,
		UINT uFlags);

	STDMETHODIMP InvokeCommand(
		LPCMINVOKECOMMANDINFO lpici);

	STDMETHODIMP GetCommandString(
		UINT_PTR    idCmd,
		UINT        uType,
		UINT      * pwReserved,
		LPSTR       pszName,
		UINT        cchMax);

};

OBJECT_ENTRY_AUTO(__uuidof(ShellExt), CShellExt)
