import pycurl
import os

class DownUrl:
	def __init__( self, url, start, path ):
		self.url = url
		self.start = start
		self.path = path
		self.F  = open( self.path, ab )
		c = pycurl.Curl()
		c.setopt( pycurl.URL, self.url )
		c.setopt( pycurl.FOLLOWLOCATION, 5 )
		c.setopt( pycurl.WRITEFUNCTION, self.on_file_write )
		c.setopt( pycurl.RESUME_FROM_LARGE, start )
		self.pos = start
	
	def on_file_write( self,  s ):
		self.pos += len( s )
		self.F.seek( self.pos, os.SEEK_SET )
		self.F.write( s )

status = 0
tasks = []
splitsize = 2000*1024
def on_file_info( s ):
	global status
	if s.find("HTTP/1.0 200 OK") == 0 :
		status = 1
	
	if s.find("Content-Length:") == 0 and status == 1:
		s = s.strip()
		length = int(s[16:])
		print "Total length:", length
		if length > splitsize:
			for e in range( length/splitsize + 1 ):
				tasks.append( DownUrl(url, splitsize * e,  path  ) )

#	c.close()

def on_file_write( s  ):
#	print "file_write"
#	print len(s)
	pass


m = pycurl.CurlMulti()
url = "http://163.com"
path = ""
c = pycurl.Curl()
c.setopt( pycurl.URL, "http://www.1tian.net:8080/BCRLaunchTrailerdl.wmv" )
c.setopt( pycurl.URL, url )
c.setopt( pycurl.FOLLOWLOCATION, 5  )
c.setopt( pycurl.WRITEFUNCTION, on_file_write )
#c.setopt( pycurl.RESUME_FROM_LARGE, 250000 )

c.setopt( pycurl.HEADERFUNCTION, on_file_info )
m.add_handle( c )

while 1:
    ret, num_handles = m.perform( )
    if ret != pycurl.E_CALL_MULTI_PERFORM: break
while num_handles:
    ret = m.select( 1.0 )
    if ret == -1:  continue
    while 1:
        ret, num_handles = m.perform ( )
        if ret != pycurl.E_CALL_MULTI_PERFORM: break


print "down"
