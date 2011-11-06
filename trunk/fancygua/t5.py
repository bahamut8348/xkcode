
import socket
import jot
import struct
import time


class A : pass
jot.def_enc( A, ['m','n'] )



bb = ["12","dsfdf","sfdf","dsfd"] * 10
aa = A()
aa.m  = bb
aa.n = 12323
def test():
	sock = socket.socket()
	sock.connect( ( "1.1.1.11", 3005 ) )

	def send( o ):
		
		sock.send( buf )
	
	d = dict( a= "dsffdfdf" )
	d["dsfdf"] = "dddddddddddddd"

	a = ["ssssssssssssssss", [ aa , '{"dsf":"dsfsdfdfdf"}, {"dsf":"sdfdsfdfdfdfdf"}', aa ] ] * 4000
	o = [ "1111111","222222222","33333333333" ,"False" ] * 6
	
	o[1] = o
	o[3] = a
	o[4] = a
	o[5] = o
	a[1] = o  
	a[3] = o
	o[2] = a
	o[3] = a
	
	
	a1 = ["111",aa]
	b1 = ["333", "444" ]
	o1 = ["11", a1, b1, a1, b1, "22"] * 10
	
	buf = jot.xenc( o )
	buf =  struct.pack( ">L", len(buf) ) + buf
	

	print "size:", len( jot.xenc(o) ) 
	
	t1 = time.time()
	for i in range( 1000 ):
		sock.send( buf )
		continue
		dl = 6000
		bb = 0
#		print "send"
		while bb is 0 or len(bb)==dl:
			bb = sock.recv( dl )
		
	#	print len(bb)
	#	while 1: sock.recv( 10240 )
		

	
	t2 = time.time()
	print t2 - t1

	sock.close()


for i in range( 10000 ):
	test()
