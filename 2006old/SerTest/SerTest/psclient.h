#pragma  once 



#define  ADDM(s)  {#s,s,METH_VARARGS},

static PyObject * ver(PyObject* self, PyObject* args)
{

	return Py_BuildValue("i", 3);
}

static PyMethodDef psclientMethods[] = { 
	ADDM(ver)

	{NULL, NULL}
};

extern "C" __declspec( dllexport )  void initPSClient() { 
	PyObject* m; 
	m = Py_InitModule("PSClient",psclientMethods );
}
