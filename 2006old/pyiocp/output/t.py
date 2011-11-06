import socket
import pyiocp as iocp
from sys import stderr

a = iocp.iocp()
print "iocp", a

F = a.file( "c:/k", mode="w", key=1 )
#F.read( 10, key=0 )
F.write( "test" , start = 100)
while 1:
	r = a.get()
	print r
	if r[0]:
		stderr.write ( r[4][:r[1]] )
		#key = r[3] + r[1]
		#print key
		F.read( len = 10, start = key, key = key )


			
