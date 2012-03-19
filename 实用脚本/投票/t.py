
import socket
socket.setdefaulttimeout(4)
import urllib2, urllib
import time, os

def f():
	F = urllib2.urlopen( "http://www.265g.com/app/article.php", urllib.urlencode(dict(aid=174321,op="xq", xqnum=1)) )
	F.read()
	F.close()


if __name__ == "__main__":
	a = 1
	while a:
		print a
		a = a + 1
		try:
			f()
		except:
			pass
		time.sleep( 1 )

