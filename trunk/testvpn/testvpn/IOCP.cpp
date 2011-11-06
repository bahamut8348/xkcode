#include "StdAfx.h"
#include "IOCP.h"

IOCP* IOCP::sInstance = NULL;

IOCP::IOCP(void)
{
	mhIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE , NULL, NULL, 1 );
	mSequence = 1;
}

IOCP::~IOCP( void )
{
}

IOCP* IOCP::Instance( )
{
	if( sInstance == NULL )
	{
		sInstance = new IOCP;
	}
	return sInstance;
}

BOOL IOCP::AddHandler( IOCPHandler *handler )
{
	HANDLE handle = ::CreateIoCompletionPort( handler->GetHandle(), mhIOCP, (ULONG_PTR)handler, 1 );
	if( handle != INVALID_HANDLE_VALUE )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD IOCP::Update( )
{
	DWORD  transfer_number = 0;
	ULONG_PTR key = 0;
	PerIO* perio = NULL;
	BOOL bret =  ::GetQueuedCompletionStatus( mhIOCP, &transfer_number, &key, (LPOVERLAPPED*)&perio, 1000 );
	if( bret )
	{
		perio->mBuffer->mLength = transfer_number;
		assert(transfer_number>0);
		IOCPHandler* handler = (IOCPHandler*)key;
		if( perio->mType == PerIO::_FILE_READ )
		{
			handler->OnDataRead( perio->mBuffer );
		}
		else if( perio->mType == PerIO::_FILE_WRITE )
		{
			handler->OnDataWrite( perio->mBuffer );
		}
		else if( perio->mType == PerIO::_UDP_READ )
		{
			stAddr addr;
			addr.mIP.mIP.mIPLong = perio->mAddr.sin_addr.S_un.S_addr;
			addr.mPort = ntohs( perio->mAddr.sin_port );
#ifdef LOG
			wprintf( L"Udp Data: %d, From: %s:%d\r\n", perio->mBuffer->GetLength(), addr.mIP.toString().c_str(), addr.mPort );
#endif
			handler->OnDataRead( perio->mBuffer, addr );
		}
		else if( perio->mType == PerIO::_UDP_WRITE )
		{
			stAddr addr;
			addr.mIP.mIP.mIPLong = perio->mAddr.sin_addr.S_un.S_addr;
			addr.mPort = ntohs( perio->mAddr.sin_port );
			handler->OnDataWrite( perio->mBuffer, addr );
		}
		;
		mIOMgr.ReleaseObject( perio );
	}
	
	return bret;
	
}

BOOL IOCP::ReadFile( IOCPHandler* handler, stBuffer *pbuf  )
{
	
	DWORD readbytes = 0;
	mSequence++;
	PerIO *pio = mIOMgr.CreateObject();
	pio->SetFileOpt( PerIO::_FILE_READ, pbuf );
	pio->mSeq = mSequence;
	BOOL bret = ::ReadFile( handler->GetHandle(), pbuf->GetBuffer(), stBuffer::_MaxSize, &readbytes, pio );
	if( bret > 0  )
	{
		pio->mBuffer->mLength = readbytes;
	}
	else
	{
		if( ::GetLastError() != ERROR_IO_PENDING )
		{
			printf("ReadFile Error: %d\n", ::GetLastError());
			assert(0);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL IOCP::WriteFile( IOCPHandler* handler , stBuffer* buf)
{
	DWORD writebytes = 0;
	mSequence ++;
	PerIO *pio = mIOMgr.CreateObject();
	pio->SetFileOpt( PerIO::_FILE_WRITE, buf );
	pio->mSeq = mSequence;
	BOOL bret = ::WriteFile( handler->GetHandle(), buf->GetBuffer(), buf->GetLength() , &writebytes, pio );
	if( bret > 0 )
	{
		pio->mBuffer->mLength = writebytes;
	}
	else
	{
		if( ::GetLastError() != ERROR_IO_PENDING )
		{
			printf("WriteFile Error: %d\n", ::GetLastError());
			assert(0);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL IOCP::SendTo( IOCPHandler* handler , stBuffer* pbuf, const stAddr& to )
{
	DWORD writebytes = 0;
	mSequence ++;
	PerIO *pio = mIOMgr.CreateObject();
	pio->SetUdpOpt( FALSE, pbuf, to );
	SOCKADDR_IN sa = {0};
	sa.sin_addr.S_un.S_addr = to.mIP.mIP.mIPLong;
	sa.sin_family = 2;
	sa.sin_port = htons( to.mPort );
	BOOL bret = ::WSASendTo( (SOCKET)handler->GetHandle(), &pio->mWSABuf, 1, &writebytes, 0, (sockaddr*)&sa, sizeof(sa), pio, NULL  );
	if( bret < 0)
	{
		if( ::WSAGetLastError() != ERROR_IO_PENDING )
		{
			printf("SendTo Error: %d\n", ::GetLastError());
			assert(0);
			return FALSE;
		}
	}
	return TRUE;

}

BOOL IOCP::RecvFrom( IOCPHandler* handler, stBuffer* buf )
{
	DWORD readbytes = 0;
	mSequence ++;
	PerIO *pio = mIOMgr.CreateObject();
	pio->SetUdpOpt( TRUE, buf, stAddr());
	pio->mWSABuf.len = stBuffer::_MaxSize;
	DWORD flag = 0;
	BOOL bret = ::WSARecvFrom( (SOCKET)handler->GetHandle(), &pio->mWSABuf, 1, &readbytes, &flag, (sockaddr*)&pio->mAddr, &pio->mAddrLen, pio, NULL ) ;
	if( bret < 0)
	{
		if( ::WSAGetLastError() != ERROR_IO_PENDING )
		{
			printf("RecvFrom Error: %d\n", ::GetLastError());
			assert(0);
			return FALSE;
		}
	}
	else
	{
		
	}
	return TRUE;
}