#pragma once

namespace pyiocp
{
	PyObject* param_error();
	PyObject* iocp_error( int last_error = 0 );
}


