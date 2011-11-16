#! encoding:utf8

import urllib2, urllib
import cookielib
import socket
socket.setdefaulttimeout( 5 )
global cj
import sys
import time
cj = cookielib.MozillaCookieJar()

opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
urllib2.install_opener(opener)

global url
url = "http://www.meituan.com/deal/verify/799745"

class Mt:
	USERNAME = "lovecpp@gmail.com"
	PASSWORD = "3281044"
	def __init__( self ):
		pass

	def login( self ):
		F = urllib2.urlopen( "http://www.meituan.com/account/login", urllib.urlencode( dict(email=self.USERNAME, password=self.PASSWORD) ) )
		buf = F.read()
		open("kk","wb").write( buf )
		if buf.find("错误") >0 :
			print "!!error"
		else:
			print "login ok"
			cj.save("cookie")


	def buy( self ):
		F = urllib2.urlopen( "http://www.meituan.com/deal/verify/799745" )
		buf = F.read()
		open("kkk","wb").write( buf )
		if buf.find("错误") >0 :
			print "!!error"
		else:
			print "buy ok | ", time.asctime()

		

if __name__ == "__main__" :
	mt = Mt()
	if len(sys.argv) > 1:
		mt.login()
	
	while 1:
		mt.buy()

