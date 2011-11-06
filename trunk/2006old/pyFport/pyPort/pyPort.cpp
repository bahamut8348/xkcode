// pyPort.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif




#define TABLE_SIZE 10240



typedef enum 
{
	TCP_TABLE_BASIC_LISTENER,
	TCP_TABLE_BASIC_CONNECTIONS,
	TCP_TABLE_BASIC_ALL,
	TCP_TABLE_OWNER_PID_LISTENER,
	TCP_TABLE_OWNER_PID_CONNECTIONS,
	TCP_TABLE_OWNER_PID_ALL,
	TCP_TABLE_OWNER_MODULE_LISTENER,
	TCP_TABLE_OWNER_MODULE_CONNECTIONS,
	TCP_TABLE_OWNER_MODULE_ALL
}TCP_TABLE_CLASS, *PTCP_TABLE_CLASS;


enum TCPSTATE
{

	CLOSED = 1,
	LISTEN = 2,
	SYN_SENT = 3,
	SYN_RCVD = 4,
	ESTABLISH = 5,
	FIN_WAIT1 = 6,
	FIN_WAIT2 = 7,
	CLOSE_WAIT = 8,
	
	CLOSING = 9,
	LAST_ACK = 10,
	TIME_WAIT =11,
	DELETE_TCB = 12,

};


typedef enum 
{
	UDP_TABLE_BASIC ,
	UDP_TABLE_OWNER_PID ,
	UDP_TABLE_OWNER_MODULE,
} UDP_TABLE_CLASS;


extern "C" DWORD __stdcall    GetExtendedTcpTable( 
	PVOID pTcpTable, 
	PDWORD pdwSize, 
	ULONG bOrder, 
	ULONG ulAf, 
	ULONG TableClass, 
	ULONG Reserved 
	); 


extern "C" DWORD __stdcall  GetExtendedUdpTable(
	PVOID pUdpTable,
	PDWORD pdwSize,
	BOOL bOrder,
	ULONG ulAf,
	UDP_TABLE_CLASS TableClass,
	ULONG Reserved
	);


HINSTANCE hLibrary;


struct TCPConn : MIB_TCPROW_OWNER_PID
{
	std::wstring Type;
	std::wstring Local() const
	{
		BYTE *b = (BYTE *) &dwLocalAddr;
		WCHAR buf[20];
		swprintf_s(buf,L"%d.%d.%d.%d", b[0], b[1],b[2],b[3]);
		return buf;
	}

	std::wstring Remote() const
	{
		BYTE *b = (BYTE *) &dwRemoteAddr;
		WCHAR buf[20];
		swprintf_s(buf,L"%d.%d.%d.%d", b[0], b[1],b[2],b[3]);
		return buf;
	}

	std::wstring Desc() const
	{
		CAtlString s;
		s.Format(L"<TCP PID:%d Local:(%s:%d) Remote:(%s:%d), State:%d>",
			dwOwningPid, Local().c_str(), dwLocalPort, Remote().c_str(), dwRemotePort, dwState);
		return (LPCTSTR)s;
	}

	TCPSTATE State;

	
};


struct UDPConn : MIB_UDPROW_OWNER_PID
{
	std::wstring Type;
	std::wstring Local() const
	{
		BYTE *b = (BYTE *) &dwLocalAddr;
		WCHAR buf[20];
		swprintf_s(buf,L"%d.%d.%d.%d", b[0], b[1],b[2],b[3]);
		return buf;
	}
};



list GetConns()
{
	DWORD bufsize  = 0;

	list l;

	std::vector<MIB_TCPROW_OWNER_PID>  TCPtables;
	std::vector<MIB_UDPROW_OWNER_PID>  UDPtables;

	DWORD ret= GetExtendedTcpTable(0, &bufsize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	char *buf = new char[bufsize];
	ret = GetExtendedTcpTable(buf, &bufsize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	if(ret == NO_ERROR )
	{
		MIB_TCPTABLE_OWNER_PID *pTCPTable = (MIB_TCPTABLE_OWNER_PID *)buf;
		TCPtables.assign(pTCPTable->table, pTCPTable->table+pTCPTable->dwNumEntries);

	}

	delete [] buf;

	bufsize = 0;
	ret= GetExtendedUdpTable(0, &bufsize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);
	buf = new char[bufsize];
	ret = GetExtendedUdpTable(buf, &bufsize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);
	if(ret == NO_ERROR )
	{
		MIB_UDPTABLE_OWNER_PID *pUDPTable = (MIB_UDPTABLE_OWNER_PID *)buf;
		UDPtables.assign(pUDPTable->table, pUDPTable->table+pUDPTable->dwNumEntries);

	}



	delete [] buf;
	
	for (DWORD a=0; a<TCPtables.size(); a++)
	{
		TCPConn Conn;
		MIB_TCPROW_OWNER_PID & row = TCPtables[a];
		Conn.dwLocalAddr = row.dwLocalAddr;
		Conn.dwLocalPort = ntohs((u_short)row.dwLocalPort);
		Conn.dwOwningPid = row.dwOwningPid;
		Conn.dwRemoteAddr = row.dwRemoteAddr;
		Conn.dwRemotePort = ntohs((u_short)row.dwRemotePort);
		Conn.State =(TCPSTATE) row.dwState;
		Conn.Type = L"tcp";
		l.append( object(Conn) );
	}

	return l;

}


BOOST_PYTHON_MODULE(pyPort)
{
	
	class_<TCPConn>("TCPConn")
		.def_readonly("Type",		&TCPConn::Type)
		.def("Local",				&TCPConn::Local)
		.def("Remote",				&TCPConn::Remote)
		.def_readonly("State",		&TCPConn::State)
		.def_readonly("LocalAddr",	&TCPConn::dwLocalAddr)
		.def_readonly("LocalPort",	&TCPConn::dwLocalPort)
		.def_readonly("RemoteAddr",	&TCPConn::dwRemoteAddr)
		.def_readonly("RemotePort",	&TCPConn::dwRemotePort)
		.def_readonly("PID",		&TCPConn::dwOwningPid)
		.def("__str__",				&TCPConn::Desc)
	;

	//TCPSTATE
	enum_<TCPSTATE>("TCPSTATE")
		.value("CLOSED", 	CLOSED)
		.value("LISTEN", 	LISTEN)
		.value("SYN_SENT", 	SYN_SENT)
		.value("SYN_RCVD", 	SYN_RCVD)
		.value("ESTABLISH", 	ESTABLISH)
		.value("FIN_WAIT1", 	FIN_WAIT1)
		.value("FIN_WAIT2", 	FIN_WAIT2)
		.value("CLOSE_WAIT", 	CLOSE_WAIT)
		.value("CLOSING", 	CLOSING)
		.value("LAST_ACK", 	LAST_ACK)
		.value("DELETE_TCB", 	DELETE_TCB)
		.export_values()
		;



	def("GetConns",GetConns);
}




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

