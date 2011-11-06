#pragma once
#include "oleidl.h"

class CInPlaceFrame :
	public IOleInPlaceFrame
{
public:
	CInPlaceFrame(CHost *pHost);
	CHost *m_pHost;

	virtual HRESULT STDMETHODCALLTYPE InsertMenus( 
		/* [in] */ HMENU hmenuShared,
		/* [out][in] */ LPOLEMENUGROUPWIDTHS lpMenuWidths);

	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE SetMenu( 
		/* [in] */ HMENU hmenuShared,
		/* [in] */ HOLEMENU holemenu,
		/* [in] */ HWND hwndActiveObject);

	virtual HRESULT STDMETHODCALLTYPE RemoveMenus( 
		/* [in] */ HMENU hmenuShared);

	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE SetStatusText( 
		/* [unique][in] */ LPCOLESTR pszStatusText);

	virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
		/* [in] */ BOOL fEnable);

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
		/* [in] */ LPMSG lpmsg,
		/* [in] */ WORD wID);



	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE GetBorder( 
		/* [out] */ LPRECT lprectBorder) ;

	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE RequestBorderSpace( 
		/* [unique][in] */ LPCBORDERWIDTHS pborderwidths) ;

	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE SetBorderSpace( 
		/* [unique][in] */ LPCBORDERWIDTHS pborderwidths) ;

	virtual HRESULT STDMETHODCALLTYPE SetActiveObject( 
		/* [unique][in] */ IOleInPlaceActiveObject *pActiveObject,
		/* [unique][string][in] */ LPCOLESTR pszObjName) ;



	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE GetWindow( 
		/* [out] */ HWND *phwnd);

	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( 
		/* [in] */ BOOL fEnterMode);


	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef( void);

	virtual ULONG STDMETHODCALLTYPE Release( void);






public:
	~CInPlaceFrame(void);
};
