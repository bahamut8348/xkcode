// DbOper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void usage(const char *fn)
{
	printf("\nUsage: \n\t%s <configfile>\n", fn
		);
}

int _tmain(int argc, _TCHAR* argv[])
{
	Py_Initialize( );
	CoInitialize(NULL);
	
	USES_CONVERSION;
	PyDateTime_IMPORT;
	

	char * file= "run.py";
	if(argc == 2)
	{
		file = W2A(argv[1]);
	}
	else
	{
		usage(W2A(argv[0]));
		exit(-1);
	}


	Py_InitModule("sql",sqlmethod);

	
	PyObject *pyFile = PyFile_FromString(file,"r");
	if(pyFile == NULL)
	{
		printf("%s does't exist\nAbort\n",file);
		return -1;
	}
	
	FILE *f = PyFile_AsFile(pyFile);
	PyRun_AnyFile(f,file);



	Py_Finalize();
	
	return 0;
}

