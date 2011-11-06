////////////////////////////////////////////////////////////////
// Microsoft Systems Journal -- December 1999
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//

#if !defined(_HTMLCTL_H_)
#define _HTMLCTL_H_

#include <mshtmhst.h>
#include <mshtml.h>
#include <afxhtml.h>			// MFC HTML view support

#include "Html_Host_Handlers.h"
#include "HtmlScript.h"
//

class CHtmlCtrl : public CHtmlView, public CHtmlScript {

public:

	int m_html_width,m_html_height ;
	CString m_html_Title;
	

	// HWND m_exWnd;	
	// CDialog* m_pParentDlg;
	CHtmlCtrl() : m_pOccManager( NULL ) { m_bCtl_mode=false; }
	~CHtmlCtrl() { }

	BOOL CreateFromStatic(UINT nID, CWnd* pParent, CHtml_Host_Handlers * pHtml_Host_Handlers);

	// Overrides
protected:
	class CHtml_Host_Handlers* m_pHtml_Host_Handlers;
	class CDocHostOccManager * m_pOccManager;

	// CHtmlView overrides
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName,
		DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd,
		UINT nID,
		CCreateContext* pContext = NULL);

#if _MFC_VER >= 0x0700
	virtual BOOL CreateControlSite(COleControlContainer* pContainer, 
	   COleControlSite** ppSite, UINT nID, REFCLSID clsid);
#endif


	// Normally, CHtmlView destroys itself in PostNcDestroy,
	// but we don't want to do that for a control since a control
	// is usually implemented as a stack object in a dialog.
	//
	virtual void PostNcDestroy() { if (!m_bCtl_mode) CHtmlView::PostNcDestroy(); }

	// overrides to bypass MFC doc/view frame dependencies
	afx_msg void OnDestroy();
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);


	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	// override to trap "app:" pseudo protocol
	virtual void OnBeforeNavigate2( LPCTSTR lpszURL,
		DWORD nFlags,
		LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData,
		LPCTSTR lpszHeaders,
		BOOL* pbCancel );

	// override to handle links to "app:mumble...". lpszWhere will be "mumble"
	virtual void OnAppCmd(LPCTSTR lpszWhere);	
	
	CComPtr<IHTMLDocument> m_pDocument;
	CComPtr<IHTMLDocument2> m_pDocument2;
	CComPtr<IHTMLElement> p_Body;

	DECLARE_MESSAGE_MAP();
	DECLARE_DYNAMIC(CHtmlCtrl)

private:
	bool m_bCtl_mode;
};

#endif