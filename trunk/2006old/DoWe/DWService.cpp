#include "StdAfx.h"

#include "DWAcceptor.h"
#include "DWService.h"

CDWService::CDWService(void):
	bNeedStop(FALSE) 
{
}

CDWService::~CDWService(void)
{
}

void CDWService::handle_control( DWORD control_code )
{
	if (control_code == SERVICE_CONTROL_SHUTDOWN
		|| control_code == SERVICE_CONTROL_STOP)
	{
		report_status (SERVICE_STOP_PENDING);
		
		

		ACE_DEBUG ((LM_INFO,
			ACE_TEXT ("Service control stop requested\n")));
		
		bNeedStop = TRUE;
		ACE_Proactor::instance()->end_event_loop();

		ACE_Reactor::instance()->notify (this,
			ACE_Event_Handler::EXCEPT_MASK);
	}
	else
		ACE_NT_Service::handle_control (control_code);
}

int CDWService::handle_exception( ACE_HANDLE h )
{
	return 0;
}

int CDWService::svc( void )
{
	ACE_DEBUG((LM_INFO, L"Service OK\n"));
	CDWAcceptor mainsock;
	mainsock.open(2111);
	
	ACE_Proactor::instance()->run_event_loop();
	

	return 0;	
}

int CDWService::handle_timeout( const ACE_Time_Value& tv, const void *arg /*= 0*/ )
{
	return 0;
}