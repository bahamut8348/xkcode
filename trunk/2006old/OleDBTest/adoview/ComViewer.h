#pragma once


namespace adoview
{
	
	class Error 
	{
	public:
		Error(){}
	};
	
	
	class CComViewer
	{
	public:
		CComViewer( BSTR name );
		~CComViewer(void);

		int GetTypeCount();
		void GetInfo();
	
	private:

		IUnknownPtr m_baseptr;
		IDispatchPtr m_dispptr;
		CAtlArray<ITypeInfoPtr> m_infoptr;
		HRESULT hRet;
		
		ITypeInfoPtr m_typeinfo;
		ITypeCompPtr m_typecomp;
		TYPEATTR *m_pAttr;

	};

}

