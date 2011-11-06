#include "StdAfx.h"

#include "Packetdef.h"
#include "IEvent.h"

#include "XKQQ.h"



asio::io_service XKQQ::g_IO;

XKQQ::XKQQ(void) : mSock( g_IO, udp::v4() )
, mSequence( 0x7b8a )
, mTimer( g_IO )
, mCheckTimer( g_IO )
{
	mSession.Set( (byte*)"4KsHA7RAV7Tgkve7" );
	mInitKey.Set('@');
}

XKQQ::~XKQQ(void)
{
}

void XKQQ::SendData( const char* buffer, int length  )
{
//	mSock.async_send_to(
//		asio::buffer(buffer, length ), mServer,
//		boost::bind(&XKQQ::SendDataResult, this,
//		asio::placeholders::error,
//		asio::placeholders::bytes_transferred));
	dword len = mSock.send_to( asio::buffer(buffer, length ), mServer );
	if ( 0 <  len )
	{
		printf("Send OK %d\n", len );
	}
}

void XKQQ::SendData( word cmd, const byte* buffer, dword length )
{
	SendData(cmd, buffer, length, 0, 0);
}

void XKQQ::SendData( word cmd, const byte* buffer1, dword length1, const byte* buffer2, dword length2 )
{
	SendData(cmd, GetSequence(), buffer1, length1, buffer2, length2 );
}

void XKQQ::SendData( word cmd, word seq, const byte* buffer1, dword length1, const byte* buffer2, dword length2 )
{
	char buffer[2048];
	stQQReqHeader *pHeader = (stQQReqHeader*)buffer;
	pHeader->mCommand	= nets( cmd );
	pHeader->mQQnum		= netl( mQQnum );
	pHeader->mHeader	= 0x02;
	pHeader->mSequence	= nets( seq );
	pHeader->mSource	= nets( _GLOBAL_VERSION );

	memcpy( buffer + sizeof(stQQReqHeader),				buffer1, length1 );
	memcpy( buffer + sizeof(stQQReqHeader) + length1 ,	buffer2, length2 );

	stQQFoot *pFoot = (stQQFoot*) (buffer + length1 + length2 + sizeof( stQQReqHeader ) );
	pFoot->mEnd = 0x03;
	SendData( buffer, sizeof(stQQReqHeader) + length1 + length2 + sizeof(stQQFoot) );
}

void XKQQ::SendDataResult( const boost::system::error_code & error, size_t bytes_sent )
{
	printf("Send Result: %d\n", bytes_sent);
}

void XKQQ::SetServer( const std::string& ip, int port )
{
	mServer.address( asio::ip::address::from_string(ip));
	mServer.port(port);
}

void XKQQ::UpDate()
{
	g_IO.run();
}

void XKQQ::StartRecvData()
{
	mSock.async_receive_from(
		boost::asio::buffer(mData, _DATA_LENGTH), mSender,
		boost::bind(&XKQQ::OnDataReceive, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

#define  PROC( event ) case st##event::_CMD:\
{\
	st##event _Res;\
	_Res.FromBuffer( realbuffer, reallength );\
	OnEvent_##event( header, &_Res );\
	break;\
}

void XKQQ::OnDataReceive( const boost::system::error_code& error, size_t bytes_recvd )
{
	StartRecvData();

	if( bytes_recvd == 0)
	{
		printf( "Error\n" );
		return ;
	}

	stQQResHeader header;
	header.FromBuffer( mData );
	
	// decrypt
	byte debuffer[ _DATA_LENGTH ];
	byte* realbuffer = NULL;
	dword reallength = 0;
	int outlen = 0;
	dword bodylen = bytes_recvd - sizeof(stQQResHeader) - sizeof( stQQFoot);
	outlen = Utils::Tea::Decrypt( mData + sizeof(stQQResHeader), 0, bodylen , mMd5Pass.GetBuffer(), debuffer, _DATA_LENGTH );
	if( outlen == -1 )
	{
		outlen = Utils::Tea::Decrypt( mData + sizeof(stQQResHeader), 0, bodylen, mSession.GetBuffer(), debuffer, _DATA_LENGTH );
		if( outlen == -1 )
		{
			outlen = Utils::Tea::Decrypt( mData + sizeof(stQQResHeader), 0, bodylen, mInitKey.GetBuffer(), debuffer, _DATA_LENGTH );
		}
	}
	if( outlen == -1 )
	{
		realbuffer = mData + sizeof(stQQResHeader);
		reallength = bodylen;
	}
	else
	{
		realbuffer = debuffer;
		reallength = outlen;
	}

	//Utils::CopyHex( realbuffer, reallength );
	RemoveCommand( header.mSequence );
	switch ( header.mCommand )
	{
		PROC( TokenRes );
		PROC( LoginRes );
		PROC( IMPacketRes );
		PROC( KeepAliveRes );
		PROC( GetBuddyListRes );
		PROC( IMPacketReply );

	default:
		printf("Recv OK %d, Command: %d\n", bytes_recvd, header.mCommand );
	}
	return ;
}

void XKQQ::SendTokenRequest()
{
	stTokenReq req;
	req.mBody = 0;
	SendCommand( &req );
	StartRecvData( );
}

word XKQQ::GetSequence( )
{
	return ++mSequence;
}

void XKQQ::OnEvent_TokenRes( const stQQResHeader & header, stTokenRes *pTokenRes )
{
	if( pTokenRes->mResult == 0 )
	{
		mToken = pTokenRes->mToken;
	}
	mToken = pTokenRes->mToken;
	mpEvent->OnToken( pTokenRes->mResult, pTokenRes->mToken );
	return;
}

void XKQQ::SendLogin( const std::string& password )
{
	Utils::Md5 md5( (byte*)password.c_str( ), password.length( ) );
	Utils::Md5 md5_2( md5.ToBuffer(), Utils::Md5::_LENGTH );
	mMd5Pass.Set( md5_2.ToBuffer( ) );

	word len = Utils::Tea::Encrypt( (byte*)&"", 0, 0,mMd5Pass.GetBuffer(), mEncryptPass.GetBuffer(), stQQEncryptPass::_LENGTH );
	printf("pass: %s\n", mEncryptPass.ToString().c_str() );
	
	byte tbuffer[1024];
	stLoginReq Req;
	Req.Set( (byte*)&mEncryptPass, 10, mToken );
	len = Utils::Tea::Encrypt( (byte*)&Req, 0, Req.GetSize(), mInitKey.GetBuffer(), tbuffer , 1024 );

	SendData( stLoginReq::_CMD, mInitKey.GetBuffer(), stQQInitKey::_LENGTH, tbuffer, len);
}

void XKQQ::OnEvent_LoginRes( const stQQResHeader & header,stLoginRes *pRes )
{
	if( pRes->mResult == 0 )
	{
		mSession = pRes->mLoginOK.mSession;
		mClientKey = pRes->mLoginOK.mClientKey;

		SendRequestKey();
		SendPropOp();
		SendKeepAlive();
	}
	if( pRes->mResult == 5 )
	{
		mpEvent->OnLoginResult( pRes->mResult, pRes->mLoginFailed.mResultInfo );
	}
	else
	{
		mpEvent->OnLoginResult( pRes->mResult , "");
	}
	
	return ;
}

void XKQQ::OnEvent_IMPacketRes( const stQQResHeader & header, stIMPacketRes *pRes )
{
	SendIMRes( pRes->mSender, pRes->mSeq, pRes->mSenderAddr.mIP, header.mSequence );
	dword sender = pRes->mSender ;
	if( pRes->mIMType == _IM_FRIEND )
	{
		if(pRes->mFriendQQ.mIMMsgType == _IM_MSG_TEXT )
		{
			if( pRes->mFriendQQ.mTextMessage.mHasFontAttr == 0)
			{
				mpEvent->OnFriendMessage( sender, pRes->mFriendQQ.mTextMessage.mNoAttrText.mMsg );
			}
			else
			{
				mpEvent->OnFriendMessage( sender, pRes->mFriendQQ.mTextMessage.mHasAttrText.mMsg );
			}
		}
	}
}

void XKQQ::SendIMRes( dword recver, dword msgid, const stQQIP & sendip, dword sequence )
{
	stIMResponsReq req;
	req.Set( recver, mQQnum, msgid, sendip);
	SendCommand( &req, sequence );
}

void XKQQ::SendRequestKey()
{
	stRequestKeyReq Req;
	Req.Set( 3 );
	SendCommand( &Req );
	Req.Set( 4 );
	SendCommand( &Req );
}

void XKQQ::SendPropOp()
{
	stPropOpReq req;
	req.Set( 1, 0 );
	SendCommand( &req );
}

void XKQQ::SendKeepAlive()
{
	boost::asio::strand strand(g_IO);
	stKeepAliveReq req;
	req.Set( mQQnum );
	SendCommand( &req );
	mTimer.expires_from_now( boost::posix_time::seconds(25) );
	mTimer.async_wait(strand.wrap(boost::bind(&XKQQ::SendKeepAlive, this)));
}

void XKQQ::OnEvent_KeepAliveRes( const stQQResHeader & header, stKeepAliveRes *pRes )
{
	
}

void XKQQ::SendChat( dword qq, const std::string& msg, dword seqid, dword msgid)
{
	stQQBuffer<20> sess;
	stQQNum stqq =  mQQnum;
	sess.Set((byte*)&stqq);
	memcpy( sess.GetBuffer() + 4, mSession.GetBuffer() , stQQSession::_LENGTH );
	Utils::Md5 md5( sess.GetBuffer(), 20 );
	if( seqid == 0 )
	{
		seqid = GetSequence( );
	}
	if( msgid == 0)
	{
		msgid = GetSequence( );
	}
	
	stQQSession s;
	s.Set( md5.ToBuffer() );	stIMPacketReq req;
	req.Set(mQQnum, qq, s, seqid , msgid, msg.c_str());

	byte buffer[1024];
	dword length = 0;

	req.ToBuffer(buffer, &length);
	Utils::CopyHex( buffer,length );
	SaveCommand( seqid, buffer, length );
	// SendData(stIMPacketReq::_CMD, seqid, buffer, length,0,0 );
	// SendCommand( stIMPacketReq::_CMD, buffer, length );
	SendCommand( stIMPacketReq::_CMD, mSession, seqid, buffer, length );
}

void XKQQ::SendCommand( word cmd, stQQBuffer<16> & key, word seq, const byte* buffer, dword length )
{
	// encrypt
	static byte outbuf[ _DATA_LENGTH ];
	dword sendlen = Utils::Tea::Encrypt( buffer, 0, length, key.GetBuffer(), outbuf, _DATA_LENGTH );
	SendData( cmd, seq, outbuf, sendlen, 0,0 );
}

void XKQQ::SendCommand( word cmd, const byte* buffer, dword length )
{
	SendCommand( cmd, mSession, GetSequence(), buffer, length );
}

void XKQQ::SendGetBuddyList( dword startpos )
{
	stGetBuddyListReq req;
	req.Set( startpos );
	SendCommand( &req );
}

void XKQQ::OnEvent_GetBuddyListRes( const stQQResHeader & header, stGetBuddyListRes *pRes )
{
	for( dword a=0; a< pRes->mBuddyList.size(); a++)
	{
		BuddyInfo Info;
		Info.mQQ = pRes->mBuddyList[a].mQQ;
		Info.mNickname = pRes->mBuddyList[a].mNickName;
	}
}

void XKQQ::OnEvent_IMPacketReply( const stQQResHeader & header, stIMPacketReply *pRes )
{
	word result = pRes->mResult;
}

void XKQQ::SaveCommand( word seq, const byte* buffer, dword bufferlength )
{
	ResponseCheckInfo info;
	info.Create( seq, XKQQ::g_IO, bufferlength, buffer );
	mResponseInfo[seq] = info;
	CheckCommand( seq );
}

void XKQQ::CheckCommand( word seq ,dword delay)
{
	if( delay == 0)
	{
		ResponseCheckInfoMap::iterator  iter= mResponseInfo.find( seq);
		if( iter != mResponseInfo.end() )
		{
			ResponseCheckInfo& info= iter->second;
			info.mCheckCount ++;
			if( info.mCheckCount > 6 )
			{
				RemoveCommand( seq );
				return;
			}
			char buffer[100];
			sprintf( buffer, "retry %d", seq );
			mpEvent->OnLog( buffer );
			SendData((char*) info.mBuffer, info.mLength );
			CheckCommand(seq);

		}
	}
	else
	{
		ResponseCheckInfoMap::iterator  iter= mResponseInfo.find( seq );
		if( iter != mResponseInfo.end() )
		{
			ResponseCheckInfo& info= iter->second;
			boost::asio::strand strand(g_IO);
			info.mCheckTimer->expires_from_now( boost::posix_time::milliseconds(300) );
			info.mCheckTimer->async_wait(strand.wrap(boost::bind(&XKQQ::CheckCommand, this, seq, 0 )));

		}
	}
	
}

void XKQQ::RemoveCommand( word seq )
{
	ResponseCheckInfoMap::iterator  iter= mResponseInfo.find( seq );
	if( iter != mResponseInfo.end() )
		mResponseInfo.erase( iter );
}