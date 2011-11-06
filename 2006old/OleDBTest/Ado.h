#pragma once 


class AdoError
{
public:
	AdoError(const TCHAR *msg)
	{
		_tprintf(_T("%s"), msg);
	}

	AdoError(const BSTR & msg)
	{
		
		_tprintf(_T("%s"), OLE2T(msg));
	}
};

class AdoConnection
{
public:
	AdoConnection() throw (...);
	bool Open(const TCHAR *connstring, const TCHAR *username, const TCHAR *password);
	bool Execute(const TCHAR *sql);
	bool Close();
	const TCHAR *GetLastError() const;
	
	
private:
	void	SetLastError(const _bstr_t & s){m_ErrorDesp = s;};


	_ConnectionPtr	m_pConn;
	_bstr_t			m_ErrorDesp;
};


class AdoRecordSet
{
public:
	AdoRecordSet() throw(...);
	bool Open(const TCHAR *sql);

	const TCHAR *GetLastError() const{return m_ErrorDesp;}
	

private:

	void	SetLastError(const _bstr_t & s){m_ErrorDesp = s;}

	_RecordsetPtr m_pRs;
	_bstr_t		m_ErrorDesp;
};