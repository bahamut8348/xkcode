//============================================================================
// ado.cpp
//
// Copyright 2006 Possibility Space Incorporated.
//============================================================================

#include "stdafx.h"



AdoConnection::AdoConnection() throw (...)
{
	if(FAILED(m_pConn.CreateInstance("ADODB.Connection")))
	{
		throw AdoError(_T("Ado CreateInstance Failed"));
	}
}

bool AdoConnection::Open( const TCHAR *connstring, const TCHAR *username, const TCHAR *password) throw(...)
{
	BSTR connstr = T2BSTR(connstring);
	BSTR user = T2BSTR(username);
	BSTR pass = T2BSTR(password);
	try
	{
		m_pConn->Open(connstr,user,pass,-1 );
	}
	catch (_com_error   & e)
	{
		SetLastError(e.Description());
		return false;
	}
	
	return true;
}

const TCHAR * AdoConnection::GetLastError() const
{
	return (TCHAR *)m_ErrorDesp;
}

bool AdoConnection::Close()
{
	if(FAILED(m_pConn->Close()))
		return false;
	else
		return true;
}

bool AdoConnection::Execute( const TCHAR *sql )
{
	m_pConn->Execute(T2BSTR(sql),NULL,0);
	return true;
}



AdoRecordSet::AdoRecordSet()throw (...)
{
	if(FAILED(m_pRs.CreateInstance("ADODB.RecordSet")))
	{
		throw AdoError(_T("Ado CreateInstance Failed"));
	}
}

bool AdoRecordSet::Open( const TCHAR *sql )
{
	/*
	try
	{
		m_pRs->Open()
	}
	catch (_com_error  & e)
	{
		
	}

	*/

	return true;
}

