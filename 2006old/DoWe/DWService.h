#pragma once



class CDWService : public ACE_NT_Service
{
public:
	CDWService(void);
	~CDWService(void);

	virtual void handle_control (DWORD control_code);
	virtual int  handle_exception (ACE_HANDLE h);
	virtual int svc (void);
	virtual int handle_timeout (const ACE_Time_Value& tv,
		const void *arg = 0);

	BOOL bNeedStop;
};


typedef ACE_Singleton<CDWService, ACE_Mutex> SERVICE;
