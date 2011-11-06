// PSClient2.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "StructDef.h"
#include "MessageCallback.h"
#include "Client2.h"


using namespace PyPSClient2;

BOOST_PYTHON_MODULE(PSClient2)
{
	class_<Address>("Address",init<>())
		.def_readwrite("port",	&Address::mPort)
		.def_readwrite("ip",	&Address::mIP)
		.def_readwrite("type",	&Address::mType)
		;

	class_<_strBIRTHDAY>("Birthday")
		.def_readwrite("Year", &_strBIRTHDAY::Year)
		.def_readwrite("Month", &_strBIRTHDAY::Month)
		.def_readwrite("Day", &_strBIRTHDAY::Day)
		;

	class_<stAP_CLIENT_INFO>("DetailInfo")
		.def_readonly("Username", &stAP_CLIENT_INFO::UserName)
		.def_readonly("AccountID",&stAP_CLIENT_INFO::AccID)
		.def_readonly("Password", &stAP_CLIENT_INFO::Password)
		.def_readonly("NickName", &stAP_CLIENT_INFO::NickName)
		.def_readonly("Location", &stAP_CLIENT_INFO::Country)
		.def_readonly("City",	  &stAP_CLIENT_INFO::City)
		.def_readonly("Gender",   &stAP_CLIENT_INFO::Sex)
		.def_readonly("Birthday", &stAP_CLIENT_INFO::Birthday)
		.def_readonly("Email",	  &stAP_CLIENT_INFO::Email)
		.def_readonly("bNewsLetter", &stAP_CLIENT_INFO::EmailNews)
		.def_readonly("right",		& stAP_CLIENT_INFO::right)
		.def("SetInfo",				& stAP_CLIENT_INFO::Set)
		;


	class_<PyPSUID>("PSUID", init<>())
		.def_readwrite("AccountID", &PyPSUID::mAccountID)
		.def_readwrite("SSID",		&PyPSUID::mSSID)
		.def_readwrite("Seq",		&PyPSUID::mSeq)
		.def_readwrite("IDType",	&PyPSUID::mIDType)
		.def_readwrite("TimeStamp", &PyPSUID::mTimeStamp)
		;

	class_<PSUID>("psuid",init<>())
		;

	class_<CClient2>("PSClient",init<>())
		.def("SetEventCb",	&CClient2::RegisterMessageCallback)
		.def("SetCBLog",	&CClient2::SetLogHandler)
		.def("InitIS",		&CClient2::SetInitIS)
		.def("Login",		&CClient2::Login)
		.def("Update",		&CClient2::Update)
		.def("ConnectToIS",	&CClient2::ConnectToIS)
		.def("SetAppInfo",	&CClient2::SetAppInfo)
		.def("SetIS",		&CClient2::SetIS)
		.def("QueryBuddyList",		&CClient2::QueryBuddyList)
		.def("LoginToMMS",			&CClient2::LoginToMMS)
		.def("HandleDispatchGame",	&CClient2::HandleDispatchGame)
		.def("Disconnect",			&CClient2::Disconnect)
		.def("QueryAreaList",			&CClient2::QueryAreaList)
		;

	class_<AreaInfo_>("AreaInfo")
		.def_readonly("AreaType",	&AreaInfo_::AreaType)
		.def_readonly("Id",			&AreaInfo_::ID)
		.def_readonly("Status",		&AreaInfo_::Status)
		.def_readonly("SubAreaNum", &AreaInfo_::subAreaNum)
		.def_readonly("subRoomNum", &AreaInfo_::subRoomNum)
		.def("SetInfo",				&AreaInfo_::Set)
		.add_property("Name",		&AreaInfo_::get_AreaName, &AreaInfo_::set_AreaName)
		.def("bCanCreateRoom", &stCP_AreaInfo::IsCanCreateRoom)
		;

	class_<stCP_RoomInfo>("RoomInfo")
		.def_readonly("Id",			&stCP_RoomInfo::ID)
		.def_readonly("Name",		&stCP_RoomInfo::Desp)
		.def_readonly("bNeedPass",	&stCP_RoomInfo::beNeedPass)
		.def_readonly("AreaID",		&stCP_RoomInfo::AreaID)
		.def_readonly("RoomType",	&stCP_RoomInfo::RoomType)
		.def_readonly("Status",		&stCP_RoomInfo::Status)
		.def_readonly("UserNum",	&stCP_RoomInfo::UserNum)
		.def_readonly("OwnerID",	&stCP_RoomInfo::OwnerAccID)
		.def_readonly("MaxNum",		&stCP_RoomInfo::MaxNumber)
		.def_readonly("ChatServer", &stCP_RoomInfo::ChatServerUID)
		.def("SetInfo",				&stCP_RoomInfo::Set)
		;

	class_<BUDDY_INFO>("BuddyInfo")
		.def_readwrite("Uid",		&BUDDY_INFO::BuddyID)
		.def_readwrite("AccountID", &BUDDY_INFO::AccountID)
		.def_readwrite("State",		&BUDDY_INFO::State)
		.def_readonly("Nickname",	&BUDDY_INFO::BuddyNick)
		.def("SetBuddyInfo",		&BUDDY_INFO::SetBuddyInfo)
		;

	class_<SessionAddress>("SessionAddress")
		.def_readwrite("Address",		&SessionAddress::mAddress)
		.def_readwrite("SessionID",		&SessionAddress::mSessionID)
		.def_readwrite("Reserved",		&SessionAddress::mReserved)
		;

	def("str2addr", StringToAddr);

}








BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	return 1; 
}




