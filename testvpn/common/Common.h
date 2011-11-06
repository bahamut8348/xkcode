#pragma  once 

#define  _Listen_Port 5222
#define  _PreServer_Port 5333
#define  _Server_Port 6333
struct stIP
{
	stIP()
	{
		mIP.mIPLong = 0;
	}

	stIP( LPCTSTR ip )
	{
		fromString( ip );
	}

	BOOL fromString( LPCTSTR ip )
	{
		if( ip == NULL )
		{
			mIP.mIPLong = 0;
			return TRUE;
		}
		if( _tcslen(ip) == 0 )
		{
			mIP.mIPLong = 0;
			return TRUE;
		}
		USES_CONVERSION;
		mIP.mIPLong =  inet_addr( W2A(ip) );
		return TRUE;
	}
	std::wstring toString() const
	{
		TCHAR buf[32];
		swprintf_s( buf, L"%d.%d.%d.%d", mIP.mIPByte.mIP1, mIP.mIPByte.mIP2, mIP.mIPByte.mIP3, mIP.mIPByte.mIP4 );
		return buf;
	}
	union 
	{
		ULONG mIPLong;
		struct 
		{
			BYTE mIP1;
			BYTE mIP2;
			BYTE mIP3;
			BYTE mIP4;
		} mIPByte;
	} mIP;
	
};

struct stAddr
{
	stIP mIP;
	u_short mPort;
	std::wstring toString() const
	{
		TCHAR buf[32];
		swprintf_s( buf, L"%s:%d", mIP.toString().c_str(), mPort );
		return buf;
	}
};

struct stTime
{
	ULONG mTime;
};

struct stBuffer
{
	enum
	{
		_MaxSize = 4096,
	};
	stBuffer( )
	{
		mLength = 0;
		mBuffer = NULL;
	}

	VOID Create()
	{
		if( mBuffer == NULL)
			mBuffer = new BYTE[ _MaxSize ];
	}

	VOID Set( LPBYTE buffer, DWORD length )
	{
		mLength = length;
		memcpy( mBuffer,buffer, length );
	}

	VOID Delete()
	{
		delete mBuffer;
		mLength = 0;
	}
	
	BYTE* GetBuffer()
	{ 
		assert( mBuffer );
		return mBuffer;
	}

	DWORD GetLength() const
	{
		return mLength ;
	}

	DWORD mLength;
	BYTE* mBuffer;
};

#pragma pack(push,1)

template< int Size>
struct stBufferBase
{
	BYTE mBuffer[Size];
	void Set( LPBYTE *buffer )
	{
		::memcpy_s( mBuffer, buffer, Size);
	}
};

typedef stBufferBase<6> stMac;

enum NetProto
{
	_Net_ARP = 0x0806,
	_Net_IP = 0x0800,
};

struct stNetProto
{
	WORD mProto;
	NetProto Type()const
	{
		return (NetProto) ntohs( mProto );
	}
};



struct NetEthernet
{
	stMac mFrom;
	stMac mTo;
	stNetProto  mProto;
};

struct NetIP
{
	BYTE 	mh_lenver; 	// 4位首部长度+4位IP版本号
	BYTE 	mTos; 			//8位服务类型TOS 
	WORD	mTotalLen; 	// 16位总长度（字节） 
	WORD	mIdent; 		//16位标识 
	WORD	mFrag_and_flags; //3位标志位 
	BYTE 	mTTL; 			//8位生存时间 TTL 
	BYTE 	mProto; 			//8位协议 (TCP, UDP 或其他) 
	WORD	mChecksum; 	// 16位IP首部校验和 
	stIP 	mSourceIP; 	//32位源IP地址 
	stIP	mDestIP; 		//32位目的IP地址 
};

struct NetPing
{
	WORD mHardwareType;
	WORD mProto;
	BYTE mHardwareSize;
	BYTE mProtoSize;
	WORD mOpcode;
	stMac mSenderMac;
	stIP  mSenderIP;
	stMac mRecverMac;
	stIP  mRecverIP;
};

#pragma pack(pop)


static DWORD ParseBuffer( LPBYTE buffer, stIP& from , stIP& to  )
{
	NetEthernet *pNet = ( NetEthernet* )buffer;
	LPBYTE netdata = (LPBYTE) (pNet+1);
	if( pNet->mProto.Type() == _Net_IP )
	{
		NetIP *pip = ( NetIP * )netdata;
		from = pip->mSourceIP;
		to = pip->mDestIP;
	}
	else if( pNet->mProto.Type() == _Net_ARP )
	{
		NetPing *ping = ( NetPing* )netdata;
		from = ping->mSenderIP;
		to = ping->mRecverIP;
	}
	else
	{
		return 0;
	}
	return (DWORD) pNet->mProto.Type();
}

template<typename T>
class SingletonT
{
public:
	static T* Instance( )
	{
		
		static T object;
		return &object;
	}
};



template<typename T>
class ObjectManagerT
{
	std::list<T*> mIdle;
public:
	T* CreateObject( )
	{
		T* ret = NULL;
		if( mIdle.begin() != mIdle.end() )
		{
			ret = *( mIdle.begin() );
			mIdle.pop_front();
		}
		else
		{
			ret = new T;
		}
		return ret;
	}

	void ReleaseObject( T* o )
	{
		mIdle.push_back( o );
	}
};

class BufferMgr : public SingletonT<BufferMgr> , public ObjectManagerT<stBuffer>
{

};



