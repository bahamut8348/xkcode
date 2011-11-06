#include "StdAfx.h"
#include <python.h>
#include <structmember.h>
#include "IOCP.h"
#include "IOCP_BUFFER.h"
#include "IOCP_ERROR.h"
#include "IOCP_FILE.h"

namespace pyiocp
{

	
	static void
		file_dealloc(PyIOCPFileObject *s)
	{
		//printf("sock_dealloc called %d\n", s->sock_fd );
		if ( s->fd != -1)
		{
			if( 0 == ::CloseHandle( (HANDLE)s->fd ) )
			{
				printf("file_dealloc error\n");	
			}
			else
			{
				s->fd = 0;
				s->iocp_h = 0;
			}


		}
		Py_DecRef( s->key );
		s->ob_type->tp_free( ( PyObject * ) s );
	}

	static PyObject *
		file_repr(PyIOCPFileObject *s)
	{
		char buf[512];
		PyOS_snprintf( buf, sizeof(buf), "<iocp file object, handle=%ld, iocp=%d >", (long)s->fd, s->iocp_h );
		return PyString_FromString(buf);
	}

	static PyObject *
		file_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyObject *new_file;

		new_file = type->tp_alloc(type, 0);
		if (new_file != NULL) 
		{
			((PyIOCPFileObject *)new_file)->fd = -1;
			((PyIOCPFileObject *)new_file)->iocp_h = 0;

		}
		return new_file;
	}

	static PyMemberDef file_memberlist[] = {
		{"fd", T_INT, offsetof(PyIOCPFileObject, fd), READONLY, "the file handle"},
		{0},
	};
	PyObject* file_close( PyIOCPFileObject* self, PyObject *args, PyObject *kwds )
	{
		if(!::CloseHandle( (HANDLE)self->fd ) )
		{
			return iocp_error( );
		}
		self->fd = -1;
		Py_RETURN_NONE;
	}

	PyObject* file_write( PyIOCPFileObject* self, PyObject *args, PyObject *kwds )
	{
		DWORD len = 0;
		LONGLONG start = 0;
		PyObject* key = Py_None;
		PyObject* buffer = 0;
		
		static char *kwlist[] = { "buf","start", "key","len", 0 };
		int r = PyArg_ParseTupleAndKeywords( args, kwds, "O|KOO", kwlist, &buffer, &start, &key, &len );
		if( r == 0 ) return param_error( );

		const void *input_buffer_ptr = 0;
		DWORD input_buffer_len = 0;

		r = PyObject_AsReadBuffer( buffer, &input_buffer_ptr, (Py_ssize_t*)&input_buffer_len );
		if( r != 0 )
		{
			return param_error( );
		}

		if( len == 0 )
		{
			len = input_buffer_len;
		}

		

		if( PyIOCPBuffer_Check( buffer ) == 0 )
		{
			
			char *buffer_ptr = 0;
			DWORD buffer_len = 0;
			buffer = PyIOCPBuffer_New( len );
			r = PyObject_AsWriteBuffer( buffer, (void **)&buffer_ptr, (Py_ssize_t*)&buffer_len );
			if( r ) return param_error( );
			memcpy_s( buffer_ptr, buffer_len,input_buffer_ptr, len );

			input_buffer_ptr = buffer_ptr;
			input_buffer_len = buffer_len;
		}
						
		IocpPerio* perio = (IocpPerio*) PyMem_Malloc( sizeof( IocpPerio) );
		memset( perio, 0, sizeof( IocpPerio ) );
		perio->Offset =  (DWORD)start & 0xffffffff;
		perio->OffsetHigh = (DWORD)(start >> 32);
		perio->buffer = buffer;
		perio->obj = (PyObject*)self;
		perio->type = _IO_WRITE;
		perio->key = key;

		int ret = ::WriteFile( (HANDLE) self->fd, input_buffer_ptr, len, NULL, perio );
		if( ret !=0 )
		{
			Py_IncRef( perio->buffer );
			Py_IncRef( perio->obj );
			Py_IncRef( perio->key );
			Py_RETURN_NONE;
		}
		else
		{
			int err = GetLastError( );
			if ( err == ERROR_IO_PENDING )
			{
				Py_IncRef( perio->buffer );
				Py_IncRef( perio->obj );
				Py_IncRef( perio->key );

				Py_RETURN_NONE;
			}
			PyMem_Free( perio );
			return iocp_error( );
		}
	}


	PyObject* file_read( PyIOCPFileObject* self, PyObject *args, PyObject *kwds )
	{
		DWORD len = 0;
		LONGLONG start = 0;
		PyObject* key = Py_None;
		PyObject* buffer = 0;
		static char *kwlist[] = { "len","start", "key","buf", 0 };
		int r = PyArg_ParseTupleAndKeywords( args, kwds, "I|KOO", kwlist, &len, &start, &key, &buffer );
		if( r == 0 ) return param_error( );

		if( buffer == 0 )
		{
			buffer = PyIOCPBuffer_New( len );
		}
		else
		{
			 if( PyIOCPBuffer_Check( buffer ) == 0 )
				return param_error( );
		}

		char *buffer_ptr = 0;
		DWORD buffer_len = 0;

		r = PyObject_AsWriteBuffer( buffer, (void **)&buffer_ptr, (Py_ssize_t*)&buffer_len);
		if( r ) return param_error( );

		IocpPerio* perio = (IocpPerio*) PyMem_Malloc( sizeof( IocpPerio) );
		memset( perio, 0, sizeof( IocpPerio ) );
		perio->Offset =  (DWORD)start & 0xffffffff;
		perio->OffsetHigh = (DWORD)(start >> 32);
		perio->buffer = buffer;
		perio->obj = (PyObject*)self;
		perio->type = _IO_READ;
		perio->key = key;

		int ret = ::ReadFile( (HANDLE) self->fd, buffer_ptr, buffer_len, NULL, perio );
		if ( ret != 0 )
		{
			Py_IncRef( perio->buffer );
			Py_IncRef( perio->obj );
			Py_IncRef( perio->key );
			Py_RETURN_NONE;
		}
		else
		{
			int err = GetLastError( );
			if ( err == ERROR_IO_PENDING )
			{
				Py_IncRef( perio->buffer );
				Py_IncRef( perio->obj );
				Py_IncRef( perio->key );

				Py_RETURN_NONE;
			}
			PyMem_Free( perio );
			return iocp_error( );
		}

	}

	static PyMethodDef file_methods[] = {
		{
			"read", (PyCFunction)file_read, METH_KEYWORDS, "read"
		},
		{
			"write", (PyCFunction)file_write, METH_KEYWORDS, "write"
			},
			{
				"close", (PyCFunction)file_close, METH_KEYWORDS, "close"
			},

		{
			0,0,0,0
		}
	};

	PyDoc_STRVAR(file_doc, "iocp file from pyiocp.file" );
	static PyTypeObject iocp_file_type = {
		PyObject_HEAD_INIT(&PyType_Type)	/* Must fill in type value later */
		0,					/* ob_size */
		"pyiocp.socket",			/* tp_name */
		sizeof(PyIOCPFileObject),		/* tp_basicsize */
		0,					/* tp_itemsize */
		(destructor)file_dealloc,		/* tp_dealloc */
		0,					/* tp_print */
		0,					/* tp_getattr */
		0,					/* tp_setattr */
		0,					/* tp_compare */
		(reprfunc)file_repr,			/* tp_repr */
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
		file_doc,				/* tp_doc */
		0,					/* tp_traverse */
		0,					/* tp_clear */
		0,					/* tp_richcompare */
		0,					/* tp_weaklistoffset */
		0,					/* tp_iter */
		0,					/* tp_iternext */
		file_methods,		/* tp_methods */
		file_memberlist,	/* tp_members */
		0,					/* tp_getset */
		0,					/* tp_base */
		0,					/* tp_dict */
		0,					/* tp_descr_get */
		0,					/* tp_descr_set */
		0,					/* tp_dictoffset */
		0,				/* tp_init */
		PyType_GenericAlloc,			/* tp_alloc */
		0,				/* tp_new */
		PyObject_Del,				/* tp_free */
	};

	PyObject* PyIOCPFile_New( const char* filename, const char* mode , PyObject* key ,  int iocp_h )
	{
		DWORD open_mode = GENERIC_READ;
		DWORD create_mode = OPEN_EXISTING;
		if( strchr( mode, 'w' ) || strchr( mode , 'a' ) )
		{
			open_mode = GENERIC_WRITE;
			create_mode = OPEN_ALWAYS;
		}
		HANDLE f = ::CreateFileA( filename, open_mode, FILE_SHARE_READ, NULL, create_mode, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,  NULL );
		if( f == INVALID_HANDLE_VALUE ) return NULL;
		PyIOCPFileObject *o =( PyIOCPFileObject* ) PyType_GenericNew( &iocp_file_type, 0, 0 );
		o->fd = (int) f;
		o->iocp_h = iocp_h;
		o->key = key;
		Py_IncRef( key );
		return ( PyObject* ) o;
	}

}

