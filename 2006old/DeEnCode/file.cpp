#include "StdAfx.h"
#include "file.h"

file::file(void)
{
}

file::~file(void)
{
}
int DecodeQuoted(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
	int nDstLen;        // 输出的字符计数
	int i;

	i = 0;
	nDstLen = 0;

	while (i < nSrcLen)
	{
		if (strncmp(pSrc, "=\r\n", 3) == 0)        // 软回车，跳过
		{
			pSrc += 3;
			i += 3;
		}
		else
		{
			if (*pSrc == '=')        // 是编码字节
			{
				sscanf(pSrc, "=%02X", pDst);
				pDst++;
				pSrc += 3;
				i += 3;
			}
			else        // 非编码字节
			{
				*pDst++ = (unsigned char)*pSrc++;
				i++;
			}

			nDstLen++;
		}
	}

	// 输出加个结束符
	*pDst = '\0';

	return nDstLen;

}

int GetBit(const unsigned char *bit, int start , int end )
{
	
	return 1;
}

int Base64Encode( const unsigned char* pSrc,  char *Out, int nSrcLen )
{
	int ret = 0;
	static char *table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static char *pad = "=";

	// 123456 78   1234 5678   12 345678
	
	if(nSrcLen == 0)
	{
		return 0;
	}
	else if(nSrcLen == 1)
	{
		Out[0] = table[pSrc[0]>>2];
		Out[1] = table[(pSrc[0] & 0x03) <<4 ];
		Out[2] = pad[0];
		Out[3] = pad[0];
		return 4;
	}
	else if(nSrcLen == 2)
	{
		Out[0] = table[pSrc[0]>>2];
		Out[1] = table[ ( ( pSrc[0] & 0x03) <<4) | ( (pSrc[1] & 0xf0)>>4) ];
		Out[2] = table[ (pSrc[1] & 0x0f)<<2 ];
		Out[3] = pad[0];
		return 4;
	}
	else if(nSrcLen == 3)
	{
		Out[0] = table[pSrc[0]>>2];
		Out[1] = table[ ( ( pSrc[0] & 0x03) <<4) | ( (pSrc[1] & 0xf0)>>4)];
		Out[2] = table[( (pSrc[1] & 0x0f)<<2 ) + (pSrc[2]>>6) ];
		Out[3] = table[ pSrc[2]  & 0x3f];
		return 4;
	}
	else
	{
		ret += Base64Encode(pSrc, Out,3);
		ret += Base64Encode(pSrc+3,Out+4,nSrcLen-3);
		return ret;
	}

	

}

int Base64Decode( const char *pSrc, char *Out, int nSrcLen )
{
	if(nSrcLen % 4 != 0) return -1;
	int ret = 0;
	static char table[128] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 62 , 0 , 0 , 0 , 63 , 52 , 53 , 54 , 55 , 56 , 57 , 58 , 59 , 60 , 61 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 0 , 0 , 0 , 0 , 0 , 0 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 , 49 , 50 , 51 , 0 , 0 , 0 , 0 , 0 };
	//"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static char *pad = "=";
	if(nSrcLen == 0)
	{
		return 0;
	}
	else if(nSrcLen == 4)
	{
		if(pSrc[2]==pad[0])
		{
			Out[0] = (table[pSrc[0]]<<2) | (table[pSrc[1]]>>4);
			return 1;
		}
		else if(pSrc[3]==pad[0])
		{
			Out[0] = (table[pSrc[0]]<<2) | (table[pSrc[1]]>>4);
			Out[1] = (table[pSrc[1]] & 0x0f)<<4 | table[pSrc[2]]>>2 ;
			return 2;
		}
		else
		{
			Out[0] = (table[pSrc[0]]<<2) | (table[pSrc[1]]>>4);
			Out[1] = (table[pSrc[1]] & 0x0f)<<4 | table[pSrc[2]]>>2 ;
			Out[2] = (table[pSrc[2]]&0x03)<<6 | table[pSrc[3]];
			return 3;
		}
	}
	else
	{
		ret += Base64Decode(pSrc, Out, 4);
		ret += Base64Decode(pSrc+4, Out+3,nSrcLen-4);

	}

	return ret;
}
