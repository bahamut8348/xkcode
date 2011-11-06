#pragma once



#define TAP_CONTROL_CODE(request,method) \
	CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)

// Present in 8.1

#define TAP_IOCTL_GET_MAC               TAP_CONTROL_CODE (1, METHOD_BUFFERED)
#define TAP_IOCTL_GET_VERSION           TAP_CONTROL_CODE (2, METHOD_BUFFERED)
#define TAP_IOCTL_GET_MTU               TAP_CONTROL_CODE (3, METHOD_BUFFERED)
#define TAP_IOCTL_GET_INFO              TAP_CONTROL_CODE (4, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_POINT_TO_POINT TAP_CONTROL_CODE (5, METHOD_BUFFERED)
#define TAP_IOCTL_SET_MEDIA_STATUS      TAP_CONTROL_CODE (6, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_MASQ      TAP_CONTROL_CODE (7, METHOD_BUFFERED)
#define TAP_IOCTL_GET_LOG_LINE          TAP_CONTROL_CODE (8, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_SET_OPT   TAP_CONTROL_CODE (9, METHOD_BUFFERED)


class Device : public IOCPHandler
{

	struct stCommandVersion
	{
		enum
		{
			_CMD = TAP_IOCTL_GET_VERSION,
		};
		ULONG mMinMajor;
		ULONG mMInManor;
		ULONG mbDebug;
	};

	struct stCommandMTU
	{
		enum {
			_CMD = TAP_IOCTL_GET_MTU,
		};
		ULONG mMTU;
	};

	struct stCommandP2P
	{
		enum{
			_CMD = TAP_IOCTL_CONFIG_POINT_TO_POINT,
		};
		stIP mLocal;
		stIP mNetmask;
	};

	struct stCommandDHCP
	{
		enum
		{
			_CMD = TAP_IOCTL_CONFIG_DHCP_MASQ,
		};
		stIP mLocal;
		stIP mAdaptNetmask;
		stIP mRemoteNetmask;
		stTime mDHCPLeaseTime;
	};

	struct  stCommandStatus 
	{
		enum
		{
			_CMD = TAP_IOCTL_SET_MEDIA_STATUS,
		};

		ULONG mbConnect;
	};

	enum IOStat
	{
		_IOStat_Reading_Done,
		_IOStat_Reading,
	};

public:
	
	std::wstring mName;
	std::wstring mGuid;
	stIP mLocalIP;

	DWORD mOperNumber;

	HANDLE mHandle;

	stBuffer mReadBuf;

	BOOL Create( LPCTSTR path );
	BOOL GetVersion( ULONG & major, ULONG& manor, ULONG& debug );
	BOOL SetIPAddr( LPCTSTR localip, LPCTSTR localNetmask, LPCTSTR remoteNetmask );
	BOOL SetConnect( BOOL bConnect );
	VOID Update( );
	VOID StartRecv();

	template <typename T >  BOOL DoCommand( T & cmd )
	{
		DWORD len = sizeof(T);
		return DeviceIoControl ( mHandle , T::_CMD, &cmd, sizeof (T), &cmd, sizeof (T), &len, NULL );
	}

	Device(void);
	~Device(void);

	stBuffer* GetReadBuffer( )
	{
		return &mReadBuf;
	}
	
	stBuffer* GetWriteBuffer( )
	{
		return NULL;
	}

	HANDLE GetHandle( );

	void OnDataRead( stBuffer* buf );
	void OnDataWrite( stBuffer* buf );

};

