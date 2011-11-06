#pragma once 

class CHost;

class CClientSite: public IOleClientSite
{
public:
	
	IDocHostUIHandler * m_doc_host_ui_handler;
	DWebBrowserEvents2* m_web_browser_events;

	CClientSite(CHost *pHost);
	CClientSite(CHost *pHost,IDocHostUIHandler * doc,  DWebBrowserEvents2* event);
	CHost *m_pHost;
	

	/************************************************************************/
	/* IUnknow Interface                                                    */
	/************************************************************************/
	
	HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG STDMETHODCALLTYPE AddRef( void);

	ULONG STDMETHODCALLTYPE Release( void);

	
	/************************************************************************/
	/* IOleClientSite Interface                                             */
	/************************************************************************/
	
	virtual HRESULT STDMETHODCALLTYPE SaveObject( void);

	virtual HRESULT STDMETHODCALLTYPE GetMoniker( 
		/* [in] */ DWORD dwAssign,
		/* [in] */ DWORD dwWhichMoniker,
		/* [out] */ IMoniker **ppmk);

	virtual HRESULT STDMETHODCALLTYPE GetContainer( 
		/* [out] */ IOleContainer **ppContainer);

	virtual HRESULT STDMETHODCALLTYPE ShowObject( void);

	virtual HRESULT STDMETHODCALLTYPE OnShowWindow( 
		/* [in] */ BOOL fShow);

	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout( void);

	
};