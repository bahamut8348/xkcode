#include "StdAfx.h"
#include "pycfg.h"


using namespace pycfg;

CFG::CFG(void)
{
	Py_Initialize( );

	mMod = PyImport_ImportModule( "server_cfg" );
	Py_IncRef( mMod );
	
	PyObject* dic = PyModule_GetDict( mMod );

	mGetUserAddr = PyDict_GetItem( dic, PyString_FromString( "GetUserAddr" ) );
	Py_IncRef( mGetUserAddr );

	mGetUserGroup = PyDict_GetItem( dic, PyString_FromString( "GetUserGroup" ) );
	Py_IncRef( mGetUserGroup );

	mServerPort = PyDict_GetItem( dic, PyString_FromString( "ServerPort" ) );
	Py_IncRef( mServerPort );
}

CFG::~CFG(void)
{
	Py_DecRef( mServerPort );
	Py_DecRef( mGetUserGroup );
	Py_DecRef( mGetUserAddr );
	Py_DecRef( mMod );
	Py_Finalize( );
}

std::wstring pycfg::CFG::getUserAddr( char * userkey, int keylen )
{
	PyObject* r = PyObject_CallFunction( mGetUserAddr, "w#", userkey, keylen );
	if( r == 0 )
	{
		return L"";
	}
	USES_CONVERSION;
	return A2W( PyString_AS_STRING(r) );
}

int pycfg::CFG::getServerPort()
{
	return PyInt_AS_LONG( mServerPort );
}