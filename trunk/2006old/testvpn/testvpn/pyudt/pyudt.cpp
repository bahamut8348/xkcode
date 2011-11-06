// pyudt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include "../common/Common.h"
#include <boost/thread/detail/singleton.hpp>
#include <boost/pool/object_pool.hpp>
#include "../testvpn/UDTManager.h"
#include <Python.h>
#include <boost/python.hpp>
using namespace boost::python;

using namespace UDT_Wrapper;

static std::string get_error()
{
	char buf[1024], *buf_p = buf;
	PyObject *type_obj, *value_obj, *traceback_obj;
	PyErr_Fetch(&type_obj, &value_obj, &traceback_obj);
	if (value_obj == NULL)
		return FALSE;

	PyObject *oldobj = value_obj ;
	value_obj = PyObject_Str(oldobj);

	if (!PyString_Check(value_obj))
		return FALSE;

	char *value = PyString_AsString(value_obj);
	std::string s = value;
	Py_XDECREF( oldobj );
	Py_XDECREF(type_obj);
	Py_XDECREF(value_obj);
	Py_XDECREF(traceback_obj);
	return s;
}


class Event : public ISocketEvent
{
	 void OnDataRecv( UDTSOCKET sock,  LPCSTR data, DWORD length )
	 {
		 PyObject* handler = (PyObject*) SocketManager::Instance()->GetSocketUserData( sock );
		
		 try
		 {
			  call_method<void>( handler, "on_data", sock, str( data, length )  );
		 }
		 catch (...)
		 {
			 PyErr_SetString(PyExc_Exception, get_error().c_str() );
			 throw_error_already_set();
		 }
	 }
	 void OnConnect(UDTSOCKET sock,  bool bSuccess )
	 {

	 }
	 void OnDisconnect( UDTSOCKET sock )
	 {
		 PyObject* handler = (PyObject*) SocketManager::Instance()->GetSocketUserData( sock );
		 try
		 {
			 call_method<void>( handler, "on_error", sock );
		 }
		 catch (...)
		 {
			 PyErr_SetString(PyExc_Exception, get_error().c_str() );
			 throw_error_already_set();
		 }
	 }
	 void OnAccept(UDTSOCKET sock,  UDTSOCKET  newsock )
	 {
		 PyObject* handler = (PyObject*) SocketManager::Instance()->GetSocketUserData( sock );
		 try
		 {
			 call_method<void>( handler, "on_accept", sock, newsock  );
		 }
		 catch (...)
		 {
			 PyErr_SetString(PyExc_Exception,  get_error().c_str() );
			 throw_error_already_set();
		 }
		 
		
	 }
};

static Event g_handler;


static int CreateClient( const std::wstring & server, int port, PyObject* handler  )
{
	stAddr addr;
	addr.mIP.fromString( server.c_str() );
	addr.mPort = port;
	
	int sock = SocketManager::Instance()->CreateClientSocket( addr, & g_handler );
	if ( sock == UDT::INVALID_SOCK )
	{
		PyErr_SetString(PyExc_Exception, UDT::getlasterror( ).getErrorMessage( ) );
		throw_error_already_set();
	}
	SocketManager::Instance()->SetSocketUserData( sock,( DWORD ) handler );
	xincref( handler );
	return sock;
}

static int CreateServer( int port , PyObject * handler, const std::wstring host = L"" )
{
	stAddr addr;
	addr.mIP.fromString( host.c_str( ) );
	addr.mPort = port;
	int sock = SocketManager::Instance( )->CreateServerSocket( addr, & g_handler );
	if ( sock == UDT::INVALID_SOCK )
	{
		PyErr_SetString(PyExc_Exception, UDT::getlasterror( ).getErrorMessage( ) );
		throw_error_already_set();
	}
	SocketManager::Instance()->SetSocketUserData( sock,( DWORD ) handler );
	xincref( handler );
	return sock;
}

static tuple GetSocketName( UDTSOCKET sock )
{
	stAddr addr;
	if( false == SocketManager::Instance()->GetSocketName( sock, addr ) )
	{
		PyErr_SetString(PyExc_Exception, UDT::getlasterror( ).getErrorMessage( ) );
		throw_error_already_set();
	}
	return make_tuple( addr.mIP.toString(), addr.mPort );
}

static tuple GetPeerName( UDTSOCKET sock )
{
	stAddr addr;
	if( false == SocketManager::Instance()->GetSocketPeerName( sock, addr ) )
	{
		PyErr_SetString(PyExc_Exception, UDT::getlasterror( ).getErrorMessage( ) );
		throw_error_already_set();
	}
	return make_tuple( addr.mIP.toString(), addr.mPort );
}

static void Run()
{
	if( UDT::ERROR == SocketManager::Instance()->Run( ) )
	{
		PyErr_SetString(PyExc_Exception, UDT::getlasterror( ).getErrorMessage( ) );
		throw_error_already_set();
	}
}

static void Close( UDTSOCKET sock )
{
	PyObject* handler = (PyObject*) SocketManager::Instance()->GetSocketUserData( sock );
	xdecref( handler );
	SocketManager::Instance()->CloseSocket( sock );
}

static void Send( UDTSOCKET sock, const str & s )
{
	char *buffer;
	int length;
	PyString_AsStringAndSize(s.ptr(), &buffer, &length );
	if( UDT::ERROR == SocketManager::Instance()->Send( sock, (void*)buffer, length ) )
	{
		PyErr_SetString(PyExc_Exception, UDT::getlasterror( ).getErrorMessage( ) );
		throw_error_already_set();
	}
}

static void SetHandler( UDTSOCKET sock,  PyObject* o )
{
	xincref( o );
	SocketManager::Instance()->SetSocketHandler( sock, &g_handler);
	SocketManager::Instance()->GetSocketInfo( sock )->mUserData = (DWORD)o;
}

static void RunOnce( )
{
	if( UDT::ERROR ==  SocketManager::Instance()->RunOnce( 200 ) )
	{
		PyErr_SetString( PyExc_Exception, UDT::getlasterror( ).getErrorMessage( ) );
		throw_error_already_set( );
	}
}

BOOST_PYTHON_MODULE( pyudt )
{
	def("create_client",  CreateClient );
	def("create_server",  CreateServer, args("host")= std::wstring(L"") );
	def("getsockname",    GetSocketName );
	def("getpeername",    GetPeerName );
	def("close",          Close );
	def("send",           Send );
	def("set_handler",    SetHandler);
	def("run",            Run );
	def("run_one",        RunOnce );
}
