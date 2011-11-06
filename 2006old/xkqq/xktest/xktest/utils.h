#pragma once

#define CRYPTER_PUT_DWORD(pbBuf, dw)	\
{	\
	*pbBuf = (dw & 0xFF000000) >> 24;	\
	*(pbBuf + 1) = (dw & 0xFF0000) >> 16;	\
	*(pbBuf + 2) = (dw & 0xFF00) >> 8;	\
	*(pbBuf + 3) = dw & 0xFF;	\
	pbBuf += sizeof(DWORD);	\
}

#define CRYPTER_GET_DWORD(pbBuf, dw)	\
{	\
	dw = *pbBuf << 24;	\
	dw += *(pbBuf + 1) << 16;	\
	dw += *(pbBuf + 2) << 8;	\
	dw += *(pbBuf + 3);	\
	pbBuf += sizeof(DWORD);	\
}

inline u_int netl(u_int d)
{
	return htonl(d);
}

inline u_short nets( u_short d)
{
	return htons(d);
}

inline u_int loc( u_int d)
{
	return ntohl(d);
}
inline u_short loc( u_short d)
{
	return ntohs(d);
}

inline void check( u_short& d)
{
	d = loc(d);
}

inline void check( u_int & d)
{
	d = loc(d);
}

namespace Utils
{
	class Tea  
	{
	public:
		Tea();
		virtual ~Tea();

	public:
		static int Decrypt( const BYTE* pbIn, int iInOffset, int iInLen, const BYTE* pbKey, BYTE* pbRet, int iRetLen) ;
		static int Encrypt( const BYTE* pbIn, int iInOffset, int iInLen, const BYTE* pbKey, BYTE* pbRet, int iRetLen) ;

	protected:
		static void Decipher(BYTE* pbIn, int iInOffset,const BYTE* pbKey, BYTE* pbRet) ;
		static void Encipher(BYTE* pbIn,const BYTE* pbKey, BYTE* pbRet) ;
		static void Decrypt8Bytes(BYTE* pbPrePlain, const BYTE* pbKey, int& iContextStart, int& iCrypt, int& iPreCrypt,const BYTE* pbIn, int iInOffset, int iInLen) ;
		static void Encrypt8Bytes(bool bHeader, BYTE* pbPlain, BYTE* pbPrePlain, const BYTE* pbKey, int& iCrypt, int& iPreCrypt, BYTE* pbOut) ;
	};


	typedef unsigned char *POINTER;

	/* UINT2 defines a two byte word */
	typedef unsigned short int UINT2;

	/* UINT4 defines a four byte word */
	typedef unsigned long int UINT4;


	/* MD5 context. */
	typedef struct {
		UINT4 state[4];                                   /* state (ABCD) */
		UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
		unsigned char buffer[64];                         /* input buffer */
	} MD5_CTX;

	void MD5Init(MD5_CTX *);
	void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
	void MD5Final(unsigned char [16], MD5_CTX *);

	BOOL _EncryptString(const char* pszInputString, BYTE *pbOutputBuf, int& iBufLen);
	int _ConvertMD5ToString(const BYTE *pbMD5Buffer, int iMD5BufLen, char *pszMD5String, int iStringBufLen);

	class  Md5
	{
		
	public:
		enum
		{
			_LENGTH = 16,
		};

		Md5( const unsigned char * s = 0, int length = 0 );
		void Update( const unsigned char * s, int length );
		const unsigned char* ToBuffer(  ) const;
		std::string ToString( ) const;
		void Final() const;
	private:
	

		MD5_CTX mContext;
		unsigned char mDigest[16];
	};


	void CopyHex( const unsigned char *buffer, u_int length );
}
