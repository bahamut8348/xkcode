#pragma once

class CConfig
{
public:
	CConfig(void);
	CString m_FileName;
	
	void Set( const TCHAR *Section, const TCHAR *Key, const  CString  & Value);
	void Set( const TCHAR *Section, const TCHAR *Key, int Value);
	int Get(const TCHAR *Section, const TCHAR *Key, int Default = 0);
	CString Get(const TCHAR *Section, const TCHAR *Key, CString Default = L"");
public:
	~CConfig(void);
};
