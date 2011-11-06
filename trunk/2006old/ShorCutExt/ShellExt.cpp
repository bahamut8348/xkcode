// ShellExt.cpp : Implementation of CShellExt

#include "stdafx.h"



#include "ShellExt.h"


// CShellExt


STDMETHODIMP CShellExt::QueryContextMenu( HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags )
{
	
	UINT idCmd = idCmdFirst;
	InsertMenu(hmenu, indexMenu++, MF_SEPARATOR|MF_BYPOSITION, idCmd, L"Haha");
	return ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, 0, (USHORT)(1)));
}

STDMETHODIMP CShellExt::InvokeCommand( LPCMINVOKECOMMANDINFO lpici )
{
	HRESULT hr = E_INVALIDARG;
	if (!HIWORD(lpici->lpVerb))
	{
		 UINT idCmd = LOWORD(lpici->lpVerb);
		 switch (idCmd)
		 {
		 case 0:
			 ::MessageBox(NULL, L"Test",NULL,64);
			 break;
		 }


	}

	return hr;
}

STDMETHODIMP CShellExt::GetCommandString( UINT_PTR idCmd, UINT uType, UINT * pwReserved, LPSTR pszName, UINT cchMax )
{
	
	USES_CONVERSION;
	switch (idCmd)
	{
	case 0:
		::StrCpyNA(pszName, W2A(L"testitem number 1"), cchMax);
		break;
	}
	return NOERROR;
}

STDMETHODIMP CShellExt::Initialize( /* [in] */ LPCITEMIDLIST pidlFolder, /* [in] */ IDataObject *pDataObj, /* [in] */ HKEY hkeyProgID )
{
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	// Look for CF_HDROP data in the data object.
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ) ))
	{
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG;
	}

	// Get a pointer to the actual data.
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// Make sure it worked.
	if ( NULL == hDrop )
		return E_INVALIDARG;

	// Sanity check - make sure there is at least one filename.
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );
	HRESULT hr = S_OK;

	if ( 0 == uNumFiles )
	{
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg );
		return E_INVALIDARG;
	}

	// Get the name of the first file and store it in our member variable m_szFile.
	if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ) )
		hr = E_INVALIDARG;

	GlobalUnlock ( stg.hGlobal );
	ReleaseStgMedium ( &stg );

	return hr;
	return NOERROR;
}