/////////////////////////////////////////////////////////////////
//             Written by Alexander Shilonosov                      
//                     November,2003                                
//                   shilo@dekart.com                               
//               Last Update: November, 2003                        
/////////////////////////////////////////////////////////////////


#if !defined(AFX_HTML_HOST_HANDLERS_H__149A817D_A56C_4C5E_96F0_836452004104__INCLUDED_)
#define AFX_HTML_HOST_HANDLERS_H__149A817D_A56C_4C5E_96F0_836452004104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mshtmhst.h>
#include <mshtml.h>

class CHtml_Host_Handlers  
{
public:
	CHtml_Host_Handlers();
	~CHtml_Host_Handlers();

	
protected:
// IDocHostUIHandler overridables
	 virtual HRESULT OnDocWindowActivate(BOOL fActivate);
	 virtual HRESULT OnEnableModeless(BOOL fActivate);
	 virtual HRESULT OnFilterDataObject(IDataObject * pDataObject,
		IDataObject ** ppDataObject);
	 virtual HRESULT OnFrameWindowActivate(BOOL fActivate);
	 virtual HRESULT OnGetDropTarget(IDropTarget * pDroptarget,
		IDropTarget ** ppDropTarget);
	virtual HRESULT OnGetExternal(IDispatch ** lppDispatch);
	 virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO * pInfo);
	 virtual HRESULT OnGetOptionKeyPath(LPOLESTR * pchKey,
		DWORD dwReserved);
	 virtual HRESULT OnHideUI();
	 virtual HRESULT OnResizeBorder(LPCRECT prcBorder,
		IOleInPlaceUIWindow * pUIWindow,
		BOOL fFrameWindow);
	 virtual HRESULT OnShowContextMenu(DWORD dwID,
		LPPOINT ppt,
		IUnknown * pcmdtReserved,
		IDispatch * pdispReserved);
	 virtual HRESULT OnShowUI(DWORD dwID,
		IOleInPlaceActiveObject * pActiveObject,
		IOleCommandTarget * pCommandTarget,
		IOleInPlaceFrame * pFrame,
		IOleInPlaceUIWindow * pDoc);
	 virtual HRESULT OnTranslateAccelerator(LPMSG lpMsg,
		const GUID * pguidCmdGroup,
		DWORD nCmdID);
	 virtual HRESULT OnTranslateUrl(DWORD dwTranslate,
		OLECHAR * pchURLIn,
		OLECHAR ** pchURLOut);
	 virtual HRESULT OnUpdateUI();

#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher

	// IDocHostUIHandler2 overrideables
	 virtual HRESULT OnGetOverrideKeyPath(LPOLESTR * pchKey,
		DWORD dw);
#endif

	// IDocHostShowUI overridables
	 virtual HRESULT OnShowHelp(HWND hwnd,
		LPOLESTR pszHelpFile,
		UINT uCommand,
		DWORD dwData,
		POINT ptMouse,
		IDispatch * pDispatchObjectHit);

	 virtual HRESULT OnShowMessage(HWND hwnd,
		LPOLESTR lpstrText,
		LPOLESTR lpstrCaption,
		DWORD dwType,
		LPOLESTR lpstrHelpFile,
		DWORD dwHelpContext,
		LRESULT * plResult);


	 virtual void _onHtmlCmd(UINT cmd, LPCTSTR params);
	 virtual void _onDocumentComplete();
	
/*	
#ifdef _DOCHOSTSITE_H_
	friend class CDocHostSite::XDocHostShowUI;
	friend class CDocHostSite::XDocHostUIHandler;
#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher
	friend class CDocHostSite::XDocHostUIHandler2;
#endif	
#endif
*/

#ifdef _HTMLCTL_H_
	friend class CHtmlCtrl;
#endif
	
	
};

#endif // !defined(AFX_HTML_HOST_HANDLERS_H__149A817D_A56C_4C5E_96F0_836452004104__INCLUDED_)
