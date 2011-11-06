// DeEnCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "file.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	UCHAR buf[1024];
	char buffer[1024]={0};
	char Out[100]={0};
	string  s= "=?gb2312?B?vsXM7Mnxwfo=?=";
	DecodeQuoted(s.c_str(), buf, s.size());
	int n =Base64Encode((const unsigned char *)"ÖÐ¹úÈË°¡", buffer, 8);
	Base64Decode("vsXM7Mnxwfo=", Out, n);
	return 0;
}

