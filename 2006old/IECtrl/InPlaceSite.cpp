#include "StdAfx.h"
#include "InPlaceSite.h"



CInPlaceSite::CInPlaceSite( CHost *pHost ) :m_Host(pHost)
{
	m_pFrame = new CInPlaceFrame(pHost);
}
CInPlaceSite::~CInPlaceSite(void)
{
}

HRESULT STDMETHODCALLTYPE CInPlaceSite::GetWindow( /* [out] */ HWND *phwnd )
{
	*phwnd = m_Host->GetWindow();
	return S_OK;
}




HRESULT STDMETHODCALLTYPE CInPlaceSite::ContextSensitiveHelp( /* [in] */ BOOL fEnterMode )
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE CInPlaceSite::CanInPlaceActivate( void )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CInPlaceSite::OnUIActivate( void )
{
	ATLTRACE2(L"Call OnUIActivate\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CInPlaceSite::GetWindowContext( /* [out] */ IOleInPlaceFrame **ppFrame, /* [out] */ IOleInPlaceUIWindow **ppDoc, /* [out] */ LPRECT lprcPosRect, /* [out] */ LPRECT lprcClipRect, /* [out][in] */ LPOLEINPLACEFRAMEINFO lpFrameInfo )
{
	NOTIMPLEMENTED;

}

HRESULT STDMETHODCALLTYPE CInPlaceSite::Scroll( /* [in] */ SIZE scrollExtant )
{
	NOTIMPLEMENTED;

}

HRESULT STDMETHODCALLTYPE CInPlaceSite::OnUIDeactivate( /* [in] */ BOOL fUndoable )
{
	NOTIMPLEMENTED;

}

HRESULT STDMETHODCALLTYPE CInPlaceSite::OnInPlaceDeactivate( void )
{
	NOTIMPLEMENTED;

}

HRESULT STDMETHODCALLTYPE CInPlaceSite::DiscardUndoState( void )
{
	NOTIMPLEMENTED;

}

HRESULT STDMETHODCALLTYPE CInPlaceSite::DeactivateAndUndo( void )
{
	NOTIMPLEMENTED;

}

HRESULT STDMETHODCALLTYPE CInPlaceSite::OnPosRectChange( /* [in] */ LPCRECT lprcPosRect )
{
	NOTIMPLEMENTED;

}

HRESULT STDMETHODCALLTYPE CInPlaceSite::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject )
{
	return m_Host->QueryInterface(riid, ppvObject);
}

ULONG STDMETHODCALLTYPE CInPlaceSite::AddRef( void )
{
	return m_Host->AddRef();
}

ULONG STDMETHODCALLTYPE CInPlaceSite::Release( void )
{
	return m_Host->Release();
}

HRESULT STDMETHODCALLTYPE CInPlaceSite::OnInPlaceActivate( void )
{
	NOTIMPLEMENTED;
}