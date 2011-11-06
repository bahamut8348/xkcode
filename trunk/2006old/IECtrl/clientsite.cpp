#include "stdafx.h"



HRESULT STDMETHODCALLTYPE CClientSite::SaveObject( void )
{
	ATLTRACE2(L"Call Save Object\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CClientSite::GetMoniker( /* [in] */ DWORD dwAssign, /* [in] */ DWORD dwWhichMoniker, /* [out] */ IMoniker **ppmk ) /*= 0 */
{
	ATLTRACE2(L"Call GetMoniker\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CClientSite::GetContainer( /* [out] */ IOleContainer **ppContainer ) /*= 0 */
{
	ATLTRACE2(L"Call GetContainer\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CClientSite::ShowObject( void ) /*= 0*/
{
	ATLTRACE2(L"Call ShowObjectt\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CClientSite::OnShowWindow( /* [in] */ BOOL fShow ) /*= 0 */
{
	ATLTRACE2(L"Call OnShowWindow\n");
	return S_OK;

}

HRESULT STDMETHODCALLTYPE CClientSite::RequestNewObjectLayout( void ) /*= 0*/
{
	ATLTRACE2(L"Call RequestNewObjectLayout\n");
	return S_OK;
 }

HRESULT STDMETHODCALLTYPE CClientSite::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject )
{
	return m_pHost->QueryInterface(riid, ppvObject);
	
}

ULONG STDMETHODCALLTYPE CClientSite::AddRef( void )
{
	
	return m_pHost->AddRef(); ;
}

ULONG STDMETHODCALLTYPE CClientSite::Release( void )
{
	return	m_pHost->Release();

}

CClientSite::CClientSite( CHost *pHost,IDocHostUIHandler * doc, DWebBrowserEvents2* event )
: m_pHost(pHost), m_doc_host_ui_handler(doc), m_web_browser_events(event)
{
	
}

CClientSite::CClientSite( CHost *pHost):m_pHost(pHost)
{

}
																							   
																																	

