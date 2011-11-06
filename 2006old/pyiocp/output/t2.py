
import socket

import pyiocp as iocp

from msvcrt import get_osfhandle


a = iocp.iocp()
print "new iocp:", a


s = a.socket(key="[mine sock]")

print (s)

import sys
#sys.exit(0)



addr = ("192.168.1.13", 80)
s.bind( ("",0) )


s.connect( addr, "[connect]" )

n = 0

#F = open("c:/k")

#print F.fileno( )

#fd = reopen( get_osfhandle( F.fileno( ) ) )
#print fd
#a.add( fd )


#read(fd, 5121)
send_str = ''
recv_str = "s" * 1000
while 1:
	f, l, k, pk, buf, err = a.get()
	if f:
		#print f, k, pk, type(buf), buf , l, err
		if pk == '[connect]':
			if err:
				print 'error', err
				break
			f.send( "get\r\n", "[send]" )
			s = a.socket()
			n+=1
			if n > 10000:
				break
			s.bind( ("",0) )
			s.connect( addr, "[connect]" )
		
		if pk=='[send]':
			f.recv( iocp.buffer(1000) , "[recv]")
			
		if pk=='[recv]':
			#print buf[:l]
			pass

			
			

		
		
	

