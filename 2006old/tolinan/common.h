

#pragma  comment (lib, "ws2_32")


#define  PORT 8888
#define  MAX_FILENAME 256



typedef struct 
{
	DWORD mVersion;
	char  mFileName[MAX_FILENAME];
	DWORD mLength;

}Packet_StartFile;

void InitWSA()
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

}