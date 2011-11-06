#include "StdAfx.h"
using namespace Utils;

Tea::Tea()
{

}

Tea::~Tea()
{

}

/**
* ����
* @param in �����ֽ����� 
* @param offset ��ʼ���ܵ�ƫ�� 
* @param len ���ܳ��� 
* @param k ��Կ 
* @return �����ֽ����� 
*/
int Tea::Encrypt( const BYTE *pbIn, int iInOffset, int iInLen, const BYTE *pbKey, BYTE *pbRet, int iRetLen) 
{
	// ����ͷ������ֽ���
	int iPos = (iInLen + 0x0A) % 8;

	if( iPos != 0 )
		iPos = 8 - iPos;

	// ������������ĳ���
	int iOutLen = iInLen + iPos + 10;

	if( iRetLen < iOutLen )
		return -1;

	BYTE* pbPlain = (BYTE*)malloc(8);
	BYTE* pbPrePlain = (BYTE*)malloc(8);
	int iCrypt = 0;
	int iPreCrypt = 0;

	BYTE* pbOut = (BYTE*)malloc(iOutLen);

	// ����Ĳ�����pos�浽��plain�ĵ�һ���ֽ�����
	// 0xF8������λ�ǿյģ���������pos����Ϊpos��0��7��ֵ����ʾ�ı���ʼ���ֽ�λ��
	pbPlain[0] = (BYTE)((rand() & 0xF8) | iPos);

	// ��������������������plain[1]��plain[pos]֮�������
	for( int i = 1; i <= iPos; i++ )
		pbPlain[i] = (BYTE)(rand() & 0xFF);
	iPos++;

	// �������prePlain����һ��8�ֽڿ鵱Ȼû��prePlain������������һ��ȫ0�ĸ���һ��8�ֽڿ�
	for(int i = 0; i < 8; i++ )
		pbPrePlain[i] = 0x0;

	// �������2���ֽڵ������������������������8�ֽھͼ���֮
	bool bHeader = true;
	int iPadding = 1;

	while( iPadding <= 2 )
	{
		if( iPos < 8 )
		{
			pbPlain[iPos++] = (BYTE)(rand() & 0xFF);
			iPadding++;
		}
		if( iPos == 8 )
		{
			Encrypt8Bytes(bHeader, pbPlain, pbPrePlain, pbKey, iCrypt, iPreCrypt, pbOut);
			iPos = 0;
			bHeader = false;
		}
	}

	// ͷ��������ˣ����￪ʼ�������������ˣ�Ҳ������8�ֽھͼ��ܣ�һֱ�����Ķ���
	int i = iInOffset;

	while( iInLen > 0 )
	{
		if( iPos < 8 )
		{
			pbPlain[iPos++] = pbIn[i++];
			iInLen--;
		}
		if( iPos == 8 )
		{
			Encrypt8Bytes(bHeader, pbPlain, pbPrePlain, pbKey, iCrypt, iPreCrypt, pbOut);
			iPos = 0;
			bHeader = false;
		}
	}

	// �������0���Ա�֤��8�ֽڵı���
	iPadding = 1;
	while( iPadding <= 7 )
	{
		if( iPos < 8 )
		{
			pbPlain[iPos++] = 0x0;
			iPadding++;
		}
		if( iPos == 8 )
		{
			Encrypt8Bytes(bHeader, pbPlain, pbPrePlain, pbKey, iCrypt, iPreCrypt, pbOut);
			iPos = 0;
			bHeader = false;
		}
	}

	memcpy(pbRet, pbOut, iOutLen);
	free(pbOut);
	free(pbPrePlain);
	free(pbPlain);
	return iOutLen;

}

/**
* ����
* @param in ����
* @param offset ���Ŀ�ʼ��λ��
* @param len ���ĳ���
* @param k ��Կ
* @return ����
*/
int Tea::Decrypt( const BYTE *pbIn, int iInOffset, int iInLen, const BYTE *pbKey, BYTE *pbRet, int iRetLen) 
{
	// ��ΪQQ��Ϣ����֮��������16�ֽڣ����ҿ϶���8�ı������������������
	if( (iInLen % 8 != 0) || (iInLen < 16) )
		return -1;

	BYTE* pbPrePlain = (BYTE*)malloc(8);
	int iCrypt = 0;
	int iPreCrypt = 0;
	int iCount;
	int iPos;

	// �õ���Ϣ��ͷ�����ؼ��ǵõ��������Ŀ�ʼ��λ�ã������Ϣ���ڵ�һ���ֽ����棬�������ý��ܵõ��ĵ�һ���ֽ���7����
	Decipher( const_cast<BYTE *>( pbIn), iInOffset, pbKey, pbPrePlain);
	iPos = pbPrePlain[0] & 0x7;
	// �õ��������ĵĳ���
	iCount = iInLen - iPos - 10;
	if( iCount < 0 )	// ������ĳ���С��0���ǿ϶��ǳ����ˣ����紫�����֮��ģ�����
	{
		free(pbPrePlain);
		return -1;
	}

	// ͨ��������Ĵ��룬����Ӧ����û�������ˣ����Ƿ������������
	int iOutLen = iCount;

	if( iRetLen < iCount )
	{
		free(pbPrePlain);
		return -1;
	}

	BYTE* pbOut = (BYTE*)malloc(iOutLen);

	// �������ʱ��preCrypt���ͼ���ʱ��һ��8�ֽڿ�û��prePlainһ��������ʱ
	// ��һ��8�ֽڿ�Ҳû��preCrypt���������ｨһ��ȫ0��
	BYTE* pbM = (BYTE*)malloc(iInOffset + 8);

	for( int i = 0; i < 8; i++ )
		pbM[iInOffset + i] = 0;

	// ����preCrypt��λ�õ���0��ע��Ŀǰ��preCryptλ����ָ��m�ģ���Ϊjavaû��ָ�룬���������ں���Ҫ���Ƶ�ǰ����buf������
	iPreCrypt = 0;
	// ��ǰ������λ�ã�Ϊʲô��8����0�أ�ע��ǰ�������Ѿ�������ͷ����Ϣ�ˣ����ڵ�Ȼ��8��
	iCrypt = 8;

	// ��Ȼ���Ҳ��8
	int iContextStart = 8;

	// ��1���ͼ����㷨�Ƕ�Ӧ��
	iPos++;

	// ��ʼ����ͷ����������������������8�ֽڣ��������һ��
	// ��Ϊ�ǽ�����һ�飬����������һ����� m = in����һ�鵱Ȼ��preCrypt�ˣ����ǲ�����m��
	// �����������8����˵����ʲô��˵����ͷ8���ֽڵ������ǰ�����������Ϣ�ģ���Ȼ����Ҫ��m������Ū����
	// ���ԣ�����Ȼ������8�Ļ���˵����ͷ8���ֽڵ����ĳ���һ��������Ϣ����֮�⣬�����������õ����
	int iPadding = 1;

	while( iPadding <= 2 )
	{
		if( iPos < 8 )
		{
			iPos++;
			iPadding++;
		}
		if( iPos == 8 )
		{
			free(pbM);
			pbM = (BYTE*)malloc(iInLen);
			memcpy(pbM, pbIn, iInLen);
			Decrypt8Bytes(pbPrePlain, pbKey, iContextStart, iCrypt, iPreCrypt, pbIn, iInOffset, iInLen);
			iPos = 0;
		}
	}

	// �����ǽ��ܵ���Ҫ�׶Σ����ʱ��ͷ������䶼�Ѿ������ˣ���ʼ����
	// ע���������һ��whileû����8�������һ��if�����õľ���ԭʼ��m���������m����in��
	int	i = 0;

	while( iCount != 0 )
	{
		if( iPos < 8 )
		{
			pbOut[i] = (BYTE)(pbM[iInOffset + iPreCrypt + iPos] ^ pbPrePlain[iPos]);
			i++;
			iCount--;
			iPos++;
		}
		if( iPos == 8 )
		{
			free(pbM);
			pbM = (BYTE*)malloc(iInLen);
			memcpy(pbM, pbIn, iInLen);
			iPreCrypt = iCrypt - 8;
			Decrypt8Bytes(pbPrePlain, pbKey, iContextStart, iCrypt, iPreCrypt, pbIn, iInOffset, iInLen);
			iPos = 0;
		}
	}

	// ���Ľ��ܲ��֣�����һ��while�Ѿ������Ķ�������ˣ���ʣ��β��������ˣ�Ӧ��ȫ��0
	// ���������м���Ƿ������֮���ǲ���0��������ǵĻ��ǿ϶������ˣ�����null
	for( iPadding = 1; iPadding < 8; iPadding++ )
	{
		if( iPos < 8 )
		{
			if( (pbM[iInOffset + iPreCrypt + iPos] ^ pbPrePlain[iPos]) != 0 )
			{
				free(pbOut);
				free(pbPrePlain);
				free(pbM);
				return -1;
			}
			iPos++;
		}
		if( iPos == 8 )
		{
			free(pbM);
			pbM = (BYTE*)malloc(iInLen);
			memcpy(pbM, pbIn, iInLen);
			iPreCrypt = iCrypt;
			Decrypt8Bytes(pbPrePlain, pbKey, iContextStart, iCrypt, iPreCrypt, pbIn, iInOffset, iInLen);
			iPos = 0;
		}
	}

	memcpy(pbRet, pbOut, iOutLen);
	free(pbOut);
	free(pbM);
	free(pbPrePlain);
	return iOutLen;

}

/**
* ����8�ֽ�
*/
void Tea::Encrypt8Bytes(bool bHeader, BYTE *pbPlain, BYTE *pbPrePlain, const BYTE *pbKey, int &iCrypt, int &iPreCrypt, BYTE *pbOut) 
{
	// �ⲿ�������������˵�� pbPlain ^ pbPreCrypt��ע�������ж����ǲ��ǵ�һ��8�ֽڿ飬
	//  ����ǵĻ����Ǹ�pbPrePlain�͵���iPreCrypt��
	for( int iPos = 0; iPos < 8; iPos++ )
	{
		if( bHeader )
			pbPlain[iPos] ^= pbPrePlain[iPos];
		else
			pbPlain[iPos] ^= pbOut[iPreCrypt + iPos];
	}
	// ������������˵�� f(pbPlain ^ iPreCrypt)
	BYTE* pbCrypted = (BYTE*)malloc(8);

	Encipher(pbPlain, pbKey, pbCrypted);
	memcpy(pbOut + iCrypt, pbCrypted, 8);
	free(pbCrypted);

	// �������� f(pbPlain ^ pbPreCrypt) ^ pbPrePlain��ok�����濽��һ�¾�����
	for(int iPos = 0; iPos < 8; iPos++ )
		pbOut[iCrypt + iPos] ^= pbPrePlain[iPos];
	memcpy(pbPrePlain, pbPlain, 8);

	// ����˼��ܣ������ǵ���crypt��preCrypt�ȵȶ�����ʱ����
	iPreCrypt = iCrypt;
	iCrypt += 8;

}

/**
* ����8���ֽ�
* 
* @param in
* 		�����ֽ�����
* @param offset
* 		�Ӻδ���ʼ����
* @param len
* 		���ĵĳ���
* @return
* 		true��ʾ���ܳɹ�
*/
void Tea::Decrypt8Bytes(BYTE *pbPrePlain,const BYTE *pbKey, int &iContextStart, int &iCrypt,  int &iPreCrypt,const BYTE *pbIn, int iInOffset, int iInLen) 
{
	// �����һ�������жϺ��滹��û�����ݣ�û�оͷ��أ�����У���ִ�� crypt ^ prePlain
	for( int iPos = 0; iPos < 8; iPos++ )
	{
		if( iContextStart + iPos >= iInLen )
			return;
		pbPrePlain[iPos] ^= pbIn[iInOffset + iCrypt + iPos];
	}

	// �ã�����ִ�е��� d(crypt ^ prePlain)
	BYTE* pbDecrypted = (BYTE*)malloc(8);

	Decipher(pbPrePlain, 0, pbKey, pbDecrypted);
	memcpy(pbPrePlain, pbDecrypted, 8);
	free(pbDecrypted);

	// ������ɣ����һ������û����
	// �������һ���ŵ�decrypt����ȥ���ˣ���Ϊ���ܵĲ����е㲻̫һ��
	// ������Щ������ֵ��
	iContextStart += 8;
	iCrypt += 8;

}

/**
* ����һ��8�ֽڿ�
* 
* @param in
* 		�����ֽ�����
* @return
* 		�����ֽ�����
*/
void Tea::Encipher(BYTE *pbIn, const BYTE *pbKey, BYTE *pbRet) 
{
	// �õ����ĺ���Կ�ĸ������֣�ע��javaû���޷������ͣ�����Ϊ�˱�ʾһ���޷��ŵ�����
	// ��������long�����long��ǰ32λ��ȫ0�ģ�����ͨ�����ַ�ʽģ���޷��������������õ���longҲ����һ����
	// ����Ϊ�˱�֤ǰ32λΪ0����Ҫ��0xFFFFFFFF��һ��λ��
	unsigned long y, z;
	unsigned long a, b, c, d;

	CRYPTER_GET_DWORD(pbIn, y);
	CRYPTER_GET_DWORD(pbIn, z);
	CRYPTER_GET_DWORD(pbKey, a);
	CRYPTER_GET_DWORD(pbKey, b);
	CRYPTER_GET_DWORD(pbKey, c);
	CRYPTER_GET_DWORD(pbKey, d);

	// �����㷨��һЩ���Ʊ�����Ϊʲôdelta��0x9E3779B9�أ�
	// �������TEA�㷨��delta��ʵ���Ǿ���(sqr(5) - 1) * 2^31 (����5����1���ٳ�2��31�η�)
	unsigned __int64 ui64Sum = 0;
	unsigned long ulDelta = 0x9E3779B9;

	ulDelta &= 0xFFFFFFFF;

	// ����������16��
	int iLoop = 0x10;

	// ��ʼ�����ˣ����߰���ģ���Ҳ��������������DES֮��Ĳ�࣬��������������ȥ
	while( iLoop-- > 0 )
	{
		ui64Sum += ulDelta;
		ui64Sum &= 0xFFFFFFFFF;

		unsigned __int64 y1, y2, y3, z1, z2, z3;

		y1 = (z << 4) + a;
		y2 = z + ui64Sum;
		y3 = (z / 32) + b;
		y += (u_long) (y1 ^ y2 ^ y3);
		y &= 0xFFFFFFFF;
		z1 = (y << 4) + c;
		z2 = y + ui64Sum;
		z3 = (y / 32) + d;
		z += (u_long) (z1 ^ z2 ^ z3);
		z &= 0xFFFFFFFF;
	}

	// �������������ģ���Ϊ���õ�long��������Ҫǿ��ת��һ�±��int
	CRYPTER_PUT_DWORD(pbRet, (int)y);
	CRYPTER_PUT_DWORD(pbRet, (int)z);

}

/**
* ���ܴ�offset��ʼ��8�ֽ�����
* 
* @param in
* 		�����ֽ�����
* @param offset
* 		���Ŀ�ʼλ��
* @return
* 		����
*/
void Tea::Decipher(BYTE *pbIn, int iInOffset, const BYTE *pbKey, BYTE *pbRet) 
{
	pbIn += iInOffset;

	// �õ����ĺ���Կ�ĸ������֣�ע��javaû���޷������ͣ�����Ϊ�˱�ʾһ���޷��ŵ�����
	// ��������long�����long��ǰ32λ��ȫ0�ģ�����ͨ�����ַ�ʽģ���޷��������������õ���longҲ����һ����
	// ����Ϊ�˱�֤ǰ32λΪ0����Ҫ��0xFFFFFFFF��һ��λ��
	unsigned long y, z;
	unsigned long a, b, c, d;

	CRYPTER_GET_DWORD(pbIn, y);
	CRYPTER_GET_DWORD(pbIn, z);
	CRYPTER_GET_DWORD(pbKey, a);
	CRYPTER_GET_DWORD(pbKey, b);
	CRYPTER_GET_DWORD(pbKey, c);
	CRYPTER_GET_DWORD(pbKey, d);

	// �㷨��һЩ���Ʊ�����sum������Ҳ�����ˣ����sum�͵��������й�ϵ
	// ��Ϊdelta����ô�࣬����sum�������ô��Ļ���������ʱ�����������16�Σ����
	// �õ�0�����������Ϊ�˵õ��ͼ���ʱ�෴˳��Ŀ��Ʊ������������ܽ���ѽ����
	unsigned __int64 ui64Sum = 0xE3779B90;
	unsigned long ulDelta = 0x9E3779B9;

	ui64Sum &= 0xFFFFFFFF;
	ulDelta &= 0xFFFFFFFF;

	// ����������16��
	int iLoop = 0x10;

	// ������ʼ�ˣ� @_@
	while( iLoop-- > 0 )
	{
		unsigned __int64 y1, y2, y3, z1, z2, z3;

		z1 = (y << 4) + c;
		z2 = y + ui64Sum;
		z3 = (y / 32) + d;
		z -= (u_long) (z1 ^ z2 ^ z3);
		z &= 0xFFFFFFFF;
		y1 = (z << 4) + a;
		y2 = z + ui64Sum;
		y3 = (z / 32) + b;
		y -= (u_long) (y1 ^ y2 ^ y3);
		y &= 0xFFFFFFFF;
		ui64Sum -= ulDelta;
		ui64Sum &= 0xFFFFFFFF;
	}

	CRYPTER_PUT_DWORD(pbRet, (int)y);
	CRYPTER_PUT_DWORD(pbRet, (int)z);

}


#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

void MD5Transform(UINT4 [4], unsigned char [64]);
void Encode(unsigned char *, UINT4 *, unsigned int);
void Decode(UINT4 *, unsigned char *, unsigned int);
void MD5_memcpy(POINTER, POINTER, unsigned int);
void MD5_memset(POINTER, int, unsigned int);

static unsigned char PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}

/* MD5 initialization. Begins an MD5 operation, writing a new context.
*/
void Utils::MD5Init (
			  MD5_CTX *context                                     /* context */
			  )
{
	context->count[0] = context->count[1] = 0;
	/* Load magic initialization constants.
	*/
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void Utils::MD5Update (
				MD5_CTX *context,                                        /* context */
				unsigned char *input,                                /* input block */
				unsigned int inputLen                     /* length of input block */
				)
{
	unsigned int i, index, partLen;

	/* Compute number of bytes mod 64 */
	index = (unsigned int)((context->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((context->count[0] += ((UINT4)inputLen << 3))
		< ((UINT4)inputLen << 3))
		context->count[1]++;
	context->count[1] += ((UINT4)inputLen >> 29);

	partLen = 64 - index;

	/* Transform as many times as possible.
	*/
	if (inputLen >= partLen) {
		MD5_memcpy
			((POINTER)&context->buffer[index], (POINTER)input, partLen);
		MD5Transform (context->state, context->buffer);

		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform (context->state, &input[i]);

		index = 0;
	}
	else
		i = 0;

	/* Buffer remaining input */
	MD5_memcpy
		((POINTER)&context->buffer[index], (POINTER)&input[i],
		inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the context.
*/
void Utils::MD5Final (
			   unsigned char digest[16],                         /* message digest */
			   MD5_CTX *context                                       /* context */
			   )
{
	unsigned char bits[8];
	unsigned int index, padLen;

	/* Save number of bits */
	Encode (bits, context->count, 8);

	/* Pad out to 56 mod 64.
	*/
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update (context, PADDING, padLen);

	/* Append length (before padding) */
	MD5Update (context, bits, 8);

	/* Store state in digest */
	Encode (digest, context->state, 16);

	/* Zeroize sensitive information.
	*/
	MD5_memset ((POINTER)context, 0, sizeof (*context));
}

/* MD5 basic transformation. Transforms state based on block.
*/
void MD5Transform (
				   UINT4 state[4],
				   unsigned char block[64]
)
{
	UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode (x, block, 64);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information.
	*/
	MD5_memset ((POINTER)x, 0, sizeof (x));
}

/* Encodes input (UINT4) into output (unsigned char). Assumes len is
a multiple of 4.
*/
void Encode(
			unsigned char *output,
			UINT4 *input,
			unsigned int len
			)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/* Decodes input (unsigned char) into output (UINT4). Assumes len is
a multiple of 4.
*/
void Decode(
			UINT4 *output,
			unsigned char *input,
			unsigned int len
			)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |
		(((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);
}

/* Note: Replace "for loop" with standard memcpy if possible.
*/

void MD5_memcpy(
				POINTER output,
				POINTER input,
				unsigned int len
				)
{
	unsigned int i;

	for (i = 0; i < len; i++)
		output[i] = input[i];
}

/* Note: Replace "for loop" with standard memset if possible.
*/
void MD5_memset(
				POINTER output,
				int value,
				unsigned int len
				)
{
	unsigned int i;

	for (i = 0; i < len; i++)
		((char *)output)[i] = (char)value;
}


//----------------------------------------------------------------------------------------
// Purpose:	
//----------------------------------------------------------------------------------------
BOOL Utils::_EncryptString(const char* pszInputString, BYTE *pbOutputBuf, int& iBufLen)
{
	MD5_CTX context;
	unsigned char digest[16];

	memset(digest, 0, sizeof(digest));
	if( (pszInputString != NULL) && (strlen(pszInputString) > 0) && (iBufLen >= sizeof(digest)) )
	{
		unsigned int len = strlen(pszInputString);
		MD5Init(&context);
		MD5Update(&context, (unsigned char *)pszInputString, len);
		MD5Final(digest, &context);

		memcpy(pbOutputBuf, digest, sizeof(digest));
		iBufLen = sizeof(digest);

		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------------
// Purpose:	
//----------------------------------------------------------------------------------------
int Utils::_ConvertMD5ToString(const BYTE *pbMD5Buffer, int iMD5BufLen, char *pszMD5String, int iStringBufLen)
{
	char *pTemp = pszMD5String;
	for( int i = 0; i < 16; i++ )
	{
		sprintf(pTemp, "%02x", pbMD5Buffer[i]);
		pTemp += 2;
	}

	return strlen(pszMD5String);
}

void Utils::CopyHex( const unsigned char *buffer, u_int length)
{
	static char buf[4096];
	for( int a=0; a<length; a++)
	{
		sprintf(buf+a*4,"\\x%02x", buffer[a]);
	}
	if ( ::OpenClipboard( 0 ) == 0 )
		return ;

	int size = strlen(buf);

	HGLOBAL globalmem = ::GlobalAlloc( GHND | GMEM_DDESHARE, size + 1 );
	if ( globalmem == 0 )
	{
		::CloseClipboard( );
		return ;
	}

	memcpy( ::GlobalLock( globalmem ), buf, size + 1 );
	::GlobalUnlock( globalmem );

	::EmptyClipboard( );
	::SetClipboardData( CF_TEXT, globalmem );

	::GlobalFree( globalmem );
	::CloseClipboard( );
	return ;
 
}

Utils::Md5::Md5( const unsigned char * s /*= 0*/, int length /*= 0 */ )
{
	memset( mDigest, 0, sizeof( mDigest ) );
	MD5Init( &mContext );
	if( s!= 0 )
	{
		Update( const_cast<unsigned char*>( s ), length );
	}
}

void Utils::Md5::Update( const unsigned char * s, int length )
{
	MD5Update( &mContext, (unsigned char *)s, length );
}

const unsigned char* Utils::Md5::ToBuffer() const
{
	Final();
	return mDigest;
}

std::string Utils::Md5::ToString() const
{
	Final();
	static char s[33];
	char *ps = s;
	for( int i = 0; i < 16; i++ )
	{
		sprintf(ps, "%02x", mDigest[i]);
		ps += 2;
	}
	return s;
}

void Utils::Md5::Final() const
{
	MD5_CTX ctx = mContext;
	MD5Final( (unsigned char *)mDigest,  &ctx );
}