#pragma once


struct mssqlError
{
	mssqlError():m_err(0){}
	mssqlError( const mssqlError & err ):m_err(err.m_err){}
	const mssqlError & operator = ( const mssqlError & err ){ m_err = err.m_err ;}
	_com_error m_err;
	int get_code() const
	{
		return m_err.WCode();
	}
	std::wstring what() const
	{
		return (LPCTSTR)m_err.Description();
	}
	std::wstring __str__() const
	{
		
		CString s;
		s.Format(L"Error: (%d) %s", get_code(), what().c_str());
		wchar_t kk[1024]={0};
		swprintf_s(kk, L": (%d) %s", get_code(), what().c_str());
		//::WriteConsole(::GetStdHandle(STD_ERROR_HANDLE), (LPCTSTR)s ,s.GetLength(),NULL,0);
		return kk;
		//return (wchar_t *)L"incref(incref(object(std::wstring((LPCTSTR)s)).ptr()))";
	}
};



class dataset
{
public:
	dataset( _RecordsetPtr & prs );
	int size() const;
	list fields() const;
	object getitem(int n);
	object iter();
	
	std::wstring __str__() const
	{
		return L"<mssql.dataset>";
	}
private:
	_RecordsetPtr m_prs;

};

struct datarow
{
	datarow( _RecordsetPtr & prs);
	object getitem(const object & key);
	void setitem(const object &key, const object & value);
	object dict();
	void update();
	_RecordsetPtr m_prs;
	mssqlError m_err;
};

struct dataiter
{
	dataiter(_RecordsetPtr & prs );


	PyObject* next();
	object iter();
	
	bool m_bStart;
	_RecordsetPtr m_prs;
};


class mssql
{
public:
	mssql();
	int conn(const std::wstring & addr, const std::wstring & username, const std::wstring & password, const std::wstring & dbname);
	int exec( const std::wstring & sqlstring );
	dataset query( const std::wstring & sqlstring );
	~mssql(void);

private:
	_ConnectionPtr m_pconn;
	mssqlError m_err;
};


void init_mssql();
