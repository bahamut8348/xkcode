// DocHostSite.h: interface for the CDocHostSite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_DOCHOSTSITE_H_)
#define _DOCHOSTSITE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDocHostSite : public COleControlSite  
{
public:
	CDocHostSite(COleControlContainer * pOCC);
	virtual ~CDocHostSite();

private:
	class CHtml_Host_Handlers * m_pView;

public:
	void SetView(class CHtml_Host_Handlers * pView)
	{
		m_pView = pView;
	}

// IDocHostShowUI
protected:
	BEGIN_INTERFACE_PART(DocHostShowUI, IDocHostShowUI)
		INIT_INTERFACE_PART(CDocHostSite, DocHostShowUI)
		STDMETHOD(ShowHelp)(
			/* [in ] */	HWND hwnd,
			/* [in ] */	LPOLESTR pszHelpFile,
			/* [in ] */	UINT uCommand,
			/* [in ] */	DWORD dwData,
			/* [in ] */	POINT ptMouse,
			/* [out] */	IDispatch __RPC_FAR *pDispatchObjectHit);
		STDMETHOD(ShowMessage)(
			/* [in ] */	HWND hwnd,
			/* [in ] */	LPOLESTR lpstrText,
			/* [in ] */	LPOLESTR lpstrCaption,
			/* [in ] */	DWORD dwType,
			/* [in ] */	LPOLESTR lpstrHelpFile,
			/* [in ] */	DWORD dwHelpContext,
			/* [out] */	LRESULT __RPC_FAR *plResult);
	END_INTERFACE_PART(DocHostShowUI)

// IDocHostUIHandler
protected:
	BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
		STDMETHOD(ShowContextMenu)(/* [in] */ DWORD dwID,
			/* [in] */ POINT __RPC_FAR *ppt,
			/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
			/* [in] */ IDispatch __RPC_FAR *pdispReserved);
		STDMETHOD(GetHostInfo)( 
			/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
		STDMETHOD(ShowUI)( 
			/* [in] */ DWORD dwID,
			/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
			/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
			/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
			/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
		STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
		STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
		STDMETHOD(ResizeBorder)( 
			/* [in] */ LPCRECT prcBorder,
			/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
			/* [in] */ BOOL fRameWindow);
		STDMETHOD(TranslateAccelerator)( 
			/* [in] */ LPMSG lpMsg,
			/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
			/* [in] */ DWORD nCmdID);
		STDMETHOD(GetOptionKeyPath)( 
			/* [out] */ LPOLESTR __RPC_FAR *pchKey,
			/* [in] */ DWORD dw);
		STDMETHOD(GetDropTarget)(
			/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
			/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
		STDMETHOD(GetExternal)( 
			/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
		STDMETHOD(TranslateUrl)( 
			/* [in] */ DWORD dwTranslate,
			/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
			/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
		STDMETHOD(FilterDataObject)( 
			/* [in] */ IDataObject __RPC_FAR *pDO,
			/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
	END_INTERFACE_PART(DocHostUIHandler)

#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher
// IDocHostUIHandler2
protected:
	BEGIN_INTERFACE_PART(DocHostUIHandler2, IDocHostUIHandler2)
		STDMETHOD(GetOverrideKeyPath)(
			/* [out] */ LPOLESTR __RPC_FAR *pchKey,
			/* [in] */ DWORD dw);
		STDMETHOD(ShowContextMenu)(/* [in] */ DWORD dwID,
			/* [in] */ POINT __RPC_FAR *ppt,
			/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
			/* [in] */ IDispatch __RPC_FAR *pdispReserved);
		STDMETHOD(GetHostInfo)( 
			/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
		STDMETHOD(ShowUI)( 
			/* [in] */ DWORD dwID,
			/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
			/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
			/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
			/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
		STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
		STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
		STDMETHOD(ResizeBorder)( 
			/* [in] */ LPCRECT prcBorder,
			/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
			/* [in] */ BOOL fRameWindow);
		STDMETHOD(TranslateAccelerator)( 
			/* [in] */ LPMSG lpMsg,
			/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
			/* [in] */ DWORD nCmdID);
		STDMETHOD(GetOptionKeyPath)( 
			/* [out] */ LPOLESTR __RPC_FAR *pchKey,
			/* [in] */ DWORD dw);
		STDMETHOD(GetDropTarget)(
			/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
			/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
		STDMETHOD(GetExternal)( 
			/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
		STDMETHOD(TranslateUrl)( 
			/* [in] */ DWORD dwTranslate,
			/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
			/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
		STDMETHOD(FilterDataObject)( 
			/* [in] */ IDataObject __RPC_FAR *pDO,
			/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
	END_INTERFACE_PART(DocHostUIHandler2)
#endif

	DECLARE_INTERFACE_MAP()
};

class CDocHostOccManager : public COccManager
{
public:
	CDocHostOccManager();

	virtual COleControlSite * CreateSite(COleControlContainer * pOCC);

private:
	class CHtml_Host_Handlers * m_pView;
	CDocHostSite * m_pSite;

public:
	void SetView(class CHtml_Host_Handlers * pView)
	{
		m_pView = pView;
	}
};

#endif // !defined(_DOCHOSTSITE_H_)
