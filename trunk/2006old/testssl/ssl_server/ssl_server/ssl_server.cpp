// ssl_server.cpp : Defines the entry point for the console application.
//


// Gen root key 
// >openssl genrsa -des3 -out key.pem 2048

// Gen request
// >openssl req -new -key key.pem -out cert.csr -config c:\openssl.cnf

// Gen Cert
// >>openssl req -new -x509 -key key.pem -out cert.pem -days 3650 -config c:\openssl.cnf

#include "stdafx.h"

void test1()
{
	ssl_st *ssl = NULL;
	ssl_ctx_st *ssl_ctx = NULL;

	char msg[1024];

	unsigned char strRand[ 32 ] ;
	memset( strRand, 0, sizeof( strRand ) );
	sprintf_s( (char*)strRand, sizeof( strRand ), "%d%ld",::GetCurrentProcessId() , time(NULL) );
	RAND_seed( strRand, sizeof( strRand ) );

	char *certFile = "demo.crt";
	char *keyFile = "demo.key";
	
	ERR_load_crypto_strings ();
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();

	
	// init ssl_ctx
	ssl_ctx = SSL_CTX_new( SSLv23_server_method() );
	ERR_error_string_n( ERR_get_error(), msg, sizeof( msg ) );

	int ret = SSL_CTX_use_certificate_file( ssl_ctx, certFile, SSL_FILETYPE_PEM );
	ERR_error_string_n( ERR_get_error(), msg, sizeof( msg ) );

	ret =  SSL_CTX_use_PrivateKey_file( ssl_ctx, keyFile, SSL_FILETYPE_PEM );
	ERR_error_string_n( ERR_get_error(), msg, sizeof( msg ) );
	
	ret = SSL_CTX_check_private_key( ssl_ctx );

	
	SOCKET sock = socket( AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr = {0};
	addr.sin_port = htons( 443 );
	addr.sin_family = 2;

	bind( sock, (sockaddr*)&addr, sizeof(addr));
	listen(sock, 5);
	
	SOCKADDR_IN safrom;
	int salen = sizeof( safrom );
	

	while(1)
	{
		SOCKET newsock = accept(sock, (sockaddr *)&safrom, &salen );
		ssl_st *newssl = SSL_new( ssl_ctx );
		SSL_set_fd( newssl, newsock);


		ret = SSL_accept( newssl );
		if( ret<= 0 )
		{
			int err = SSL_get_error( newssl, ret );
			ERR_error_string_n(  err, msg, sizeof( msg ) );
		}
		else
		{
			BYTE buffer[1024];
			SSL_read( newssl, buffer, 1024 );

			SSL_write(newssl, "HTTP/1.1 200 OK\nServer: XK\nConnection: Close\nContent-Length: 4\n\nhahahaha", 72 );
			SSL_shutdown(newssl);
			SSL_free( newssl );
		}

	}
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA data ;
	WSAStartup(0x0202,&data );
	
	test1();

	
	
	CRYPTO_malloc_init();
	
	return 0;
}

