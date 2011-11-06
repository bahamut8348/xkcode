#include "stdafx.h"
#include "StructDef.h"

void PSNetwork::InitStruct()
{

	/*
	class_<NetworkStatistic>("NetworkStatic")
		.def_readwrite("TotalDuration",			&NetworkStatistic::mTotalDuration)
		.def_readwrite("TotalSocketUsed",		&NetworkStatistic::mTotalSocketUsed)
		.def_readwrite("CurrentSocketUsed",		&NetworkStatistic::mCurrentSocketUsed)
		.def_readwrite("MaximalSocketUsed",		&NetworkStatistic::mMaximalSocketUsed)
		.def_readwrite("TotalBytesSent",		&NetworkStatistic::mTotalBytesSent)
		.def_readwrite("TotalBytesResent",		&NetworkStatistic::mTotalBytesResent)
		.def_readwrite("TotalBytesReceived",	&NetworkStatistic::mTotalBytesReceived)
		.def_readwrite("TotalSendTimes",		&NetworkStatistic::mTotalSendTimes)
		.def_readwrite("TotalResendTimes",		&NetworkStatistic::mTotalResendTimes)
		.def_readwrite("TotalReceiveTimes",		&NetworkStatistic::mTotalReceiveTimes)
		.def_readwrite("PacketsInOrder",		&NetworkStatistic::mPacketsInOrder)
		.def_readwrite("PacketsInDisorder",		&NetworkStatistic::mPacketsInDisorder)
		.def_readwrite("PacketsDroped",			&NetworkStatistic::mPacketsDroped)
		;
	*/


	class_<Address>("Address",init<>())
		.def_readwrite("port",	&Address::mPort)
		.def_readwrite("ip",	&Address::mIP)
		.def_readwrite("type",	&Address::mType)
		;

	class_<SessionAddress>("SessionAddress")
		.def_readwrite("Address",		&SessionAddress::mAddress)
		.def_readwrite("SessionID",		&SessionAddress::mSessionID)
		.def_readwrite("Reserved",		&SessionAddress::mReserved)
		;
}