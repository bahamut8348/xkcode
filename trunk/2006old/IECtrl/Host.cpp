#include "stdafx.h"

HWND CHost::GetWindow() const
{
	return m_hWnd;
}

ULONG STDMETHODCALLTYPE CHost::AddRef( void )
{
	++m_dwRefCount;
	return S_OK;
}

ULONG STDMETHODCALLTYPE CHost::Release( void )
{
	m_dwRefCount--;
	if(m_dwRefCount == 0)
	{
		delete this;
	}
	return S_OK;
}

CHost::CHost( HWND hwnd )
{
	
	m_pInPlaceSite = new CInPlaceSite(this);
	m_pUIHandler = new CUIHandler(this);
	m_pWebEvent = new CWebEvent(this);
	m_pClientSite = new CClientSite(this, this->m_pUIHandler, m_pWebEvent );

	
	AddRef();
	m_hWnd = hwnd;
}

HRESULT STDMETHODCALLTYPE CHost::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject )
{
	if(riid   ==   IID_IUnknown)   
		*ppvObject   =   this;   
	else   if(riid   ==   IID_IOleClientSite)   
		*ppvObject   =   m_pClientSite;   
	else   if(riid   ==   IID_IOleInPlaceSite)   
		*ppvObject   =   m_pInPlaceSite;   
	else   if(riid   ==   DIID_DWebBrowserEvents2) 
		*ppvObject   =   m_pWebEvent;   
	else if (riid	 ==	  IID_IOleInPlaceFrame)
		*ppvObject   = m_pInPlaceSite->m_pFrame;

	if(*ppvObject   ==   NULL)   return   E_NOINTERFACE;   
	AddRef();   
	return   S_OK;   
}
