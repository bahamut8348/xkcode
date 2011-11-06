#pragma once

#define ADAPTER_KEY L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}"
#define NETWORK_CONNECTIONS_KEY L"SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}"
#define  TAP_COMPONENT_ID L"tap0901"

class Utils
{
public:

	struct TapInfo
	{
		std::wstring mGuid;
	};

	struct PanelReg
	{
		std::wstring mGuid;
		std::wstring mName;
	};

	static std::vector<Utils::TapInfo> GetAllTap();
	static std::vector<Utils::PanelReg> GetPanelReg();
	static int GetInterfaceIndex( LPCTSTR guid );
	
};
