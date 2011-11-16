
import urllib2, urllib
import cookielib
cj = cookielib.CookieJar()
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
		F = urllib2.urlopen( url, urllib.urlencode( dict(email=self.USERNAME, password=self.PASSWORD) ) )
		print F.read()

if __name__ == "__main__" :
	mt = Mt()
	mt.login()

