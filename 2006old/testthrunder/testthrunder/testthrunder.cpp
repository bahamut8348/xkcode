// testthrunder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <winsock2.h>
#include "aes.h"

#pragma comment(lib, "Ws2_32.lib")

bool MakePackage(const char *address, const char *id, unsigned char *package)
{
	size_t len = strlen(address);
	if (len > 0x3C)
	{
		return false;
	}

	memset(package, 0, 0x7C);
	unsigned char *p = package;
	*p = 0x29;//固定的头
	p += 4;
	*p = 0x05;//命令标示
	p += 4;
	*p = 0x70;//长度
	p += 4;

	*p = 0x05;
	p += 4;

	memcpy(p, "QUERY", 6);
	p += 6;

	*p = len;//地址长度
	p += 4;

	memcpy(p, address, len);//地址
	p += len;

	*p = 0x10;
	p += 4;

	memcpy(p, id, 0x10);
	p += 17;

	*p = 0x14;
	p += 4;

	*p = 0x14;
	p += 12;

	unsigned char temp = package+0x7C - p;
	for (; p!=package+0x7C; ++p)
	{
		*p = temp;
	}

	AES aes;
	unsigned char key[16] = { 0x4C, 0xBA, 0xCF, 0xF2, 0xD4, 0x5F, 0x8F, 0x28, 0xBE, 0xD4, 0xDD, 0x26, 0x08, 0x36, 0x0E, 0xE1 };
	const unsigned char *plainText;

	for (int i=0xC; i<0x7C; i+=16)
	{
		p = (unsigned char*)package + i;
		plainText = aes.Cipher(p, key, 16);
		memcpy(p, plainText, 16);
	}

	return true;
}

unsigned char *MemSearch(const unsigned char *mem, const int memSize, const unsigned char *patt, const int pattSize)
{
	if (memSize <= 0 || pattSize <= 0)
	{
		return 0;
	}

	int i;

	int td[256];
	for (int c=0; c<256; ++c)
	{
		td[c] = pattSize + 1;
	}
	const unsigned char *p;
	for (p=patt, i=0; i<pattSize; ++p, ++i)
	{
		td[*p] = pattSize - (p - patt);
	}

	const unsigned char *t, *tx = mem;

	while (tx + pattSize <= mem + memSize)
	{
		for (p=patt, t=tx, i=0; i<pattSize; ++p, ++t, ++i)
		{
			if (*p != *t)
			{
				break;
			}
		}
		if (i == pattSize)
		{
			return (unsigned char*)tx;
		}
		tx += td[tx[pattSize]];
	}
	return 0;
}

bool main()
{
	cout << "please input full address:" << endl;

	char address[0x5c];
	cin >> address;

	cout << endl;

	unsigned char package[0x7C];
	if(!MakePackage(address, "0004618F9B760000", package))
	{
		return 1;
	}

	AES aes;
	unsigned char key[16] = { 0x4C, 0xBA, 0xCF, 0xF2, 0xD4, 0x5F, 0x8F, 0x28, 0xBE, 0xD4, 0xDD, 0x26, 0x08, 0x36, 0x0E, 0xE1 };

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
	{
		printf("Error at WSAStartup()\n");
		return false;
	}

	SOCKET ConnectSocket;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( "210.22.14.6" );
	clientService.sin_port = htons( 3076 );

	if ( connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR)
	{
		printf( "Failed to connect.\n" );
		WSACleanup();
		return false;
	}

	int bytesSent;
	int bytesRecv = 0;

	char recvbuf[0xB00] = "";

	bytesSent = send( ConnectSocket, (const char*)package, 124, 0 );

	bytesRecv = recv( ConnectSocket, recvbuf, 0xB00, 0 );

	if (bytesRecv == 0)
	{
		return false;
	}

	const unsigned char *plainText;
	unsigned char *p;

	size_t *pSize = (size_t*)(recvbuf+8);

	for (size_t i=0; i<*pSize; i+=16)
	{
		p = (unsigned char*)recvbuf + i + 12;
		plainText = aes.InvCipher(p, key, 16);
		memcpy(p, plainText, 16);
	}

	const unsigned char *string = (const unsigned char*)(recvbuf+12);

	while(1)
	{
		string = MemSearch(string+1, p-string+1, (const unsigned char*)"http://", 7);
		if (string == 0)
		{
			break;
		}
		cout << string << endl;
	}

	string = (const unsigned char*)(recvbuf+12);

	while(1)
	{
		string = MemSearch(string+1, p-string+1, (const unsigned char*)"ftp://", 6);
		if (string == 0)
		{
			break;
		}
		cout << string << endl;
	}

	system("pause");

	return true;
}



