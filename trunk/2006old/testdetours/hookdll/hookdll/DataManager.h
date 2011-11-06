#pragma once




class DataManager : public Singleton<DataManager>
{

public:

	static const DWORD _CALL_MSG = WM_USER + 2;

	struct NetInfo
	{
		enum emType
		{
			_SEND,
			_SENDTO,
			_RECV,
			_RECVFROM,
			_WSASEND,
			_WSASENDTO,
		};

		emType mType;
		SOCKET mSock;
		SOCKADDR_IN mUdpPeerAddr;
		DWORD  mLength;
		BYTE  mData[1];
	};

	struct MemHeader
	{
		HWND mhWnd;
		DWORD mThreadID;
		DWORD mMsgType;
	};

private:
	HANDLE mhMem;
	HANDLE mhMutex;

	std::wstring mMemName;
	std::wstring mMutexName;
	
	DWORD mMemSize;

	HWND mhWnd;
	
	BYTE* mMemPtr;

public:
	DataManager(void);
	~DataManager(void);

	void SetHWND( HWND hwnd );
	HWND GetHWND() const;
	
	void SetData( DWORD threadID, DWORD msgtype, NetInfo::emType subtype, SOCKET sock, BYTE* data, DWORD datalen, SOCKADDR_IN addr   );
	void GetData( DWORD& threadID, DWORD& msgtype, NetInfo::emType& subtype, SOCKET& sock, BYTE* data, DWORD& datalen, SOCKADDR_IN& addr   );

	void Lock();
	void UnLock();
};
