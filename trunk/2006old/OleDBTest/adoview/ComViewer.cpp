#include "StdAfx.h"


#include "ComViewer.h"


adoview::CComViewer::CComViewer( BSTR name ) : m_baseptr(name)
{
	hRet = m_baseptr->QueryInterface(IID_IDispatch, (LPVOID *)&m_dispptr);
	ATLASSERT(SUCCEEDED(hRet));
}
adoview::CComViewer::~CComViewer(void)
{
	m_typeinfo->ReleaseTypeAttr(m_pAttr);
}

int adoview::CComViewer::GetTypeCount() 
{
	UINT ret;
	hRet = m_dispptr->GetTypeInfoCount(&ret);
	AtlTrace(L"+ GetTypeCount: %d\n", ret);
	ATLASSERT(SUCCEEDED(hRet));
	return ret;
}

void adoview::CComViewer::GetInfo( )
{
	
	for(int a=0; a<GetTypeCount(); a++)
	{
		hRet = m_dispptr->GetTypeInfo(0, a, &m_typeinfo );
		
		if(SUCCEEDED(hRet))
		{
			ITypeLibPtr typelib;
			UINT        typeindex;
			m_typeinfo->GetContainingTypeLib(&typelib, &typeindex);
			
			m_typeinfo->GetTypeAttr(&m_pAttr);

			//functions
			for(UINT a=0; a<m_pAttr->cFuncs; a++)
			{
				FUNCDESC *desc = 0;
				hRet = m_typeinfo->GetFuncDesc(a, &desc);
				

				BSTR  names[1024];
				UINT  count;
				hRet = m_typeinfo->GetNames(desc->memid,names, 1024, &count);
				
				BSTR mop;
				m_typeinfo->GetMops(desc->memid, &mop);

				BSTR name, doc, helpfile;
				DWORD context;
				m_typeinfo->GetDocumentation(desc->memid, &name, &doc, &context, &helpfile);
//				
				BSTR dllname, bstrname;
				WORD  ordinal;
				m_typeinfo->GetDllEntry(desc->memid, desc->invkind,  &dllname, &bstrname, &ordinal);

				m_typeinfo->ReleaseFuncDesc(desc);
				continue;
			}

			// vars
			



			m_typeinfo->GetTypeComp(&m_typecomp);

			m_infoptr.Add(m_typeinfo);
		}
		else
		{
			ATLASSERT(0);
		}
	}
	
}