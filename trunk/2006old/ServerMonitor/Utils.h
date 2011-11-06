#pragma once



class ServiceChecker
{
public:


	static ServiceChecker * Instance();
	static ServiceChecker *s_Instance;
	struct Service
	{
		CAtlString m_CommandLine;
		CAtlString m_ServiceName;
		DWORD	   m_Status;
	};
	
	BOOL Init();
	
	void AddService(HANDLE sc, LPENUM_SERVICE_STATUSW pser_status, DWORD number);
	BOOL GetServerNameByCommandLine( const TCHAR * commondline, TCHAR *ser_name );
	
	CAtlArray<Service> m_ServiceList;
};


class ViewType
{
public:
	enum
	{
		SUMMARY_TYPE,
		STATUS_TYPE,
	};
	virtual DWORD type() = 0;
};


class CPUsage
{
public:
	ULARGE_INTEGER m_LastAllKernelTime;
	ULARGE_INTEGER m_LastAllUserTime;
	ULARGE_INTEGER m_LastAllIdleTime;

	ULARGE_INTEGER m_LastCurrentKernelTime;
	ULARGE_INTEGER m_LastCurrentUserTimeTime;
	ULARGE_INTEGER m_LastCurrentIdleTime;

	ULARGE_INTEGER m_AllKernelTime;
	ULARGE_INTEGER m_AllUserTime;
	ULARGE_INTEGER m_AllIdleTime;

	ULARGE_INTEGER m_CurrentKernelTime;
	ULARGE_INTEGER m_CurrentUserTimeTime;
	ULARGE_INTEGER m_CurrentIdleTime;

	~CPUsage();
	CPUsage(DWORD PID);
	
	HANDLE m_hProc;

	void Update();
	void Save() ;
	FLOAT GetUsage(BOOL bUpdate = TRUE);
	FLOAT GetSysUsage(BOOL bUpdate = TRUE);
	DWORD GetMemUsage();
	DWORD GetVMemUsage();
};


class NetUsage
{
public:
	BYTE *m_OutBuf;
	DWORD m_OutLen;
	NetUsage();
	~NetUsage();

	DWORD m_InSpeed;
	DWORD m_OutSpeed;

	struct NetIf
	{
		std::wstring Name;
		DWORD InSpeed;
		DWORD OutSpeed;
		DWORD LastIn;
		DWORD LastOut;

	};

	std::vector<NetIf> m_IfRow;

	BOOL Update();

public:
	ULONGLONG m_LastTime;
};



class Utils
{
public:
	typedef long ( __stdcall *NtQuerySystemInformation )( DWORD, PVOID, DWORD, PULONG );


	struct ProcessInfo
	{
		CAtlString Path;
		CAtlString Name;
		DWORD		PID;
		VS_FIXEDFILEINFO FileInfo;
		WORD LangID;
		WORD Charset;
		CAtlString CompanyName;
		CAtlString FileVersion;

	};

	struct TRANSLATION {
		WORD langID;         // language ID
		WORD charset;        // character set (code page)
	};




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

// for process cpu
	typedef struct _THREAD_INFO
	{
		LARGE_INTEGER CreateTime;
		DWORD dwUnknown1;
		DWORD dwStartAddress;
		DWORD StartEIP;
		DWORD dwOwnERPID;
		DWORD dwThreadId;
		DWORD dwCurrentPriority;
		DWORD dwBasePriority;
		DWORD dwContextSwitches;
		DWORD Unknown;
		DWORD WaitReason;

	}THREADINFO, *PTHREADINFO;


	typedef struct _UNICODE_STRING
	{
		USHORT Length;
		USHORT MaxLength;
		PWSTR Buffer;
	} UNICODE_STRING;



	typedef struct _PROCESS_INFO
	{
		DWORD dwOffset;
		DWORD dwThreadsCount;
		DWORD dwUnused1[6];
		LARGE_INTEGER CreateTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER KernelTime;
		UNICODE_STRING ProcessName;

		DWORD dwBasePriority;
		DWORD dwProcessID;
		DWORD dwParentProcessId;
		DWORD dwHandleCount;
		DWORD dwUnused3[2];

		DWORD dwVirtualBytesPeak;
		DWORD dwVirtualBytes;
		ULONG dwPageFaults;
		DWORD dwWorkingSetPeak;
		DWORD dwWorkingSet;
		DWORD dwQuotaPeakPagedPoolUsage;
		DWORD dwQuotaPagedPoolUsage;
		DWORD dwQuotaPeakNonPagedPoolUsage;
		DWORD dwQuotaNonPagedPoolUsage;
		DWORD dwPageFileUsage;
		DWORD dwPageFileUsagePeak;

		DWORD dCommitCharge;
		THREADINFO ThreadSysInfo[1];

	} PROCESSINFO, *PPROCESSINFO;

// for process command line
	typedef struct
	{
		ULONG          AllocationSize;
		ULONG          ActualSize;
		ULONG          Flags;
		ULONG          Unknown1;
		UNICODE_STRING Unknown2;
		HANDLE         InputHandle;
		HANDLE         OutputHandle;
		HANDLE         ErrorHandle;
		UNICODE_STRING CurrentDirectory;
		HANDLE         CurrentDirectoryHandle;
		UNICODE_STRING SearchPaths;
		UNICODE_STRING ApplicationName;
		UNICODE_STRING CommandLine;
		PVOID          EnvironmentBlock;
		ULONG          Unknown[9];
		UNICODE_STRING Unknown3;
		UNICODE_STRING Unknown4;
		UNICODE_STRING Unknown5;
		UNICODE_STRING Unknown6;
	} PROCESS_PARAMETERS, *PPROCESS_PARAMETERS;


	typedef struct
	{
		ULONG               AllocationSize;
		ULONG               Unknown1;
		HINSTANCE           ProcessHinstance;
		PVOID               ListDlls;
		PPROCESS_PARAMETERS ProcessParameters;
		ULONG               Unknown2;
		HANDLE              Heap;
	} PEB, *PPEB;


	typedef struct
	{
		DWORD ExitStatus;
		PPEB  PebBaseAddress;
		DWORD AffinityMask;
		DWORD BasePriority;
		ULONG UniqueProcessId;
		ULONG InheritedFromUniqueProcessId;
	}   PROCESS_BASIC_INFORMATION;


	template<class __Kty, int __BucketSize = 20,
	class __Pr = _STD less<__Kty> >
	class hash_xk : public	 stdext::hash_compare<__Kty, __Pr>
	{	
		public:
			enum
			{	
				bucket_size = __BucketSize,	// 0 < bucket_size
				min_buckets = 8
			};
	};

	struct IPComp

	{   //   define   hash   function   for   strings   

		enum   
		{   
			bucket_size   =   3,   
			min_buckets   =   8,
		};  


		size_t   operator()(const    MIB_TCPROW_OWNER_PID *  o1)   const   
		{   
			return o1->dwLocalPort + o1->dwRemoteAddr + o1->dwRemotePort;
		}   

		bool   operator()(const MIB_TCPROW_OWNER_PID *  o1,   const  MIB_TCPROW_OWNER_PID *  o2)   const   
		{   
			return   (o1->dwLocalAddr < o2->dwLocalAddr) | (o1->dwLocalPort < o2->dwLocalPort) | (o1->dwRemoteAddr<o2->dwRemoteAddr) | (o1->dwRemotePort < o2->dwRemotePort);
		}   
	};   
	

	
	static Utils *m_Instance;
	Utils();
	static Utils *instance();

	

	NtQuerySystemInformation m_NtQuerySystemInformation;

	static BOOL GetProcessInfo( CAtlArray< Utils::ProcessInfo > & ProcList);
	static BOOL GetOpenedPort( std::vector<MIB_TCPROW_OWNER_PID> & TCPtables, std::vector<MIB_UDPROW_OWNER_PID> & UDPtables);


	static CString MakeAddr(DWORD Address, DWORD Port);
	static CString ConnStatus(DWORD value);

	static BOOL StartProcess(LPCTSTR ProcPath, LPCTSTR Title);
	static BOOL KillProcess(DWORD PID);
	static BOOL ProcessExit(DWORD PID);

	static BOOL StartService( LPCTSTR serviceName);
	static BOOL StopService(LPCTSTR serviceName);
	static BOOL RestartService(LPCTSTR serviceName);

	static BOOL GetProcessCommandLine(DWORD dwId,LPTSTR CmdLine, LPDWORD CmdLen);
	static DWORD GetCPUUsage(DWORD dwID,PULONG MemUsage);
	static DWORD GetMemUsage(DWORD dwID);
	static HWND GetProcessHWND(DWORD PID);

	static BOOL GetDebugPriv();

	static BOOL Rest();

};
