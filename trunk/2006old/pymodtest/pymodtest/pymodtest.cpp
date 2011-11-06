// pymodtest.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "../include/mod.h"

namespace xkmod
{

	H1::H1( )
	{
		
	}




BOOST_PYTHON_MODULE( m1 )
{
	bp::class_<H1>("H1")
		.def("print", &H1::print )
	;
}


}




