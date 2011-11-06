#include "StdAfx.h"
#include <string>
#include <vector>
#include <windows.h>
#include <IPHlpApi.h>
#include "Utils.h"



std::vector<Utils::TapInfo> Utils::GetAllTap()
{
	std::vector<Utils::TapInfo> ret;
	HKEY adapter_key;
	LONG   status = RegOpenKeyEx( HKEY_LOCAL_MACHINE, ADAPTER_KEY, 0, KEY_READ, &adapter_key );
	if ( status != ERROR_SUCCESS )
	{
		return ret;
	}
	DWORD i = 0;
	while ( true )
	{
		DWORD data_type;

		TCHAR enum_name[256];
		DWORD len = 256;
		status = RegEnumKeyEx( adapter_key, i, enum_name, &len , NULL, NULL, NULL, NULL );
		i++;
		if( status != ERROR_SUCCESS )
		{
			break;
		}

		TCHAR unit_string[256];
		len = 256;
		wsprintf(unit_string, L"%s\\%s", ADAPTER_KEY, enum_name );
		HKEY unit_key;

		status = RegOpenKeyEx( HKEY_LOCAL_MACHINE, unit_string, 0, KEY_READ, &unit_key);
		if( status != ERROR_SUCCESS )
		{
			break;
		}
		TCHAR component_id[256];
		len = sizeof( component_id );
		status = RegQueryValueEx( unit_key, L"ComponentId", NULL, &data_type, (LPBYTE)component_id, &len );
		if( status != ERROR_SUCCESS )
		{
			::RegCloseKey (unit_key);
			continue;
		}
		
		TCHAR net_cfg_instance_id[256];
		len = sizeof( net_cfg_instance_id );
		status = RegQueryValueEx( unit_key, L"NetCfgInstanceId", NULL, &data_type, (LPBYTE)net_cfg_instance_id, &len );
		if( status != ERROR_SUCCESS )
		{
			::RegCloseKey (unit_key);
			continue;
		}

		if( REG_SZ != data_type )
		{
			::RegCloseKey( unit_key );
			continue;
		}

		if( _tcscmp( TAP_COMPONENT_ID, component_id ) == 0 )
		{
			TapInfo info;
			info.mGuid = net_cfg_instance_id;
			ret.push_back( info );
		}
		::RegCloseKey (unit_key);
		

	}

	::RegCloseKey( adapter_key );

	return ret;
}

std::vector<Utils::PanelReg> Utils::GetPanelReg()
{
	std::vector<Utils::PanelReg> ret;
	HKEY network_connections_key;
	LONG status = 0;
	status = RegOpenKeyEx( HKEY_LOCAL_MACHINE, NETWORK_CONNECTIONS_KEY, 0, KEY_READ, &network_connections_key );
	if( status != ERROR_SUCCESS )
	{
		return ret;
	}
	
	DWORD i = 0;
	while( true )
	{
		DWORD len;
		TCHAR enum_name[256];
		len = 256;
		status = RegEnumKeyEx( network_connections_key, i++, enum_name, &len, NULL, NULL, NULL, NULL );
		if( status != ERROR_SUCCESS )
		{
			break;
		}
		
		HKEY connection_key ;
		TCHAR connection_string[1024];
		wsprintf(connection_string, L"%s\\%s\\Connection" , NETWORK_CONNECTIONS_KEY, enum_name);
		status = RegOpenKeyEx( HKEY_LOCAL_MACHINE, connection_string, 0, KEY_READ, &connection_key );
		if( status != ERROR_SUCCESS )
		{
			continue;
		}

		TCHAR name_data[256];
		DWORD name_type;
		len = sizeof( name_data );
		status = RegQueryValueEx( connection_key, L"Name", NULL, &name_type, (LPBYTE)name_data, &len);
		if( status != ERROR_SUCCESS )
		{
			::RegCloseKey( connection_key );
			continue;
		}
		
		if( name_type != REG_SZ )
		{
			::RegCloseKey( connection_key );
			continue;
		}

		PanelReg reg;
		reg.mGuid = enum_name;
		reg.mName = name_data;
		
		::RegCloseKey( connection_key );
		
		ret.push_back( reg );
	}
	::RegCloseKey( network_connections_key );

	return ret;
}

int Utils::GetInterfaceIndex( LPCTSTR guid )
{
	TCHAR buf[256];
	swprintf_s ( buf, L"\\DEVICE\\TCPIP_%s", guid);
	ULONG idx = 0;
	DWORD ret = ::GetAdapterIndex( buf, &idx );
	if( ret == NO_ERROR )
	{
		return ret;
	}
	return -1;

}