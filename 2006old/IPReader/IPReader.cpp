// IPReader.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "QQWry.h"

BOOST_PYTHON_MODULE( IPReader )
{
	using namespace boost::python;

	class_<IPInfo>("IPInfo")
		.def_readwrite("IP",		&IPInfo::mIP )
		.def_readwrite("Country",	&IPInfo::mCountry )
		.def_readwrite("Area",		&IPInfo::mArea )
		.def( "__str__" , &IPInfo::__str__ )
		;

	class_<QQWry>("Reader")
		.def("load", &QQWry::Load)
		.def("__len__", &QQWry::GetIPNumber)
		.def("__getitem__", &QQWry::GetIPInfo )
		;
}