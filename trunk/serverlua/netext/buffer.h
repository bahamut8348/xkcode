#include <stdio.h>

#ifndef _BUFFER_H_
#define _BUFFER_H_

class MemBuffer
{
	public:
	
	MemBuffer();
	char *Get( int size );
	void Release(char * ptr);	
	static MemBuffer * Instance()
	{
		static MemBuffer o;
		return &o;
	}

};


#endif
