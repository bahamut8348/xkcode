from twisted.names import client
from twisted.internet import reactor

import time

sends = {}
ips = []
def dnsdone( name, ip ):
	print "name:",  ip
	del sends[ip]
	for k in name:
		for k1 in k:
			if k1.type == 1 :
				print("!!! kk", k1.payload.dottedQuad() )
				ips.append( k1.payload.dottedQuad() )

def dnserr( name , ip ):
	print "error", ip , name
	del sends[ip]

res = client.createResolver( [ ('8.8.8.8',53) ], None, "C:\windows\hosts" )

def testdns( i ):
	ip = "a%d.phobos.apple.com" % i
	print i
	sends[ip] = True
	res.lookupAddress( ip, [2] ).addCallback( dnsdone, ip ).addErrback( dnserr, ip )

testdns(1)

for i in range( 2, 2000 ):
	reactor.callLater( i*3, testdns, i )

def testdone():
	if len( sends ) == 0:
		reactor.stop()
		return
	reactor.callLater( 10, testdone )

testdone()

reactor.run()

F = open("dns.txt", "wb")
F.write( "\r\n".join( ips ) )
F.close()

