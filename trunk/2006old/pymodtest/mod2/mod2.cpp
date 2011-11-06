// mod2.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "..\include\mod.h"


namespace xkmod
{

	BOOST_PYTHON_MODULE(m2)
	{

		
//		bp::object m1( bp::handle<>(bp::borrowed(PyImport_AddModule("xkmod.m1"))));
		
//		bp::scope().attr("m1") = m1; 
//		bp::scope().scope( m1 );
		
//		bp::scope io_scope = m1;  

		bp::class_<H2>("H2")
			.def( "foo", &H2::foo )
			.def( "bar", &H2::bar )
			;
	}

}

