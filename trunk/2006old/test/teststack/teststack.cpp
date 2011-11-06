// teststack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

u_int g_d = 1;

int __cdecl CheckDiget()
{
	signed int v1; // eax@1

	v1 = 214013 * g_d + 2531011;
	g_d = 214013 * g_d + 2531011;
	return (v1 >> 16) & 0x7FFF;
}


/*
int oo()
{
	char String[100] = {0};
	char v9[100] = {0};
	int v3 = 0;
	int v4;
	char v6;
	int v7;
	int v0;
	if ( strlen(&String) != 1 )
	{
		do
		{
			if ( 123 * (char)(*(&String + v3) >> 5) > 0 )
			{
				v4 = 123 * (char)(*(&String + v3) >> 5);
				do
				{
	
					--v4;
				}
				while ( v4 );
			}
			int v5 =rand();
			v6 = v9[v3];
			v7 = v5 % 26 + 65;

			v0 += abs(v7 - v6);
			++v3;
		}
		while ( v3 < strlen(&String) - 1 );
	}
int 	result = v0;

return result;
}
*/

int _tmain(int argc, _TCHAR* argv[])
{
	int bb =CheckDiget();
	bb = CheckDiget();

	bb = CheckDiget();

	WORD a;
	BYTE buffer[16];
	
	DWORD c = (DWORD)&a;
	c = (DWORD)buffer;

	a = 10;
	buffer[0] = 7;
	buffer[15]= 9;
	::memset( buffer, 0, 16 );
	
	return 0;
}

