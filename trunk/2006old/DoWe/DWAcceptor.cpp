#include "StdAfx.h"
#include "Control.h"
#include "DWAcceptor.h"

CDWAcceptor::CDWAcceptor(void)
{
}

CDWAcceptor::~CDWAcceptor(void)
{
}

void CDWAcceptor::open( u_short port )
{
	acceptor_.open(ACE_INET_Addr(port), 0, 1);
	
}

void CDWAcceptor::open( ACE_HANDLE handle,ACE_Message_Block &message_block )
{
	ACE_DEBUG ((LM_INFO, L"handle_accept\n"));
	new CControl(handle);
}