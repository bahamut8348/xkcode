import jot
import socket
import struct

if __name__ == "__main__":
	s = socket.create_connection( ("localhost", 3001) )
	e = jot.Event("UserNames", "signIn", ["xuke","123456"] )
	e.n = 1
	v = jot.xenc( e )
	print v
	l = len(v)
	s.send( struct.pack( ">L", l) + v )
	buf = s.recv(4096)
	v, p = jot.xdec( buf[4:] )
	
	user = v.re[1]
	print user

	print v.re[2]
	