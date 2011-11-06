#! coding: utf-8

import urllib2, urllib, socket
import  simplejson as json
import codecs

from ctypes import *
ker = WinDLL("kernel32.dll")
m2w = ker.MultiByteToWideChar

class Farm:
	
	def __init__( self ):
		self.cookie = open("cookie").read()
		self.qq = 3222630
		self.tempbuf = create_string_buffer( 40000 )
		self.users = {}
	
	def get_user( self, uid ):
		return self.users[uid]

	def send( self, url , data = None ):
		print url
		req = urllib2.Request('http://nc.qzone.qq.com/cgi-bin/' + url, data )
		req.add_header('Referer', 'http://appimg.qq.com/happyfarm/module/Main_v_46.swf')
		req.add_header("User-Agent","Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3" )
		req.add_header("Cookie", self.cookie )
		F = urllib2.urlopen(req)
		b = F.read()
		F.close()
		v = m2w( 65001, 0, b, len(b), self.tempbuf, len( self.tempbuf )/2 )
		b = self.tempbuf.raw.decode( "utf-16" )
		b = b[ :v]
		try:
			return json.loads( b )
		except Exception , r:
			print r
			print b


	def get_friends( self ): # http://nc.qzone.qq.com/cgi-bin/cgi_farm_getFriendList?mod=friend 
		r= self.send( "cgi_farm_getFriendList?mod=friend","uIdx=3743981&uinY=%d&user=true&refresh=true&farmTime=1272193295&uinX=42704698&farmKey=9b2b25a806ac6eb16e09bf658b2aaa41" % (self.qq ) )
		for o in r:
			self.users[ o['uin'] ] = o
		return r

	def get_farm( self, id ): # http://nc.qzone.qq.com/cgi-bin/cgi_farm_index?mod=user&act=run&uinX=649832388&ownerId=649569875&uinY=3222630
		u = self.get_user(  id )
		return self.send( "cgi_farm_index?mod=user&act=run&uinX=%(uin)d&ownerId=%(uId)d&uinY=3222630" % u )
	
	def steal_farm( self, id, ps ): #http://nc.qzone.qq.com/cgi-bin/cgi_farm_steal?mod=farmlandstatus&act=scrounge
		return self.send( "cgi_farm_steal?mod=farmlandstatus&act=scrounge", "farmTime=1272203726&uinX=649832388&farmKey=c0d5d0535bd0a0fd20de1f6bec203544&tName=%E9%9D%92%E8%8F%9C%E7%B4%AB%E7%BD%97%E5%86%AC&uIdx=3743981&uinY=3222630&fName=%E4%B9%9D%E5%A4%A9%E7%A5%9E%E9%BE%99&place=1%2C3%2C4%2C5%2C6%2C7%2C8%2C11&ownerId=649569875" )

if __name__ == "__main__":
	f = Farm()
	f.get_friends()
	print f.get_farm( 3222630 )
