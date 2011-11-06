#include "StdAfx.h"
#include "Utils.h"
#include "Tlhelp32.h"
#include <shlwapi.h>
#include "Winternl.h"
#include "Psapi.h"


#define  UTILSBUF 40000*sizeof(MIB_TCPROW_OWNER_PID)

char *g_buf = new char[UTILSBUF];

BOOL Utils::GetProcessInfo( CAtlArray< Utils::ProcessInfo > & ProcList)
{
	HANDLE hToolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	
	if(hToolHelp != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32  Entry;
		Entry.dwSize = sizeof(Entry);
		
		if(Process32First(hToolHelp, &Entry))
		{
			Utils::ProcessInfo Info;
			do 
			{
				Info.PID = Entry.th32ProcessID;
				HANDLE hModuleHelp =CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Info.PID);
				Info.Name = Entry.szExeFile;
				Info.CompanyName = L"";
				
				if(hModuleHelp != INVALID_HANDLE_VALUE )
				{
					MODULEENTRY32 ModEntry = {0};
					ModEntry.dwSize = sizeof(ModEntry);
					Module32First(hModuleHelp, &ModEntry);
					Info.Path = ModEntry.szExePath;
					CloseHandle(hModuleHelp);


					// file info
					DWORD dwHandle = 0;
					DWORD InfoLen = GetFileVersionInfoSize(Info.Path, &dwHandle);
					if(InfoLen > 0)
					{
						BYTE *VerData = new BYTE[InfoLen];
						ZeroMemory(VerData, InfoLen);
						BOOL ret = GetFileVersionInfo(Info.Path, NULL, InfoLen, VerData);
						
						LPVOID lpvi;
						UINT iLen;
						if (VerQueryValue(VerData, _T("\\"), &lpvi, &iLen))
						{
							memcpy(&Info.FileInfo,lpvi, iLen);

							if (VerQueryValue(VerData, _T("\\VarFileInfo\\Translation"), &lpvi, &iLen) && iLen >= 4) {
								memcpy(&Info.LangID,lpvi,  4);
							}

							CString query;
							LPCTSTR pVal = NULL;
							UINT iLenVal;

#define  GetVersionValue(v) query.Format(_T("\\StringFileInfo\\%04x%04x\\%s"), Info.LangID,Info.Charset,_T(#v)); \
	VerQueryValue(VerData, (LPTSTR)(LPCTSTR)query,	(LPVOID*)&pVal, &iLenVal) ; \
	Info.##v = pVal; 
							
							GetVersionValue(CompanyName);
							GetVersionValue(FileVersion);

						}
						

					
						delete VerData;


					}
					
					

				
					
				}
				ProcList.Add(Info);
				
			} while (Process32Next(hToolHelp , &Entry));
		}
	}
	CloseHandle(hToolHelp);
	return TRUE;
	
}


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
	Utils::UDP_TABLE_CLASS TableClass,
	ULONG Reserved
	);


BOOL Utils::GetOpenedPort( std::vector<MIB_TCPROW_OWNER_PID> & TCPtables, std::vector<MIB_UDPROW_OWNER_PID> & UDPtables )
{
	
	DWORD bufsize  = UTILSBUF;

	DWORD ret = 0;//= GetExtendedTcpTable(0, &bufsize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	
	char *buf = g_buf;
	ret = GetExtendedTcpTable(buf, &bufsize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	if(ret == NO_ERROR )
	{
		MIB_TCPTABLE_OWNER_PID *pTCPTable = (MIB_TCPTABLE_OWNER_PID *)buf;
		TCPtables.assign(pTCPTable->table, pTCPTable->table+pTCPTable->dwNumEntries);

	}

	return TRUE;

	bufsize = 0;
	ret= GetExtendedUdpTable(0, &bufsize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);

	//ret = GetExtendedUdpTable(buf, &bufsize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);
	if(ret == NO_ERROR )
	{
		MIB_UDPTABLE_OWNER_PID *pUDPTable = (MIB_UDPTABLE_OWNER_PID *)buf;
		//UDPtables.assign(pUDPTable->table, pUDPTable->table+pUDPTable->dwNumEntries);

	}





	return TRUE;
}

CString Utils::MakeAddr( DWORD Address, DWORD Port )
{
	static TCHAR s[30];
	swprintf_s(s,L"%S:%d", inet_ntoa( *(in_addr *) &Address ), ntohs((u_short)Port));
	return s;
}

CString Utils::ConnStatus( DWORD value )
{
	/*
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
	*/
	
	switch (value)
	{
	case 1:
		return L"CLOSED";
	case 2:
		return L"LISTEN";
	case 3:
		return L"SYN_SEND";
	case 4: return L"SYN_RCVD";
	case 5: return L"ESTABLISH";
	case 6: return L"FIN_WAIT1";
	case 7: return L"FIN_WAIT2";
	case 8: return L"CLOSE_WAIT";
	case 9: return L"CLOSING";
	case 10: return L"LAST_ACK";
	case 11: return L"TIME_WAIT";
	case 12:return L"DELETE_TCB";
	default:
		{
			CString s;
			s.Format(L"Unknown:%d", value);
			return s;
		}
		
	}
}

BOOL Utils::GetDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
	{
		return FALSE;
	}

	if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}
	return TRUE;
}

BOOL Utils::KillProcess( DWORD PID )
{
	HANDLE hProc = ::OpenProcess(PROCESS_TERMINATE, FALSE, PID);
	if(hProc )
	{
		if(TRUE == ::TerminateProcess(hProc, 2))
		{
			while ( Utils::ProcessExit(PID)  )
			{
				::Sleep(10);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL Utils::ProcessExit( DWORD PID )
{
	HANDLE hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION ,FALSE,PID);
	if(hProc != NULL)
	{
		DWORD ExitCode = 0;
		GetExitCodeProcess(hProc, &ExitCode);
		::CloseHandle(hProc);
		if(ExitCode == STILL_ACTIVE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
	}
	return FALSE;
}

BOOL Utils::GetProcessCommandLine(DWORD dwId, LPTSTR CmdLine, LPDWORD CmdLen )
{
	ZeroMemory(CmdLine, *CmdLen);
	typedef LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);
	PROCNTQSIP  NtQueryInformationProcess;
	// get function
	
	HMODULE lib = LoadLibrary(L"ntdll.dll");
	NtQueryInformationProcess = (PROCNTQSIP)::GetProcAddress(lib, "NtQueryInformationProcess");
	if(NtQueryInformationProcess == NULL)
	{
		return FALSE;
	}

	LONG                      status;
	HANDLE                    hProcess;
	PROCESS_BASIC_INFORMATION pbi;
	PEB                       Peb;
	PROCESS_PARAMETERS        ProcParam;
	DWORD                     dwDummy;
	DWORD                     dwSize = 0;
	LPVOID                    lpAddress;
	BOOL                      bRet = FALSE;

	// Get process handle
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwId);
	if (!hProcess)
		return FALSE;

	// Retrieve information
	status = NtQueryInformationProcess( hProcess,
		ProcessBasicInformation,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL
		);


	if (status)
		goto cleanup;

	if (!ReadProcessMemory( hProcess,
		pbi.PebBaseAddress,
		&Peb,
		sizeof(PEB),
		&dwDummy
		)
		)
		goto cleanup;

	if (!ReadProcessMemory( hProcess,
		Peb.ProcessParameters,
		&ProcParam,
		sizeof(PROCESS_PARAMETERS),
		&dwDummy
		)
		)
		goto cleanup;

	lpAddress = ProcParam.CommandLine.Buffer;
	dwSize = ProcParam.CommandLine.Length;
	
	if (*CmdLen<dwSize)
		goto cleanup;

	if (!ReadProcessMemory( hProcess,
		lpAddress,
		CmdLine,
		dwSize,
		&dwDummy
		)
		)
		goto cleanup;


	bRet = TRUE;

cleanup:
	*CmdLen = dwSize; 
	CloseHandle (hProcess);

	StrTrim(CmdLine, L"\"' ");
	*CmdLen = _tcslen(CmdLine);
	return bRet;


}



DWORD Utils::GetCPUUsage( DWORD dwID, PULONG MemUsage )
{
	int cpuusage;
	PVOID pProcInfo = NULL;
	const DWORD dwInfoSize = 40960;
	PPROCESSINFO pProcessInfo;
	DWORD dwWorkingSet = 0;
	


	__int64 LastTotalProcessCPUUsage = 0;
	__int64 LastCurrentProcessCPUUsage = 0;

	int CurrentDelta;
	int TotalDelta;

	__int64 TotalProcessCPUUsage = 0;
	__int64 CurrentProcessCPUUsage = 0;

	/////////////////////////////////
	BYTE ProcData[dwInfoSize];
	pProcInfo = (PVOID)ProcData;
	DWORD dwReturnLen = 0;
	Utils::m_Instance->m_NtQuerySystemInformation(5,pProcInfo,dwInfoSize,&dwReturnLen);

	pProcessInfo = (PPROCESSINFO)pProcInfo;

	do
	{
		TotalProcessCPUUsage += (__int64)pProcessInfo->KernelTime.QuadPart + (__int64)pProcessInfo->UserTime.QuadPart;

		if(pProcessInfo->dwProcessID == dwID)
		{
			dwWorkingSet = pProcessInfo->dwWorkingSet; 
			CurrentProcessCPUUsage += (__int64)pProcessInfo->KernelTime.QuadPart + (__int64)pProcessInfo->UserTime.QuadPart;
		}

		/////////
		if(pProcessInfo->dwOffset == 0)
		{
			break;
		}

		pProcessInfo = (PPROCESSINFO)((byte*)pProcessInfo + pProcessInfo->dwOffset);
	}
	while(true);

	TotalDelta = (int) (TotalProcessCPUUsage - LastTotalProcessCPUUsage);
	CurrentDelta =(int) (CurrentProcessCPUUsage - LastCurrentProcessCPUUsage);

	if(TotalDelta != 0)
		cpuusage = 100 * CurrentDelta / TotalDelta;

	//this->Caption = "CPU = " + IntToStr(100 * CurrentDelta / TotalDelta) + 
	//"Memory = "+ IntToStr(dwWorkingSet / 1024) " K";

	LastTotalProcessCPUUsage = TotalProcessCPUUsage;
	LastCurrentProcessCPUUsage = CurrentProcessCPUUsage;

	//*MemUsage = dwWorkingSet>>10;
	return cpuusage;


}

BOOL Utils::Rest()
{
	Sleep(0);
	return TRUE;
}

BOOL Utils::StartProcess( LPCTSTR ProcPath, LPCTSTR Title )
{
	CString CmdPath = ProcPath;
	CString DirPath = ProcPath;
	CString PTitle = Title;
	int pos = DirPath.ReverseFind(L'\\');
	DirPath = DirPath.Mid(0, pos);
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof si;
	si.lpTitle = PTitle.GetBuffer();
	si.wShowWindow = SW_MINIMIZE;
	si.dwFlags = STARTF_USESHOWWINDOW;
	LPWSTR pCmd = CmdPath.GetBuffer();
	PROCESS_INFORMATION pi;
	BOOL ret = CreateProcess(NULL, pCmd, NULL, NULL,FALSE, NULL, NULL,(LPCTSTR)DirPath, &si, &pi );
	PTitle.ReleaseBuffer();
	CmdPath.ReleaseBuffer();
	return TRUE;
}

HWND Utils::GetProcessHWND( DWORD PID )
{
	HWND h = ::GetTopWindow(0);
	TCHAR Text[1024] = {0};
	while ( h )
	{
		DWORD pid;
		DWORD dwTheardId = ::GetWindowThreadProcessId( h,&pid);
		if ( pid == PID )
		{
			// here h is the handle to the window
			break;
		}
		h = ::GetNextWindow( h , GW_HWNDNEXT);
	}

	return h;
}


Utils* Utils::m_Instance = NULL;

Utils::Utils()
{
	Utils::m_Instance = this;
	m_NtQuerySystemInformation = (NtQuerySystemInformation)
		GetProcAddress( GetModuleHandle( L"ntdll.dll" ),"NtQuerySystemInformation" );
}

Utils * Utils::instance()
{
	return Utils::m_Instance;
}

DWORD Utils::GetMemUsage( DWORD dwID )
{
	PROCESS_MEMORY_COUNTERS pmc = {0};
	pmc.cb = sizeof(pmc);
	HANDLE hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE, dwID);
	if(hProc)
	{
		
		if( GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc)) )
		{

		}
	}
	::CloseHandle(hProc);
	return pmc.WorkingSetSize;
}

BOOL Utils::StartService( LPCTSTR serviceName )
{
	BOOL startReturn = FALSE;
	SC_HANDLE sc = OpenSCManager(NULL, NULL, GENERIC_EXECUTE)	;
	if(sc)
	{
		SC_HANDLE ser = ::OpenService(sc, serviceName, GENERIC_EXECUTE);
		if(ser)
		{
			startReturn = ::StartService(ser,0, NULL);
			::CloseServiceHandle(ser);
		}
		::CloseServiceHandle(sc);
	}
	else
	{
		
	}
	return startReturn;
}

BOOL Utils::StopService( LPCTSTR serviceName )
{
	BOOL startReturn = FALSE;
	SC_HANDLE sc = OpenSCManager(NULL, NULL, GENERIC_EXECUTE)	;
	if(sc)
	{
		SC_HANDLE ser = ::OpenService(sc, serviceName, GENERIC_EXECUTE);
		if(ser)
		{
			SERVICE_STATUS ser_status;
			startReturn = ::ControlService(ser, SERVICE_CONTROL_STOP,  &ser_status);
			::CloseServiceHandle(ser);
		}
		::CloseServiceHandle(sc);
	}
	else
	{

	}
	return startReturn;
}

BOOL Utils::RestartService( LPCTSTR serviceName )
{
	BOOL startReturn = FALSE;
	SC_HANDLE sc = OpenSCManager(NULL, NULL, GENERIC_EXECUTE)	;
	if(sc)
	{
		SC_HANDLE ser = ::OpenService(sc, serviceName, GENERIC_EXECUTE);
		if(ser)
		{
			SERVICE_STATUS ser_status;
			startReturn = ::ControlService(ser, SERVICE_CONTROL_STOP,  &ser_status);
			if(startReturn)
			{
				startReturn = ::StartService(ser, 0, NULL);
			}
			::CloseServiceHandle(ser);
		}
		::CloseServiceHandle(sc);
	}
	else
	{

	}
	return startReturn;
}
void CPUsage::Update()
{
	GetSystemTimes((LPFILETIME)&m_AllIdleTime, (LPFILETIME)&m_AllKernelTime, (LPFILETIME)&m_AllUserTime);
	static FILETIME CreateTime;
	GetProcessTimes(m_hProc, &CreateTime, (LPFILETIME)&m_CurrentIdleTime, (LPFILETIME)&m_CurrentKernelTime, (LPFILETIME)&m_CurrentUserTimeTime);
}

CPUsage::CPUsage(DWORD PID)
{
	m_hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE, PID);
	if(m_hProc)
	{
		Update();
		Save();
	}

	
}

CPUsage::~CPUsage()
{
	::CloseHandle(m_hProc);
}

FLOAT CPUsage::GetUsage(BOOL bUpdate)
{
	if(bUpdate)Update();
	FLOAT f = 0.0f;
	
	ULONGLONG u = m_CurrentUserTimeTime.QuadPart - m_LastCurrentUserTimeTime.QuadPart
		+ m_CurrentKernelTime.QuadPart - m_LastCurrentKernelTime.QuadPart;
		//- m_CurrentIdleTime.QuadPart + m_LastCurrentIdleTime.QuadPart;
	ULONGLONG s = m_AllKernelTime.QuadPart - m_LastAllKernelTime.QuadPart 
		+ m_AllUserTime.QuadPart - m_LastAllUserTime.QuadPart;
		//- m_AllIdleTime.QuadPart + m_LastAllIdleTime.QuadPart;
	
	if( u != 0)
		f = (float)(100 * u)/(float)s;

	if(bUpdate) Save();
	return f;
}

void CPUsage::Save()
{
	m_LastAllKernelTime		=	m_AllKernelTime;
	m_LastAllUserTime		=	m_AllUserTime;
	m_LastAllIdleTime		=	m_AllIdleTime;
							 
	m_LastCurrentKernelTime	=	m_CurrentKernelTime;
	m_LastCurrentUserTimeTime=	m_CurrentUserTimeTime;
	m_LastCurrentIdleTime	=	m_CurrentIdleTime;
	
}

DWORD CPUsage::GetMemUsage()
{
	PROCESS_MEMORY_COUNTERS pmc = {0};
	pmc.cb = sizeof(pmc);
	GetProcessMemoryInfo(m_hProc, &pmc, sizeof(pmc));
	return pmc.WorkingSetSize;
}

DWORD CPUsage::GetVMemUsage()
{
	PROCESS_MEMORY_COUNTERS pmc = {0};
	pmc.cb = sizeof(pmc);
	GetProcessMemoryInfo(m_hProc, &pmc, sizeof(pmc));
	return pmc.PagefileUsage;
}

FLOAT CPUsage::GetSysUsage(BOOL bUpdate)
{
	FLOAT usage = 0.0f;
	if(bUpdate) Update();
	ULONGLONG i = m_AllIdleTime.QuadPart - m_LastAllIdleTime.QuadPart;
	ULONGLONG u = m_AllKernelTime.QuadPart - m_LastAllKernelTime.QuadPart + m_AllUserTime.QuadPart - m_LastAllUserTime.QuadPart;
	ULONGLONG s = u - i;
	usage = (float)(100 *s)/(float)u;

	if(bUpdate) Save();

	return usage;
}

NetUsage::NetUsage()
{
	m_OutLen = 0;
	GetIfTable(NULL, &m_OutLen, FALSE);
	m_OutBuf = new BYTE[m_OutLen];
	DWORD ret = GetIfTable((PMIB_IFTABLE)m_OutBuf, &m_OutLen, FALSE);
	USES_CONVERSION;
	if(ret == NO_ERROR)
	{
		MIB_IFTABLE *pIfTable = (MIB_IFTABLE *)m_OutBuf;
		for(DWORD a=0; a<pIfTable->dwNumEntries; a++)
		{
			MIB_IFROW & row = pIfTable->table[a];
			NetIf ni;
			ni.InSpeed = 0;
			ni.LastIn = row.dwInOctets;
			ni.LastOut = row.dwOutOctets;
			ni.Name = A2W((LPCSTR)row.bDescr);
			ni.OutSpeed = 0;
			m_IfRow.push_back(ni);

		}
		
	}
	SYSTEMTIME st;
	::GetLocalTime(&st);
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	//m_LastTime = time((time_t *)&m_LastTime);
	memcpy(&m_LastTime, &ft, sizeof(ULONGLONG));
}

NetUsage::~NetUsage()
{
	delete [] m_OutBuf;
}

BOOL NetUsage::Update()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	
	ULONGLONG t = 0;
	memcpy(&t, &ft, sizeof(ULONGLONG));
	DWORD ret = GetIfTable((PMIB_IFTABLE)m_OutBuf, &m_OutLen, FALSE);
	if(ret == NO_ERROR)
	{
		MIB_IFTABLE *pIfTable = (MIB_IFTABLE *)m_OutBuf;
		for(DWORD a=0; a<pIfTable->dwNumEntries; a++)
		{
			MIB_IFROW & row = pIfTable->table[a];
			NetIf & ni = m_IfRow[a];
			DWORD n =(DWORD) (t - m_LastTime)/10000;
			if(n==0) break;
			ni.InSpeed = (1000*(row.dwInOctets - ni.LastIn))/n;
			ni.OutSpeed = (1000*(row.dwOutOctets - ni.LastOut))/n;
			ni.LastOut = row.dwOutOctets;
			ni.LastIn = row.dwInOctets;
			
		}
		
	}
	memcpy(&m_LastTime, &t, sizeof(ULONGLONG));
	return ret;
}


ServiceChecker *ServiceChecker::s_Instance = NULL;

ServiceChecker * ServiceChecker::Instance()
{
	if( ServiceChecker::s_Instance == NULL)
	{
		ServiceChecker::s_Instance = new ServiceChecker;
	}
	return ServiceChecker::s_Instance;
}

BOOL ServiceChecker::Init()
{
	BOOL bInitReturn = FALSE;
	SC_HANDLE hscman = OpenSCManager( NULL, NULL, GENERIC_READ );
	if(hscman)
	{
		DWORD needsize =0 ;
		DWORD resumehandle = 0;
		DWORD returnednumber = 0;
		BOOL bret = EnumServicesStatus(hscman, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &needsize, &returnednumber, &resumehandle );
		
		if(bret == FALSE)
		{
			BYTE *buffer = new BYTE[needsize];
			bret = EnumServicesStatus(hscman, SERVICE_WIN32, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUSW)buffer, needsize, &needsize, &returnednumber, &resumehandle );
			if(bret)
			{
				AddService( hscman , (LPENUM_SERVICE_STATUSW)buffer, returnednumber )	;
			}
			else
			{
				while( ( bret == FALSE )  && ( ::GetLastError() == ERROR_MORE_DATA ) )
				{
					AddService((HANDLE)hscman, (LPENUM_SERVICE_STATUSW)buffer, returnednumber );
					bret = EnumServicesStatus( hscman,SERVICE_WIN32, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUSW)buffer, needsize, &needsize, &returnednumber, &resumehandle );
				}
				AddService( (HANDLE)hscman, ( LPENUM_SERVICE_STATUSW)buffer, returnednumber );
				
			}
			delete buffer;
			
		}
		CloseServiceHandle(hscman);
	}
	return bInitReturn;
}

void ServiceChecker::AddService( HANDLE sc, LPENUM_SERVICE_STATUSW pser_status, DWORD number )
{
	BYTE *buffer = new BYTE[1024];
	LPQUERY_SERVICE_CONFIG pser_config = (LPQUERY_SERVICE_CONFIG)buffer;
	DWORD needsize = 0;
	Service ser;
	for(DWORD a=0; a<number; a++)
	{
		SC_HANDLE  handle = ::OpenService((SC_HANDLE)sc, pser_status[a].lpServiceName, GENERIC_READ );
		if(handle)
		{
			BOOL bret = QueryServiceConfig(handle,pser_config, 1024, &needsize);
			if(bret)
			{
				ser.m_CommandLine = pser_config->lpBinaryPathName;
				ser.m_ServiceName = pser_status[a].lpServiceName;
				ser.m_Status	  = pser_status[a].ServiceStatus.dwCurrentState;
				m_ServiceList.Add(ser);
				
			}
			CloseServiceHandle(handle);
		}

		
	}
}

BOOL ServiceChecker::GetServerNameByCommandLine( const TCHAR * commondline ,TCHAR *ser_name  )
{
	for(DWORD a=0; a< m_ServiceList.GetCount(); a++)
	{
		if(m_ServiceList[a].m_CommandLine == commondline)
		{
			lstrcpy(ser_name,  m_ServiceList[a].m_ServiceName);			
			return TRUE;
		}
	}
	return FALSE;
}