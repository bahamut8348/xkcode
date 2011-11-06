
#include "python.h"
#include "tchar.h"
#include <datetime.h>


#include "pyplus.h"

pyplus::pyinstance __pyinstance;

pyplus::pyinstance::pyinstance()
{
	Py_Initialize();
	init();
}

pyplus::pyinstance::~pyinstance()
{
	if(Py_IsInitialized())
	{
		release();
	}
}

void pyplus::pyinstance::release()
{
	global::pyrelease();
}

void pyplus::pyinstance::init()
{
	PyDateTime_IMPORT;
}
pyplus::pytuple::pytuple( int size )
{
	obj = PyTuple_New(size);
}

pyplus::pytuple::pytuple()
{
	obj = PyTuple_New(0) ;
}

pyplus::pytuple::pytuple( const pyobject & o )
{
	attach(o.ptr());
}

bool pyplus::pytuple::check() const
{
	return PyTuple_Check(ptr());
}

int pyplus::pytuple::size() const
{
	return PyTuple_GET_SIZE(ptr());
}


pyplus::pyobject  pyplus::pytuple::get( int pos ) const
{
	return PyTuple_GET_ITEM(ptr(), pos);	
}
void pyplus::pytuple::set( int pos, pyobject & o )
{
	PyTuple_SET_ITEM(ptr(), pos, o.ptr());
}

pyplus::pyobject  pyplus::pytuple::operator[]( const int idx ) const
{
	return 	get(idx);
}



pyplus::pyobject::pyobject() : obj(NULL)
{

}

pyplus::pyobject::pyobject( PyObject *o ) :obj(o)
{
	incref();
}

PyObject* pyplus::pyobject::ptr() const
{
	return obj;
}

bool pyplus::pyobject::isNone() const
{
	return obj == NULL;
}

bool pyplus::pyobject::import( const char *name )
{
	obj = PyImport_ImportModule(name);
	return obj != NULL;
}

pyplus::pyobject pyplus::pyobject::callfunction( char *format, ... )
{
	pyobject args = NULL;
	va_list va;
	if (format && *format) {
		va_start(va, format);
		args = Py_VaBuildValue(format, va);
		va_end(va);
	}
	else
		args = PyTuple_New(0);
	return call(args);
}

pyplus::pyobject pyplus::pyobject::attr( const char *name )
{
	return PyObject_GetAttrString(ptr(), name);
}

pyplus::pyobject pyplus::pyobject::call( pyobject & args, pyobject  kw /*= NULL*/ )
{
	return PyObject_Call(ptr(), args.ptr(), kw.ptr());
}



void pyplus::pyobject::decref()
{
	Py_DecRef(ptr());
}

void pyplus::pyobject::incref()
{
	Py_IncRef(ptr());
}

pyplus::pyobject::~pyobject()
{
	dettach();
}

pyplus::pyobject & pyplus::pyobject::operator=( const pyobject & o )
{
	if (&o == this) return *this;
	if(!isNone())
	{
		dettach();
	}
	obj = o.ptr();
	incref();
	return *this;
}

long pyplus::pyobject::tolong() const
{
	return PyInt_AS_LONG(ptr());
}

unsigned long pyplus::pyobject::toulong() const
{
	return PyLong_AsUnsignedLong(ptr());
}

long long pyplus::pyobject::toll() const
{
	return PyLong_AsLongLong(ptr());
}

unsigned long long pyplus::pyobject::toull() const
{
	return PyLong_AsUnsignedLongLong(ptr());
}



float pyplus::pyobject::tofloat() const
{
	return (float)PyFloat_AsDouble(ptr());
}

double pyplus::pyobject::todouble() const
{
	return PyFloat_AsDouble(ptr());	
}

pyplus::pyobject::operator float() const
{
	return tofloat();	
}

pyplus::pyobject::operator long() const
{
	return tolong();	
}

void pyplus::pyobject::attach( PyObject * o )
{
	if(!isNone())
	{
		decref();
	}
	obj = o;
	incref();
}

PyObject * pyplus::pyobject::dettach()
{
	PyObject *ret = obj;
	if(!isNone())
	{
		decref();
		obj = NULL;
	}
	return ret;
}

pyplus::pyobject pyplus::pyobject::dict() const
{
	return PyModule_GetDict(ptr());
}
pyplus::pyobject pyplus::global::import( const char *name )
{
	return PyImport_ImportModule(name);
}

void pyplus::global::pyrelease()
{
	Py_Finalize();	
}

pyplus::pylong::pylong( long i )
{
	obj = PyLong_FromLong(i);
}

pyplus::pylong::pylong( long long i )
{
	obj = PyLong_FromLongLong(i);
}

pyplus::pylong::pylong( unsigned long i )
{
	obj = PyLong_FromUnsignedLong(i);
}

pyplus::pylong::pylong( unsigned long long i )
{
	obj = PyLong_FromUnsignedLongLong(i);
}

pyplus::pyfloat::pyfloat( double v )
{
	obj = PyFloat_FromDouble(v);
}

pyplus::pyfloat::pyfloat( float v )
{
	obj = PyFloat_FromDouble( (double)v);
}

static PyMethodDef exportmethod[]={0,0,0,0};

pyplus::pymodule::pymodule( const char *modname )
{
//Py_InitModule(modname, exportmethod);
	obj = PyImport_AddModule(modname);
}

const char * pyplus::pymodule::name() const
{
	return PyModule_GetName(ptr());
}
pyplus::pydict::pydict()
{
	obj = PyDict_New();
}

pyplus::pydict::pydict( const pyobject & o )
{
	if(obj)
	{
		dettach();
	}
	attach(o.ptr());
}

int pyplus::pydict::size() const
{
	return PyDict_Size(ptr());
}

pyplus::pyobject pyplus::pydict::get( const pyobject & key ) const
{
	return PyDict_GetItem(ptr(), key.ptr());
}

int pyplus::pydict::set( const pyobject & key, const pyobject & value )
{
	int ret = PyDict_SetItem(ptr(), key.ptr(), value.ptr());
	return ret;
}

void pyplus::pydict::clear()
{
	PyDict_Clear(ptr());
}

int pyplus::pydict::has_key( const pyobject & key ) const
{
	return PyDict_Contains(ptr(), key.ptr());
}

pyplus::pyobject pyplus::pydict::operator[]( const pyobject & key ) const
{
	return get(key);
}

