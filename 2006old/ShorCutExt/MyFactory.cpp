// MyFactory.cpp : Implementation of CMyFactory

#include "stdafx.h"
#include "ShellExt.h"
#include "MyFactory.h"


// CMyFactory


STDMETHODIMP CMyFactory::CreateInstance( LPUNKNOWN pUnkOuter,
										REFIID riid,
										LPVOID *ppvObj)
{
	*ppvObj = NULL;

	// Shell extensions typically don't support aggregation (inheritance)

	if (pUnkOuter)
		return CLASS_E_NOAGGREGATION;

	// Create the main shell extension object.  The shell will then call
	// QueryInterface with IID_IShellExtInit--this is how shell extensions are
	// initialized.

//	CShellExt * pShellExt = new CShellExt();  //Create the CShellExt object
//	if (NULL == pShellExt)
//		return E_OUTOFMEMORY;

//	return pShellExt->QueryInterface(riid, ppvObj);
}

STDMETHODIMP CMyFactory::LockServer( BOOL  fLock)
{
	return NOERROR;
}