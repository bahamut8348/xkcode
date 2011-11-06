#include "StdAfx.h"
#include "InPlaceFrame.h"


CInPlaceFrame::CInPlaceFrame( CHost *pHost ):m_pHost(pHost)
{
	
}
CInPlaceFrame::~CInPlaceFrame(void)
{
}

HRESULT STDMETHODCALLTYPE CInPlaceFrame::InsertMenus( /* [in] */ HMENU hmenuShared, /* [out][in] */ LPOLEMENUGROUPWIDTHS lpMenuWidths )
{
	NOTIMPLEMENTED;
}

/* [input_sync] */ HRESULT STDMETHODCALLTYPE CInPlaceFrame::SetMenu( /* [in] */ HMENU hmenuShared, /* [in] */ HOLEMENU holemenu, /* [in] */ HWND hwndActiveObject )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CInPlaceFrame::RemoveMenus( /* [in] */ HMENU hmenuShared )
{
	NOTIMPLEMENTED;
}

/* [input_sync] */ HRESULT STDMETHODCALLTYPE CInPlaceFrame::SetStatusText( /* [unique][in] */ LPCOLESTR pszStatusText )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CInPlaceFrame::EnableModeless( /* [in] */ BOOL fEnable )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CInPlaceFrame::TranslateAccelerator( /* [in] */ LPMSG lpmsg, /* [in] */ WORD wID )
{
	NOTIMPLEMENTED;
}

/* [input_sync] */ HRESULT STDMETHODCALLTYPE CInPlaceFrame::GetBorder( /* [out] */ LPRECT lprectBorder )
{
	NOTIMPLEMENTED;
}

/* [input_sync] */ HRESULT STDMETHODCALLTYPE CInPlaceFrame::RequestBorderSpace( /* [unique][in] */ LPCBORDERWIDTHS pborderwidths )
{
	NOTIMPLEMENTED;
}

/* [input_sync] */ HRESULT STDMETHODCALLTYPE CInPlaceFrame::SetBorderSpace( /* [unique][in] */ LPCBORDERWIDTHS pborderwidths )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CInPlaceFrame::SetActiveObject( /* [unique][in] */ IOleInPlaceActiveObject *pActiveObject, /* [unique][string][in] */ LPCOLESTR pszObjName )
{
	NOTIMPLEMENTED;
}

/* [input_sync] */ HRESULT STDMETHODCALLTYPE CInPlaceFrame::GetWindow( /* [out] */ HWND *phwnd )
{
	*phwnd = m_pHost->GetWindow();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CInPlaceFrame::ContextSensitiveHelp( /* [in] */ BOOL fEnterMode )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CInPlaceFrame::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject )
{
	return m_pHost->QueryInterface(riid, ppvObject);
}

ULONG STDMETHODCALLTYPE CInPlaceFrame::AddRef( void )
{
	return m_pHost->AddRef();
}

ULONG STDMETHODCALLTYPE CInPlaceFrame::Release( void )
{
	return m_pHost->Release();
}