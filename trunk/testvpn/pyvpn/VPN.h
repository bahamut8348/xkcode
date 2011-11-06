#pragma once


namespace pyvpn 
{

	namespace io = boost::asio;
	namespace bp = boost::python;


#define TAP_CONTROL_CODE(request,method) 	CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)

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


class VPN
{
public:
	VPN( const std::wstring & guid, const bp::object & o , bp::object  key = bp::object( ) );

	int mTimeout;
	HANDLE mWinHandle;

	bp::object mDataCallback;
	bp::object mDataCallbackKey;

	io::io_service & mService;
	io::windows::stream_handle mHandle;

	void SetConnect( bool bconnect );
	void SetIP( const std::wstring & localip, const std::wstring & localnetmask = L"255.255.255.0"  );
	bp::tuple GetVersion( );
	ULONG GetMTU( );

	void Read( );
	void Write( const bp::str &  s );

	char mReadBuffer[2048];
	char mWriteBuffer[2048];

	void HandleRead( const boost::system::error_code & err, std::size_t translen  );
	void HandleWrite( const  boost::system::error_code & err, std::size_t translen  );

	template< class T >
	bool DoCommand( int code,  T & cmd )
	{
		DWORD len = sizeof(T);
		return DeviceIoControl ( mWinHandle , code , &cmd, sizeof (T), &cmd, sizeof (T), &len, NULL );
	}

	~VPN(void);
};

void InitVPN();


}

