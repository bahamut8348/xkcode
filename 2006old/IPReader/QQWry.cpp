#include "StdAfx.h"
#include "QQWry.h"

QQWry::QQWry(void):mDataBuffer(0)
{
}

QQWry::~QQWry(void)
{
	delete [] mDataBuffer;
}

inline unsigned int get_3b(const BYTE *mem)
{
	return 0x00ffffff & *(unsigned int*)(mem);
}

inline std::string StrIP( BYTE* ipptr)
{
	static char ipbuf[16];
	sprintf_s( ipbuf, "%d.%d.%d.%d" ,ipptr[3], ipptr[2], ipptr[1], ipptr[0] );
	return ipbuf;
}

bool QQWry::Load( const std::wstring & filename )
{
	HANDLE f = ::CreateFile( filename.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( f != INVALID_HANDLE_VALUE  )
	{
		DWORD filesize = ::GetFileSize( f, NULL  );
		mDataSize = filesize;
		mDataBuffer = new BYTE[ mDataSize ];
		::ReadFile( f, mDataBuffer, mDataSize, &filesize, NULL );
		::CloseHandle( f );
		GetIPNumber();
	}
	else
	{
		PyErr_SetString( PyExc_StandardError, "File cant open." );
		boost::python::throw_error_already_set();
	}
	
	return true;
}

DWORD QQWry::GetIPNumber()
{
	struct Header
	{
		DWORD start;
		DWORD end;
	};
	Header *pInfo = (Header *)mDataBuffer;
	mFirstIndex = pInfo->start;
	mLastIndex = pInfo->end;
	return (pInfo->end - pInfo->start)/7 + 1;
}

IPInfo QQWry::GetIPInfo( int idx )
{
	if( idx < 0 || idx >= GetIPNumber() )
	{	
		PyErr_SetString( PyExc_IndexError, "not valid index" );
		boost::python::throw_error_already_set();
	}
	
	DWORD offset = mFirstIndex + idx * 7;

	BYTE*  ipptr = mDataBuffer + offset;
	DWORD   addroffset = get_3b( mDataBuffer + offset + 4 );
	
	IPInfo ipinfo = GetAddr(  addroffset );
	
	ipinfo.mIP = StrIP( ipptr );
	return ipinfo;
}

IPInfo QQWry::GetAddr( DWORD offset, DWORD ip /*= 0*/ ) const
{
	IPInfo info;
	BYTE* IPptr = mDataBuffer + offset;
	BYTE* addrtype = mDataBuffer + offset + 4;
	if( *addrtype == 1)
	{
		DWORD countryoffset = get_3b( addrtype+1 );
		info.mCountry = GetAreaInfo( countryoffset );
		if( mDataBuffer[countryoffset] == 2 )
		{
			info.mArea = GetAreaInfo(  countryoffset + 4 );
		}
		else
		{
			info.mArea = GetAreaInfo(  countryoffset  + info.mCountry.size() +1 );
		}

	}
	else if ( *addrtype == 2 )
	{
		DWORD countryoffset  = get_3b( addrtype+1 );
		info.mCountry = GetAreaInfo( countryoffset );
		info.mArea = GetAreaInfo(  offset + 8 );
	}
	else
	{
		info.mCountry = GetAreaInfo( offset + 4 );
		info.mArea = GetAreaInfo(  offset + 4 + info.mCountry.size() + 1 );
	}
	return info;
}

DWORD QQWry::GetLong( DWORD offset ) const
{
#pragma pack(push,1)
	struct Header
	{
		USHORT low;
		BYTE   high;
	};
#pragma pack(pop)
	Header* value = ( Header*) (mDataBuffer + offset);
	return (value->high<<16) + value->low;
}

std::string QQWry::GetAreaInfo( DWORD offset ) const
{
	if( mDataBuffer[offset] == 2 )
	{
		DWORD newoffset = get_3b( mDataBuffer +offset + 1);
		return (char *)(mDataBuffer + newoffset);
	}
	else
	{
		return (char *) ( mDataBuffer + offset);
	}
}