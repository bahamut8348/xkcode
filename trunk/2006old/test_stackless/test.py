import urllib
import stackless as SL
import socket
from stacklesssocket import install
from iocp import install
install()

import re

total = [0]
def download(  ):
	print "start"
	
	F = urllib.urlopen("http://www.163.com")
	return

	buf = F.read()
	print buf
	ma =	re.findall('"(http://\w+\.163.com/[^"]+)', buf)
	for count, e in enumerate(ma):
		SL.tasklet(downurl)(e, count)
		SL.schedule()
		if count > 400:
			break
	print len(buf)

def downurl( url, n ):
	print "start", n, url
	F = urllib.urlopen(url)
	buf = F.read()
	total[0]+=1
	print "End", n, len(buf), total

def test(e):
	s = socket.socket()
	try:
		s.connect(("localhost",8080))
		print "connect ok",e
	except:
		print "connect failed",e
import time

"""
for e in range(5000):
	SL.tasklet(test)( e )
	SL.schedule()
	time.sleep(0.001)
"""
SL.tasklet(download)()
SL.run(10000)
print "end"


