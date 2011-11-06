
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef _LOG_H_
#define _LOG_H_

extern int log_level;
extern int logf_g;
static void loglog( const  char *str );

static void logerr( const  char * format, ... )
{
	if( log_level < 1 ) return;
	static char errinfo[1024];
	static char tmp[1024];
	va_list args;
	va_start ( args, format );
	vsprintf( tmp, format,  args );
	va_end(args);
	auto l = sprintf( errinfo, "\033[37m\033[41merror:(%d) %s, %s\033[0m", errno, strerror( errno), tmp ); 
	if( logf_g > 0 )	
		write( logf_g, errinfo, l );
	loglog(errinfo);
}


static void loginfo( const char * format, ... )
{
	if( log_level < 2 ) return;
	static char tmp[1024];
	va_list args;
	va_start ( args, format );
	auto l = vsprintf( tmp, format, args );
	va_end(args);
	if( logf_g > 0 )
		write( logf_g, tmp, l );
	loglog( tmp );
}

static void logdebug( const char * format, ... )
{
	if( log_level < 3 ) return ;
	static char tmp[1024];
	va_list args;
	va_start ( args, format );
	auto l = vsprintf( tmp, format, args );
	va_end(args);
	if( logf_g > 0 )
	write( logf_g, tmp, l );
	loglog( tmp );
}

static void logdump(const char * desc, const char * s, int len )
{
	printf("Dump %s :", desc );
	int line = 16;
	for( int i = 0 ; i < len; i ++ )
	{
		unsigned char ch = ( unsigned char )s[i];
		
		if ( i >= line ) break;

		printf( "%02X ", ch );
	}
	if( len < 16 )
	{
		for( int i = 0; i < 16-len; i ++ )
		{
			printf("   ");
		}

	}
	printf("|");
	for( int i = 0; i < len ; i++)
	{
		unsigned char ch = (unsigned char) s[i];
		if( i >= line ) break;
		if( ch <33 || ch > 145 )
		{
			printf("%c", '\20');
		}
		else
		{
			printf( "%c", ch );
		}
	}
	printf("\n");
	len -= 16;
	s += 16;
	if( len > 0 )
	{
		return logdump( desc, s, len );
	}
	

}

static void loglog( const char * str)
{
	printf("%s\n", str );
}

#endif
