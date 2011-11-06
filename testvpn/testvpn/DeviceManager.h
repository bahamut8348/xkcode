#pragma once

class DeviceManager
{
public:
	static DeviceManager* s_instance;
	static DeviceManager*  Instance();

	Device* CreateDevice( LPCTSTR guid );

	BOOL AddDevice( Device* dev );

	BOOL Write( const stIP& ip, LPBYTE buffer, DWORD length );

	Device* GetDeviceFromIP( const stIP& ip );

	DeviceManager(void);
	~DeviceManager(void);

	std::vector<Device*> mDevs;
	HANDLE mhIOCP;
};
