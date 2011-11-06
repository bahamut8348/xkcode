// pyFport.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <Tlhelp32.h>
#include <vector>

#include <Iphlpapi.h>

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



void Init()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}

	


}

struct ProcInfo
{
	DWORD PID;
	std::wstring ExeName;
	std::wstring ExePath;
	std::wstring CommandLine;
	DWORD ThreadNum;
	DWORD ParentPID;

 
};

int _tmain(int argc, _TCHAR* argv[])
{
	Init();

	
	

	std::vector<MIB_TCPROW_OWNER_PID>  TCPtables;
	std::vector<MIB_UDPROW_OWNER_PID>  UDPtables;
	
	DWORD bufsize  = 0;
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

	
	// cpu
	std::vector< PROCESSENTRY32 > ProcTable;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap > 0)
	{
		PROCESSENTRY32 ProcEntry = {0};	
		ProcEntry.dwSize = sizeof(ProcEntry);
		if(Process32First(hSnap, &ProcEntry))
		{
			do 
			{
				ProcTable.push_back(ProcEntry);
			} while (Process32Next(hSnap, &ProcEntry));
			
		
		}
	}

	CloseHandle(hSnap);




	
	return 0;
}

