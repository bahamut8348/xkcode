
#! coding:utf-8

import jot
import socket
import struct
from twisted.python.failure import Failure
import stackless as SL

import fansy as FAN

from fansy import *

def keepAlive( cc ):
	cc.sn("Ch").zzz( 1 )
	print "zzz", s1(cc.ch.name) 
	callLater( 60, keepAlive, cc )

def sell( cc, s, npc, saves= [] ):
	ch = cc.ch
	pc = ch.scene
	walk( cc, pc, s )
	buy = NpcBuy( 0, Npc( ch.place, s, npc ) )
	for item in ch.items.s:
		if item == None or item.idx in saves:continue
		print cc.s( buy ).sell( item.id, True )
	cc.s( ch.items ).sort()
	walk( cc, s, pc )

def mail( cc, to, its ):
	print cc.sn( cc.ch.mails ).send ( to,"gogo","gogo",0,its )

def walk( cc, start, end, w = None ):
	if start == end : return
	ch = cc.ch
	for i in range( start ,end, 1 if end > start else -1 ):
		if w == None:
			cc.s( ch ).walk( i + (1 if end > start else -1) )
		else:
			cc.sn( ch ).walk( i + (1 if end > start else -1), w )
	ch.scene = end

def pick( cc, s,  npcid ):
	ch = cc.ch
	p = NpcPick( 0, Npc( ch.place, s, npcid ) )
	cc.sn( p ).go()

def hunt ( cc, s, npcid ):
	ch = cc.ch 
	h = NpcHunt( 0, Npc( ch.place, s, npcid ) )
	cc.sn( h ).go()

def huntfb ( cc, s, npcid ):
	print "HuntFb", s, npcid 
	ch = cc.ch 
	npc = Npc( ch.place, s, npcid )
	npc.owner.owner = Fb( ch.place )
	h = NpcHunt( 0, npc )
	cc.sn( h ).go()


def login( cc, user, pw, uu = 0 ):
	us= getLoginStr( user, pw )

	u = cc.s( "UserNames").signKey( us["uid"][0], int(us["t"][0]) , 1, us["token"][0] )
	
	v = cc.s( cc.user ).signCh( uu, 1 )
	ch = cc.ch
	
	cc.s( v[2] ).sort()
	
	cc.name = ch.name
	keepAlive( cc )
	print "]",s1(cc.name), "登陆",  ch.place, ch.scene,"铜钱:", ch.gold, "等级:", ch.lv
	print "]", s1(cc.name), "状态", ch.st
	check_mail( cc )
	return ch

def check_mail( cc ):
	for mail in cc.ch.mails.s:
		if mail.h == "gogo":
			r = cc.s( cc.ch.mails ).take( mail.id )
			if not issub( r, Failure ):
				cc.sn( cc.ch.mails ).rem( mail.id )
				mail.id = 0
	cc.ch.mails.s = filter( lambda x:x.id != 0 , cc.ch.mails.s )

class Client:
	pw = "3281044"
	def onNotify( self, cc, o ):
		n = o.__class__.__name__
		fn = "on_" + n
		if hasattr( self, fn ):
			callLater( 0, getattr( self, fn ), cc, o )
	
	def on_Mail( self, cc, o ):
		check_mail( cc )

class X2(Client):
	user = "xkou"
	pw = ""
	ss = [ [], [1,1,1 ], [1,1,0,1], [ 1,1,1 ] ] # 25
	ss = [ [], [1,1], [1,1,1], [1,1,0,1] ]  # 30
	ss = [ [], [1,1,1,1], [1,1] ] # 20
	ss = [ [], [1,1,1] ]
	level = 0
	def onStart( self, cc ):
		self.cc = cc
	#	self.ss = [ [], [1,1,1], [1,1,0,1] ]  # 25
	#	self.ss = [ [], [1,1], [1,1,1], [1,1,0,1] ]  # 30
	#	self.ss = [ [], [1,1,1,1], [1,1] ] # 20
	#	self.ss = [ [], [1,1], [1,1,1] ]  # 35
	#	self.ss = [ [], [1,1,1] ]
		self.needBuff = False
		ch = login( cc, self.user, self.pw )
		print "位置:", ch.place, ch.scene, ch.fb
		self.target = 0
		if ch.st :
			print "状态", ch.st
			ch.st.owner = ch
			if issub( ch.st , Die ):
				cc.s( ch.st ).yuan()
			else:
				return
		
		if ch.scene != 0 :
			walk( cc, ch.scene, 0, self.level )
	
		self.reset()
	
	def getNext( self ):
		r = None
		s = self.scene
		n = self.npcc
		for k in self.ss[s:]:
			cs = k[n+1:]
			for npc in cs:
				n += 1
				if npc == 0:
					continue
				elif npc == 1:
					return ( s, n )
			s += 1
			n = -1
		return r


	def reset( self ):
		cc = self.cc
		walk( cc, 0, 1, self.level )
		self.npcc = 0	
		self.scene  = 1
		
		huntfb( cc, self.scene , self.npcc )
		self.target = 0

	def bag( self , cc ):
		cc.s( cc.ch.items ).sort()
		print "包裹剩余:", cc.ch.items.empty()

	def on_Pk( self, cc, o ):
		print ">>> PK", "win:", o.win, "st:", o.st, "foo:", o.foo
		if o.win < 0:
			if o.st == 0:
				for e in o.s:
					if e == None:
						self.target = 3
					
				cc.sn( o ).go( 0, self.target )
			else:
				t = 12 - o.st
				callLater( t, cc.sn( o ).go, 0, self.target )
		else:
			cc.s( cc.ch.items ).sort()
			print "包裹剩余:", cc.ch.items.empty()
			for v in o.s:
				for v2 in v:
					if issub( v2, ItemJ ):
						print ">>", v2.idx ,"耐久度", v2.f
						if v2.f < 200:
							it = cc.ch.items.find( 45416 )
							if it:
								print "使用日月精华:" , cc.s( v2 ).feed( it.id )
					elif issub( v2, ItemP ):
						print ">>", v2.idx ,"饱食度", v2.f
						if v2.f < 200:
							it = cc.ch.items.find(45408)
							if it: 
								print "使用麦子:" , cc.s( v2 ).feed( it.id )
							
		#	callLater( 1, self.bag, cc )
		
	def on_Die( self, cc, o ):
		o.owner = cc.ch
		cc.sn( o ).yuan()
		print cc.ch.name , "死亡"
		callLater( 1, walk, cc, self.scene, 0, self.level )
		callLater( 3, self.reset )

	def on_Hunt( self, cc, o ):
		print "Hunt:", o
		
		if o.done == 0:
			self.target = 0
			
			if not hasattr( self, "npcc" ):
				print "没有进度", cc.ch.scene, 0
				walk( cc, cc.ch.scene, 0, self.level )
				callLater( 1, self.reset )
				return
			print "Npc:", self.npcc, "Scene:", self.scene
			n = self.getNext()
			print "下一副本:",n
			if n:
				s, npc = n
				walk( cc, self.scene, s, self.level )
				self.scene = s
				self.npcc = npc
				huntfb ( cc, self.scene, self.npcc )
			else:
				walk( cc, self.scene, 0, self.level )
				callLater( 3, self.reset )
					
	def  on_Buff( self, cc, o ):
		print "Buff", o
		if o.t < -2:
			it = cc.ch.items.find( 30077 )
			if it:
				cc.sn( it ).use( )
			it = cc.ch.items.find( 30078 )
			if it:
				cc.sn( it ).use( )
				

class MAKE( Client ):
	user = ""
	pw = "3281044"
	pick_npc = 2
	pick_scene = 0
	make_npc = 1
	make_scene = 3
	acts = (1, (), 2, () )
	drop_is = ( 30947, 30948 )
	make_is = ( )

	def onStart( self, cc ):
		ch = login( cc, self.user, self.pw )
		if ch.st:
			ch.st.owner = ch
			cc.sn( ch.st ).stop()

		walk( cc, ch.scene, self.pick_scene )
		pick( cc, self.pick_scene, self.pick_npc  )
	
	def onFull ( self, cc ):
		walk( cc, self.pick_scene, self.make_scene )
		print "acts",self.acts
		cc.s ( cc.ch.items ).sort()
		ch = cc.ch
	
		for actid, ms in self.acts:
			mk = NpcMake( actid, Npc( cc.ch.place, self.make_scene, self.make_npc ) )
			for m in ms:
				for i in range( ch.items.num( makes[m] )/30 ):
					cc.s( mk ).go( m, None, False )

		cc.s ( cc.ch.items ).sort()
		walk( cc, self.make_scene, self.pick_scene )
		pick( cc, self.pick_scene, self.pick_npc )

	def on_Pick( self, cc, o ):
		
		ch = cc.ch
		if o.i:
			if ch.items.empty() <= 5:
				print s1( ch.name ), "包裹满2"
				cc.sn( o ).stop()
				callLater( 1, self.onFull, cc )
				return

			if o.i.idx in self.drop_is:
				print s1( ch.name ) , "drop", o.i.idx, o.i.n
				cc.sn( o.i ).drop( o.i.n )
			else:
				cc.onGetItem( o.i )
			cc.sn( o ).go( )
		
		elif o.i == False:
			print s1( ch.name ), "包裹满"
			cc.sn( o ).stop()
			callLater( 0, self.onFull, cc ) 
			



class FUSE( Client ):
	user = ""
	fuse_npc = 1
	fuse_id = 3
	fuse_scene = 1

	buy_npc = 0
	buy_scene = 1
	buy_item = 0
	fuse_item_idx = (32001,)

	save_items = ( 30951, 31094,31060, 31061
#		31055,31056,31057,31058,31059,  
#		31060,31061,31062,31063,31064,  
#		31065,31066,31067,31068,31069,
#		31070,31071,31072,31073,31074
	)
	
	def doit( self ):
		cc = self.cc
			
		rs = cc.s( self.npcbuy).go( self.buy_item )
		print rs
		if issub( rs, Failure ):
			print s1( cc.ch.name ) , "包裹满"

			for item in cc.ch.items.s:
				if item == None : continue
				if item.idx not in self.save_items:
					cc.s( self.npcbuy ).sell( item.id, True )
			cc.s( cc.ch.items ).sort()
			callLater( 0.4, self.doit )
			return

		print s1( cc.ch.name), "买入", rs[1].idx

		rs = cc.s( self.npcfuse ).go( rs[1].id )
		if issub( rs, Failure ): return self.doit()
		print s1( cc.ch.name ), "熔炼", rs[0]
		callLater( 0.4, self.doit )

	def onStart( self, cc ):
		self.cc = cc
		ch = login( cc, self.user, self.pw )
		
		self.npcbuy = NpcBuy( 0, Npc( ch.place, self.buy_scene, self.buy_npc ) )
		self.npcfuse = NpcFuse( self.fuse_id, Npc( ch.place, self.fuse_scene, self.fuse_npc ) )
		self.doit( )

		

class HUNT( Client ):
	user = "xkou"
	pw = "3281044"
	hunt_npc = 1
	hunt_scene = 1

	def onStart( self, cc ):
		self.cc = cc
		ch = login( cc, self.user, self.pw )
		if ch.st :
			ch.st.owner = ch
			if ch.st.t == 0:
				hunt ( cc, self.hunt_scene, self.hunt_npc )
		else:
			walk( cc, ch.scene, self.hunt_scene )
			hunt ( cc, self.hunt_scene, self.hunt_npc )
	
	def on_Hunt( self, cc, o ):
		print "hunt", o
		if o.done == 0 :
			hunt( cc, self.hunt_scene, self.hunt_npc )
		elif o.t  == 0:
			cc.s( o ).go()
	def on_Pk( self, cc, pk ):
		print "PK", "st:", pk.st, "win:", pk.win
		if pk.st == 0:
			cc.sn( pk ).go()
		elif pk.win > 0:
			pass
		else:
			callLater( 7, cc.s( pk ).go )
	
			

class PICK(Client):
	user = "xkou2"
	pick_npc = 2
	pick_scene = 0
	buy_npc = 0
	buy_scene = 1
	saves = ( 30951, )
	send_to = "www"
	sends = [  ]
	def onStart( self, cc ):
		ch = login( cc, self.user, self.pw )
		for it in ch.items.s:
			if it and it.idx in self.sends and it.n == 20:
				print s1(ch.name) , "发送邮件"
				mail( cc, self.send_to, [it.id] )
				slwait(0.4)
		if ch.st:
			ch.st.owner = ch
			cc.sn( ch.st ).stop()
		
		walk( cc, ch.scene , self.pick_scene )
		pick( cc, self.pick_scene, self.pick_npc )
		
	def on_Pick( self, cc, o ):
		ch = cc.ch
		if o.done == 0:
			pick( cc, self.pick_scene, self.pick_npc )
			return
		if o.i:
			cc.onGetItem( o.i )
			if o.i.idx in self.sends and o.i.n == 20:
				mail( cc, self.send_to, [o.i.id] )
		if o.i == False:
			print s1(ch.name), "包裹满"
			cc.sn( o ).stop( )
			sell ( cc, self.buy_scene, self.buy_npc, self.saves )
			pick ( cc, self.pick_scene, self.pick_npc )
		else:
			cc.sn( o ).go( )


class Pick_WWW( PICK ):  # 柴桑
	user = "xkou2"
	pick_npc = 1
	pick_scene = 0
	buy_npc = 0
	buy_scene = 1
	saves = ( 30950, 30951, 31822, 31847 )

class Hunt_OK(HUNT):
	user = "xkou"
	hunt_npc = 6
	hunt_scene = 5

class Make_WWW( MAKE):
	user = "xkou2"
	pick_npc = 5
	pick_scene = 4
	make_npc = 6
	make_scene = 2
	acts = ( (3, (31055,31060,31065,31070) ), )  # 低
	acts = ( (3, (31056,31061,31066,31071) ), )  # 初
	acts = ( (4, (31057,31062,31067,31072) ), )  # 中



class Make_TCDJ( MAKE):  # 江陵
	user = "xkou1"
	pick_npc = 1
	pick_scene = 0
	make_npc = 1
	make_scene = 1
	acts = ( (1, ( 30961,30964,30967,30970,30973 )), ) # 铜
	acts = ( (0, ( 30976,30979,30982,30985,30988 )), ) # 铁


class Pick_TCDJ( PICK ):  # 柴桑 铁
	user = "xkou1"
	pick_npc = 2
	pick_scene = 4
	buy_npc = 1
	buy_scene = 3

	saves = ( 30951, 31067, 31815, 31801, 31047 )
	saves = ( 30950, 30951, 31822, 31847 )
#	send_to = "www"
#	sends = [ 31040, 31041, 31042, 31043, 31044 ]
#	sends = [ 31047 ]
	

class Pick_TT( PICK ):
	user = "xkou3"
	pick_npc = 1
	pick_scene = 5
	buy_npc = 0
	buy_scene = 4
	saves = ( 30950, 30951, 31822, 31847 )
#	send_to = "www"
#	sends = [ 31040, 31041, 31042, 31043, 31044 ]
#	sends = [ 31047 ]

class ONLINE:
	user = "xkou2"
	pw = "3281044"
	useridx = 0
	def onStart( self, cc ):
		ch = login( cc, self.user, self.pw )
	
	def onNotify( self, cc, o ):
		pass

class Online_WWW(ONLINE):
	user = "xkou2"


class Online_TCDJ( ONLINE):
	user = "xkou1"

class Online_OK123( ONLINE):
	user = "xkou"

class Online_TT( ONLINE):
	user = "xkou3"

class FB_OK123( X2 ):
	user = "xuke"

class FB_BIRD( X2 ):
	user = "bird"
	ss = [ [], [1] ]

class FB_OK( X2 ):
	user = "xkou"
	ss = [ [], [1,1,1],[1,1,0,1] ]
	ss = [ [], [1,1,1,1], [1,1] ]
	ss = [ [], [1] ]
	level = 0

class Fuse_TT( FUSE ):
	user = "xkou3"
	buy_item = 0
	save_items = ( 30951, 31094, 31061, 31060 )
	save_items = ( 30951, 31094, 31065, 31066 )
	save_items = ( 30951, 31094 )
	save_items = ( 31094, )

def work( C ):
#	server2( "1.1.1.11", 3005, C )
	server2( "s001.tj.game5.cn", 3001, C )


if __name__ == "__main__":


	callLater( 0, work, Pick_WWW )
	callLater( 0, work, Pick_TCDJ )
#	callLater( 0, work, Fuse_TT )
	callLater( 0, work, Pick_TT )
#
	callLater( 0, work, FB_OK )
#	server2( "s003.tj.game5.cn", 3003, FB_BIRD )
#	callLater( 0, work, Hunt_OK )
	
#	server2( "s001.tj.game5.cn", 3001, Online_WWW )
#	server2( "s001.tj.game5.cn", 3001, Online_TCDJ )
#	server2( "s001.tj.game5.cn", 3001, Online_TT )
#	server2( "s001.tj.game5.cn", 3001, Online_OK123 )

	run()
	

