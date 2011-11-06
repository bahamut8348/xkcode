// Client.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <conio.h>


#include <WinSock2.h>
#include <Windows.h>


#include "../common.h"

#define  MAX_CLIENT_NUM 1024

#define RECVBUFSIZE 10240


void PrintUsage( const char *filename )
{
	printf("%s\r\n", filename);
}


typedef struct 
{
	SOCKET mSock;
	DWORD  mFileSize;
	DWORD  mRecvSize;
	FILE*  mFile;
	char   mFileName[MAX_FILENAME];
	char   mClientIP[20];
	int    mClientPort;
	
	
}ClientInfo;


ClientInfo * FindClientInfo(SOCKET sock,   ClientInfo **infolist )
{
	ClientInfo *pinfo = NULL;
	int a;
	for ( a=0; a<FD_SETSIZE; a++ )
	{
		if(infolist[a] == NULL)
		{
			continue; 
		}
		if( infolist[a]->mSock == sock )
		{
			pinfo = infolist[a];
			break;
		}
	}
	return pinfo;
}

ClientInfo * NewClientInfo(SOCKET sock, ClientInfo **infolist, fd_set * set)
{
	ClientInfo *pRet = NULL;
	int a;
	int salen;
	SOCKADDR_IN clientsa;

	ClientInfo *pNewInfo = (ClientInfo *) malloc(sizeof(ClientInfo));
	memset(pNewInfo, 0, sizeof(ClientInfo));
	pNewInfo->mSock = sock;

	salen = sizeof(clientsa);
	getpeername(sock, (struct sockaddr *) &clientsa, &salen );
	strcpy(pNewInfo->mClientIP, inet_ntoa(clientsa.sin_addr));
	pNewInfo->mClientPort = ntohs(clientsa.sin_port);

	
	for (a= 0; a<FD_SETSIZE; a++)
	{
		if(infolist[a] == 0)
		{
			infolist[a] = pNewInfo;
			pRet = pNewInfo;
			break;
		}
	}
	if(pRet == NULL)
	{
		free(pNewInfo);
	}
	else
	{
		FD_SET(sock, set);
	}
	return pRet;
}

void DeleteClient( SOCKET sock, ClientInfo **infolist, fd_set * set )
{
	
	ClientInfo *pInfo = FindClientInfo(sock, infolist);
	closesocket(sock);
	if(pInfo)
	{
		if(pInfo->mFile)
		fclose(pInfo->mFile);
	}
	
	FD_CLR(sock, set);

	printf("[+] Client Disconnect From: %s:%d\r\n", pInfo->mClientIP, pInfo->mClientPort);
	free(pInfo);
}

int main(int argc, char* argv[])
{
	ClientInfo * Clients[FD_SETSIZE];


	fd_set fs;
	fd_set read_fs;
	fd_set err_fs;
	
	SOCKADDR_IN clientsa;
	SOCKADDR_IN sa;

	SOCKET mainsock;

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10;

	

	memset(&clientsa, 0, sizeof(sa));
	memset(&sa, 0, sizeof(sa));
	sa.sin_addr.S_un.S_addr = 0;
	sa.sin_family = 2;
	sa.sin_port = htons(PORT);

	if( argc < 0)
	{
		PrintUsage(argv[0]);
	}

	InitWSA();

	memset(Clients, 0, sizeof(Clients));
	
	mainsock = socket(AF_INET, SOCK_STREAM, 0);

	if ( 0 != bind(mainsock, (struct sockaddr *)&sa, sizeof(sa)))
	{
		printf("bind faild\r\n");
		return 0;
	}

	listen(mainsock, 5);

	printf("[+] Server Listen Port: %d\r\n", PORT );
	
	FD_ZERO(&fs);
	FD_ZERO(&read_fs);
	FD_ZERO(&err_fs);

	FD_SET( mainsock, &fs);
	

	while (1)
	{
		char recvbuf[RECVBUFSIZE];
		int ret;
		u_int i;

		Packet_StartFile *pFile;

		memcpy(&read_fs, &fs, sizeof(fs));
		memcpy(&err_fs, &fs, sizeof(fs));
		ret = select( 0, &read_fs, NULL, &err_fs, &tv );
		if( ret > 0 )
		{
			
			for(i=0; i<err_fs.fd_count; i++)
			{
				if( mainsock != err_fs.fd_array[i] )
				{
					DeleteClient(err_fs.fd_array[i], Clients, &fs);
				}
			}
			
			for(i=0; i<read_fs.fd_count; i++  )
			{
				if( mainsock == read_fs.fd_array[i] )
				{
					int a = sizeof(clientsa);
					SOCKET newsock = accept(mainsock, (struct sockaddr *)&clientsa, &a);
					if( newsock >0 )
					{
						printf("[+] New Client From: %s:%d, ID: %d\r\n", inet_ntoa(clientsa.sin_addr), ntohs(clientsa.sin_port), newsock);
						if(NULL == NewClientInfo(newsock, Clients, &fs))
						{
							printf("[-] Reach Max User Number.\r\n");
						}
					}
				}
				else
				{
					SOCKET sock = read_fs.fd_array[i];
					ClientInfo *pClient = FindClientInfo(sock, Clients);
					if(ret>0)
					{
						if(pClient)
						{
							if( pClient->mFile )
							{
								int recvlen = pClient->mFileSize - pClient->mRecvSize;
								if(  recvlen > RECVBUFSIZE)
								{
									recvlen = RECVBUFSIZE;
								}
								ret = recv(sock, recvbuf, recvlen, 0);
								if(ret>0)
								{
									fwrite(recvbuf, ret, 1, pClient->mFile);
									pClient->mRecvSize += ret;
									if(pClient->mRecvSize == pClient->mFileSize)
									{
										fclose(pClient->mFile);
										pClient->mFile = NULL;
										printf("[+] [%d] Finish File: %s, Length: %d\r\n",pClient->mSock, pClient->mFileName, pClient->mFileSize);
									}
								}
								else
								{
									if(pClient->mRecvSize != pClient->mFileSize)
									{
										printf("[-] [%d] Client Receive Error, RecvLength: %d\r\n",pClient->mSock, pClient->mRecvSize);
									}
									
									DeleteClient(sock, Clients, &fs);
								}
								
							}
							else
							{
								ret = recv(sock, recvbuf, sizeof(Packet_StartFile) , 0);
								if( ret == sizeof(Packet_StartFile) )
								{
									pFile = (Packet_StartFile *)recvbuf;
									printf("[+] [%d] Start Receive File: %s, Length: %d\r\n",pClient->mSock,  pFile->mFileName, pFile->mLength);
									pClient->mRecvSize = 0;
									pClient->mFileSize = pFile->mLength;
									strcpy(pClient->mFileName, pFile->mFileName);
									pClient->mFile = fopen(pClient->mFileName, "wb");
								}
								else
								{
									if(pClient->mRecvSize != pClient->mFileSize)
									{
										printf("[-] [%d] Start RecvFile Error\r\n", pClient->mSock);
									}
									
									DeleteClient(sock, Clients, &fs);
								}
							}
							
						}
						
					}
					else
					{

					}
				}
			}
		}
	}
	
	
	

	return 0;
}

