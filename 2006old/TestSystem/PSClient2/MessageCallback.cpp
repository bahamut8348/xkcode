#include "StdAfx.h"

#include "StructDef.h"
#include "MessageCallback.h"

using namespace PyPSClient2;

PSDash::_void CMessageCallback::OnValidateResult( _dword byCode )
{
	return call_method<void>(m_CbHandler, "OnValidateResult", byCode);
}

PSDash::_void PyPSClient2::CMessageCallback::OnCreateSessionResult( _dword dwCode )
{
	return call_method<void>(m_CbHandler, "OnCreateSessionResult", dwCode);
}

PSDash::_void PyPSClient2::CMessageCallback::OnConnectIS( _bool bConnect, const PSUID is_id, 
														 _dword Reason, const _char *desp /*= _null*/ )
{
	if(desp == _null) desp=L"";
	return call_method<void>(m_CbHandler, "OnConnectIS", bConnect, 
		PyPSUID::CreateFromPSUID(is_id), Reason, std::wstring(desp));
}

PSDash::_void PyPSClient2::CMessageCallback::OnAreaListRecv( _dword Result, const Array<stCP_AreaInfo> & alist, 
															_dword AreaID )
{
	list al = ArrayToList(alist);
	return call_method<void>(m_CbHandler, "OnAreaListRecv", Result, al, AreaID);
}

PSDash::_void PyPSClient2::CMessageCallback::OnRegisterResult( _dword Callback, const RegisterResult & Result )
{
	list errors, names;
	names = ArrayToList(Result.RecommandUserName, AP_RECOMMAND_USER_NAME_NUM);
	errors = ArrayToList(Result.ErrorCode, Result.ErrorNum);
	return call_method<void>(m_CbHandler, "OnRegisterResult", errors, names);
}

PSDash::_void PyPSClient2::CMessageCallback::OnRoomListRecv( _dword Result, _dword AreaID,_byte Num, 
												const Array<stCP_RoomInfo> & RoomList, _dword RoomType )
{
	return call_method<void>(m_CbHandler, "OnRoomListRecv", Result, AreaID, ArrayToList(RoomList), RoomType);
}

PSDash::_void PyPSClient2::CMessageCallback::OnCreateRoomRecv( _dword Callback, 
															  _dword Result, const stCP_RoomInfo & Info )
{
	return call_method<void>(m_CbHandler, "OnCreateRoomRecv", Result, Info);
}

PSDash::_void PyPSClient2::CMessageCallback::OnJoinRoomRecv( _dword Callback, _dword Result, const stCP_RoomInfo & Info )
{
	return call_method<void>(m_CbHandler, "OnJoinRoomRecv", Result, Info);
}

PSDash::_void PyPSClient2::CMessageCallback::OnRecvChatMsg( _byte datatype, 
		_dword RoomID, const PSUID & ToPSUID, const PSUID & FromPSUID, const _char *Msg )
{
	return call_method<void>(m_CbHandler, "OnRecvChatMsg", datatype, RoomID, PyPSUID::CreateFromPSUID(ToPSUID),
		PyPSUID::CreateFromPSUID(FromPSUID), std::wstring(Msg));
}

PSDash::_void PyPSClient2::CMessageCallback::OnQuitRoomResult( _dword Callback, 
		_dword Result, _dword RoomID, const stCP_RoomInfo & RoomInfo )
{
	return call_method<void>(m_CbHandler, "OnQuitRoomResult", Result, RoomID, RoomInfo);
}

PSDash::_void PyPSClient2::CMessageCallback::OnBuddyListRecv( _dword _Result, _dword Callback, const Array<BUDDY_INFO> & BuddyList )
{
	return call_method<void>(m_CbHandler, "OnBuddyListRecv", _Result, ArrayToList(BuddyList));
}

PSDash::_void PyPSClient2::CMessageCallback::OnLoginMMSResult( _dword Result )
{
	return call_method<void>(m_CbHandler, "OnLoginMMSResult", Result);
}

PSDash::_void PyPSClient2::CMessageCallback::OnDispatchGame( _dword GameID, _word MaxUserNum )
{
	return call_method<void>(m_CbHandler, "OnDispatchGame", GameID, MaxUserNum);
}