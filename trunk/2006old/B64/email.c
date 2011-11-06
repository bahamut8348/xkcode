/*send email*/
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MAXPACKLEN	(1024 * 70)
#define MAXREADLEN	(100)

unsigned int s;

struct sockaddr_in remote;
unsigned short port;
int rt;
char *usersrc,*userdes,*passsrc,*passdes;
char *From,*To,*Date,*Subject;
char *Filename;

//-----------------------------------------------------------------------------------------
// Declaration
//-----------------------------------------------------------------------------------------
void Base64_Code( unsigned char *chsrc, unsigned int srclen ,unsigned char *chdes  );
BOOL SendPacket( const char* buff );


//-----------------------------------------------------------------------------------------
// Common Function
//-----------------------------------------------------------------------------------------
void Base64_Code( unsigned char *chsrc, unsigned int srclen ,unsigned char *chdes )
{
	char chadd[3] = { 0 };
	unsigned char temp[4] = { 0 };
	unsigned char t = 0;
	int len = 0;
	int i = 0;
	int n = 0;
	len= srclen;
	while (len > 0)
	{
		// temp[0]	| temp[2]	|  temp[3]	| temp[4]
		// xxxxxx	| xx xxxx	|  xxxx xx	| xxxxxx
		
		temp[0] = (*chsrc) >> 2;
		
		if (len >= 3)
		{
			t = (*chsrc & 0x03) << 4;
			temp[1] = t | (*(chsrc+1) >> 4);

			t = ((*(chsrc+1) & 0x0f) << 2);
			temp[2] = t | (*(chsrc+2) >> 6);

			temp[3] = (*(chsrc+2) & 0x3f );
			n = 3;
		}
		else if (len >= 2)
		{
			t = (*chsrc & 0x03) << 4;
			temp[1] = t | (*(chsrc+1) >> 4);

			t = ((*(chsrc+1) & 0x0f) << 2);
			temp[2] = t ;
			n = 2;
		}
		else
		{
			t = (*chsrc & 0x03) << 4;
			temp[1] = t ;
			n = 1;
		}
		
		
		for (i = 0;i < 4;i++)
		{
			if (i <= n)
			{
				if(temp[i]>=0 && temp[i]<=25)
					*(chdes+i)=temp[i]+65;
				if(temp[i]>=26 && temp[i]<=51)
					*(chdes+i)=temp[i]+71;
				if(temp[i]>=52 && temp[i]<=61)
					*(chdes+i)=temp[i]-4;
				if(temp[i]==62)
					*(chdes+i)=43;
				if(temp[i]==63)
					*(chdes+i)=47;
			}
			else
			{
				*(chdes+i) = '=';
			}

		}

		len-=3;
		chsrc+=3;
		chdes+=4;
	}

	*chdes='\0';
}

void Base64_Decode(unsigned char *chsrc, unsigned char *chdes)
{
	unsigned char temp[4],t;
	int len,i;
	len=strlen((char *)chdes);
	while(len>=4)
	{
		for(i=0;i<4;i++)
		{
			if(*(chdes+i)>=65 && *(chdes+i)<=90)
				temp[i]=*(chdes+i)-65;
			if(*(chdes+i)>=97 && *(chdes+i)<=122)
				temp[i]=*(chdes+i)-71;
			if(*(chdes+i)>=48 && *(chdes+i)<=57)
				temp[i]=*(chdes+i)+4;
			if(*(chdes+i)==43)
				temp[i]=62;
			if(*(chdes+i)==47)
				temp[i]=63;
			if(*(chdes+i)==61)
				temp[i]=0;
		}
		t=(temp[1]>>4)&0x03;
		*chsrc=(temp[0]<<2)|t;
		t=(temp[2]>>2)&0x0f;
		*(chsrc+1)=(temp[1]<<4)|t;
		t=temp[3];
		*(chsrc+2)=(temp[1]<<6)|t;
		chsrc+=3;
		chdes+=4;
		len-=4;
	}
	chsrc-=3;
	for(i=0;i<3;i++)
	{
		if(*(chsrc+i)==0)
		{
			*(chsrc+i)='\0';
			break;
		}
	}
	if(i>=2)
		*(chsrc+3)='\0';
}

BOOL AddPacket( char descstr[MAXPACKLEN], const char* scrstr )
{
	size_t len = strlen(descstr) + strlen(scrstr);
	if ( len > MAXPACKLEN )
	{
		return FALSE;
	}
	else
	{
		strcat( descstr, scrstr );
	}
	return TRUE;
}

//-----------------------------------------------------------------------------------------
// Network Function
//-----------------------------------------------------------------------------------------

void Log( int type, const char* format, ... )
{
	int rt = 0;
	char buffer[2048] = {0};
	long paramnumber = 0;
	long i;
	char* parameters;

	// Find how many parameters we need.
	for (  i = 0; format[i] != 0; i ++ )
	{
		if ( format[i] == '%' && format[ i + 1 ] != '%' && format[ i + 1 ] != '*' )
			paramnumber ++;
	}

	paramnumber *= sizeof( void* );

	parameters = (char*)( &format ) + sizeof( format );

	// Make a local stack, and fill local stack the way wscanf likes it.
	__asm sub esp, paramnumber;
	__asm mov ecx, paramnumber;
	__asm mov esi, parameters;
	__asm mov edi, esp;
	__asm rep movsb;

	sprintf( buffer, format);

	__asm add esp, paramnumber;

	switch( type )
	{
	case 110:
		{
			rt = WSAGetLastError();
			printf("%s error code:%d\n", buffer, rt);
		}
		break;
	case 1:
	case 2:
	case 101:
	case 10:
		{
			printf("%s", buffer);
		}
		break;
	default:
		{
			printf("UnKown Error Type: %d \n", type);
		}
	}
}

int SendPacket(const char* format, ...)
{
	char sendbuff[MAXPACKLEN] = {0};
	int sendlen = 0;
	long paramnumber = 0;
	long i;
	char* parameters;
	int rt = 0;

	// Find how many parameters we need.
	for ( i = 0; format[i] != 0; i ++ )
	{
		if ( format[i] == '%' && format[ i + 1 ] != '%' && format[ i + 1 ] != '*' )
			paramnumber ++;
	}

	paramnumber *= sizeof( void* );

	parameters = (char*)( &format ) + sizeof( format );

	// Make a local stack, and fill local stack the way wscanf likes it.
	__asm sub esp, paramnumber;
	__asm mov ecx, paramnumber;
	__asm mov esi, parameters;
	__asm mov edi, esp;
	__asm rep movsb;

	sprintf( sendbuff, format);

	__asm add esp, paramnumber;

	sendlen = strlen(sendbuff);
	FILE *FF = fopen("log.l","ab");
	fwrite(sendbuff, sendlen, 1, FF);
	fclose(FF);
	rt = send(s, sendbuff, sendlen, 0);
	if (SOCKET_ERROR == rt)
	{
		Log(110, "Send Error:");
		closesocket(s);
		return -1;
	}
	else
	{
		Log(10, "Send Packet %d byte\n", rt);
	}

	return 0;
}

int GetResponse()
{
	char recv_data[MAXPACKLEN] = { 0 };

	if(SOCKET_ERROR == recv( s, recv_data, MAXPACKLEN, 0 ))
	{
		Log(110, "Recv Nothing");
		closesocket(s);
		return -1;
	}

	if(recv_data[0] == '5')
	{
		Log(102, "Error: %s\n",recv_data);
		return -1;
	}
	else
	{
		Log(2, "Recv: %s\n",recv_data);
	}

	return 0;
}

int CreateSocket()
{
	WSADATA dw;
	WSAStartup(MAKEWORD(2, 2), &dw );
	s=socket(AF_INET,SOCK_STREAM,0);
	if(SOCKET_ERROR==s)
	{
		Log(110, "Socket Init Error\n");
		closesocket(s);
		return -1;
	}

	return 0;
}

int ConnectHost(const char *hostname)
{
	//printf("socket id:%d\n",s);
	if(hostname==NULL)
		return -1;
	memset(&remote,0,sizeof(struct sockaddr));
	remote.sin_family=AF_INET;
	remote.sin_port=htons(port);
	remote.sin_addr.s_addr=inet_addr(hostname);

	if(SOCKET_ERROR == connect(s,(struct sockaddr *)&remote,sizeof(struct sockaddr)))
	{
		Log(110, "Socket Connect Error");
		closesocket(s);
		return -1;
	}
	//printf("connect the host ok:%d\n",port);
	if(GetResponse())
		return -1;

	return 0;
}

//-----------------------------------------------------------------------------------------
// Smtp Function
//-----------------------------------------------------------------------------------------
int LogIn(char *username,char *password)
{
	char userdes[100] = { 0 };
	char passdes[100] = { 0 };
	if(username==NULL||password==NULL)
		return -1;

	//SAY HELLO
	if (SendPacket("EHLO ps-keen\r\n"))
		return -1;

	if(GetResponse())
		return -1;

	//Login
	if (SendPacket("AUTH LOGIN\r\n"))
		return -1;

	if(GetResponse())
		return -1;
	
	//User
	Base64_Code((unsigned char *)username, strlen(username),(unsigned char *)userdes);

	if (SendPacket("%s\r\n", userdes))
		return -1;

	if(GetResponse())
		return -1;

	//Password
	Base64_Code((unsigned char *)password, strlen(password), (unsigned char *)passdes);

	if (SendPacket("%s\r\n", passdes))
		return -1;

	if(GetResponse())
		return -1;
	
	return 0;
}

int SendMail(const char *from,const char *to,const char *date,const char *subject,const char *data)
{
	if(from == NULL || to==NULL || date==NULL || subject==NULL)
		return -1;

	//From
	if (SendPacket("MAIL FROM:<%s>\r\n", from))
		return -1;

	if(GetResponse())
		return -1;

	//RCPT TO
	if (SendPacket("RCPT TO:<%s>\r\n", to))
		return -1;

	if(GetResponse())
		return -1;

	//DATA
	if (SendPacket("DATA\r\n"))
		return -1;

	//DATA:FROM	TO DATA 
	if (SendPacket("From:%s\r\nTo:%s\r\nDate:%s\r\nSubject:%s\r\n", from, to, date, subject))
		return -1;


	//Boundary
	if (SendPacket("Content-type:multipart/mixed;Boundary=\"=====KEEN=====\"\r\n\r\n"))
		return -1;


	//Content Head
	if (SendPacket("%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n", "--=====KEEN=====", "Content-Type: text/plain;", "\tcharset=\"gb2312\""
		, "Content-Transfer-Encoding: 7bit", "\r\n"))
		return -1;

	//Content
	if (SendPacket("%s\r\n", data))
		return -1;

	return 0;
}

int AttchFile(const char *filename)
{	
	char sendbuff[MAXPACKLEN] = {0};
	char readbuff[MAXREADLEN] = {0};
	unsigned int readnum = 0;
	FILE *f = NULL;
	//DATA
 
	f = fopen( filename, "rb" );

	if (NULL == f)
		return -1;

	//Send Info Head
	if (SendPacket("%s\r\n%s\r\n%s%s%s\r\n%s\r\n%s\r\n%s%s%s\r\n%s", "--=====KEEN=====", "Content-Type: application/octet-stream;", "\tname=\"", filename, "\""
		, "Content-Transfer-Encoding: base64", "Content-Disposition: attachment;", "\tfilename=\"", filename, "\"", "\r\n"))
	{
		fclose( f );
		return -1;   
	}

	fseek( f, 0, SEEK_END);
	Log(2, "File Size %d\n", ftell(f));
	fseek( f, 0, SEEK_SET);

	readnum = fread( readbuff, sizeof(char), MAXREADLEN - 2, f );

	while (0 != readnum)
	{
		memset(sendbuff, 0, MAXPACKLEN);
		Base64_Code( readbuff, readnum, sendbuff );
		strcat(sendbuff,"\r\n");

		memset(readbuff, 0, MAXREADLEN);

		readnum = fread( readbuff,  sizeof(char), MAXREADLEN - 2, f );

		if ( 0 == readnum )
		{
			if (SendPacket("%s\r\n%s\r\n", sendbuff, "--=====KEEN=====--"))
			{
				fclose( f );
				return -1;
			}
		}
		else
		{
			if (SendPacket(sendbuff))
			{
				fclose( f );
				return -1;
			}
		}
	}

	fclose(f);
	return 0;
 
}



int End()
{
	if (SendPacket( "\r\n.\r\n" ))
	{
		return -1;
	}
	if(GetResponse() )
		return -1;


	if (SendPacket( "QUIT\r\n" ))
	{
		return -1;
	}
	if(GetResponse() )
		return -1;

	return 0;

}

//-----------------------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------------------

int main(int argc,char *argv[])
{

	char *host		="202.108.5.81"; //smtp.163.com
	//char *username	="lcc812";
	//char *password	="811212";
	//char *m_From	="lcc812@163.com";

	char *username	="xiaoxie9802";
	char *password	="zgr6424144";
	char *m_From	="xiaoxie9802@163.com";

	//char *host		="202.108.252.140"; //smtp.tom.com
	//char *username	="lcc812";
	//char *password	="811212";
	//char *m_From	="lcc812@tom.com";

	//char *m_To	="lcc812@tom.com";
	char *m_To		="cnxuke@hotmail.com";
	char *m_date	="2007";
	char *m_Subject	="hello";
	char *m_data	="it is test";
	char *m_File	="1234.rar";

	if(argc<1)
	{
		printf("agrv error\n");
	}
	port = 25;
	s = INVALID_SOCKET;
	
	//Email Send Process
	do
	{
		Log(1, "Create the Socket\n");
		if(CreateSocket())
		{
			Log(101, "socket create error\n");
			break;
		}

		Log(1, "Connect to SMTP Host %s\n", host);
		if(ConnectHost(host))
			break;

		Log(1, "Validate Name and Password: %s \n", username);
		if(LogIn(username,password))
			break;

		Log(1, "Send Email from %s to %s\n", m_From, m_To );
		if(SendMail(m_From,m_To,m_date,m_Subject,m_data))
			break;
		//printf("send mail\n");

		Log(1, "Send Attachment: %s\t", m_File);
		if(AttchFile(m_File))
			break;

	
		if(End())
			break;

	}while(0);

	Log(1, "press any key to be continue...\n");
	getch();
	return 0;
}

