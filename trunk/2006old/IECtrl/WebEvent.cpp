#include "StdAfx.h"
#include "WebEvent.h"


CWebEvent::CWebEvent( CHost *pHost ):m_pHost(pHost)
{
	
}
CWebEvent::~CWebEvent(void)
{
}

HRESULT STDMETHODCALLTYPE CWebEvent::GetTypeInfoCount( /* [out] */ UINT *pctinfo )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CWebEvent::GetTypeInfo( /* [in] */ UINT iTInfo, /* [in] */ LCID lcid, /* [out] */ ITypeInfo **ppTInfo )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CWebEvent::GetIDsOfNames( /* [in] */ REFIID riid, /* [size_is][in] */ LPOLESTR *rgszNames, /* [in] */ UINT cNames, /* [in] */ LCID lcid, /* [size_is][out] */ DISPID *rgDispId )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CWebEvent::Invoke( /* [in] */ DISPID dispIdMember, /* [in] */ REFIID riid, /* [in] */ LCID lcid, /* [in] */ WORD wFlags, /* [out][in] */ DISPPARAMS *pDispParams, /* [out] */ VARIANT *pVarResult, /* [out] */ EXCEPINFO *pExcepInfo, /* [out] */ UINT *puArgErr )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CWebEvent::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject )
{
	return m_pHost->QueryInterface(riid, ppvObject);
}

ULONG STDMETHODCALLTYPE CWebEvent::AddRef( void )
{
	return m_pHost->AddRef();
}

ULONG STDMETHODCALLTYPE CWebEvent::Release( void )
{
	return m_pHost->Release();
}