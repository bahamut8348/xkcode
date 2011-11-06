#include "StdAfx.h"
#include <Python.h>
#include <WinSock2.h>
#include <Mswsock.h>
#include <WS2tcpip.h>
#include <structmember.h>

#include "IOCP_BUFFER.h"
#include "IOCP_ERROR.h"
#include "IOCP_SOCKET.h"

#include "IOCP.h"

IOCP_SOCKET::IOCP_SOCKET(void)
{
}

IOCP_SOCKET::~IOCP_SOCKET(void)
{
}
namespace pyiocp
{

/* Type object for socket objects. */

static void
sock_dealloc(PyIOCPSockObject *s)
{
	
	//printf("sock_dealloc called %d\n", s->sock_fd );
	if ( s->sock_fd != -1)
	{
		if( 0 != ::closesocket( (SOCKET) s->sock_fd ) )
		{
			printf("sock_dealloc error\n");	
		}
		else
		{
			s->sock_fd = 0;
			s->sock_timeout = 0;
			s->iocp_h = 0;
		}

		
	}
	Py_DecRef( s->key );
	s->ob_type->tp_free( ( PyObject * ) s );
}

static PyObject *
sock_repr(PyIOCPSockObject *s)
{
	char buf[512];
	PyOS_snprintf( buf, sizeof(buf), "<iocp socket object, handle=%ld, iocp=%d >", (long)s->sock_fd, s->iocp_h );
	return PyString_FromString(buf);
}

static PyObject *
sock_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyObject *new_sock;

	new_sock = type->tp_alloc(type, 0);
	if (new_sock != NULL) 
	{
		((PyIOCPSockObject *)new_sock)->sock_fd = -1;
		((PyIOCPSockObject *)new_sock)->sock_timeout = -1.0;
			((PyIOCPSockObject *)new_sock)->iocp_h = 0;
		
	}
	return new_sock;
}

/*ARGSUSED*/
static int
sock_initobj(PyObject *self, PyObject *args, PyObject *kwds)
{
	PyIOCPSockObject *s = (PyIOCPSockObject *)self;
	int fd;
	int family = AF_INET, type = SOCK_STREAM, proto = 0;
	static char *keywords[] = {"family", "type", "proto", 0};

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
		"|iii:socket", keywords,
		&family, &type, &proto))
		return -1;

	Py_BEGIN_ALLOW_THREADS
		fd = socket(family, type, proto);
	Py_END_ALLOW_THREADS


		if (fd == INVALID_SOCKET)
		{
			iocp_error( );
			return -1;
		}
//		init_sockobject(s, fd, family, type, proto);

		return 0;

}

PyDoc_STRVAR(sock_doc, "iocp socket from pyiocp.socket" );

PyObject* sock_close( PyIOCPSockObject* s, PyObject *args )
{
	if( 0 != ::closesocket( (SOCKET) s->sock_fd ) )
	{
		return iocp_error( ::WSAGetLastError( ) );
	}
	s->sock_fd = -1;
	Py_RETURN_NONE;
}

PyObject* sock_bind( PyIOCPSockObject* self, PyObject *args )
{
	int fd = self->sock_fd;
	PyObject* server_t = 0;
	char *server = 0;
	int port;
	int r = PyArg_ParseTuple( args, "O", &server_t );
	if( r )
	{
		if( PyTuple_Check(server_t) )
		{
			r = PyArg_ParseTuple( server_t, "si", &server, &port);
			if( r == 0) return param_error( );
			if( server[0] == 0 ) server = "0.0.0.0";
		}
		else if( PyNumber_Check( server_t ) )
		{
			server = "0.0.0.0";
			port = PyInt_AsLong( server_t );
		}
		else return param_error( ); 
		
		
		
		WSAPROTOCOL_INFOA wsa_protocol_info;
		int wsa_info_len = sizeof(WSAPROTOCOL_INFOA);
		r = ::getsockopt( fd, SOL_SOCKET, SO_PROTOCOL_INFOA,(char*) &wsa_protocol_info, &wsa_info_len );
		if( r!= 0 ) return iocp_error( );
		addrinfo info = { 0 };
		info.ai_family = wsa_protocol_info.iAddressFamily;
		info.ai_socktype = wsa_protocol_info.iSocketType;
		info.ai_protocol = wsa_protocol_info.iProtocol;

		addrinfo * info_out;
		char buf[20];
		_ltoa_s( port, buf,20, 10 );
		
		r = ::getaddrinfo( server, buf, &info, &info_out );
		if( r!= 0 ) return iocp_error( );
		if( 0 == ::bind( fd, info_out->ai_addr, info_out->ai_addrlen ) )
		{
			::freeaddrinfo( info_out );
			Py_RETURN_NONE;
		}
		else
		{
			::freeaddrinfo( info_out );
			return iocp_error( );
		}
	}
	return param_error( );
}

PyObject* sock_sendto( PyIOCPSockObject* self, PyObject *args, PyObject *kwds )
{
	PyObject* buffer = 0;
	int send_len = -1;
	static char *kwlist[] = { "addr", "buf", "key","len", 0 };
	PyObject* perio_key = Py_None;
	PyObject* server = 0;
	int r = PyArg_ParseTupleAndKeywords( args, kwds, "OO|Oi:send", kwlist, &server, &buffer, &perio_key, &send_len );
	if( r == 0 ) return param_error( );

	// server addr
	char *server_addr;
	int server_port;
	r = PyArg_ParseTuple( server, "si", &server_addr, &server_port );
	if( r ==0 ) return param_error( );
	if( server_addr[0] ==0 ) server_addr="0.0.0.0";
	int b_self_buffer = 1;
	char * send_buffer_ptr = 0;
	int send_buffer_len = 0 ;

	if( ! PyIOCPBuffer_Check( buffer ) )
	{
		r = PyObject_AsReadBuffer( buffer, (const void **)&send_buffer_ptr, &send_buffer_len );
		if( r != 0)
		{
			PyErr_BadArgument( );
			return NULL;
		}

		PyIOCPBufferObject* new_buffer = ( PyIOCPBufferObject* ) PyIOCPBuffer_New( send_buffer_len );
		memcpy_s( new_buffer->ptr_start, new_buffer->len, send_buffer_ptr, send_buffer_len );
		buffer = ( PyObject* ) new_buffer;
	}
	
	PyObject_AsReadBuffer( buffer, (const void **)&send_buffer_ptr, &send_buffer_len );

	IocpPerio *perio = (IocpPerio*) PyMem_Malloc( sizeof( IocpPerio ) );
	memset( perio, 0, sizeof( IocpPerio ) );
	perio->buffer = buffer;
	perio->key = perio_key;
	perio->obj = (PyObject*) self;
	perio->type = _IO_SENDTO;

	WSABUF DataBuf;
	DataBuf.buf = (char *)send_buffer_ptr;
	if( send_len == -1 )
		DataBuf.len = send_buffer_len;
	else
		DataBuf.len = send_len;
	
	WSAPROTOCOL_INFOA wsa_protocol_info;
	int wsa_info_len = sizeof(WSAPROTOCOL_INFOA);
	::getsockopt( self->sock_fd, SOL_SOCKET, SO_PROTOCOL_INFOA,(char*) &wsa_protocol_info, &wsa_info_len );
	SOCKADDR_IN sa = {0};
	int sa_len = sizeof( sa );
	sa.sin_family = wsa_protocol_info.iAddressFamily;
	sa.sin_port = ::htons( server_port );
	sa.sin_addr.S_un.S_addr = ::inet_addr( server_addr );

	int ret = ::WSASendTo( self->sock_fd,  &DataBuf, 1, NULL, 1, ( sockaddr* )&sa, sa_len,  perio, NULL );
	if( ret == 0 )
	{
		PyMem_Free( perio );
		return iocp_error( );
	}
	else
	{
		if ( GetLastError( ) != WSA_IO_PENDING )
		{
			PyMem_Free( perio );
			return iocp_error ( );
		}
		else
		{
			Py_INCREF(  perio->key );
			Py_INCREF(  perio->obj  );
			Py_INCREF( perio->buffer );
			Py_RETURN_NONE;
		}
	}
	
}

PyObject* sock_send( PyIOCPSockObject* self, PyObject *args, PyObject *kwds )
{
	PyObject* buffer = 0;
	int send_len = -1;
	static char *kwlist[] = { "buf", "key","len", 0 };
	PyObject* perio_key = Py_None;
	int r = PyArg_ParseTupleAndKeywords( args, kwds, "O|Oi:send", kwlist, &buffer, &perio_key, &send_len );
	if( r == 0 ) return param_error( );
	int b_self_buffer = 1;
	if( PyString_Check( buffer ) )
	{
		char *str_ptr;
		int str_len = 0;
		PyString_AsStringAndSize( buffer,  &str_ptr, &str_len );
		PyObject* new_buffer = PyIOCPBuffer_New( str_len );
		void *new_buffer_ptr ;
		int new_buffer_len ;
		int r = PyObject_AsWriteBuffer( new_buffer, &new_buffer_ptr, &new_buffer_len );
		if( r != 0 ) return param_error( );
		memcpy_s( new_buffer_ptr, new_buffer_len, str_ptr, str_len );
		buffer = new_buffer;
	}
	else if( PyIOCPBuffer_Check( buffer ) )
	{
		b_self_buffer = 0;
	}
	else
	{
		return param_error( ); 
	}

	IocpPerio *perio = (IocpPerio*) PyMem_Malloc( sizeof(IocpPerio) );
	memset( perio, 0, sizeof( IocpPerio ) );
	perio->buffer = buffer;
	perio->key = perio_key;
	perio->obj = (PyObject*)self;
	perio->type = _IO_SEND;
	
	const void *buffer_ptr = 0 ;
	int buffer_len  = 0 ;
	PyObject_AsReadBuffer( buffer, &buffer_ptr, &buffer_len );

	WSABUF DataBuf;
	DataBuf.buf = (char *)buffer_ptr;
	if( send_len == -1 )
		DataBuf.len = buffer_len;
	else
		DataBuf.len = send_len;

	int ret = ::WSASend( self->sock_fd, &DataBuf, 1, NULL, 1, perio, NULL );
	if( ret == 0 )
	{
		PyMem_Free( perio );
		return iocp_error( );
	}
	else
	{
		if ( GetLastError( ) != WSA_IO_PENDING )
		{
			PyMem_Free( perio );
			return iocp_error ( );
		}
		else
		{
			Py_INCREF(  perio->key );
			Py_INCREF(  perio->obj  );
			//if( b_self_buffer == 0 )
				Py_INCREF( perio->buffer );

			Py_RETURN_NONE;
		}
	}

	
}

PyObject* sock_getsockname( PyIOCPSockObject* self, PyObject *args )
{
	SOCKADDR_IN sa;
	int sa_len = sizeof( sa );
	if( 0 != ::getsockname( self->sock_fd, (sockaddr*)&sa, &sa_len ) )
		return iocp_error( );
	return Py_BuildValue("si", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port) );
}

PyObject* sock_getpeername( PyIOCPSockObject* self, PyObject *args )
{
	SOCKADDR_IN sa;
	int sa_len = sizeof( sa );
	if( 0 != ::getpeername( self->sock_fd, (sockaddr*)&sa, &sa_len ) )
		return iocp_error( );
	return Py_BuildValue("si", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port) );
}

PyObject* sock_recv( PyIOCPSockObject* self, PyObject *args, PyObject* kwds )
{
	PyObject* buffer = 0;
	static char *kwlist[] = { "len", "key","buf", 0 };
	int recv_len = -1;
	PyObject* perio_key = Py_None;
	int r = PyArg_ParseTupleAndKeywords( args, kwds, "i|OO:recv", kwlist, &recv_len, &perio_key, &buffer );
	if( r == 0 ) return param_error( );
	int b_self_buffer = 1;
	if( buffer == 0 )
	{
		buffer = PyIOCPBuffer_New( recv_len );
	}
	else if( PyBuffer_Check( buffer ) )
	{
		b_self_buffer = 0;
	}

	void *buffer_ptr = 0;
	int buffer_len = 0;
	if( 0!= PyObject_AsWriteBuffer( buffer, &buffer_ptr, &buffer_len ) )
	{
		PyErr_SetString( PyExc_Exception, "Buffer must be write able." );
		return NULL;
	}

	
	
	WSABUF DataBuf;
	DataBuf.buf = (char *)buffer_ptr;
	DataBuf.len = recv_len;
	
	IocpPerio *perio = (IocpPerio*) PyMem_Malloc( sizeof( IocpPerio ) );
	memset( perio, 0, sizeof( IocpPerio ) );
	perio->buffer = buffer;
	perio->key = perio_key;
	perio->obj = (PyObject*) self;
	perio->type = _IO_RECV;

	DWORD flag = 0;
	int ret = ::WSARecv( self->sock_fd, &DataBuf, 1, NULL, &flag, perio, NULL );
	if( ret == 0 )
	{
		PyMem_Free( perio );
		return iocp_error( );
	}
	else
	{
		if ( GetLastError( ) != WSA_IO_PENDING )
		{
			PyMem_Free( perio );
			return iocp_error ( );
		}
		else
		{
			Py_INCREF(  perio->key );
			Py_INCREF(  perio->obj  );
			Py_INCREF( perio->buffer );

			Py_RETURN_NONE;
		}
	}

	
	return param_error( ); 
}



PyObject* sock_accept( PyIOCPSockObject* self, PyObject *args, PyObject*kwds )
{
	int fd = self->sock_fd;
	char *kwlist[] = { "sock", "key", "buf", 0 };
	PyObject* perio_key = Py_None;
	PyObject* perio_buffer = 0;
	PyObject* new_sock = 0;
	const int c_buffer_len  = sizeof( SOCKADDR_IN) * 2;
	int r = PyArg_ParseTupleAndKeywords( args, kwds, "O|OO", kwlist,&new_sock, &perio_key, &perio_buffer );
	if( r == 0 ) return param_error( );
	if( PyIOCPSock_Check( new_sock )== 0 ) return param_error( );
	
	if( perio_buffer == 0 )
	{
		perio_buffer = PyIOCPBuffer_New( c_buffer_len * 2 );
	}
	
	char* perio_buffer_ptr = 0;
	int perio_buffer_len = 0;
	if( 0 != PyObject_AsWriteBuffer( perio_buffer,( void **) &perio_buffer_ptr, &perio_buffer_len ) )
		return param_error( );
	
	IocpPerio* perio = ( IocpPerio* )PyMem_Malloc( sizeof(IocpPerio) );
	memset( perio, 0, sizeof( IocpPerio ) );
	perio->buffer = perio_buffer;
	perio->key = perio_key;
	perio->obj = (PyObject*)self;
	perio->type = _IO_ACCEPT;

	int ret = ::AcceptEx( fd, (( PyIOCPSockObject* )new_sock )->sock_fd, perio_buffer_ptr, 0, c_buffer_len, c_buffer_len, NULL, perio );
	if( ret )
	{
		PyMem_Free( perio );
		return iocp_error( );
	}
	else
	{
		if( GetLastError( ) == WSA_IO_PENDING )
		{
			Py_INCREF( perio->buffer );
			Py_INCREF( perio->key );
			Py_INCREF( perio->obj );
			
			Py_RETURN_NONE;
		}
		else
		{
			PyMem_Free( perio );
			return iocp_error( );
		}
	}

}

PyObject* sock_listen( PyIOCPSockObject* self, PyObject *args )
{
	int backlog = 5;
	int r = PyArg_ParseTuple( args, "|i", &backlog );
	if ( r == 0 ) return param_error( );
	r = ::listen( self->sock_fd , backlog );
	if( r == 0) 
	{
		Py_RETURN_NONE;
	}
	else
	{
		return iocp_error( );
	}
}

PyObject* sock_connect( PyIOCPSockObject* self, PyObject *args )
{
	int fd = self->sock_fd ;
	char *server;
	int port;
	PyObject* pyaddr;
	PyObject* perio_key = Py_None;
	int r = PyArg_ParseTuple( args, "O|O", &pyaddr, &perio_key);
	if( r == 0 ) return param_error( );
	r = PyArg_ParseTuple( pyaddr, "si", &server, &port);
	if( r == 0 ) return param_error( );

	GUID GUIDName = WSAID_CONNECTEX;
	LPFN_CONNECTEX ConnectEx;
	DWORD dwReturn;
	if(WSAIoctl( fd , SIO_GET_EXTENSION_FUNCTION_POINTER, &GUIDName, sizeof(GUIDName),
		&ConnectEx, sizeof(ConnectEx), &dwReturn, NULL, NULL) == 0 )
	{
		SOCKADDR_IN sa = {0};
		sa.sin_family = AF_INET;
		sa.sin_addr.S_un.S_addr = inet_addr( server );
		sa.sin_port = htons( port );
		IocpPerio* perio = ( IocpPerio* )PyMem_Malloc( sizeof(IocpPerio) );
		memset( perio, 0, sizeof( IocpPerio ) );
		perio->type = _IO_CONNECT;
		
		perio->buffer = Py_None;
		perio->key = perio_key;
		perio->obj = (PyObject*)self;
		
		WSAPROTOCOL_INFOA wsa_protocol_info;
		int wsa_info_len = sizeof(WSAPROTOCOL_INFOA);
		::getsockopt( fd, SOL_SOCKET, SO_PROTOCOL_INFOA,(char*) &wsa_protocol_info, &wsa_info_len );
		addrinfo info = { 0 };
		info.ai_family = wsa_protocol_info.iAddressFamily;
		info.ai_socktype = wsa_protocol_info.iSocketType;
		info.ai_protocol = wsa_protocol_info.iProtocol;

		addrinfo * info2;
		char buf[20];
		_ltoa_s( port, buf,20, 10 );
		::getaddrinfo( server, buf, &info, &info2 );
		if ( 0 == ConnectEx( fd, info2->ai_addr, info2->ai_addrlen,NULL, NULL, NULL, (LPOVERLAPPED)perio ) )
		{
			::freeaddrinfo( info2 );
			if( GetLastError( ) == WSA_IO_PENDING )
			{
				Py_IncRef( perio->obj );
				Py_IncRef( perio->key );
				Py_IncRef( perio->buffer );

				Py_RETURN_NONE;
			}
			PyMem_Free( perio );
			return iocp_error( );
		}
		else
		{
			PyMem_Free( perio );
			::freeaddrinfo( info2 );
			return iocp_error( );
		}

	}
	else
	{
		return iocp_error( );
	}

	return param_error( );
}



static PyMethodDef sock_methods[] = {
	{
		"bind", (PyCFunction)sock_bind, METH_VARARGS, "bind"
	},
	{
		"connect", (PyCFunction)sock_connect, METH_VARARGS, "connect"
	},
	{
		"close", (PyCFunction)sock_close, METH_VARARGS, "close"
	},
	{
		"send", (PyCFunction)sock_send, METH_KEYWORDS, "send"
	},
	{
		"recv", (PyCFunction)sock_recv, METH_KEYWORDS, "recv"
	},
	{
		"accept", (PyCFunction)sock_accept, METH_KEYWORDS, "accept"
	},
	{
		"listen", (PyCFunction)sock_listen, METH_VARARGS, "listen"
	},
	{
		"getsockname", (PyCFunction)sock_getsockname, METH_NOARGS, "getsockname"
	}
	,
	{
		"getpeername", (PyCFunction)sock_getpeername, METH_NOARGS, "getpeername"
	},
	{
		"sendto", (PyCFunction)sock_sendto, METH_KEYWORDS, "sendto"
	},

	{
		0,0,0,0
	}
};

static PyMemberDef sock_memberlist[] = {
	{"fd", T_INT, offsetof(PyIOCPSockObject, sock_fd), READONLY, "the socket handle"},
	{"timeout", T_DOUBLE, offsetof(PyIOCPSockObject, sock_timeout), READONLY, "the socket timeout"},
	{0},
};

static PyTypeObject iocp_sock_type = {
	PyObject_HEAD_INIT(&PyType_Type)	/* Must fill in type value later */
	0,					/* ob_size */
	"pyiocp.socket",			/* tp_name */
	sizeof(PyIOCPSockObject),		/* tp_basicsize */
	0,					/* tp_itemsize */
	(destructor)sock_dealloc,		/* tp_dealloc */
	0,					/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	(reprfunc)sock_repr,			/* tp_repr */
	0,					/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	0,					/* tp_hash */
	0,					/* tp_call */
	0,					/* tp_str */
	PyObject_GenericGetAttr,		/* tp_getattro */
	0,					/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	sock_doc,				/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	sock_methods,				/* tp_methods */
	sock_memberlist,			/* tp_members */
	0,					/* tp_getset */
	0,					/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	sock_initobj,				/* tp_init */
	PyType_GenericAlloc,			/* tp_alloc */
	sock_new,				/* tp_new */
	PyObject_Del,				/* tp_free */
};
	
PyObject * PyIOCPSock_New( int family , int type, int proto )
{
	return PyType_GenericNew( &iocp_sock_type, NULL, NULL );
}

	int PyIOCPSock_Check( PyObject* o )
	{
		return o->ob_type == &iocp_sock_type;
	}
}

