#include "StdAfx.h"

#include <python.h>
#include "IOCP_ERROR.h"
#include "IOCP_BUFFER.h"



namespace pyiocp
{

	static void buffer_dealloc(PyIOCPBufferObject *s)
	{
		PyMem_Free( s->ptr_start );
		s->ob_type->tp_free( ( PyObject * ) s );
	}

	static PyObject *
		buffer_repr(PyIOCPBufferObject *s)
	{
		char buf[512];
		PyOS_snprintf( buf, sizeof(buf), "<iocp buffer object, addr=%x, length=%d >", s->ptr_start, s->len );
		return PyString_FromString(buf);
	}

	static PyObject *
		buffer_str(PyIOCPBufferObject *s)
	{
		return PyString_FromStringAndSize( s->ptr_start, s->len );
	}

	
	static PyObject *
		buffer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
	{
		PyObject *new_buffer;
		new_buffer = type->tp_alloc(type, 0);
		if (new_buffer != NULL) 
		{
			((PyIOCPBufferObject *)new_buffer)->ptr_start = 0;
			((PyIOCPBufferObject *)new_buffer)->len = 0;
			
		}
		return new_buffer;
	}

	static int
		buffer_initobj(PyObject *self, PyObject *args, PyObject *kwds)
	{
		PyIOCPBufferObject *s = (PyIOCPBufferObject *)self;
		PyObject* arg1, *arg2;
		int r = PyArg_ParseTuple( args, "O|O", &arg1, &arg2 );
		if( r == 0) return -1;
		char *init_buffer = 0;
		int init_buffer_len = 0;
		if( PyNumber_Check( arg1 ) )
		{
			int len = PyInt_AsLong( arg1 );
			s->ptr_start = (char *) PyMem_Malloc( len );
			s->len = len;
		
		}

		else if( PyString_Check( arg1 ) )
		{
			PyString_AsStringAndSize( arg1, &init_buffer, &init_buffer_len );
			if( PyNumber_Check( arg2 ) )
			{
				s->ptr_start = (char *) PyMem_Malloc( PyInt_AsLong( arg2 ) );
				s->len = PyInt_AsLong( arg2 );
				
			}
			else
			{
				s->ptr_start = (char *) PyMem_Malloc( init_buffer_len );
				s->len = init_buffer_len;
			}
			if( init_buffer )
				if( 0!= memcpy_s( s->ptr_start, s->len, init_buffer, init_buffer_len ) )
					return -1;

		}
		else
		{
			return -1;
		}

		return 0;

	}

	static PyObject* buffer_set( PyIOCPBufferObject* s, PyObject *args )
	{
		const void * buffer_ptr = 0 ;
		DWORD buffer_len = 0;
		DWORD buffer_set_length = -1;
		PyObject* o = 0;
		int r = PyArg_ParseTuple( args, "O|I", &o, &buffer_set_length );
		if( r == 0 ) return param_error( );
		r = PyObject_AsReadBuffer( o, &buffer_ptr, (Py_ssize_t *) &buffer_len );
		if( r != 0 ) return param_error( );
		if( buffer_set_length < buffer_len )
			buffer_len = buffer_set_length;
		DWORD mov_num = buffer_len;
		if( s->len < mov_num ) mov_num = s->len ;
		memcpy_s( s->ptr_start, s->len, buffer_ptr, mov_num );
		Py_RETURN_NONE;
	}
	
	static PyMethodDef buffer_methods[] = {
		{
			"set", (PyCFunction)buffer_set, METH_VARARGS, "set string into buffer"
		},
		{
			0,0,0,0
		}
	};
	
	static Py_ssize_t
		buffer_getreadbuf(PyIOCPBufferObject *self, Py_ssize_t index, const void **ptr)
	{
		if ( index != 0 ) {
			PyErr_SetString(PyExc_SystemError,
				"accessing non-existent string segment");
			return -1;
		}
		*ptr = (void *)self->ptr_start;
		return self->len;
	}

	static Py_ssize_t
		buffer_getwritebuf(PyIOCPBufferObject *self, Py_ssize_t index, const void **ptr)
	{
		PyErr_SetString(PyExc_TypeError,
			"Cannot use string as modifiable buffer");
		*ptr = (void *)self->ptr_start;
		return self->len;
	}

	static Py_ssize_t
		buffer_getsegcount(PyIOCPBufferObject *self, Py_ssize_t *lenp)
	{
		if ( lenp )
			*lenp = self->len;
		return 1;
	}

	static Py_ssize_t
		buffer_getcharbuf(PyIOCPBufferObject *self, Py_ssize_t index, const char **ptr)
	{
		if ( index != 0 ) {
			PyErr_SetString(PyExc_SystemError,
				"accessing non-existent string segment");
			return -1;
		}
		*ptr = self->ptr_start;
		return self->len;
	}

	static PyBufferProcs buffer_as_buffer = {
		(readbufferproc)buffer_getreadbuf,
		(writebufferproc)buffer_getwritebuf,
		(segcountproc)buffer_getsegcount,
		(charbufferproc)buffer_getcharbuf,
	};
	
	static Py_ssize_t
		buffer_length(PyIOCPBufferObject *a)
	{
		return a->len;
	}

	static PyObject *
		buffer_slice(register PyIOCPBufferObject *a, register Py_ssize_t i, register Py_ssize_t j) 	/* j -- may be negative! */
	
	{
		if (i < 0)
			i = 0;
		if (j < 0)
			j = 0; /* Avoid signed/unsigned bug in next line */
		if ( (DWORD)j > a->len)
			j = a->len;
		if (i == 0 && j == a->len && PyIOCPBuffer_Check( (PyObject*)a ) ) {
			/* It's the same as a */
			Py_INCREF(a);
			return (PyObject *)a;
		}
		if (j < i)
			j = i;
		return PyString_FromStringAndSize(a->ptr_start + i, j-i);
	}
	
	static PyObject * buffer_item(PyIOCPBufferObject *a, register Py_ssize_t i)
	{
		
		PyObject *v;
		if (i < 0 || (DWORD)i >= a->len) {
			PyErr_SetString(PyExc_IndexError, "string index out of range");
			return NULL;
		}
		
		return PyString_FromStringAndSize(a->ptr_start+i, 1);
		return v;
	}

	static PySequenceMethods buffer_as_sequence = {
		(lenfunc)buffer_length, /*sq_length*/
		0,
		0,
		(ssizeargfunc)buffer_item, /*sq_item*/
		(ssizessizeargfunc)buffer_slice, /*sq_slice*/
		0,		/*sq_ass_item*/
		0,		/*sq_ass_slice*/
		0 /*sq_contains*/
	};

	
	

	
	static PyTypeObject iocp_buffer_type = {
		PyObject_HEAD_INIT(&PyType_Type)	/* Must fill in type value later */
		0,					/* ob_size */
		"pyiocp.buffer",			/* tp_name */
		sizeof(PyIOCPBufferObject),		/* tp_basicsize */
		0,					/* tp_itemsize */
		(destructor)buffer_dealloc,		/* tp_dealloc */
		0,					/* tp_print */
		0,					/* tp_getattr */
		0,					/* tp_setattr */
		0,					/* tp_compare */
		(reprfunc)buffer_repr,			/* tp_repr */
		0,					/* tp_as_number */
		&buffer_as_sequence,/* tp_as_sequence */
		0,					/* tp_as_mapping */
		0,					/* tp_hash */
		0,					/* tp_call */
		(reprfunc)buffer_str,					/* tp_str */
		PyObject_GenericGetAttr,		/* tp_getattro */
		0,					/* tp_setattro */
		&buffer_as_buffer,					/* tp_as_buffer */
		Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
		0,				/* tp_doc */
		0,					/* tp_traverse */
		0,					/* tp_clear */
		0,					/* tp_richcompare */
		0,					/* tp_weaklistoffset */
		0,					/* tp_iter */
		0,					/* tp_iternext */
		buffer_methods,				/* tp_methods */
		0,			/* tp_members */
		0,					/* tp_getset */
		0,					/* tp_base */
		0,					/* tp_dict */
		0,					/* tp_descr_get */
		0,					/* tp_descr_set */
		0,					/* tp_dictoffset */
		buffer_initobj,				/* tp_init */
		PyType_GenericAlloc,			/* tp_alloc */
		buffer_new,				/* tp_new */
		PyObject_Del,				/* tp_free */
	};

	void PyIOCPBuffer_AddObject( PyObject* m )
	{
		Py_IncRef((PyObject*)&iocp_buffer_type);
		PyModule_AddObject( m, "buffer", (PyObject *)&iocp_buffer_type ) ;
	}

	int PyIOCPBuffer_Check( PyObject *m )
	{
		return m->ob_type == &iocp_buffer_type;
	}

	PyObject* PyIOCPBuffer_New( LONG size )
	{
		PyObject *new_buffer;
		new_buffer = PyType_GenericAlloc(&iocp_buffer_type, 0);
		if (new_buffer != NULL) 
		{
			((PyIOCPBufferObject *)new_buffer)->ptr_start = ( char *) PyMem_Malloc( (DWORD)size );
			((PyIOCPBufferObject *)new_buffer)->len = (DWORD)size;

		}
		return new_buffer;
	}
}