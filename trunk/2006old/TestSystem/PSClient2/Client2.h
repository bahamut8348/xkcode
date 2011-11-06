#pragma once


namespace PyPSClient2
{
	
	class Clog : public ILog
	{
	public:
		Clog():mObj(NULL){}
		_char buffer[4096];
		virtual _void log(const _char* Format, ... )
		{
			StringFormatter::FormatBufferV( buffer, 4096, Format, (_chara*)( &Format ) + sizeof( Format ) );
			if(mObj)
			{
				PyLock lock;
				call<void>(mObj, std::wstring(buffer));
			}
		}

		PyObject *mObj;

	};
	
	
	class CClient2
	{
	public:
		CClient2(void);
	public:
		~CClient2(void);


		void RegisterMessageCallback(object & obj);
		void SetLogHandler(object & obj);

		void SetAppInfo(_word AppID, _word SubID, _word MajorVer, _word SubVer);
		void SetInitIS(list & islist);
		void SetIS(const Address & addr);
		void ConnectToIS();
		std::wstring GetError(_dword ErrorCode);
		void Login( std::wstring username, std::wstring password);
		void Register(const stAP_CLIENT_INFO & Info);
		void QueryBuddyList( );
		
		// room
		void QueryAreaList( _dword AreaID );
		void QueryRoomList( _dword AreaID, _dword RoomType);


		// game
		void LoginToMMS(_dword PayLoad);
		void HandleDispatchGame(_dword Result, _dword GameID, list tokens, const SessionAddress & saddr);
		
		
		// update
		void Update();
		void Disconnect();
		
		IClientFramework* GetFramework() const;
		IUIProcessor *GetProcessor() const;
		


		object m_MsgCallback;
	
		IClientFramework *m_pFramework;
		CMessageCallback *m_Callback;
		Clog *m_pLog;
	};
}

