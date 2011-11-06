// All event rountines reside here ...
//
//

/////////////////////////////////////////////////////////////////
//             Written by Alexander Shilonosov                      
//                     November,2003                                
//                   shilo@dekart.com                               
//               Last Update: November, 2003                        
/////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include <tchar.h>

#include "Html_Host_Handlers.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtml_Host_Handlers::CHtml_Host_Handlers()
{

}

CHtml_Host_Handlers::~CHtml_Host_Handlers()
{

}

// IDocHostUIHandler overridables

HRESULT CHtml_Host_Handlers::OnDocWindowActivate(BOOL)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnEnableModeless(BOOL)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnFilterDataObject(IDataObject *, IDataObject ** ppDataObject)
{
	*ppDataObject = NULL;

	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnFrameWindowActivate(BOOL)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnGetDropTarget(IDropTarget *, IDropTarget **)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnGetExternal(IDispatch ** lppDispatch)
{
	*lppDispatch = NULL;

	return E_NOTIMPL;
}

// 
//
HRESULT CHtml_Host_Handlers::OnGetHostInfo(DOCHOSTUIINFO * pInfo)
{
	pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER |  // remove border
					DOCHOSTUIFLAG_SCROLL_NO |  // disable scroll bar
						0x00040000; //DOCHOSTUIFLAG_THEME;// enable WindowsXP Theme

	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	

	return S_OK;
}

// This event handler do not allow to WebBrowser to get settings
//  from registry path of IE.
//
HRESULT CHtml_Host_Handlers::OnGetOptionKeyPath(LPOLESTR * pchKey, DWORD dwReserved)
{
	//return E_NOTIMPL;
	
    HRESULT hr;
    WCHAR* szKey = L"Software\\CHtmlDialog";

    if (pchKey)
    {
        *pchKey = (LPOLESTR)CoTaskMemAlloc((wcslen(szKey) + 1) * sizeof(WCHAR));
        if (*pchKey)
            wcscpy(*pchKey, szKey);
        hr = (*pchKey) ? S_OK : E_OUTOFMEMORY;
    }
    else
        hr = E_INVALIDARG;

    return hr;

}

HRESULT CHtml_Host_Handlers::OnHideUI()
{
	return S_OK;
}

HRESULT CHtml_Host_Handlers::OnResizeBorder(LPCRECT, IOleInPlaceUIWindow *, BOOL)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnShowContextMenu(DWORD, LPPOINT, IUnknown *, IDispatch *)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnShowUI(DWORD, IOleInPlaceActiveObject *,
								   IOleCommandTarget *, IOleInPlaceFrame *,
								   IOleInPlaceUIWindow *)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnTranslateAccelerator(LPMSG, const GUID *, DWORD)
{
	return S_FALSE;
}

HRESULT CHtml_Host_Handlers::OnTranslateUrl(DWORD, OLECHAR *, OLECHAR **)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnUpdateUI()
{
	return S_OK;
}

#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher

HRESULT CHtml_Host_Handlers::OnGetOverrideKeyPath(LPOLESTR *, DWORD)
{
	return E_NOTIMPL;
}

#endif

HRESULT CHtml_Host_Handlers::OnShowHelp(HWND, LPOLESTR, UINT,
									 DWORD, POINT, IDispatch *)
{
	return E_NOTIMPL;
}

HRESULT CHtml_Host_Handlers::OnShowMessage(HWND hwnd,
		LPOLESTR lpstrText,
		LPOLESTR lpstrCaption,
		DWORD dwType,
		LPOLESTR lpstrHelpFile,
		DWORD dwHelpContext,
		LRESULT * plResult)
{
	
    *plResult = MessageBoxW(NULL, (LPWSTR)lpstrText, L"Caption", dwType);
    
    return S_OK;


	return E_NOTIMPL;
}

//
// Purpose:
// Send a message with parameters from Html Script to CHtmlDialog 
//
//
// Event scenario :
// 1. user press button (or other element) in HTML. 
// 3. this action goto to special event handler in Html Script (written by you) ...
// 4. HTML::onBtnPress_event() script function goto "app:%cmd%@%params%" URL 
//		 %cmd% and %params% stated in onBtnPress_event() script 
// 5. CHtmlCtrl::OnBeforeNavigate() get this url, cancels navigation, 
//		and pass %cmd% %params% to this function.
// 
// CHtmlDialog default actions on %cmd%@%params%... :
// - 1001@cx@cy - Script want to Resize HtmlDialog (increasing area)
// - 1002@cx@cy - Script want to Resize HtmlDialog (decreasing area)
//
void CHtml_Host_Handlers::_onHtmlCmd(UINT cmd, LPCTSTR params){
	return;
}

void CHtml_Host_Handlers::_onDocumentComplete(){
	return;
}

