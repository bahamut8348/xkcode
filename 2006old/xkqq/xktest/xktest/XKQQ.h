#pragma once

class XKQQ
{
public:

	struct BuddyInfo
	{
		stQQNum mQQ;
		std::string  mNickname;
	};


	struct ResponseCheckInfo
	{
		ResponseCheckInfo( )
		{
			mBuffer = 0;
			mLength = 0;
			mSeq = 0;
			mCheckTimer = 0;
			mCheckCount = 0;
		}
		void Create( dword seq, asio::io_service & io, dword bufferlen, const byte* buffer)
		{
			mSeq = seq;
			mBuffer = new byte[bufferlen];
			mLength = bufferlen;
			mCheckTimer = new asio::deadline_timer( io );
		}

		void Release()
		{
			delete mBuffer;
			delete mCheckTimer;
		}

		dword mSeq;
		byte* mBuffer;
		dword mLength;
		dword mCheckCount ;
		asio::deadline_timer *mCheckTimer;
	};


	template< typename T >
	void SendCommand( const T * cmd, stQQBuffer<16> & key , word seq )
	{
		SendCommand( T::_CMD, key,  seq, (byte*)cmd, cmd->GetSize() );
	}

	void SendCommand( word cmd, const byte* buffer, dword length );

	void SendCommand( word cmd, stQQBuffer<16> & key, word seq, const byte* buffer, dword length );

	template< typename T >
	void SendCommand( const T * cmd, stQQBuffer<16> & key )
	{
		SendCommand( cmd, key, GetSequence() );
	}

	template< typename T >
	void SendCommand( const T * cmd, word sequence )
	{
		SendCommand( cmd, mSession, sequence );
	}

	template< typename T >
	void SendCommand( const T * cmd)
	{
		SendCommand( cmd, GetSequence() );
	}
	
	void SendData( word cmd, const byte* buffer, dword length ); 
	void SendData( word cmd, const byte* buffer1, dword length1, const byte* buffer2, dword length2 );
	void SendData( word cmd, word seq, const byte* buffer1, dword length1, const byte* buffer2, dword length2);

	void SaveCommand( word seq, const byte* buffer, dword bufferlength );
	void CheckCommand( word seq, dword delay = 300 );
	void RemoveCommand( word seq );

	void SendDataResult(const boost::system::error_code & error, size_t bytes_sent);
	void OnDataReceive(const boost::system::error_code& error, size_t bytes_recvd);
	void StartRecvData( );
	word GetSequence();
	void SendData( const char* buffer, int length  );

	void OnEvent_TokenRes( const stQQResHeader & header, stTokenRes *pRes );
	void OnEvent_LoginRes( const stQQResHeader & header,stLoginRes *pRes );
	void OnEvent_IMPacketRes( const stQQResHeader & header, stIMPacketRes *pRes );
	void OnEvent_KeepAliveRes( const stQQResHeader & header, stKeepAliveRes *pRes );
	void OnEvent_GetBuddyListRes( const stQQResHeader & header, stGetBuddyListRes *pRes );
	void OnEvent_IMPacketReply( const stQQResHeader & header, stIMPacketReply *pRes );

	void SendIMRes( dword recver, dword msgid, const stQQIP & sendip, dword sequence );
	void SendRequestKey( );
	void SendPropOp( );
	void SendKeepAlive( );
	
	enum
	{
		_DATA_LENGTH = 1024,
	};

	u_int mQQnum;

	udp::socket mSock;
	udp::endpoint mServer;
	udp::endpoint mSender;

	byte	mData[ _DATA_LENGTH ];
	static asio::io_service g_IO;
	asio::deadline_timer mTimer;
	asio::deadline_timer mCheckTimer;

	dword	mSequence;
	IEvent* mpEvent;
	stQQSession		mSession;
	stQQToken		mToken;
	stQQEncryptPass	mEncryptPass;  // tea::encrypt("",0,Md5pass)
	stQQInitKey		mInitKey;
	stQQMd5Pass		mMd5Pass;
	stQQClientKey   mClientKey;

	std::vector<BuddyInfo> mBuddyList;
	typedef stdext::hash_map<dword, ResponseCheckInfo> ResponseCheckInfoMap;
	ResponseCheckInfoMap mResponseInfo;

public:
	XKQQ(void);
	~XKQQ(void);
	
	void SendTokenRequest( );
	void SendLogin( const std::string& password  );
	void SendChat( dword qq, const std::string& msg, dword seqid = 0, dword msgid = 0);
	void SendGetBuddyList( dword startpos );

	void SetServer( const std::string& ip, int port );
	static void UpDate( );

};


