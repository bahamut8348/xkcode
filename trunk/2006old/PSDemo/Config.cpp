#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
{
}

CConfig::~CConfig(void)
{
}


void CConfig::Set( const TCHAR *Section, const TCHAR *Key, const CString & Value )
{
	::WritePrivateProfileString(Section, Key, Value, m_FileName);
}

void CConfig::Set( const TCHAR *Section, const TCHAR *Key, int Value )
{
	CString s;
	s.Format(L"%d", Value);
	Set(Section, Key, s);
}

int CConfig::Get( const TCHAR *Section, const TCHAR *Key, int Default /*= 0*/ )
{
	return ::GetPrivateProfileInt(Section, Key, Default, m_FileName);
}

CString CConfig::Get( const TCHAR *Section, const TCHAR *Key, CString Default)
{
	TCHAR tmp[1024] = {0};
	::GetPrivateProfileString(Section, Key, Default, tmp, 1024, m_FileName );
	return tmp;
}