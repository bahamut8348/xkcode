#pragma once


class IEvent
{
public :
	virtual void OnToken( dword result , const stQQToken & token ) = 0;
	virtual void OnLoginResult( dword result, const std::string & s) = 0;
	virtual void OnFriendMessage( dword sender, const std::string& msg ) = 0;
	virtual void OnLog( const std::string& s) = 0;
};