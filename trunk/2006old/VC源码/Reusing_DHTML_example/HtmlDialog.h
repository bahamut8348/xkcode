/////////////////////////////////////////////////////////////////
//             Written by Alexander Shilonosov                      
//                     November,2003                                
//                   shilo@dekart.com                               
//               Last Update: November, 2003                        
/////////////////////////////////////////////////////////////////


#if !defined(AFX_HTMLDIALOG_H__49179C49_F72B_49DE_8A34_8DD5762798DC__INCLUDED_)
#define AFX_HTMLDIALOG_H__49179C49_F72B_49DE_8A34_8DD5762798DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlDialog.h : header file
//

#include "HtmlCtrl.h"
#include "Html_Host_Handlers.h"
//#include "dhtml2.h"

/////////////////////////////////////////////////////////////////////////////
// CHtmlDialog dialog
#define CMD_DELI "@"

class CHtmlDialog : public CDialog, public CHtml_Host_Handlers
{
// Construction
public:
	
	CHtmlDialog(UINT nID_Dlg,  CWnd* pParent,  UINT nID_HTML, UINT n_ID_static);   

	CHtmlCtrl m_HtmlCtrl;
	UINT m_ID_HTML, m_ID_Static;	

	// Dialog Data
	//{{AFX_DATA(CHtmlDialog)
	//enum { IDD = IDD_DHTML2_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlDialog)
	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL		

	virtual void _onHtmlCmd(UINT cmd, LPCTSTR params);
	virtual void _onDocumentComplete();
			
	/*
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
*/
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHtmlDialog)
		virtual BOOL OnInitDialog();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG

	 DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLDIALOG_H__49179C49_F72B_49DE_8A34_8DD5762798DC__INCLUDED_)
