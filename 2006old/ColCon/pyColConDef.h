

static HANDLE gethandle()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

PyObject * settextattr(PyObject* self, PyObject* args) 
{
	UINT n;  
	if (! PyArg_ParseTuple(args, "I", &n))
		return NULL;  
	SetConsoleTextAttribute(gethandle(), n);
	return Py_True;
}

PyObject *  settextgreen(PyObject* self, PyObject* args)
{
	SetConsoleTextAttribute(gethandle(), FOREGROUND_GREEN);
	return Py_True;
}

PyObject *  settextred(PyObject* self, PyObject* args) 
{
	SetConsoleTextAttribute(gethandle(), FOREGROUND_RED);
	return Py_True;
}
PyObject *  settextwhite(PyObject* self, PyObject* args) 
{
	SetConsoleTextAttribute(gethandle(), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
	return Py_True;
}





static PyMethodDef ColConMethods[] = {  
	{"settextattr", settextattr, METH_VARARGS, "settextattr"} ,  
	{
		"settextgreen", settextgreen, METH_VARARGS
	},
	{
		"settextred", settextred, METH_VARARGS
	},
	{
		"settextwhite", settextwhite, METH_VARARGS
	},

	{NULL, NULL}
};



void initColCon() { 
	PyObject* m; 
	m = Py_InitModule("ColCon",ColConMethods );
}