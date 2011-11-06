#include "StdAfx.h"
#include "sqlconvert.h"
#include "mssql.h"



static PyObject *mssqlExcept;
static mssqlError g_err;





static PyObject *error_to_str(const _com_error  & e)
{
	USES_CONVERSION;
	CString s;
	s.Format(L"(%d) *%s*", e.WCode(), (LPCTSTR)e.Description());
	
	return incref(str((const char *)W2A((LPCTSTR)s)).ptr());
	//::WriteConsole(::GetStdHandle(STD_ERROR_HANDLE), (LPCTSTR)s ,s.GetLength(),NULL,0);
	return incref(object( std::wstring((LPCTSTR)s)).ptr());
}

mssql::mssql(void)
{
	m_pconn.CreateInstance(L"Adodb.Connection");
	

}

mssql::~mssql(void)
{
	
	m_pconn = NULL;
	
}

#define  HandleCatch  PyErr_SetObject(mssqlExcept, error_to_str(e)); throw error_already_set();

int mssql::conn( const std::wstring & addr, const std::wstring & username, const std::wstring & password, const std::wstring & dbname )
{
	CString connstr;
	connstr.Format(L"Driver=SQL Server;Server=%s;Database=%s;UID=%s;PWD=%s;connect timeout=15;", addr.c_str(),
			dbname.c_str(), username.c_str(), password.c_str());
		
	try
	{
		m_pconn->Open( (LPCTSTR)connstr, L"",L"",adConnectUnspecified ) ;
	}
	catch (_com_error  & e )
	{
		HandleCatch;
	}
	
	return true;
}

dataset mssql::query( const std::wstring & sqlstring )
{
	try
	{	_RecordsetPtr prs;
		prs.CreateInstance(L"Adodb.RecordSet");
		prs->Open(sqlstring.c_str(),_variant_t( (IDispatch *)m_pconn ,true), adOpenStatic ,adLockOptimistic,adCmdText);
		return prs;
	}
	catch (_com_error &  e)
	{
		HandleCatch;
	}
		
}

int mssql::exec( const std::wstring & sqlstring )
{
	try
	{	
		_variant_t RecordEffect;
		m_pconn->Execute(sqlstring.c_str(),&RecordEffect,adCmdText);		
		return (long)RecordEffect;
	}
	catch (_com_error &  e)
	{
		HandleCatch;
	}	
}


void init_mssql()
{
	mssqlExcept = PyErr_NewException("mssql.error", PyExc_Exception, NULL);
	
	class_<mssql>("mssql")
		.def("conn", &mssql::conn)
		.def("query", &mssql::query)
		.def("exe",	&mssql::exec)
	;

	class_<mssqlError>("mssqlException", no_init)
		.def("__str__", &mssqlError::__str__)
		.add_property("code", &mssqlError::get_code)
		.add_property("what",  &mssqlError::what)
		;

	class_<dataset>("mssql.datalist", no_init)
		.def("__str__", &dataset::__str__)
		.def("__len__", &dataset::size)
		.def("fields",  &dataset::fields)
		.def("__iter__",&dataset::iter)
		.def("__getitem__", &dataset::getitem)
		;

	class_<datarow>("mssql.datarow", no_init)
		.def("__getitem__", &datarow::getitem)
		.def("__setitem__", &datarow::setitem)
		.def("update", &datarow::update)
		//.add_property("raw",&datarow::dict)
		;

	class_<dataiter>("mssql.dataiter",no_init)
		.def("next",  &dataiter::next)
		.def("__iter__", &dataiter::iter)
		;

	
}

dataset::dataset( _RecordsetPtr & prs ): m_prs (prs)
{
	 
}

int dataset::size() const
{
	return m_prs->GetRecordCount();
}

boost::python::list dataset::fields() const
{
	list d;
	int itemcount = m_prs->Fields->Count;
	for(long idx=0; idx< itemcount; idx++)
	{
		_bstr_t val = m_prs->Fields->Item[idx]->Name;
		d.append( std::wstring( (wchar_t *)val));
	}
	return d;

}

datarow::datarow( _RecordsetPtr & prs ):m_prs(prs)
{
	
}

object datarow::getitem( const object & key )
{
	object ret;
	_variant_t val;
	if (PyInt_Check(key.ptr()) || PyLong_Check(key.ptr()) )
	{
		long idx = extract<int>(key);
		try
		{
			val = m_prs->GetCollect( _variant_t(idx) );
		}
		catch (_com_error &  e)
		{
			HandleCatch;
		}
		
	}
	else if( PyString_Check(key.ptr()) || PyUnicode_Check (key.ptr()))
	{
		std::wstring k = extract<std::wstring>(key);
		try
		{
			val = m_prs->GetCollect( _variant_t(k.c_str()));
		}
		catch (_com_error &  e)
		{
			HandleCatch
		}
		
	}

	else
	{
		PyErr_SetString(PyExc_TypeError,"need int or string");
		throw error_already_set();
	}

	DWORD type = val.vt;
	switch(type)
	{
	case VT_I1:
	case VT_I2:
	case VT_I4:
	case VT_I8:
	case VT_INT:
	
		ret = object(val.llVal);
		break;
	case VT_BOOL:
		ret = object( (bool)val );
		break;

	case VT_UI1:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
	case VT_UINT:
		ret = object( val.ullVal  );
		break;

	case VT_BSTR:
		ret = object(std::wstring( (_bstr_t)val ));
		break;
	case VT_DATE:
		ret = object(COleDateTime(val));
		break;
	case VT_NULL:
		return object();
		break;
	default:
		PyErr_SetString(PyExc_RuntimeError, "Cant convert type");
		throw error_already_set();
		break;
	}

	return ret;

}

void datarow::setitem( const object &key, const object & value )
{
	_variant_t val, valueset;
	static PyDateTime_CAPI* PyDateTimeAPI = (PyDateTime_CAPI*) PyCObject_Import("datetime","datetime_CAPI");
	PyObject* v = value.ptr();


	PyDateTime_DateTime *pv =(PyDateTime_DateTime *) v;

	if(PyInt_Check(v)) valueset = _variant_t(extract<int>(value));
	else if(PyLong_Check(v)) valueset = _variant_t(extract<long long>(value));
	else if(PyUnicode_Check(v))
	{
		int len = PyUnicode_GetSize(v);
		wchar_t *buffer = new wchar_t[len+1];
		buffer[len] = 0;
		PyUnicode_AsWideChar( (PyUnicodeObject *)v, buffer, len );
		valueset =  _variant_t(buffer);
		delete buffer;
	}
	else if(PyString_Check(v)) valueset =  _variant_t(extract<char *>(value));
	else if(PyFloat_Check(v)) valueset =  _variant_t(extract<float>(value));
	
	else if(PyDateTime_Check(v))
	{
		SYSTEMTIME st;
		::ZeroMemory(&st, sizeof(st));
		st.wDay = PyDateTime_GET_DAY(v);
		st.wYear = PyDateTime_GET_YEAR(v);
		st.wMonth = PyDateTime_GET_MONTH(v);
		st.wHour = PyDateTime_DATE_GET_HOUR(v);
		st.wMilliseconds = PyDateTime_DATE_GET_MICROSECOND(v)/1000;
		st.wMinute = PyDateTime_DATE_GET_MINUTE(v);
		st.wSecond =PyDateTime_DATE_GET_SECOND(v);
				
		static CString temp;
		temp.Format(L"%04d-%02d-%02d %02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		_tprintf(L"%s", (LPCTSTR)temp);
		DATE date;
		int n =SystemTimeToVariantTime(&st, &date);
		
		valueset = _variant_t(date, VT_DATE);
		valueset.ChangeType(VT_DATE);
	}
	else valueset =  _variant_t(extract<float>(value));
	


	if (PyInt_Check(key.ptr()) || PyLong_Check(key.ptr()) )
	{
		long lkey = extract<int>(key);
		try
		{
			m_prs->PutCollect(lkey, valueset);
		}
		catch (_com_error &  e)
		{
			HandleCatch;
		}

	}
	else if( PyString_Check(key.ptr()) || PyUnicode_Check (key.ptr()))
	{
		std::wstring skey = extract<std::wstring>(key);
		try
		{
			m_prs->PutCollect(skey.c_str(), valueset);
		}
		catch (_com_error &  e)
		{
			HandleCatch;
		}

	}

	else
	{
		PyErr_SetString(PyExc_TypeError,"key need int or string");
		throw error_already_set();
	}


	

}

void datarow::update()
{
	m_prs->Update();
}


object datarow::dict()
{
	boost::python::dict d;
	for(int a=0; a<m_prs->Fields->Count; a++)
	{
		static std::wstring temp;
		try
		{
		
			_tprintf(L"%d\n", a );
			_bstr_t val = m_prs->Fields->Item[a]->Name;
			_tprintf(L"%s\n", (wchar_t *)(_bstr_t) L"df");
			temp = (const wchar_t *)(_bstr_t)m_prs->GetFields()->GetItem(0)->GetName();
			
		}
		catch (_com_error & e)
		{
			HandleCatch;
		}
		
		_tprintf(L"%s\n", temp.c_str());
		d[ object(temp)] = getitem( object(a) );
	}
	return d;
}


PyObject* dataiter::next()
{
	if(m_prs->adoEOF)
	{
		PyErr_SetNone(PyExc_StopIteration);
		throw error_already_set();
	}

	if(m_bStart)
	{
		m_bStart = false;
	}
	else
	{
		m_prs->MoveNext();
		if(m_prs->adoEOF)
		{
			PyErr_SetNone(PyExc_StopIteration);
			throw error_already_set();
		}
	}
	
	return incref(object(datarow(m_prs)).ptr());

}

dataiter::dataiter( _RecordsetPtr & prs ):m_prs(prs)
{
	if(!m_prs->adoEOF)
		m_prs->MoveFirst();
	m_bStart = true;
}

object dataiter::iter()
{
	return object(*this);
}
object dataset::iter()
{
	return object(dataiter(m_prs));
}

object dataset::getitem( int n )
{
	m_prs->Move(n);
	return object(datarow(m_prs));
}