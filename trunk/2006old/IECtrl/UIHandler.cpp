#include "StdAfx.h"
#include "UIHandler.h"

CUIHandler::CUIHandler(CHost *pHost) : m_pHost(pHost)
{
}

CUIHandler::~CUIHandler(void)
{
}

HRESULT STDMETHODCALLTYPE CUIHandler::ShowContextMenu( /* [in] */ DWORD dwID, /* [in] */ POINT *ppt, /* [in] */ IUnknown *pcmdtReserved, /* [in] */ IDispatch *pdispReserved )
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::GetHostInfo( /* [out][in] */ DOCHOSTUIINFO *pInfo )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::ShowUI( /* [in] */ DWORD dwID, /* [in] */ IOleInPlaceActiveObject *pActiveObject, /* [in] */ IOleCommandTarget *pCommandTarget, /* [in] */ IOleInPlaceFrame *pFrame, /* [in] */ IOleInPlaceUIWindow *pDoc )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::HideUI( void )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::UpdateUI( void )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::EnableModeless( /* [in] */ BOOL fEnable )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::OnDocWindowActivate( /* [in] */ BOOL fActivate )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::OnFrameWindowActivate( /* [in] */ BOOL fActivate )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::ResizeBorder( /* [in] */ LPCRECT prcBorder, /* [in] */ IOleInPlaceUIWindow *pUIWindow, /* [in] */ BOOL fRameWindow )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::TranslateAccelerator( /* [in] */ LPMSG lpMsg, /* [in] */ const GUID *pguidCmdGroup, /* [in] */ DWORD nCmdID )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::GetOptionKeyPath( /* [out] */ LPOLESTR *pchKey, /* [in] */ DWORD dw )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::GetDropTarget( /* [in] */ IDropTarget *pDropTarget, /* [out] */ IDropTarget **ppDropTarget )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::GetExternal( /* [out] */ IDispatch **ppDispatch )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::TranslateUrl( /* [in] */ DWORD dwTranslate, /* [in] */ OLECHAR *pchURLIn, /* [out] */ OLECHAR **ppchURLOut )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::FilterDataObject( /* [in] */ IDataObject *pDO, /* [out] */ IDataObject **ppDORet )
{
NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE CUIHandler::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject )
{
	return m_pHost->QueryInterface(riid, ppvObject);

}

ULONG STDMETHODCALLTYPE CUIHandler::AddRef( void )
{
	return m_pHost->AddRef();
}

ULONG STDMETHODCALLTYPE CUIHandler::Release( void )
{
	return m_pHost->Release();
}