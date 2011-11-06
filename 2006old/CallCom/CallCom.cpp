// CallCom.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//CComPtr < IUnknown > spPyDemo;
	::CoInitialize(NULL);
	IUnknownPtr spPyDemo;
	spPyDemo.CreateInstance(L"PythonDemos.Utilities");
	CComDispatchDriver spUtils(spPyDemo);
	CComVariant v1(L"sss=123"), Result;
	HRESULT hret = spUtils.Invoke1(L"Makeerrmsg", &v1, &Result);
	ATLASSERT(hret == S_OK);
	CString s = Result;
	::_tprintf(L"Result: %s\n", s);
	
	v1 = CComVariant(L"Hello Com VC");
	hret = spUtils.Invoke1(L"SplitString", &v1, &Result);
	ATLASSERT(SUCCEEDED(hret));
	int dim = SafeArrayGetDim(Result.parray);
	SAFEARRAY *array = Result.parray;
	
	// 方法一
	LONG ub, lb;
	SafeArrayGetUBound(array, 1, &ub);  // 指定维索引的上限
	SafeArrayGetLBound(array, 1, &lb);	// 指定维索引的下限
	for(int a= lb; a<=ub; a++)
	{
		CComVariant var;
		LONG dims[1];
		dims[0] = a;
		SafeArrayGetElement(array, dims, &var);
		_tprintf(L"%s\n", (LPCTSTR)(_bstr_t)(_variant_t)var);
	}


	// 方法二
	CComSafeArray<VARIANT> SArray(array);

	dim = SArray.GetDimensions();
	for(int a=SArray.GetLowerBound(); a<=SArray.GetUpperBound(); a++)
	{
		_variant_t t(SArray.GetAt(a)); 
		_tprintf(L"%s\n", (LPCTSTR)t.operator _bstr_t());
	}


	spPyDemo = NULL;

	CoUninitialize();
	return 0;
}

