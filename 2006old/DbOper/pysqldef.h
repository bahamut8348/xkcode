

#pragma  once



static _ConnectionPtr pconn;
static _RecordsetPtr prs;

static _bstr_t errormsg;


static PyObject *getuchar(const wchar_t *s)
{
	return PyUnicode_FromWideChar(s,lstrlenW(s));
}

static PyObject* geterror(PyObject *self, PyObject *args)
{
	return getuchar(errormsg);
}

static PyObject* conndb(PyObject *self, PyObject *args)
{
	USES_CONVERSION;
	const Py_UNICODE *server;
	const Py_UNICODE *user, *pass, *dbname;
	if (!PyArg_ParseTuple(args, "uuuu", &server, &dbname, &user, &pass))
		return Py_False;

	TCHAR connstr[1024]={0};
	wsprintf(connstr,L"Driver=SQL Server;Server=%s;Database=%s;UID=%s;PWD=%s;connect timeout=15;",
		server, dbname, user, pass);

	_bstr_t str = connstr;

	pconn.CreateInstance(L"Adodb.Connection");
	prs.CreateInstance("Adodb.Recordset");

	try
	{
		if(FAILED( pconn->Open(str, L"",L"",adConnectUnspecified)))
		{
			return Py_False;
		}
	}
	catch (_com_error  & e)
	{
		errormsg = e.Description();
		return Py_BuildValue("i",0);
	}
	
	return Py_True;
}

static PyObject* test(PyObject *self, PyObject *args)
{
	const wchar_t *sql;
	if (!PyArg_ParseTuple(args, "u", &sql))
		return Py_False;
	return getuchar(sql);
}

static PyObject* uexecsql(PyObject *self, PyObject *args)
{
	Py_UNICODE *sql;
	if (!PyArg_ParseTuple(args, "u", &sql))
		return Py_False;
	static int ret = 0;
	try
	{
		pconn->Execute((wchar_t *)sql,NULL,adExecuteNoRecords);
		ret = 1;
	}
	catch (_com_error  & e)
	{
		ret = 0;
		errormsg = e.Description();
	}

	return PyBool_FromLong(ret);

}


static PyObject* execsql(PyObject *self, PyObject *args)
{
	
	const char *sql;
	
	if (!PyArg_ParseTuple(args, "s", &sql))
		return Py_False;

	

	USES_CONVERSION;

	PyObject * usql = getuchar(A2W(sql));
	
	return uexecsql( self, Py_BuildValue("(O)",usql));


}



static  PyObject* uquery(PyObject *self, PyObject *args)
{
	const Py_UNICODE *sql;
	if (!PyArg_ParseTuple(args, "u", &sql))
		return Py_False;
	
	USES_CONVERSION;
	try
	{
		prs->Open((wchar_t *)sql,_variant_t( (IDispatch *)pconn ,true), adOpenStatic ,adLockOptimistic,adCmdText);
		LONG count = prs->GetRecordCount();
		int itemcount = prs->Fields->Count;
		PyObject *list =  PyList_New(count);
		PyObject *desplist = PyList_New(itemcount);
		

		// desplist
		for(long idx=0; idx< itemcount; idx++)
		{
			_bstr_t val = prs->Fields->Item[idx]->Name;
			PyList_SetItem(desplist, idx, Py_BuildValue("u", (wchar_t *)val) );
		}
		

		int a=0;
		while(!prs->adoEOF)
		{
			
			PyObject *sublist =  PyList_New(itemcount);
			PyList_SetItem(list, a++, sublist);
			for(int idx=0; idx< itemcount; idx++)
			{
				_variant_t val = prs->GetCollect(_variant_t( (long)idx ));
				DWORD type = val.vt;
				switch(type)
				{
				case VT_I1:
				case VT_I2:
				case VT_I4:
				case VT_I8:
				case VT_INT:
				case VT_BOOL:
					PyList_SetItem(sublist, idx, Py_BuildValue("L", val.llVal ) );
					break;
				case VT_UI1:
				case VT_UI2:
				case VT_UI4:
				case VT_UI8:
				case VT_UINT:
					PyList_SetItem(sublist, idx, Py_BuildValue("K", val.ullVal ) );
					break;

				case VT_BSTR:
					PyList_SetItem(sublist, idx, getuchar((_bstr_t)val));
					break;
				case VT_DATE:
					{
						COleDateTime date(val);
						SYSTEMTIME st;
						date.GetAsSystemTime(st);
						PyObject *d = PyDateTime_FromDateAndTime(st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute,st.wSecond,
							st.wMilliseconds);
						PyList_SetItem(sublist, idx, d);


					}
					break;
				case VT_NULL:
					PyList_SetItem(sublist, idx, Py_BuildValue("s",""));
					break;
				default:
					PyList_SetItem(sublist, idx, Py_BuildValue("s", W2A((TCHAR *)(_bstr_t)val)));
					break;
				}
			}
			prs->MoveNext();

		}
		return Py_BuildValue("(O,O)", desplist,list );

	}
	catch (_com_error  & e)
	{
		errormsg = e.Description();
	}
	return Py_False;
}


static PyObject* query(PyObject *self, PyObject *args)
{
	const char *sql;
	if (!PyArg_ParseTuple(args, "s", &sql))
		return Py_False;
	USES_CONVERSION;
	PyObject * usql = PyUnicode_FromWideChar(A2W(sql), (int)strlen(sql)*sizeof(wchar_t));
	return uquery( self, Py_BuildValue("(O)",usql));
}


static PyObject* md5(PyObject *self, PyObject *args)
{
	const Py_UNICODE *s;
	char ret[1024] = {0};
	
	USES_CONVERSION;
	
	if (!PyArg_ParseTuple(args, "u", &s))
		return Py_BuildValue("s", "" );

	int len = lstrlenW((wchar_t*)s);
	
	Oword pass;
	Encryption::MD5Checksum( (void *)s, len*2, pass);

	for(u_int a=0; a<sizeof(pass);a++)
	{
		//printf("%s\r\n",ret);
		sprintf(ret+(a*2), "%02x", *(((u_char *)&pass)+a) );
	}


	return Py_BuildValue("s", ret);

}




static PyMethodDef sqlmethod[] = {
	{"exec", execsql, METH_VARARGS},
	{"md5", md5, METH_VARARGS},
	{"conndb", conndb, METH_VARARGS},
	{"geterror", geterror, METH_VARARGS},
	{
		"test",test,METH_VARARGS
	},
	{
		"query",query,METH_VARARGS
	},
	{
		"uquery",uquery,METH_VARARGS
	},
	{"uexec", uexecsql, METH_VARARGS},


	{NULL,    NULL, NULL, NULL }
};
