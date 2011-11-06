// tolinan.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <WinSock2.h>
#include <Windows.h>

#include "common.h"

#define  VER 1
#define  SENDBUFFSIZE  1024


void Usage( const char *filename)
{
	printf("%s <ServerIP> <File1> [File2] [File3]...\r\n", filename);
}

int SendFile(  SOCKET sock, const char *filename)
{
	char fn[MAX_FILENAME];
	char sendbuf[SENDBUFFSIZE];

	FILE *fhandle;

	int count;
	int ret;
	
	char*pos1;

 	Packet_StartFile StartFile;

	pos1 = strrchr(filename, '\\');
	if(pos1)
	{
		strcpy(fn, pos1+1);
	}
	else
	{
		strcpy(fn, filename);
	}

	strcpy(StartFile.mFileName, fn);
	
	fhandle = fopen(filename, "rb");
	if(fhandle == 0)
	{
		return -1;
	}
	fseek(fhandle, 0, SEEK_END );
	StartFile.mLength = ftell(fhandle);
	fseek(fhandle, 0, SEEK_SET );

	StartFile.mVersion = VER;

	send(sock, (char *)&StartFile, sizeof(StartFile), 0);

	ret = 0;
	while(fhandle)
	{
		count = fread(sendbuf, 1, SENDBUFFSIZE, fhandle );
		if(count > 0)
		{
			count = send(sock, sendbuf, count, 0);
			
			if(count <= 0)
			{
				ret = -1;
				break;
			}
		}
		else
		{
			fclose(fhandle);
			fhandle = 0;
		}
	}

	
	return ret;
}

int main(int argc, char* argv[])
{

	SOCKET sock;
	SOCKADDR_IN server;
	int a;
	InitWSA();
	if(argc<3)
	{
		Usage(argv[0]);
		return 1;
	}

	sock = socket(AF_INET, SOCK_STREAM,0);
	
	memset(&server,0,sizeof(server));
	server.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	server.sin_family = 2;
	server.sin_port = htons(PORT);
	
	if( 0 == connect(sock, (struct sockaddr *)&server, sizeof(server) ) )
	{
		for (a=2; a<argc; a++)
		{
			printf("[+] Start File %s\r\n", argv[a]);
			if( SendFile(sock, argv[a]) == -1)
			{
				printf("[-] Error Transmit File %s, Error: %d\r\n", argv[a], GetLastError());
				break;
			}
			else
			{
				printf("[+] Finish File %s\r\n", argv[a]);
			}
		}
	}


	closesocket(sock);

	
	
	return 0;
}

