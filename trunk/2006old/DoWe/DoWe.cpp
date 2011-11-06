// DoWe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DWService.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"

void print_usage(const ACE_TCHAR * fn)
{
	ACE_DEBUG ((LM_INFO,
		"Usage: %s"
		" -in -r -s -k -tn -d\n"
		"  -i: Install this program as an NT service, with specified startup\n"
		"  -r: Remove this program from the Service Manager\n"
		"  -s: Start the service\n"
		"  -k: Kill the service\n"
		"  -d: Debug; run as a regular application\n",
		fn,
		0));
	ACE_OS::exit(1);
}

ACE_NT_SERVICE_DEFINE (DwServer,
					   CDWService,
					   ACE_TEXT ("DoWe Service"));

static BOOL __stdcall
ConsoleHandler (DWORD ctrlType)
{
	ACE_UNUSED_ARG (ctrlType);
	SERVICE::instance ()->handle_control (SERVICE_CONTROL_STOP);
	return TRUE;
}

int ACE_TMAIN(int argc, ACE_TCHAR * argv[])
{
	char c;
	ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("irskd"));
	BOOL bDebug = FALSE;

	SERVICE::instance ()->name (ACE_TEXT ("DwServer"),
		ACE_TEXT ("DoWe Service"));
	
	if(argc == 1)
	{
		print_usage(argv[0]);
	}

	while ((c = get_opt ()) != -1)
	{
		switch (c)
		{
		case 'i':
			{
				SERVICE::instance()->insert (SERVICE_AUTO_START);
				ACE_DEBUG (( LM_NOTICE, L"Service Install"));
				break;
			}
		case 'r':
			{
				SERVICE::instance()->remove();
				ACE_DEBUG (( LM_NOTICE, "Service Remove"));
				break;
			}
		case 's':
			{
				SERVICE::instance()->start_svc();
				ACE_DEBUG (( LM_NOTICE, "Service Start"));
				break;
			}
		case 'k':
			{
				SERVICE::instance()->stop_svc();
				ACE_DEBUG (( LM_NOTICE, "Service Stop"));
				break;
			}
		case 'd':
			{
				bDebug = TRUE;
				SetConsoleCtrlHandler (&ConsoleHandler, 1);
				SERVICE::instance ()->svc();
				break;
			}

		default:
			print_usage(argv[0]);


		}

	}

	if(bDebug)
	{
		ACE_DEBUG((LM_INFO, L"Start in console\n"));
		SetConsoleCtrlHandler (&ConsoleHandler, 1);
	}
	else
	{
		ofstream *output_file = new ofstream(L"dw.log", ios::out);
		if (output_file && output_file->rdstate() == ios::goodbit)
			ACE_LOG_MSG->msg_ostream(output_file, 1);
		ACE_LOG_MSG->open(L"DWServer",
			ACE_Log_Msg::STDERR | ACE_Log_Msg::OSTREAM,
			0);
		ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%T (%t): Starting service.\n")));

		ACE_NT_SERVICE_RUN (DwServer,
			SERVICE::instance (),
			ret);
		if (ret == 0)
			ACE_ERROR ((LM_ERROR,
			ACE_TEXT ("%p\n"),
			ACE_TEXT ("Couldn't start service")));
		else
			ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%T (%t): Service stopped.\n")));

	}



	return 0;
}

