#pragma  once 


class CHost  : IUnknown
{
public:

	

	CClientSite *m_pClientSite;
	CInPlaceSite *m_pInPlaceSite; 
	CUIHandler  *m_pUIHandler;
	CWebEvent *m_pWebEvent;
	HWND m_hWnd;
	
	LONG m_dwRefCount;

	HWND GetWindow() const;
	
	CHost(HWND hwnd);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef( void);

	virtual ULONG STDMETHODCALLTYPE Release( void);

};