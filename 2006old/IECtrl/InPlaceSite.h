#pragma once
#include "oleidl.h"


class CHost;


class CInPlaceSite :
	public IOleInPlaceSite
{
public:


	
	CInPlaceSite(CHost *pHost);
	CHost *m_Host;
	
	CInPlaceFrame *m_pFrame;

	/************************************************************************/
	/*  IUnknown                                                            */
	/************************************************************************/
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef( void);

	virtual ULONG STDMETHODCALLTYPE Release( void);


	/************************************************************************/
	/*  IOleWindow                                                          */
	/************************************************************************/


	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE GetWindow( 
		/* [out] */ HWND *phwnd);

	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( 
		/* [in] */ BOOL fEnterMode);

	
	/************************************************************************/
	/* OleInPlaceSite                                                       */
	/************************************************************************/
	
	
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate( void) ;

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate( void) ;

	virtual HRESULT STDMETHODCALLTYPE OnUIActivate( void) ;

	virtual HRESULT STDMETHODCALLTYPE GetWindowContext( 
		/* [out] */ IOleInPlaceFrame **ppFrame,
		/* [out] */ IOleInPlaceUIWindow **ppDoc,
		/* [out] */ LPRECT lprcPosRect,
		/* [out] */ LPRECT lprcClipRect,
		/* [out][in] */ LPOLEINPLACEFRAMEINFO lpFrameInfo) ;

	virtual HRESULT STDMETHODCALLTYPE Scroll( 
		/* [in] */ SIZE scrollExtant) ;

	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate( 
		/* [in] */ BOOL fUndoable) ;

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate( void) ;

	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState( void) ;

	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo( void) ;

	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange( 
		/* [in] */ LPCRECT lprcPosRect) ;



public:
	~CInPlaceSite(void);
};
