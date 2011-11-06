# Copyright 2004-2008 Roman Yakovenko.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import os
import sys

from pygccxml import parser
from pygccxml import declarations


#configure GCC-XML parser
config = parser.config_t( include_paths=["E:\\Work\\0429\\PSDash\\Source","E:\\Work\\0429\\PSDash\\include"] )

#parsing source file
decls = parser.parse( ['test.cpp'], config )
global_ns = declarations.get_global_namespace( decls )

#get object that describes unittests namespace
dash = global_ns.namespace( 'TEST' )

for free_fun in  dash.free_functions():
	print free_fun
	print dir(free_fun)

