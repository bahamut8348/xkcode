#pragma once


typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned int   dword;

enum emGLOBAL
{
	//_GLOBAL_VERSION = 3611,
	_GLOBAL_VERSION = 0x0E1C,
};

enum emCommand
{
	_COMMAND_TOKEN 			= 98,
	_COMMAND_LOGIN 			= 34,
	_COMMAND_RECV_IM		= 0x17,
	_COMMAND_SEND_IM		= 0x16,
	_COMMAND_REQUEST_KEY	= 0x1d,
	_COMMAND_PROPER_OP		= 0x65,
	_COMMAND_BUDDY_STATUS	= 0x81,
	_COMMAND_KEEP_ALIVE		= 0x02,
	_COMMAND_GET_BUDDY_LIST = 0x26,
	_COMMAND_GET_ONLINE_BUDDY = 0x27,
};

enum emIMType
{
	_IM_FRIEND = 9,
	_IM_STRANG = 0x0a,
};

enum emIMMsgType
{
	_IM_MSG_TEXT = 0x0b,
};

#pragma pack(push,1)

struct stQQReqHeader
{
	byte	mHeader;
	word	mSource;
	word	mCommand;
	word	mSequence;
	dword	mQQnum;
};

struct stQQResHeader
{
	void FromBuffer( const byte* buffer )
	{
		stQQResHeader *pheader = ( stQQResHeader* )buffer;
		mHeader		= pheader->mHeader;
		mSource		= ntohs( pheader->mSource );
		mCommand	= ntohs( pheader->mCommand);
		mSequence	= ntohs( pheader->mSequence );
	}
	byte mHeader;
	word mSource;
	word mCommand;
	word mSequence;
};

struct stQQFoot
{
	byte  mEnd;
};

struct stQQFont
{
	byte mAttr;
	byte mRed;
	byte mGreen;
	byte mBlue;
	byte mSM1;
	byte mSM2;
	word mEncoding; // #0x8602ÎªGB£¬0x0000ÎªEN
	char mFontName[16];
	
	stQQFont()
	{
		mAttr = 0;
		mRed = 12;
		mGreen = 0;
		mBlue = 0;
		mSM2 = 0;
		mSM1 = 0;
		mEncoding = nets( 0x8602 );
		strcpy(mFontName,"ËÎÌå");
	}

	dword GetSize() const
	{
		return offsetof(stQQFont, mFontName) + strlen( mFontName );
	}

};

struct stQQNum
{
	dword mNum;
	stQQNum()
	{
		mNum = 0;
	}

	stQQNum( dword d)
	{
		mNum = netl(d);
	}

	operator dword()  const
	{
		return loc( mNum );
	}
};

struct stQQIP
{
	byte  mIP1;
	byte  mIP2;
	byte  mIP3;
	byte  mIP4;
};

struct stQQAddr
{
	stQQIP mIP;
	word   mPort;
};

struct stQQDate
{
	dword mDate;
};

template<int Size>
struct stQQBuffer
{
	enum{ _LENGTH = Size };
	byte mData[Size];
	void Set( const byte * session )
	{
		memcpy( mData, session , Size);
	}
	void Set( char ch )
	{
		memset( mData, ch, Size );
	}
	byte *GetBuffer() const 
	{
		return (byte*)mData;
	}

	std::string ToString( ) const
	{
		static char s[Size*4];
		char *ps = s;
		for( int i = 0; i < 16; i++ )
		{
			sprintf(ps, "\\x%02x", mData[i]);
			ps += 4;
		}
		return s;
	}
};

struct stQQSession : stQQBuffer<16>
{
	
};

struct stQQToken : stQQBuffer<24>
{

};

struct stQQEncryptPass : stQQBuffer<16>
{

};

struct stQQInitKey : stQQBuffer<16>
{

};

struct stQQMd5Pass : stQQBuffer<16>
{

};

struct stQQClientKey : stQQBuffer<16>
{

};

struct stQQFileSession : stQQBuffer<16> 
{

};

template<typename T, dword CMD>
struct stReqBase
{
	enum{
		_CMD = CMD,
	};	
	dword GetSize() const
	{
		return sizeof(T);
	}
};

template<typename T, dword CMD >
struct stResBase
{
	enum{ _CMD = CMD };

	void FromBuffer( const byte* buffer, dword length = 0 )
	{
		memcpy((T *)this, buffer, sizeof(T));
	}
};


struct stTokenReq: stReqBase< stTokenReq, _COMMAND_TOKEN >
{
	byte mBody;
};


struct stTokenRes : stResBase<stTokenRes, _COMMAND_TOKEN >
{
	byte mResult;
	byte mLength;
	stQQToken mToken;
};

struct stLoginReq : stReqBase<stLoginReq, _COMMAND_LOGIN >
{
	stQQBuffer<16>	mEncryptPass;
	stQQBuffer<36>	mSM1;
	byte			mLoginStatus;
	stQQBuffer<16>	mSM2;
	byte			mTokenLen;
	stQQToken		mToken;
	byte			mSM3; // 0x40
	stQQBuffer<100> mSM4;
	stQQBuffer<221> mSM5;

	void Set( const byte* encryptPass, byte loginStatus, const stQQToken & token )
	{
		mEncryptPass.Set( encryptPass );
		mLoginStatus = loginStatus;
		mTokenLen =  sizeof( token );
		mToken = token;
		
		mSM1.Set( (byte*)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x86\xCC\x4C\x35\x2C\xD3\x73\x6C\x14\xF6\xF6\xAF\xC3\xFA\x33\xA4\x01");
		mSM2.Set( (byte*)"\x8d\x8b\xfa\xec\xd5\x52\x17\x42\x86\xf9\xa7\x75\xe6\x32\xd1\x6d");
		mSM3 = 0x40;
		mSM4.Set( (byte*)"\x0b\x04\x02\x00\x01\x00\x00\x00\x00\x00\x03\t\x00\x00\x00\x00\x00\x00\x00\x01\xe9\x03\x01\x00\x00\x00\x00\x00\x01\xf3\x03\x00\x00\x00\x00\x00\x00\x01\xed\x03\x00\x00\x00\x00\x00\x00\x01\xec\x03\x00\x00\x00\x00\x00\x00\x03\x05\x00\x00\x00\x00\x00\x00\x00\x03\x07\x00\x00\x00\x00\x00\x00\x00\x01\xee\x03\x00\x00\x00\x00\x00\x00\x01\xef\x03\x00\x00\x00\x00\x00\x00\x01\xeb\x03\x00\x00\x00\x00\x00\x00");
		mSM5.Set( '\x00' );
	}
};

struct stLoginRes: stResBase< stLoginRes, _COMMAND_LOGIN >
{
	
	void FromBuffer( const byte* buffer, dword length = 0 )
	{
		stLoginRes *pRes = ( stLoginRes* )buffer;
		memcpy_s( this, sizeof(*this), pRes, offsetof(stLoginRes, mResult)+1);
		if( mResult == 0 )
		{
			memcpy_s( &mLoginOK, sizeof(mLoginOK), buffer + offsetof(stLoginRes, mDummy_msg), sizeof(mLoginOK) );
			mLoginOK.Check( );
		}
		else if( mResult == 5 )
		{
			//mResultInfo.assign( ,  mContent.mLoginFailed.mDummy_Msg + length - sizeof(stLoginRes)-1);
			mLoginFailed.mResultInfo.assign( (char*)&mDummy_msg, (char*)buffer + length  );
		}
	}
	
	struct LoginOK
	{
		stQQSession mSession;
		dword		mQQ;
		stQQAddr	mUserAddr;
		stQQAddr	mServerAddr;
		stQQDate	mLastLoginTime;
		word		mSM1;
		stQQToken	mToken;
		stQQAddr	mSAddr1;
		stQQAddr	mSAddr2;
		stQQBuffer<4>  mSM2;
		stQQClientKey mClientKey;
		stQQBuffer<12> mSM3;
		stQQIP		mLastLogin;
		stQQDate	mLastLoginTime2;
		stQQBuffer<40> mSM4;
		stQQBuffer<15> mSM5;
		
		void Check()
		{
			check( mQQ );
			check( mServerAddr.mPort );
		}
	};

	struct LoginFailed
	{
		std::string mResultInfo;
	};

	struct LoginRedirect
	{
		dword mQQNum;
		stQQAddr mServerAddr;

		void Check(  )
		{
			check( mQQNum );
		}
	};
	
	byte mResult;
	byte mDummy_msg;
	LoginFailed mLoginFailed;
	LoginOK mLoginOK;
	
};

struct stIMPacketRes :  stResBase< stIMPacketRes, _COMMAND_RECV_IM >
{
	struct stNormalMsg
	{
		struct stTextMessage
		{
			struct stNoFontAttrText
			{
				std::string mMsg;

				void FromBuffer( const byte *buffer, dword length )
				{
					mMsg.append( (char*)buffer, length-1 );
				}
			};
			
			struct stHasFontAttrText
			{
				std::string mMsg;
				stQQFont mQQFont;
				byte mFontSize;

				void FromBuffer( const byte* buffer, dword length )
				{
					mFontSize = buffer[ length - 1 ];
					memset( &mQQFont, 0, sizeof(mQQFont) );
					memcpy_s( &mQQFont, sizeof(mQQFont), buffer + length - mFontSize, mFontSize -1  );
					mQQFont.mEncoding = loc( mQQFont.mEncoding );
					mMsg.append( (char*) buffer, length - mFontSize-1 );
					
				}
			};
			
			void FromBuffer( byte *buffer, dword length )
			{
				stTextMessage *msg = ( stTextMessage* )buffer;
				mHasFontAttr = loc( msg->mHasFontAttr );
				mTotalFragments = msg->mTotalFragments;
				mFragmentNo = msg->mFragmentNo;
				mMsgID = loc( msg->mMsgID );
				mReplyType = msg->mReplyType;

				if( mHasFontAttr )
				{
					mHasAttrText.FromBuffer( msg->mDummy_MsgContent , length - offsetof( stTextMessage, mDummy_MsgContent ) );
				}
				else
				{
					mNoAttrText.FromBuffer( msg->mDummy_MsgContent , length - offsetof( stTextMessage, mDummy_MsgContent )   );
				}
			}

			dword   mHasFontAttr;
			byte	mTotalFragments;  // frag number
			byte	mFragmentNo;		// frag id
			word	mMsgID; 
			byte	mReplyType; // select struct
			byte	mDummy_MsgContent[1];
			stHasFontAttrText mHasAttrText;
			stNoFontAttrText  mNoAttrText;
		};

		void FromBuffer( byte *buffer, dword length )
		{
			Utils::CopyHex(buffer, length);
			stNormalMsg *msg = ( stNormalMsg* )buffer;
			mSenderVer = loc( msg->mSenderVer );
			mSenderQQ  = msg->mSenderQQ;
			mRecvQQ    = msg->mRecvQQ.mNum;
			mFileSession = msg->mFileSession;
			mIMMsgType = loc( msg->mIMMsgType );
			mSeq = loc( msg->mSeq );
			mSendDate = msg->mSendDate;
			mFace = loc( msg->mFace );
			if( mIMMsgType == _IM_MSG_TEXT )
			{
				mTextMessage.FromBuffer( msg->mDummy_Msg, length - offsetof(stNormalMsg, mDummy_Msg) );
			}
		}
	
		word	mSenderVer;
		stQQNum	mSenderQQ;
		stQQNum mRecvQQ;
		stQQFileSession mFileSession;
		word	mIMMsgType;  // select struct
		word	mSeq;
		stQQDate mSendDate;
		word	mFace;
		byte	mDummy_Msg[1];
		stTextMessage mTextMessage;  // mIMMsgType == 0x0b
	};
	
	
	void FromBuffer( const byte* buffer, dword length = 0 )
	{
		stIMPacketRes *pRes = (stIMPacketRes*)buffer;
		//mForReply = pRes->mForReply;
		mSender = pRes->mSender;
		dword a = mSender;
		mRecver = pRes->mRecver;
		mSeq = loc( pRes->mSeq );
		mSenderAddr = pRes->mSenderAddr;
		mIMType = loc( pRes->mIMType );
		if( mIMType == _IM_FRIEND )
		{
			mFriendQQ.FromBuffer( pRes->mDummy_IMMsg, length - offsetof( stIMPacketRes, mDummy_IMMsg)  );
		}
	}
	stQQNum			mSender;
	stQQNum			mRecver;
	dword			mSeq;
	stQQAddr		mSenderAddr;
	word			mIMType;  // select struct
	byte			mDummy_IMMsg[1];

	stNormalMsg mFriendQQ;  // type = 9
	stNormalMsg mStrangeQQ; // 0x0a
	stQQBuffer<16>	mForReply;
};

struct stIMResponsReq : stReqBase< stIMResponsReq, _COMMAND_RECV_IM >
{
	void Set( dword sender, dword recver, dword msgid, const stQQIP & sendip )
	{
		mSenderQQ = sender;
		mRecverQQ = recver;
		mMsgID = netl( msgid );
		mSenderIP = sendip;
	}
	stQQNum mSenderQQ;
	stQQNum mRecverQQ;
	dword   mMsgID;
	stQQIP	mSenderIP;
};

struct stRequestKeyReq : stReqBase< stRequestKeyReq, _COMMAND_REQUEST_KEY>
{
	byte mKeyType;
	void Set( byte keytype )
	{
		mKeyType = keytype;
	}
};

struct stPropOpReq : stReqBase< stPropOpReq, _COMMAND_PROPER_OP>
{
	byte mSubCommand;
	word mPos;

	void Set( byte command, word pos )
	{
		mSubCommand = command;
		mPos = pos;
	}
};

struct stKeepAliveReq : stReqBase< stKeepAliveReq, _COMMAND_KEEP_ALIVE >
{
	char mQQString[16];
	void Set( dword mQQnum )
	{
		sprintf(mQQString, "%d", mQQnum );
	}
	dword GetSize() const
	{
		return strlen( mQQString );
	}
};

struct stKeepAliveRes : stResBase< stKeepAliveRes, _COMMAND_KEEP_ALIVE >
{
	char mStr[128];
};

struct stIMPacketReq : stReqBase< stIMResponsReq, _COMMAND_SEND_IM >
{
	stQQNum mSender;
	stQQNum mRecver;
	word	mSendVer;
	stQQNum mSenderQQ;
	stQQNum mRecverQQ;
	stQQBuffer<16> mSession;  // md5(qq+ qqsession)
	word	mIMType;
	word	mSessionID;
	stQQDate mSendDate;
	word	mSendFace;
	dword	mHasFontAttr;
	byte	mTotalFragment;
	byte	mFragmentNo;
	word	mMsgID;
	byte	mMsgType; // auto or manual
	std::string	mChatMsg;
	stQQFont mFont;
	byte	mFontSize;

	void Set(dword sender, dword recver, const stQQSession & sess, word sessionid, word msgid, const char* textmsg )
	{
		mSender = sender;
		mRecver = recver;
		mSenderQQ = sender;
		mRecverQQ = recver;
		mSendVer = nets( 3423 );
		mSession = sess;
		mIMType = nets( _IM_MSG_TEXT );
		mSessionID = nets( sessionid );
		mSendDate.mDate =  netl( (u_int)time(NULL) ) ;
		mSendFace = nets( (word)0 );
		mHasFontAttr = netl( (dword)1);
		mTotalFragment = 1;
		mFragmentNo = 0;
		mMsgID = nets( msgid );
		mMsgType = 1 ;// manual
		mChatMsg = textmsg;
		mFontSize = mFont.GetSize() + 1;
	}

	bool ToBuffer( byte* outbuff , dword* outlen)
	{
		static byte buffer[1024];
		dword cplen = offsetof( stIMPacketReq, mChatMsg );
		dword offset = 0;
		memcpy( buffer, (void*)this, cplen );
		offset += cplen;
		cplen = mChatMsg.length();
		memcpy( buffer +  offset , mChatMsg.c_str(),  cplen );
		offset += cplen;
		cplen = mFont.GetSize();
		memcpy( buffer +  offset, &mFont, cplen  );
		offset += cplen;
		cplen = sizeof( mFontSize );
		memcpy( buffer +  offset, &mFontSize, cplen  );

		memcpy( outbuff, buffer, offset+cplen );
		*outlen = offset + cplen ;

		return TRUE;
	}
};

struct stGetBuddyListReq : stReqBase< stGetBuddyListReq, _COMMAND_GET_BUDDY_LIST >
{
	word  mStartPos; // 0
	byte  mbSort; // 0 is not sorted
	byte  mSM1; // 0;
	byte  mSM2; // 1

	void Set( dword startpos )
	{
		mStartPos = startpos;
		mbSort = 1;
		mSM1 = 0;
		mSM2 = 1;
	}
};

struct stGetOnlineBuddyReq : stReqBase< stGetOnlineBuddyReq, _COMMAND_GET_ONLINE_BUDDY >
{
	byte  mType; //  0x02 is buddy, or 0x03
};

struct stGetBuddyListRes : stResBase< stGetBuddyListRes, _COMMAND_GET_BUDDY_LIST>
{
	struct stQQUserInfo
	{
		stQQNum mQQ;
		word	mFace;
		byte	mAge;
		byte	mGender;
		byte	mNickLen;
		byte	mNick[1];
		byte    mDummy_[1];
		std::string mNickName;
		dword	mTag;

		// return user size
		dword FromBuffer( const byte *buffer )
		{
			stQQUserInfo* pinfo = ( stQQUserInfo* ) buffer;
			mQQ = pinfo->mQQ;
			mFace = loc( pinfo->mFace );
			mAge  =  pinfo->mAge ;
			mGender = pinfo->mGender ;
			mNickLen = pinfo->mNickLen;
			mNickName.append( (char *)pinfo->mNick, mNickLen );
			dword infosize = offsetof(stQQUserInfo, mNick) + mNickLen + sizeof(mTag);
			memcpy(&mTag ,buffer + infosize - 4, 4);
			return infosize;
		}
	};

	word mNextPos;
	byte mDummy_[1];
	std::vector<stQQUserInfo> mBuddyList;

	void FromBuffer( const byte* buffer, dword length )
	{
		stGetBuddyListRes *pRes = ( stGetBuddyListRes* )buffer;
		mNextPos = loc( pRes->mNextPos );
		dword offset = 0;
		for ( dword a =0; a < 50; a++)
		{
			stQQUserInfo user;
			offset += user.FromBuffer( pRes->mDummy_ + offset );
			mBuddyList.push_back( user ); 
			if( offset+2 == length )
			{
				break;
			}
			if( offset+2 > length )
			{
				__asm int 3;
			}
		}

	}
};

struct stIMPacketReply : stResBase< stIMPacketReply, _COMMAND_SEND_IM>
{
	byte mResult;
};

#pragma pack(pop)


