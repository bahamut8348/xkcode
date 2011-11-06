// HtmlScript.cpp: implementation of the CHtmlScript class.
//
//////////////////////////////////////////////////////////////////////

// Original code comments:
/////////////////////////////////////////////////////////////////
//             By Eugene Khodakovsky                           //
//                  April,2002                                 //
//             Eugene@cpplab.com                               //
//            Last Update: April, 2002                         //
/////////////////////////////////////////////////////////////////
//
// Added by Alexander Shilonosov:
//  CallJScript2() - passing arrays to Script Functions
//
//

#include "stdafx.h"
#include "dhtml2.h"
#include "HtmlScript.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtmlScript::CHtmlScript()
{

}

CHtmlScript::~CHtmlScript()
{

}



#define CHECK_POINTER(p)\
	ATLASSERT(p != NULL);\
	if(p == NULL)\
	{\
		ShowError("NULL pointer");\
		return false;\
	}

const CString GetSystemErrorMessage(DWORD dwError)
{
	CString strError;
	LPTSTR lpBuffer;

	if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,  dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL))

	{
		strError = "FormatMessage Netive Error" ;
	}
	else
	{
		strError = lpBuffer;
		LocalFree(lpBuffer);
	}
	return strError;
}

CString GetNextToken(CString& strSrc, const CString strDelim,bool bTrim, bool bFindOneOf)
{
	CString strToken;
	int idx = bFindOneOf? strSrc.FindOneOf(strDelim) : strSrc.Find(strDelim);
	if(idx != -1)
	{
		strToken  = strSrc.Left(idx);
		strSrc = strSrc.Right(strSrc.GetLength() - (idx + 1) );
	}
	else
	{
		strToken = strSrc;
		strSrc.Empty();
	}
	if(bTrim)
	{
		strToken.TrimLeft();
		strToken.TrimRight();
	}
	return strToken;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


bool CHtmlScript::SetHtmlScript(IDispatch* pDisp)
{
	CHECK_POINTER(pDisp);

	spScript = NULL;
	spScript = pDisp;	
	return true;
}
/*
bool CHtmlScript::GetJScript(CComPtr<IDispatch>& spDisp)
{
	CHECK_POINTER(m_spDoc);
	//HRESULT hr = m_spDoc->get_Script(&spDisp);
	//spDisp = 
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

bool CWebPage::GetJScripts(CComPtr<IHTMLElementCollection>& spColl)
{
	CHECK_POINTER(m_spDoc);
	HRESULT hr = m_spDoc->get_scripts(&spColl);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}
*/
bool CHtmlScript::CallJScript(const CString strFunc,CString *sResult)
{
	CStringArray paramArray;
	return CallJScript(strFunc,paramArray,sResult);
}

// all "..." parameters should be strings !!
/*bool CHtmlScript::CallJScript(const CString strFunc,CString *sResult , ...)
{
	CStringArray paramArray;
	return CallJScript(strFunc, paramArray, sResult);
}*/

// Call Script function and pass array in parameters 
//
// all "..." parameters should be strings !!
//
bool CHtmlScript::CallJScript2(const CString strFunc, const CStringArray& paramArray, CString *sResult)
{
	CComBSTR bstrMember(strFunc);
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrMember,1,
											LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		ShowError(GetSystemErrorMessage(hr));
		return false;
	}

	const int arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	SAFEARRAY * psa;
	VARIANT var;
	long idx[1];        

	memset(&dispparams, 0, sizeof dispparams);
	dispparams.rgvarg = new VARIANT;

	// Creating a SafeArray with 1 element.
   psa = SafeArrayCreateVector( VT_VARIANT, 0, arraySize);
   if(psa == NULL){
      return false;
   }   
	
	for( int i = 0; i < arraySize; i++)
	{
		VariantInit(&var);
		V_VT(&var) = VT_BSTR;
		CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&var.bstrVal);		
		idx[0]=i;
		hr = SafeArrayPutElement(psa, idx, &var);
		hr = VariantClear(&var); 
		
	}
	dispparams.cNamedArgs = 0;
	dispparams.cArgs = 1;
	dispparams.rgvarg->parray = psa;
    dispparams.rgvarg->vt = VT_ARRAY | VT_VARIANT;   

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
   	CComVariant vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg
         
	hr = spScript->Invoke(dispid,IID_NULL,0,
							DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	delete [] dispparams.rgvarg;
	SafeArrayDestroy(psa);

	if(FAILED(hr))	{
		ShowError(GetSystemErrorMessage(hr));
		return false;
	}
	
	if(sResult)	{		
		if (vaResult.vt == VT_BSTR )
			sResult->SetSysString(&vaResult.bstrVal);
		else
			sResult->Empty();
	}
	return true;
}


bool CHtmlScript::CallJScript(const CString strFunc, const CStringArray& paramArray,CString *sResult)
{
	
	CComBSTR bstrMember(strFunc);
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrMember,1,
											LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		ShowError(GetSystemErrorMessage(hr));
		return false;
	}

	const int arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
	
	for( int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
   	CComVariant vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg
         
	hr = spScript->Invoke(dispid,IID_NULL,0,
							DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	delete [] dispparams.rgvarg;
	if(FAILED(hr))
	{
		ShowError(GetSystemErrorMessage(hr));
		return false;
	}
	
	if(sResult)
	{
		if (vaResult.vt == VT_BSTR )
			//sResult->SetSysString(&vaResult.bstrVal);
			*sResult = vaResult.bstrVal;
		else
			sResult->Empty();
	}
	return true;
}
/*
// returned java script function name, input string is truncating
CString CWebPage::ScanJScript(CString& strAText, CStringArray& args)
{
	args.RemoveAll();
	CString strDelim(" \n\r\t"),strSrc(strAText);
	bool bFound = false;
	while(!strSrc.IsEmpty())
	{
		CString strStart = GetNextToken(strSrc,strDelim);
		if(strStart == "function")
		{
			bFound = true;
			break;
		}
		if(strStart == "/") // finds comments
		{
			// Skip comments
			while(!strSrc.IsEmpty())
			{
				CString strStop = GetNextToken(strSrc,strDelim);
				if(strStop == "/")
				{
					break;
				}
			}
		}
	}

	if(!bFound)
		return "";
	
	CString strFunc = GetNextToken(strSrc,"(",true);
	CString strArgs = GetNextToken(strSrc,")",true);

	// Parse arguments
	CString strArg;
	while(!(strArg = GetNextToken(strArgs,",")).IsEmpty())
		args.Add(strArg);

	strAText= strSrc;
	return strFunc;
}

*/