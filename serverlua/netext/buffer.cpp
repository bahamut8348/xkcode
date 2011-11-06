#include <string>
#include <list>

#include "buffer.h"
#include "appinfo.h"
#include "log.h"
MemBuffer::MemBuffer()
{

}

char *MemBuffer::Get( int size )
{
	AppInfo::Instance()->mGetMem ++;
	char * r = new char[size];
	loginfo("### new %p",r );
	return r;
}

void MemBuffer::Release( char * ptr )
{
	AppInfo::Instance()->mReleaseMem ++;
	loginfo("### free %p", ptr );
	return delete [] ptr;
}
