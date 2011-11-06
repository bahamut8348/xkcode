#pragma once


class IOCPHandler
{
public:
	virtual HANDLE GetHandle( ) = 0 ;
	virtual void OnDataRead( stBuffer* buf ) {};
	virtual void OnDataWrite( stBuffer* buf ) {};
	virtual void OnDataRead( stBuffer* buf, const stAddr& from ){};
	virtual void OnDataWrite( stBuffer* buf, const stAddr& from ){};
};


class IOCP
{
private:
	HANDLE mhIOCP;
	DWORD  mSequence;
	struct PerIO : OVERLAPPED
	{
		enum Type
		{
			_FILE_READ,
			_FILE_WRITE,
			_TCP_WRITE,
			_TCP_READ,
			_UDP_WRITE,
			_UDP_READ,
		};
		
		PerIO()
		{
			::ZeroMemory( this, sizeof( *this ) );
		}

		void SetFileOpt( Type type, stBuffer*  buffer_ptr )
		{
			::ZeroMemory( this, sizeof( *this ) );
			mType = type;
			mBuffer = buffer_ptr;
		}

		void SetUdpOpt( BOOL bRead, stBuffer* buffer_ptr, const stAddr& addr )
		{
			mType =  bRead?_UDP_READ:_UDP_WRITE;
			mBuffer = buffer_ptr;
			mWSABuf.buf = (CHAR*)buffer_ptr->GetBuffer();
			mAddr.sin_addr.S_un.S_addr = addr.mIP.mIP.mIPLong;
			mAddr.sin_port = htons( addr.mPort );
			mWSABuf.len = buffer_ptr->GetLength();
			mAddrLen = sizeof(mAddr);
		}

		Type mType;
		stBuffer* mBuffer;
		DWORD mSeq;
		SOCKADDR_IN mAddr;
		int mAddrLen;
		WSABUF mWSABuf;
	};

	ObjectManagerT<PerIO> mIOMgr;

public:
	static IOCP* sInstance;
	static IOCP* Instance( );


	BOOL AddHandler( IOCPHandler *handler );
	BOOL ReadFile( IOCPHandler* handler, stBuffer *pbuf  );
	BOOL WriteFile( IOCPHandler* handler , stBuffer* buf );
	BOOL SendTo( IOCPHandler* handler , stBuffer* pbuf, const stAddr& to );
	BOOL RecvFrom( IOCPHandler* handler, stBuffer* buf );

	DWORD Update();

	IOCP(void);
	~IOCP(void);
};


