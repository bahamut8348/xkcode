#include "StdAfx.h"

#include <WinSock2.h>
#include <Mswsock.h>
#include <WS2tcpip.h>
#include <Python.h>
#include <bufferobject.h>
#include <structmember.h>
#include "IOCP_ERROR.h"
#include "IOCP_FILE.h"
#include "IOCP_BUFFER.h"
#include "IOCP_SOCKET.h"

#include "IOCP.h"

namespace pyiocp
{

extern PyObject *g_iocp_error;



typedef struct {
	PyObject_HEAD
	HANDLE iocp_h;	/* Socket file descriptor */
} PyIOCPObject;



//static PyTypeObject iocp_type;

/* Type object for socket objects. */

static void
iocp_dealloc(PyIOCPObject *s)
{
	if (s->iocp_h != 0 )
		(void) ::CloseHandle( s->iocp_h );
	s->iocp_h = 0;
	s->ob_type->tp_free((PyObject *)s);
}

static PyObject *
iocp_repr(PyIOCPObject *s)
{
	char buf[512];
	PyOS_snprintf(
		buf, sizeof(buf),
		"<iocp object, handle=%d>",
		s->iocp_h );
	return PyString_FromString(buf);
}

static PyObject *
iocp_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyObject *new_iocp;
	new_iocp = type->tp_alloc( type, 0 );
	if (new_iocp != NULL) {
		((PyIOCPObject *)new_iocp)->iocp_h = 0;
	}
	return new_iocp;
}

PyDoc_STRVAR(iocp_doc, "win32 iocp wrapper." );

PyObject* iocp_add( PyObject* self, PyObject *args )
{
	PyIOCPObject *s = (PyIOCPObject *)self;
	int fd;
	PyObject* key = NULL;
	int r = PyArg_ParseTuple( args, "i|O", &fd, &key );
	if( r == 0 )
		return param_error( );
	if( key )
		Py_INCREF( key );
	HANDLE ret_handle = ::CreateIoCompletionPort(  (HANDLE)fd, s->iocp_h, (DWORD_PTR) key, 1 );
	if( ret_handle > 0 )
	{
		Py_RETURN_NONE;
	}
	else
	{
		return iocp_error( );
	}
	
}

PyObject* iocp_file(  PyIOCPObject *s, PyObject *args, PyObject* kwds )
{
	static char *kwlist[] = {"name", "mode","key" , 0};
	char *name, *mode="r";
	PyObject* key = Py_None;
	int r = PyArg_ParseTupleAndKeywords( args, kwds, "s|sO", kwlist, &name, &mode, &key );
	if( r ==0 ) return param_error( );
	
	PyIOCPFileObject* f = (PyIOCPFileObject*) PyIOCPFile_New( name, mode, key , (int)s->iocp_h );
	if( f == NULL ) return iocp_error( );

	HANDLE ret_h = ::CreateIoCompletionPort( (HANDLE) f->fd, s->iocp_h, (ULONG_PTR)key, 0 );
	if( ret_h == INVALID_HANDLE_VALUE )
	{
		PyObject_DEL( f );
		return iocp_error( );
	}
	Py_INCREF( key );
	return (PyObject*)f;
}

PyObject* iocp_socket(  PyIOCPObject *s, PyObject *args, PyObject* kwds )
{
	static char *kwlist[] = {"family", "type", "proto","key", 0};
	int af = AF_INET;
	int type = SOCK_STREAM;
	int proto = 0;
	PyObject* per_key = Py_None;
	int r = PyArg_ParseTupleAndKeywords( args, kwds, "|iiiO", 	kwlist, &af, &type, &proto,&per_key );
	if( r == 0 ) return 0;

	PyIOCPSockObject* ret_s = ( PyIOCPSockObject* ) PyIOCPSock_New( af, type, proto );
	
	ret_s->sock_fd = ::socket( af, type, proto ) ;
	if( ret_s->sock_fd < 0 ) return iocp_error( );
	ret_s->iocp_h =(int) s->iocp_h;
	HANDLE ret_h = ::CreateIoCompletionPort( (HANDLE) ret_s->sock_fd, s->iocp_h, (ULONG_PTR)per_key, 0 );

	if( ret_h == INVALID_HANDLE_VALUE ){
		PyObject_Del( ret_s );
		return iocp_error( );
	}
	Py_INCREF( per_key );
	ret_s->key = per_key ;
	return (PyObject*)ret_s;
}

PyObject* iocp_get( PyObject* self, PyObject *args )
{
	PyIOCPObject *s = (PyIOCPObject *)self;
	int timeout = 200;
	int r = PyArg_ParseTuple( args, "|i", &timeout );
	
	if( r )
	{
		DWORD trans_len = 0;
		PyObject* key = 0;
		IocpPerio *perio = 0 ;
		int ret = GetQueuedCompletionStatus( s->iocp_h, &trans_len, (PULONG) &key,(LPOVERLAPPED *) &perio, timeout );
		int last_err = 0;
		if( ret == 0 )
		{
			last_err = GetLastError( );
		}
		
		if( key == NULL )
		{
			Py_INCREF(Py_None);
			key = Py_None;
		}

		PyObject* perio_key = Py_None;
		PyObject* perio_buffer = Py_None;
		PyObject *file_h = Py_None;
		
		if( perio )
		{
			perio_key = perio->key;
			perio_buffer = perio->buffer;
			file_h = perio->obj;

		}
	
		PyObject* ret_obj = Py_BuildValue( "OIOOOi", file_h, trans_len, key, perio_key, perio_buffer, last_err );

		if( perio )
		{

			Py_DECREF( file_h );
			Py_DECREF( perio_key );
			Py_DECREF( perio_buffer );
		}

		PyMem_Free( perio );
		
		return ret_obj;
	}
	return param_error( );
}

static PyMethodDef iocp_methods[] = {
	{"add",	  (PyCFunction)iocp_add, METH_VARARGS, 0 },
	{"get",	  (PyCFunction)iocp_get, METH_VARARGS, "return filehandle, translen, key, perio_key, perio_buffer, err" },
//	{"read",	  (PyCFunction)iocp_read, METH_VARARGS, 0 },
	{"socket", (PyCFunction)iocp_socket, METH_KEYWORDS, 0 },
	{"file",   (PyCFunction)iocp_file, METH_KEYWORDS, 0 },
	{ NULL, NULL, NULL, NULL }
};


static PyMemberDef iocp_memberlist[] = {
	{"handle", T_INT, offsetof(PyIOCPObject, iocp_h ), READONLY, "iocp handle"},
	{0},
	};

static int
iocp_initobj(PyObject *self, PyObject *args, PyObject *kwds)
{
	PyIOCPObject *s = (PyIOCPObject *)self;
	
	Py_BEGIN_ALLOW_THREADS
		s->iocp_h = ::CreateIoCompletionPort( (HANDLE)-1, (HANDLE)0, 0, 1 ); 
	Py_END_ALLOW_THREADS
		if( s->iocp_h <= 0 )
		{
			iocp_error( );
			return -1;
		}
		return 0;

}



static PyTypeObject iocp_type = {
	PyObject_HEAD_INIT(0)	/* Must fill in type value later */
	0,					/* ob_size */
	"pyiocp.iocp",			/* tp_name */
	sizeof(PyIOCPObject),		/* tp_basicsize */
	0,					/* tp_itemsize */
	(destructor)iocp_dealloc,		/* tp_dealloc */
	0,					/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	(reprfunc)iocp_repr,			/* tp_repr */
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
	iocp_doc,				/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	iocp_methods,				/* tp_methods */
	iocp_memberlist,			/* tp_members */
	0,					/* tp_getset */
	0,					/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	iocp_initobj,				/* tp_init */
	PyType_GenericAlloc,			/* tp_alloc */
	iocp_new,				/* tp_new */
	PyObject_Del,				/* tp_free */
};









PyObject* create_buffer( PyObject* self, PyObject *args )
{
	return PyBuffer_New( PyInt_AS_LONG(args ) );
}

PyObject* create_iocp( PyObject* self, PyObject *args )
{
	int iocp_handle = -1;
	int fd_handle = -1 ;
	PyObject* handle_key = 0 ;
	int argc = PyTuple_GET_SIZE( args );
	int r = PyArg_ParseTuple( args, "|iiO", &iocp_handle, &fd_handle, &handle_key );
	if( r > 0 )
	{	
		if( argc == 0 )
		{
			if( argc == 3 )
				Py_INCREF( handle_key );
			int ret_handle = (int)::CreateIoCompletionPort( (HANDLE)iocp_handle, (HANDLE)fd_handle, (DWORD)handle_key , 1 );
			if( ret_handle > 0 )
			{
				return PyInt_FromLong( ret_handle );
			}
			else
			{
				return iocp_error();
			}
		}	
	}
	return param_error( );
}

PyObject* reopen( PyObject* self, PyObject *args )
{
	int fd = 0;
	int r = PyArg_ParseTuple( args, "i", &fd );
	if( r )
	{
		HANDLE h =  ::ReOpenFile( (HANDLE) fd, GENERIC_READ, FILE_SHARE_READ, FILE_FLAG_OVERLAPPED );
		if( h == INVALID_HANDLE_VALUE )
		{
			return iocp_error( );
		}
		return PyInt_FromLong( (int)h );
	}
	return param_error( );
}


PyObject* connect(  PyObject* self, PyObject *args )
{
	int fd = 0;
	char *server;
	int port;
	PyObject* pyaddr;
	PyObject* perio_key = 0;
	int r = PyArg_ParseTuple( args, "iO|O", &fd, &pyaddr, &perio_key);
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
		perio->obj  = self ;
		if( perio_key )
			Py_IncRef( perio_key );
		perio->key = perio_key;
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

PyObject* bind( PyObject* self, PyObject *args )
{
	int fd;
	PyObject* server_t = 0;
	char *server;
	int port;
	int r = PyArg_ParseTuple( args, "i|O", &fd, &server_t );
	if( r )
	{
		r = PyArg_ParseTuple( server_t, "si", &server, &port);
		if( r )
		{
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
	}
	return param_error( );
}



PyObject* iocp_read( PyObject* self, PyObject *args )
{
	int fd = 0;
	int read_len = 0;

	PyObject* perio_key = 0;
	IocpPerio* perio;
	int r = PyArg_ParseTuple( args, "ii|O", &fd, &read_len, &perio_key );
	if( r )
	{
		PyObject* pybuf = PyBuffer_New( read_len );
		char *data = 0;
		int data_len = 0;
		int ret = PyObject_AsWriteBuffer( pybuf, (void **)&data, &data_len );
		perio = ( IocpPerio* )PyMem_Malloc( sizeof(IocpPerio) );
		memset( perio , 0, sizeof(IocpPerio ) );
		ret = ::ReadFile( ( HANDLE )fd, data, data_len, NULL, perio );
		if( ret == 0 )
		{
			if(  GetLastError( ) == ERROR_IO_PENDING )
				Py_RETURN_NONE;
			else
				return iocp_error( );
		}
		
		if( ret != 0 )
		{
			if( ERROR_SUCCESS != GetLastError( ) )
				return iocp_error( ); 
				//	PyIOCPObject *s = (PyIOCPObject *)self;
		//	DWORD trans_len;
		//	if( 0 == ::GetOverlappedResult( fd, perio, trans_len, FALSE ) )
		//	{
		//		Py_RETURN_NONE;
		//	}
		//	if( 0 == ::PostQueuedCompletionStatus( s->iocp_h, trans_len, 
			Py_RETURN_NONE;
		}
	}
	return param_error( ); 
}

PyObject* close( PyObject* self, PyObject *args )
{
	int fd = 0;
	int r = PyArg_ParseTuple( args, "i", &fd );
	if( r )
	{
//		if( 0 != ::closesocket( fd ) )
		{
//			return iocp_error( WSAGetLastError( ) );
		}
		if( 0 == ::CloseHandle( ( HANDLE ) fd ) )
		{
			return iocp_error( );
		}
		
		Py_RETURN_NONE;
	}
	return param_error( );
}


PyObject* addrfrombuffer( PyObject* self, PyObject* args )
{
	const void * buf_ptr = 0;
	int buf_len;
	if( 0!= PyObject_AsReadBuffer( args, &buf_ptr, &buf_len ) )
	{
		return param_error( ); 
	}
	
	int local_addr_len = 32;
	int remote_addr_len = 32;

	SOCKADDR_IN* local_addr;
	SOCKADDR_IN* remote_addr;
	
	DWORD addr_len = 32;
	::GetAcceptExSockaddrs( const_cast<void*>(buf_ptr), 0, addr_len, addr_len, (sockaddr**)&local_addr, &local_addr_len, (sockaddr**)&remote_addr, &remote_addr_len );
	
	char s_remote[20], s_local[20];
	::strcpy_s( s_local, inet_ntoa( local_addr->sin_addr ) );
	::strcpy_s( s_remote, inet_ntoa( remote_addr->sin_addr ) );

	PyObject* ret = Py_BuildValue( "((si)(si))", s_local, ntohs( local_addr->sin_port ), s_remote, ntohs( remote_addr->sin_port )  );
	return ret;
}


PyObject* send( PyObject* self, PyObject *args )
{
	int fd = 0;
	int flag = 0;
	PyObject* buffer = 0;
	PyObject* perio_key = 0;
	int r = PyArg_ParseTuple( args, "iO|iO", &fd, &buffer, &perio_key, &flag );
	if( r )
	{
		if( PyBuffer_Check(buffer) == 0 )
		{
			if( PyString_Check( buffer ) == 0 )
			{
				return param_error( );
			}
			else
			{
				int str_len;
				char *str_ptr =0;
				PyString_AsStringAndSize( buffer, &str_ptr, &str_len );
				PyObject* new_buffer = PyBuffer_New( str_len );
				void *new_buffer_ptr;
				int new_buffer_len;
				PyObject_AsWriteBuffer( new_buffer, &new_buffer_ptr, &new_buffer_len );
				memcpy_s( new_buffer_ptr,new_buffer_len, str_ptr, str_len );
				buffer = new_buffer;
			}
		}
		else
		{
			// pass
		}

		char * buffer_ptr = 0;
		int buffer_len = 0;
		if( 0 != PyObject_AsReadBuffer( buffer, (const void **)&buffer_ptr, &buffer_len) )
			return param_error( );


		if( perio_key ) Py_INCREF( perio_key );
		IocpPerio* perio = (IocpPerio*)PyMem_Malloc( sizeof(IocpPerio) );
		memset( perio, 0, sizeof( IocpPerio ) );

		perio->type = _IO_SEND;
		perio->key = perio_key;
		
		WSABUF DataBuf;
		DataBuf.buf = buffer_ptr;
		DataBuf.len = buffer_len;

		int ret = ::WSASend( fd, &DataBuf, 1, NULL, 1, perio, NULL );
		if( ret == 0 )
		{
			return iocp_error( );
		}
		else
		{
			if ( GetLastError( ) != WSA_IO_PENDING )
			{
				return iocp_error ( );
			}
			else
			{
				Py_RETURN_NONE;
			}
		}
		
	}
	return param_error( );

}


static PyMethodDef iocpMethods[] = {
//	{"buffer",  create_buffer, METH_O, "Create a buffer"},
	{"addrfrombuf",  addrfrombuffer , METH_O, "Create a buffer"},
	{"create",  create_iocp, METH_VARARGS, "Create a iocp handle"},
	{
		"reopen", reopen, METH_VARARGS, "Reopen a file handle"
	},
	
		{
			"connect", connect, METH_VARARGS, "Async connect"
		},
		{
			"send", send, METH_VARARGS, "Async send"
		},
		{
		"close", close, METH_VARARGS, "Async close"
		},
	{
		"bind", bind, METH_VARARGS, "Create socket"
	},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};


PyMODINIT_FUNC
initpyiocp(void)
{
	iocp_type.ob_type = &PyType_Type;
	g_iocp_error =  PyErr_NewException("pyiocp.error", NULL, NULL);
	Py_INCREF(g_iocp_error);
	PyObject* m= Py_InitModule("pyiocp", iocpMethods);
	if (m == NULL)
		return;

	Py_INCREF( &iocp_type );
	if (PyModule_AddObject( m, "iocp", (PyObject *)&iocp_type )  != 0)
		return;
	
	PyIOCPBuffer_AddObject( m );

}



}