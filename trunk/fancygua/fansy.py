#! coding: utf-8

import jot
import socket
import struct
import traceback, sys
import time

socket.setdefaulttimeout(10)

import stackless as SL

from twisted.internet import reactor, defer, task
from twisted.internet.protocol import Protocol, ClientFactory, ClientCreator



def __call__(self, *args, **kwargs):
	f = self.tempval

	def new_f(old_f, args, kwargs):
		try:
			old_f(*args, **kwargs)
		except Exception:
			traceback.print_exc()

	self.tempval = new_f
	SL.tasklet.setup(self, f, args, kwargs)

SL.tasklet.__call__ = __call__



class Err( jot.BaseObj ): 	pass

s1 = lambda x: x.decode("utf-8").encode("gbk")

jot.def_enc( Err )



def issub( o , cls ):
	return issubclass( o.__class__, cls )

class  MsgHelper:
	def __init__( self, o ):
		self.obj = o
	def __getattr__( self, name ):
		self.act = name
		return self.sendFun
	
	def sendFun( self, *arg ):
		if( self.wait ) :
			return self.client.send( self.obj, self.act, arg  )
		else:
			return self.client.send_n( self.obj, self.act, arg  )

class FancyServer(Protocol):
	
	def __init__( self ):
		self.ds ={}
		self._buff =""
	def dataReceived(self, data):
		if self._buff:
			data = self._buff + data
			self._buff = ""
			if len(data)<4: 
				self._buff = data
				return
		while 1:
			l = struct.unpack( ">L" , data[:4])[0]
			
			if l == len(data) - 4:
				
				self.onPacket( data[4:] )
				break
			elif l >  len(data) - 4:
				self._buff += data
				break
			elif l < len( data ) - 4:
				self.onPacket( data[4:4+l] )
				
				data  = data[4+l:]
				
	
	def onPacket( self, pack ):
		d = jot.xdec( pack )
		try:
			ii = issub( d.re, Err )
			if not ii:
				if isinstance ( d.re, list ):
					map( lambda x: self.onN(x), d.re )
				else:
					self.onN( d.re )
			else:
				print s1(d.re.hint)
			if d.n > 0 and d.n in self.ds:
				df = self.ds[ d.n ]
				del self.ds[d.n]
				if ii:
					df.errback( d.re )
				else :
					df.callback( d.re )

			else:
				if isinstance ( d.re, list ):
					map( lambda x: self.factory.onNotify( self, x ), d.re )
				else:
					self.factory.onNotify( self, d.re )
		except :
			print "--------------"
			traceback.print_exc(file=sys.stdout)
			print "--------------"
	
	def onGetItem( self, o ):
		owner = o.owner
		ot = type( owner )
		if ot == Items:
			if hasattr( self, "items" ):
				self.items.s[o.id] = o
				if o.n == 0:
					self.items.s[o.id] = None
				print "]",s1(self.ch.name), "Got", o.idx, o.n, "idle:", self.items.empty()
		elif ot == ItemEs:
			
			self.itemEs.s[o.id] = o
		
	
	def getServerTime( self ):
		return time.time()*1000 - self.dtime

	def onN( self, o ):
		na = o.__class__.__name__
		t = type(o)
	#	print t, o
		if isinstance( o, St ):
			o.owner = self.ch
		if t is Ch :
			self.ch = o
		if isinstance( o, Item ):
			self.onGetItem( o )
		
		elif t == ItemEs:
			print t
			self.itemEs = o
		
		elif t == Items :
			self.items = o
		
		elif t == Mails:
			self.mails = o

		elif t == Mail:
			if o.t != self.ch.id: return
			o.owner = self.mails
			self.mails.s.append( o )
		
		elif t == Chat:
			pass
		
		if na == "User"  : self.user = o

	def send( self,  o, a, s ):
		e = jot.Event( o, a, s )
		d = defer.Deferred()
		e.n = self.factory.getN()
		self.ds[e.n] = d
		e = jot.xenc( e )
		print "send" , e
		self.transport.write( struct.pack( ">L", len(e) ) + e )
		ch = SL.channel()
		d.addCallback( self.send_done, ch ).addErrback( self.send_err, ch )
		return ch.receive()
	
	def send_done( self , r, ch ):
		ch.send( r )
		pass
	
	def send_err( self, r, ch ):
		print s1(self.name), "Error", s1( r.value.hint )
		ch.send( r )
		
	def send_n( self, o, a, s ) : # dont wait
		e = jot.Event( o, a, s )
		e.n = self.factory.getN()
		e = jot.xenc( e )
		print "send_n" , e
		self.transport.write( struct.pack( ">L", len(e) ) + e )
		
	def s( self, n ):
		h = MsgHelper(n)
		h.client = self
		h.wait = True
		return h
	
	def sn( self, n ):
		h = self.s( n )
		h.wait = False
		return h

	def connectionMade( self ):
		self.name= "[]"
		SL.tasklet(self.factory.onConn)(self)

		
	def connectionLost( self, r ):
		print "Connection lost", r
		callLater( 60, self.factory.onDisconn )
	
	def connectionFailed( self, r ):
		print "Failed", r
		

class FancyServerFactory( ClientFactory ):
	protocol = FancyServer
	n = 0
	def __init__( self, conn, notify, disconn ):
		self.onConn = conn
		self.onNotify = notify
		self.onDisconn = disconn
	
	def clientConnectionFailed(self, connector, reason):
		print 'Connection failed. Reason:', reason
		callLater( 1, self.onDisconn )

	def getN( self ):
		self.n += 1
		return self.n

def server( host, port , conn, notify, disconn ):
	f = FancyServerFactory( conn, notify, disconn )
	reactor.connectTCP( host, port, f )

import functools
def server2( host, port, C ):
	
	fun = functools.partial( server2,  host, port , C )
	c = C()
	server( host, port,  c.onStart, c.onNotify, fun )

def call_SL():
	SL.schedule()
	

def slwait( sec ):
	wlc = SL.channel()
	reactor.callLater( sec, wlc.send, 1 )
	wlc.receive()
	return 

t = task.LoopingCall(call_SL)
t.start(0.01)

def callLater( t, fun, *arg ):
	reactor.callLater(t, SL.tasklet( fun ), *arg )

def run():
	SL.tasklet(reactor.run)()
	SL.run()
	

# class define

class Root( jot.BaseObj ):
	def __init__( self, id = 0 ):
		self.id = id

class User( jot.BaseObj ): pass
jot.def_enc( User, ["id"] )

class Ch( jot.BaseObj ): pass
jot.def_enc(Ch, ["id"] )

class City: pass
jot.def_enc( City, ["id"] )

class Guild( Root ):pass
jot.def_enc( Guild, ["id"] )

class Fb: 
	def __init__( self, id ):
		self.id = id
jot.def_enc( Fb, ["id"] )

class Scene: pass
jot.def_enc( Scene, [ "id", "owner" ] )

class Npc:
	def __init__ (self, c, s, i ):
		self.id = i
		city = City()
		city.id = c
		scene = Scene()
		scene.id = s
		scene.owner = city
		self.owner = scene
	

jot.def_enc( Npc, [ "id", "owner" ] )

class Die( jot.BaseObj ): pass
jot.def_enc( Die, ["id", "owner"] )

class NpcAct( jot.BaseObj ):
	def __init__( self, id, npc ):
		self.id = id
		self.owner = npc

class NpcPick( NpcAct ): pass
class NpcHunt ( NpcAct ): pass
class NpcBuy ( NpcAct ): pass
class NpcMake( NpcAct ): pass
class NpcFuse( NpcAct ): pass
class NpcGuild( NpcAct ): pass

jot.def_enc( NpcAct, ["id","owner"] )

class Items( jot.BaseObj ):
	def isFull( self ):
		return not any( self.s )
	
	def find( self, idx ):
		for e in self.s:
			if e and e.idx == idx:
				return e
	
	def num ( self, idx ):
		r = 0
		for e in self.s:
			if e and e.idx == idx:
				r += e.n
		return r
	
	def empty( self ):
		r = 0
		for e in self.s:
			if not e : r+=1
		return r

class ItemEs( Items ):
	pass

jot.def_enc( Items, ["id"] )
jot.def_enc( ItemEs, ["id"] )

class SkillU( jot.BaseObj ):
	def __init__( self, id ):
		self.id = id
jot.def_enc( SkillU, ["id"] )

class ItemDs( Items) :pass
jot.def_enc( ItemDs, ["id"] )

class Team( jot.BaseObj ):pass
jot.def_enc( Team)

class St( jot.BaseObj ): pass
jot.def_enc( St, ["id","owner"]  )

class Move( St ): pass
jot.def_enc( Move )

class Pick( St ): pass
jot.def_enc( Pick )
class Hunt( St ) : pass
jot.def_enc( Hunt )
class Pk( St ) : pass
jot.def_enc( Pk )

class Mails( jot.BaseObj ):pass
jot.def_enc(Mails, ["id"] )

class Deals( jot.BaseObj ):pass
jot.def_enc( Deals, ['id'] )

class Notes( jot.BaseObj ) :pass
jot.def_enc( Notes, ['id'] )

class Vends( jot.BaseObj ) :pass
jot.def_enc( Vends, ['id'] )

class Mail(jot.BaseObj ): pass
jot.def_enc( Mail )

class Vend(jot.BaseObj ): pass
jot.def_enc( Vend )

class Deal(jot.BaseObj ): pass
jot.def_enc( Deal )

class Note(jot.BaseObj ): pass
jot.def_enc( Note )

class Buff( jot.BaseObj ): pass
jot.def_enc( Buff )

class Tasks( jot.BaseObj ):pass
jot.def_enc( Tasks )

class Item( jot.BaseObj ): pass
jot.def_enc( Item, ["id", "owner"] )

class ItemU( Item ) : pass
jot.def_enc( ItemU )

class ItemEg ( Item ) :pass
jot.def_enc( ItemEg )

class ItemE ( Item ) :pass
jot.def_enc( ItemE )

class ItemJ ( Item ) :pass
jot.def_enc( ItemJ )

class ItemP ( Item ) :pass
jot.def_enc( ItemP )

class ItemX ( Item ) :pass
jot.def_enc( ItemX )

class ItemI ( Item ) : pass
jot.def_enc( ItemI )

class ItemKs ( Item ) : pass
jot.def_enc( ItemKs )

class Task( jot.BaseObj ): pass
jot.def_enc( Task )

class Walk ( jot.BaseObj ):pass
jot.def_enc( Walk )

class Chat( jot.BaseObj ): pass
jot.def_enc( Chat )

class Fbing( jot.BaseObj ): pass
jot.def_enc( Fbing )

makes = {}
makes[31092] = 31068   
makes[31098] = 31074
makes[31104] = 31080
makes[31110] = 31086

makes[31096] = 31072
makes[31102] = 31078
makes[31108] = 31084
makes[31114] = 31090

makes[30903] = 30902
makes[30905] = 30904
makes[30927] = 30926
makes[30060] = 30950

for k, v in makes.items():
	makes[ v ] = k


import cookielib, urllib2, urllib
import urlparse

def getLoginStr( user, pw ):
	
	s = ""
	try:
		F = open("login/login." + user )
		s = F.read()
		F.close()
	except:
		print "open file error "
		pass
	
	if s=="":
		cj = cookielib.CookieJar()
		opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
		urllib2.install_opener( opener )
		params = urllib.urlencode({'login[user_code]': user, 'login[user_password]': pw})
		req = urllib2.Request("http://passport.game5.com/member/login", params)
		req.add_header("Host", "passport.game5.com")
	#	req.add_header("Cookie", "SESSCYPHP=e72cbd96220e08c3bd70858cfcf72e12")
		req.add_header("Content-Type","application/x-www-form-urlencoded")
		req.add_header("User-Agent","Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8")
		F = urllib2.urlopen( req )
		buf = F.read().decode("utf-8", "ignore").encode("gbk", "ignore")

		F = urllib2.urlopen("http://www.game5.com/playgame/index/27/246")
		buf = F.read().decode("utf-8", "ignore").encode("gbk", "ignore")
		s = urlparse.urlparse( F.url ).query
		
	d = urlparse.parse_qs( s )
	print "uid=%s&token=%s&t=%s&cm=%s" % (d['uid'][0], d['token'][0],d['t'][0],d['cm'][0])
	return d


if __name__ == "__main__":
	
	def s2( n ):
		return unicode(n).encode("utf8")

	def move( cc ):
		ch = cc.ch
		print cc.s( ch ).walk( 1 )
		print cc.s( ch ).walk( 1 )
		print cc.s( ch ).walk( 1 )
		print cc.s( ch ).walk( 0 )

	def onConnect( cc ):
		u = cc.s("UserNames").signIn( "bird", "123456" )
		cc.s( cc.user ).signCh( 0, 1 )
		ch = cc.ch

		chname = "aaaaaa"
		
		# $sock.send( $ch, 'Exps', vs, this.doneOk, this, [$ch.wulv()] ) // [ 力量，聪慧, 根骨， 统帅， 生命 ]
		ws = [ ("翡翠龙20星",1) , 
			("万里独行20星",1), 
			("举荐书?九品", 40), 
			("举荐书?八品", 40) 
		]
		for w, n in ws:
			
			cc.sn( "Chat" ).item( chname, s2(w), n )
		
		
		for i in range( 10 ):
			cc.sn( "Chat" ).exp( chname, 100000000000000 )
			
			while 1:
				r = cc.s( ch ).Exps( 1000000, 1000000, 1000000,0, 1000000 )
				break
				if type(r) == Err:
					break
	
	def onDisconnect(  ):
		pass
	
	def onNotify( client, d ):
		print "notify",client, d

#	print getLoginStr( "xkou1","3281044")

	s = server( "1.1.1.66", 3001, onConnect, onNotify, onDisconnect )
	run()

