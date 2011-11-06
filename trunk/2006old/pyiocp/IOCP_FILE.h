#pragma once

namespace pyiocp
{
	typedef struct {
		PyObject_HEAD
		int fd;				/* Socket file descriptor */
		int iocp_h;
		PyObject* key;
	} PyIOCPFileObject;

	PyObject* PyIOCPFile_New( const char* filename, const char* mode , PyObject* key, int iocp_h );
	

}