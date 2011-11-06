#pragma  once


namespace vpn_packet
{

	enum Command
	{
		_CMD_LOGIN = 0,
	};

	class UserKey
	{
	public:
		WORD  mUserKey[32];
		template <typename Ar>
		void serialize( Ar & ar, const unsigned int version)
		{
			ar & mUserKey;
		}
	};
	
	class Header
	{
	public:
		WORD mFlag;
		WORD mCommand;
		DWORD mCommandSeq;
		DWORD mSequence;
		UserKey  mUserKey;
		template <typename Ar>
		void serialize( Ar& ar, const unsigned int version)
		{
			ar & mFlag;
			ar & mCommand;
			ar & mCommandSeq;
			ar & mSequence;
			ar & mUserKey;
		}
	};


	struct Login
	{
	public:
		ULONG mLocalAddr;
		
		template <typename Ar>
		void serialize( Ar& ar, const unsigned int version)
		{
			ar & mLocalAddr;
		}
	};

}

