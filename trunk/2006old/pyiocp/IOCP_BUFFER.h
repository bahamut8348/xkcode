#pragma once

namespace pyiocp
{
	typedef struct {
		PyObject_HEAD;
		char *ptr_start;
		DWORD  len;
	} PyIOCPBufferObject;
	

	void PyIOCPBuffer_AddObject( PyObject* m );
	int PyIOCPBuffer_Check( PyObject *m );
	PyObject* PyIOCPBuffer_New( LONG size );

}
