#pragma once

class UserInstance
{
public:
	UserInstance( LPCTSTR tapname, DWORD pid  );
	~UserInstance( );
	
	BOOL Login( );
	
private:
	std::wstring mTapGUID;
	DWORD		 mMsgPID;
};
