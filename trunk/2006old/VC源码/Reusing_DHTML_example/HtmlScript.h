// HtmlScript.h: interface for the CHtmlScript class.
//
//////////////////////////////////////////////////////////////////////

// Original code comments:
/////////////////////////////////////////////////////////////////
//             By Eugene Khodakovsky                           //
//                  April,2002                                 //
//             Eugene@cpplab.com                               //
//            Last Update: April, 2002                         //
/////////////////////////////////////////////////////////////////


#if !defined(AFX_HTMLSCRIPT_H__48E169B0_7344_4131_9060_6B3E951B582A__INCLUDED_)
#define AFX_HTMLSCRIPT_H__48E169B0_7344_4131_9060_6B3E951B582A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include <Mshtml.h>

class CHtmlScript  
{
public:
	CHtmlScript();
	~CHtmlScript();

	bool SetHtmlScript(IDispatch* pDisp);
	//LPDISPATCH GetHtmlDocument() const;
	const CString GetScriptError() const;	

	// this is enumerations functions
	//
	// bool GetJScripts(CComPtr<IHTMLElementCollection>& spColl);
	// CString ScanJScript(CString& strAText, CStringArray& args);

	bool CallJScript(const CString strFunc,CString *sResult = NULL);
	//bool CallJScript(const CString strFunc,CString *sResult , ...);
	bool CallJScript(const CString strFunc,const CStringArray& paramArray, CString *sResult = NULL);

	// passes all parameers as VBasic SafeArray (with BSTR)
	bool CallJScript2(const CString strFunc, const CStringArray& paramArray, CString *sResult = NULL);

protected:
	void ShowError(LPCSTR lpszText);
	bool GetJScript(CComPtr<IDispatch>& spDisp);

protected:
	
	CComPtr<IDispatch> spScript;
	CString	m_strError;

};

inline
void CHtmlScript::ShowError(LPCSTR lpszText)
{	
	m_strError = "Applicaion script error:\n" + CString(lpszText);
	AfxMessageBox( lpszText, MB_ICONSTOP );
}
inline 
const CString CHtmlScript::GetScriptError() const
{
	return m_strError;
}

CString GetNextToken(CString& strSrc, const CString strDelim,bool bTrim = false, bool bFindOneOf = true);

#endif // !defined(AFX_HTMLSCRIPT_H__48E169B0_7344_4131_9060_6B3E951B582A__INCLUDED_)
