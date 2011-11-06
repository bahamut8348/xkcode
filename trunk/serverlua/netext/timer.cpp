
#include <sys/time.h>
#include "timer.h"
#include <stdio.h>

Timer::Timer( const char* n ){
	Begin(n);
}

Timer::~Timer(){
	End();
}

void Timer::Begin( const  char* n ){
	mName = n;
	gettimeofday( &mBefore, &mTz); 
}

void Timer::End(){
	gettimeofday( &mAfter, &mTz);

	printf("[%s] use time: %f\n" , mName.c_str(), (float)(((float)mAfter.tv_sec*1000.0f + (float)mAfter.tv_usec/1000.f) - ((float)mBefore.tv_sec *1000.f + (float)mBefore.tv_usec/1000.f))/1000.f) ;
}

