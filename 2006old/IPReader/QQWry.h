#pragma once


struct IPInfo
{
	std::string mIP;
	std::string mCountry;
	std::string mArea;

	std::string __str__() const
	{
		return "<IPInfo: " + mIP + " " + mCountry + " " +mArea + ">"; 
	}
};

class QQWry
{
public:
	QQWry(void);
	~QQWry(void);
	BYTE* mDataBuffer;
	DWORD mDataSize;

	DWORD mFirstIndex;
	DWORD mLastIndex;

	bool Load( const std::wstring & filename );
	DWORD GetIPNumber( )  ;
	
	IPInfo GetIPInfo( int idx );
	IPInfo GetAddr( DWORD offset, DWORD ip = 0) const;

	DWORD GetLong( DWORD offset ) const;
	std::string GetAreaInfo( DWORD offset ) const;

	

};


