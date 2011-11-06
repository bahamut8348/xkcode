#pragma once
namespace pyiocp
{
	

	enum IO_TYPE
	{
		_IO_SEND = 1,
		_IO_RECV,
		_IO_SENDTO,
		_IO_RECVFROM,
		_IO_READ,
		_IO_WRITE,
		_IO_CONNECT,
		_IO_ACCEPT,
	};

	struct IocpPerio : OVERLAPPED
	{
		IO_TYPE type;
		PyObject* key;
		PyObject* obj;
		PyObject* buffer;
	};

}


