#pragma  once 


enum enPack_Command
{
	_COMMAND_PRELOGIN,
};

struct stPack_Header
{
	BYTE mHeader;
	WORD mVersion;
	WORD mCommand;
	WORD mCallback;
};


template< typename T, DWORD CMD >
struct stPack_ReqBase
{
	enum
	{
		_CMD = CMD,
	};

	DWORD GetSize() const
	{
		return sizeof(T)
	}
};

template< typename T, DWORD CMD >
struct stPack_ResBase
{
	enum
	{
		_CMD = CMD,
	};

	DWORD GetSize() const
	{
		return sizeof(T)
	}
};

struct stPack_PreLogin_Req : stPack_ReqBase< stPack_PreLogin_Req, _COMMAND_PRELOGIN >
{
	
};

struct stPack_PreLogin_Res : stPack_ResBase< stPack_PreLogin_Res, _COMMAND_PRELOGIN >
{
	
};