import socket
socket.setdefaulttimeout( 1 )
import urllib2

import time

ips = open( "dns.txt" ).read().split("\n")

rs = []

for ip in ips:
	start = time.time()
	try:
		if not ip.strip() : break 
		F = urllib2.urlopen( "http://%s/" % ip.strip() )
		F.info()
		F.close()
	except:
		pass

	print "time:", ip, time.time() - start

	rs.append( dict( ip = ip, time = time.time() - start ) )
	
rs.sort( lambda a, b: int(a['time'] - b['time']) )
F = open( "times.txt", "wb" )
for r in rs:
	print("!!", r )
	F.write( "%s, %f\r\n" % ( r['ip'], r['time'] ) ) 
F.close()
