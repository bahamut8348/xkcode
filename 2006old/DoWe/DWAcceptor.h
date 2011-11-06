#pragma once

class CDWAcceptor : public ACE_Service_Handler
{
public:
	CDWAcceptor(void);
	~CDWAcceptor(void);
	
	void open(u_short port);
	
	// called when connection
	virtual void open (ACE_HANDLE handle,ACE_Message_Block &message_block);
	
	ACE_Asynch_Acceptor<CDWAcceptor> acceptor_;
};
