#pragma once

class IOCP_SOCKET
{
public:
	IOCP_SOCKET(void);
	~IOCP_SOCKET(void);
};

namespace pyiocp
{
	typedef struct {
		PyObject_HEAD
		int sock_fd;	/* Socket file descriptor */
		double sock_timeout;		 /* Operation timeout in seconds; */
		int iocp_h;
		PyObject* key;
	} PyIOCPSockObject;

	PyObject *PyIOCPSock_New( int family , int type, int proto );
	int PyIOCPSock_Check( PyObject* o );
}




