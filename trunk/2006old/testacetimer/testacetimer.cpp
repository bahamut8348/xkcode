// testacetimer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TimerTask.h"



int ACE_MAIN(int argc, _TCHAR* argv[])
{
	
	ACE_Auto_Ptr<CTimerTask> ptask(new CTimerTask);
	ptask->Start();
//	ACE_Reactor::instance()->set_sig_handler()
	ACE_Reactor::instance()->run_event_loop();

	return 0;
}

