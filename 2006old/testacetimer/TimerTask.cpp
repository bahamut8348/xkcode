#include "StdAfx.h"
#include "TimerTask.h"

int CTimerTask::handle_timeout( const ACE_Time_Value &current_time, const void *act /*= 0*/ )
{
	ACE_DEBUG(( LM_INFO, L"[%P]handle time out\n"));
	Select<CTimerTask *, ACTION> *sel = (Select<CTimerTask *, ACTION> *)act;
	//(sel->self->*(sel->fun))(current_time.sec(),2);
	sel->Execute((int)2,(int)1);
	return 0;
}

void CTimerTask::Start()
{
	ACE_Reactor::instance()->schedule_timer(this, MakeEvent(this, &CTimerTask::call_2),ACE_Time_Value(3), ACE_Time_Value(1));
}

void CTimerTask::call_2( int a, int b )
{
	ACE_DEBUG(( LM_INFO, L"[%P] Hanlde call_2(%d,b) \n", a));
}

int CTimerTask::handle_signal( int signum, siginfo_t * /*= 0*/, ucontext_t * /*= 0*/ )
{
	ACE_DEBUG(( LM_INFO, L"[%P] handle_signal(%d,b) \n"));
	return 1;
}