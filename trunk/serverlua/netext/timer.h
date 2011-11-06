#include <sys/time.h>
#include <iostream>

#ifndef __STIMER_H__
#define __STIMER_H__


class Timer{
	

	private:
	struct timeval mBefore;	
	struct timeval mAfter;
	struct timezone mTz;	
	std::string  mName;
	public:
		Timer( const char * );
		~Timer();
		void Begin( const char* );
		void End();
	static unsigned  long long GetTime(){
		static struct timeval a,b;
		static struct timezone tz;
		gettimeofday( &a, &tz );
		return ( unsigned long long)a.tv_sec*1000 + ( unsigned long long) a.tv_usec / 1000 ;
	}

};

#endif
