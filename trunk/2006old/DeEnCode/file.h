#pragma once

class file
{
public:
	file(void);
	~file(void);
};
int DecodeQuoted(const char* pSrc, unsigned char* pDst, int nSrcLen);



int Base64Encode(const unsigned char* pSrc, char *Out, int nSrcLen);
int Base64Decode(const char *pSrc,  char *Out, int nSrcLen);
