#pragma once
#include "unknwn.h"

class CHost;

class CUIHandler :
	public IDocHostUIHandler
{
public:
	CUIHandler(CHost *pHost);
	CHost *m_pHost;



	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject) ;

	virtual ULONG STDMETHODCALLTYPE AddRef( void) ;

	virtual ULONG STDMETHODCALLTYPE Release( void);





	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( 
		/* [in] */ DWORD dwID,
		/* [in] */ POINT *ppt,
		/* [in] */ IUnknown *pcmdtReserved,
		/* [in] */ IDispatch *pdispReserved) ;

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( 
		/* [out][in] */ DOCHOSTUIINFO *pInfo) ;

	virtual HRESULT STDMETHODCALLTYPE ShowUI( 
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject *pActiveObject,
		/* [in] */ IOleCommandTarget *pCommandTarget,
		/* [in] */ IOleInPlaceFrame *pFrame,
		/* [in] */ IOleInPlaceUIWindow *pDoc) ;

	virtual HRESULT STDMETHODCALLTYPE HideUI( void) ;

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void) ;

	virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
		/* [in] */ BOOL fEnable) ;

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
		/* [in] */ BOOL fActivate) ;

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
		/* [in] */ BOOL fActivate) ;

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( 
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow *pUIWindow,
		/* [in] */ BOOL fRameWindow) ;

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID *pguidCmdGroup,
		/* [in] */ DWORD nCmdID) ;

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( 
		/* [out] */ LPOLESTR *pchKey,
		/* [in] */ DWORD dw) ;

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( 
		/* [in] */ IDropTarget *pDropTarget,
		/* [out] */ IDropTarget **ppDropTarget) ;

	virtual HRESULT STDMETHODCALLTYPE GetExternal( 
		/* [out] */ IDispatch **ppDispatch) ;

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( 
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR *pchURLIn,
		/* [out] */ OLECHAR **ppchURLOut) ;

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject( 
		/* [in] */ IDataObject *pDO,
		/* [out] */ IDataObject **ppDORet) ;


public:
	~CUIHandler(void);
};
