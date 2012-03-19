#! /usr/bin/python
#! coding: utf8

import socket
import cookielib, urllib2, urllib
import sys, json

jar = cookielib.LWPCookieJar()
jar.load( sys.argv[1], ignore_discard = True, ignore_expires = True )
urllib2.install_opener( urllib2.build_opener ( urllib2.HTTPCookieProcessor( jar ) ) )

for cookie in jar: 
# print cookie, cookie.name
	if cookie.name == "ptui_loginuin": 
		global QQ 
		QQ = cookie.value

print "QQ:", QQ

def info( o, s ):
	print ",".join( map( lambda x: x + ":" + o[x], s ) )

def getpage( url ):
	F = urllib2.urlopen( url )
	r = json.loads( F.read().decode("gbk").encode("utf8") )
	if r["result"] == "0":
		return r
	print r["msg"]
	return None

class QC:
	def tasklist( self, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=task&sub=1&page=0&selfuin=%s" % QQ ):
		r = getpage( url )
		if r: 
			r = r["tasklist"]
			for k in r: info( k, ["id","title", "status", "gift", "desc"] )
			return r
	# 9:½£¾ý
	def fight( self, id, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=fight&puin=%d" ):
		r = getpage( url % int(id) )
	
	def getexp( self, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=getexp"):
		r = getpage( url )
	
	def visit( self, id = QQ, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=visit&puin=%d&kind=1" ):
		r = getpage( url % int( id ) )
		self.tudis = {}
		if r : 
			for k in r["tudi"]:
				self.tudis[ k["uin"] ] = 1
			return r["baseinfo"]

	def view( self, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=view&kind=1&sub=1&selfuin=%s" % QQ ):
		r = getpage( url )
		if r:
			r = r["info"] 
			r.reverse()
			for k in r :
				if k["uin"] in self.tudis: r.remove( k ); r.insert( 0, k )
			for k in r : info( k, ["uin", "name", "lilian", "enable"] )
			return r
	def say( self, id, feedid, key, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=say&puin=%s&word=aaa&feedid=%s&key=%s" ):
		r = getpage( url % ( id, feedid, key ) )

	def endtower( self, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=towerfight&type=7" ):
		r = getpage( url )
	
	def viewtower( self, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=towerfight&type=3" ):
		r = getpage( url )
		return r

	def autotower( self, url = "http://fight.pet.qq.com/cgi-bin/petpk?cmd=towerfight&type=1" ):
		r = getpage( url )

if __name__ == "__main__" :
	q = QC()
	for k in q.tasklist():
		pass
	
	base = q.visit()
	sp = int( base["sp"] )
		
	for v in q.view():
		if sp < 10 : break
		if v["enable"] != "1" : continue
		q.fight( v["uin"] )
		sp = sp - 10

	r = q.viewtower()
	if r :
		if r["day_left_times"] != "0" : q.autotower()
	

