// DocHostSite.cpp: implementation of the CDocHostSite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#if _MFC_VER >= 0x0700
#include <afxocc.h>
#else
#ifdef _AFXDLL
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT
#endif
#include <..\src\occimpl.h>
#ifdef _AFXDLL
#undef AFX_DATA
#define AFX_DATA AFX_DATA_EXPORT
#endif
#endif

#include <mshtmhst.h>

#include "DocHostSite.h"
//#include "HtmlDialog.h"
#include "Html_Host_Handlers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_INTERFACE_MAP(CDocHostSite, COleControlSite)
	INTERFACE_PART(CDocHostSite, IID_IDocHostShowUI, DocHostShowUI)
	INTERFACE_PART(CDocHostSite, IID_IDocHostUIHandler, DocHostUIHandler)
#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher
	INTERFACE_PART(CDocHostSite, IID_IDocHostUIHandler2, DocHostUIHandler2)
#endif
END_INTERFACE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDocHostSite::CDocHostSite(COleControlContainer * pOcc)
	: COleControlSite( pOcc ),
	m_pView( NULL )
{
}

CDocHostSite::~CDocHostSite()
{
}

ULONG CDocHostSite::XDocHostShowUI::AddRef()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostShowUI);

	return pThis->ExternalAddRef();
}

ULONG CDocHostSite::XDocHostShowUI::Release()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostShowUI);

	return pThis->ExternalRelease();
}

HRESULT CDocHostSite::XDocHostShowUI::QueryInterface(REFIID riid, void ** ppvObj)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostShowUI);

	return pThis->ExternalQueryInterface( &riid, ppvObj );
}

HRESULT CDocHostSite::XDocHostShowUI::ShowHelp(HWND hwnd,
											   LPOLESTR pszHelpFile,
											   UINT nCommand,
											   DWORD dwData,
											   POINT ptMouse,
											   IDispatch * pDispatchObjectHit)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostShowUI);

	return pThis->m_pView->OnShowHelp( hwnd, pszHelpFile, nCommand, dwData, ptMouse,
		pDispatchObjectHit );
}

HRESULT CDocHostSite::XDocHostShowUI::ShowMessage(HWND hwnd,
												  LPOLESTR lpstrText,
												  LPOLESTR lpstrCaption,
												  DWORD dwType,
												  LPOLESTR lpstrHelpFile,
												  DWORD dwHelpContext,
												  LRESULT * plResult)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostShowUI);

	return pThis->m_pView->OnShowMessage( hwnd, lpstrText, lpstrCaption, dwType,
		lpstrHelpFile, dwHelpContext, plResult );
}

ULONG CDocHostSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->ExternalAddRef();
}

ULONG CDocHostSite::XDocHostUIHandler::Release()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->ExternalRelease();
}

HRESULT CDocHostSite::XDocHostUIHandler::QueryInterface(REFIID riid, void ** ppvObj)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->ExternalQueryInterface( &riid, ppvObj );
}

HRESULT CDocHostSite::XDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO * pInfo)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnGetHostInfo( pInfo );
}

HRESULT CDocHostSite::XDocHostUIHandler::ShowUI(DWORD dwID,
											  IOleInPlaceActiveObject * pActiveObject,
											  IOleCommandTarget * pCommandTarget,
											  IOleInPlaceFrame * pFrame,
											  IOleInPlaceUIWindow * pDoc)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnShowUI( dwID, pActiveObject, pCommandTarget,
		pFrame, pDoc );
}

HRESULT CDocHostSite::XDocHostUIHandler::HideUI()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnHideUI();
}

HRESULT CDocHostSite::XDocHostUIHandler::UpdateUI()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnUpdateUI();
}

HRESULT CDocHostSite::XDocHostUIHandler::EnableModeless(BOOL fEnable)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnEnableModeless( fEnable );
}

HRESULT CDocHostSite::XDocHostUIHandler::OnDocWindowActivate(BOOL fEnable)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnDocWindowActivate( fEnable );
}

HRESULT CDocHostSite::XDocHostUIHandler::OnFrameWindowActivate(BOOL fEnable)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnFrameWindowActivate( fEnable );
}

HRESULT CDocHostSite::XDocHostUIHandler::ResizeBorder(LPCRECT prcBorder,
													IOleInPlaceUIWindow * pUIWindow,
													BOOL fFrameWindow)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnResizeBorder( prcBorder, pUIWindow, fFrameWindow );
}

HRESULT CDocHostSite::XDocHostUIHandler::ShowContextMenu(DWORD dwID,
													   POINT * ppt,
													   IUnknown * pcmdtReserved,
													   IDispatch * pdispReserved)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnShowContextMenu( dwID, ppt, pcmdtReserved,
		pdispReserved );
}

HRESULT CDocHostSite::XDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg,
															const GUID * pguidCmdGroup,
															DWORD nCmdID)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnTranslateAccelerator( lpMsg,
		pguidCmdGroup, nCmdID );
}

HRESULT CDocHostSite::XDocHostUIHandler::GetOptionKeyPath(LPOLESTR * pchKey,
														DWORD dw)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnGetOptionKeyPath( pchKey, dw );
}

HRESULT CDocHostSite::XDocHostUIHandler::GetDropTarget(IDropTarget * pDropTarget,
													 IDropTarget ** ppDropTarget)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnGetDropTarget( pDropTarget, ppDropTarget );
}

HRESULT CDocHostSite::XDocHostUIHandler::GetExternal(IDispatch ** ppDispatch)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnGetExternal( ppDispatch );
}

HRESULT CDocHostSite::XDocHostUIHandler::TranslateUrl(DWORD dwTranslate,
													OLECHAR * pchURLIn,
													OLECHAR ** ppchURLOut)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnTranslateUrl( dwTranslate, pchURLIn, ppchURLOut );
}

HRESULT CDocHostSite::XDocHostUIHandler::FilterDataObject(IDataObject * pDO,
														IDataObject ** ppDORet)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler);
	return pThis->m_pView->OnFilterDataObject( pDO, ppDORet );
}

#if (_WIN32_IE >= 0x0501) // IE 5.5 and higher

ULONG CDocHostSite::XDocHostUIHandler2::AddRef()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler2);

	return pThis->ExternalAddRef();
}

ULONG CDocHostSite::XDocHostUIHandler2::Release()
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler2);

	return pThis->ExternalRelease();
}

HRESULT CDocHostSite::XDocHostUIHandler2::QueryInterface(REFIID riid, void ** ppvObj)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler2);

	return pThis->ExternalQueryInterface( &riid, ppvObj );
}

HRESULT CDocHostSite::XDocHostUIHandler2::GetHostInfo(DOCHOSTUIINFO *)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::ShowUI(DWORD /*dwID*/, 
												 IOleInPlaceActiveObject * /*pActiveObject*/,
												 IOleCommandTarget * /*pCommandTarget*/,
												 IOleInPlaceFrame * /*pFrame*/,
												 IOleInPlaceUIWindow * /*pDoc*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::HideUI()
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::UpdateUI()
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::EnableModeless(BOOL /*fEnable*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::OnDocWindowActivate(BOOL /*fActivate*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::OnFrameWindowActivate(BOOL /*fActivate*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::ResizeBorder(LPCRECT /*prcBorder*/, 
													   IOleInPlaceUIWindow* /*pUIWindow*/,
													   BOOL /*fRameWindow*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::ShowContextMenu(DWORD /*dwID*/, 
														  POINT* /*pptPosition*/,
														  IUnknown* /*pCommandTarget*/,
														  IDispatch* /*pDispatchObjectHit*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::TranslateAccelerator(LPMSG /*lpMsg*/,
															   const GUID * /*pguidCmdGroup*/,
															   DWORD /*nCmdID*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::GetOptionKeyPath(BSTR* /*pbstrKey*/, DWORD)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::GetDropTarget(IDropTarget * /*pDropTarget*/,
														IDropTarget ** /*ppDropTarget*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::GetExternal(IDispatch ** /*ppDispatch*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::TranslateUrl(DWORD /*dwTranslate*/,
													   OLECHAR * /*pchURLIn*/,
													   OLECHAR ** /*ppchURLOut*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::FilterDataObject(IDataObject * /*pDO*/,
														   IDataObject ** /*ppDORet*/)
{
	ASSERT(FALSE);

	return S_FALSE;
}

HRESULT CDocHostSite::XDocHostUIHandler2::GetOverrideKeyPath(LPOLESTR * pchKey, DWORD dw)
{
	METHOD_PROLOGUE(CDocHostSite, DocHostUIHandler2);

	return pThis->m_pView->OnGetOverrideKeyPath( pchKey, dw );
}

#endif

CDocHostOccManager::CDocHostOccManager()
	: m_pView( NULL ),
	m_pSite( NULL )
{
}

COleControlSite * CDocHostOccManager::CreateSite(COleControlContainer * pOcc)
{
	m_pSite = new CDocHostSite( pOcc );
	m_pSite->SetView( m_pView );
	return m_pSite;
}