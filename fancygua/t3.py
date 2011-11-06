#! coding:utf-8

import jot
import socket
import struct
import time
import types
from twisted.python.failure import Failure
import stackless as SL

import fansy as FAN

from fansy import *

def keepAlive( cc ):
	zz = cc.s( cc.ch ).zzz()
	print "zzz", s1(cc.ch.name)
	print zz, time.time() *1000
	cc.dtime =  time.time()*1000 - zz
	callLater( 60, keepAlive, cc )

def sell( cc, s, npc, saves= [] ):
	ch = cc.ch
	pc = ch.scene
	walk( cc, pc, s )
	buy = NpcBuy( 0, Npc( ch.place, s, npc ) )
	for item in cc.items.s:
		if item == None or item.idx in saves:continue
		print cc.s( buy ).sell( item.id, True )
	cc.s( cc.items ).sort()
	walk( cc, s, pc )

def mail( cc, to, its, gold =0 ):
	print cc.sn( cc.mails ).send ( to,"gogo","gogo",gold,its )

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

	try:
		us= getLoginStr( user, pw )

		u = cc.s( "UserNames").signKey( us["uid"][0], int(us["t"][0]) , 1, us["token"][0] )
		
		v = cc.s( cc.user ).signCh( uu, 1 )
		
		print "~~~~~~~~~~"
		start = 5 + (uu * 3)
		cc.ch = u[start]
		ch = cc.ch
		cc.itemEs = u[ start+1 ]
		cc.itemEs.s = u[ start+2 ]
		
	except :
		print "err"
		slwait(30)
		return login( cc, user, pw, uu )
	print "服务器时间", u[3]
	
	cc.dtime = time.time()*1000 - u[3]
	cc.s( v[2] ).sort()

	cc.pk = v[6]
	
	cc.name = ch.name
	keepAlive( cc )
	print "]",s1(cc.name), "登陆",  ch.place, ch.scene,"铜钱:", ch.gold, "等级:", ch.lv
	print "]", s1(cc.name), "状态", ch.st
	check_mail( cc )
	return ch

def ttime( t ):
	return t/256 + 1230796800000

def check_mail( cc ):
	for mail in cc.mails.s:
		
		if mail.h == "gogo" and mail.id < 0:
			r = cc.s( cc.mails ).take( mail.id )
			if not issub( r, Failure ):
				cc.sn( cc.mails ).rem( mail.id )
				mail.id = 0
	cc.mails.s = filter( lambda x:x.id != 0 , cc.mails.s )
	cc.sn( cc.items ).sort( )

class Client:
	pw = "3281044"
	login_user = 0
	def onNotify( self, cc, o ):
		n = o.__class__.__name__
		t = type ( o )
		if t == Buff:
			if len(o.s) > 5 and o.s[5] and len( o.s[5] ) > 1:
				bb = o.s[5]
				for i in range( 0, len( bb), 2 ):
					k = bb[i]
					v = bb[i+1]
					if k >=0 and v >=0:
						if k == 11: print "ap",v
				#		print "属性:", k, v
						getattr( cc.ch, "as")[k] = v

		fn = "on_" + n
		if hasattr( self, fn ):
			callLater( 0, getattr( self, fn ), cc, o )
	
	def on_Mail( self, cc, o ):
		check_mail( cc )

class X2(Client):
	user = "xkou"
	pw = ""
	sells = ()
	sell_npc = 0
	ss = [ [], [1,1,1 ], [1,1,0,1], [ 1,1,1 ] ] # 25
	ss = [ [], [1,1], [1,1,1], [1,1,0,1] ]  # 30
	ss = [ [], [1,1,1,1], [1,1] ] # 20
	ss = [ [], [1,1,1] ]
	level = 0

	def sell_item( self, cc ):
		buy = NpcBuy( 0, Npc( cc.ch.place, 0, self.sell_npc ) )
		buy.owner.owner.owner = Fb( cc.ch.place )
		for it in cc.items.s:
			if it and it.idx in self.sells:
				cc.s( buy ).sell( it.id ,True )

	def onStart( self, cc ):
		self.cc = cc
	#	self.ss = [ [], [1,1,1], [1,1,0,1] ]  # 25
	#	self.ss = [ [], [1,1], [1,1,1], [1,1,0,1] ]  # 30
	#	self.ss = [ [], [1,1,1,1], [1,1] ] # 20
	#	self.ss = [ [], [1,1], [1,1,1] ]  # 35
	#	self.ss = [ [], [1,1,1] ]
		self.needBuff = False
		self.start = time.time()
		ch = login( cc, self.user, self.pw, self.login_user )
		print "位置:", ch.place, ch.scene, ch.fb
		self.target = 0
		self.lastSend = 0
		if ch.st :
			print "状态", ch.st
			ch.st.owner = ch
			if issub( ch.st , Die ):
			#	cc.s( ch.st ).yuan()
				pass
			else:
				if cc.pk:
					self.on_Pk(cc,  cc.pk )
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
		
		self.arangeBag( cc )

		self.sell_item( cc )

		walk( cc, 0, 1, self.level )
		self.npcc = 0	
		self.scene  = 1
		
		huntfb( cc, self.scene , self.npcc )
		self.target = 0

	def arangeBag( self , cc ):
		cc.s( cc.items ).sort()
		print "包裹剩余:", cc.items.empty()
	
		for it in cc.itemEs.s:
			if type(it) == ItemJ:
				print it.idx , "耐久度", it.f
				if it.f < 300:
					vv = cc.items.find( 45416 )
					if vv:
						print "使用日月精华:" , cc.s( it ).feed( vv.id )

			elif type(it) == ItemP:
				print it.idx, "饱食度", it.f
				if it.f <300:
					vv = cc.items.find(45408)
					if vv: 
						print "使用麦子:" , cc.s( it ).feed( vv.id )

	def on_Pk( self, cc, o ):
		
		self.pk = o
		setattr( cc.ch, "as", o.cs[ str(cc.ch.id) ][8] )
		
		aas = getattr(cc.ch,"as")
		
		print ">>> PK", "t:", self.pk.t,  aas[11], aas[12]
		self.do_pk( cc )
	
	def do_pk(self, cc):
		aas = getattr(cc.ch, 'as')
		v1 = aas[11]
		v2 = aas[12] + 50
		t = 0
		if cc.getServerTime() >= self.pk.t - v1*10:
			t = 0
		else:
			t = (self.pk.t - v1*10 - cc.getServerTime() )/1000
		print "下次时间",t, self.cc.dtime
		
	#	if t < 2.5 : t = 2.5
		callLater( t, self.pk_auto )

	def send_auto( self ):
		cc.sn( "Pk" ).auto()

	def on_ItemJ ( self, cc, o ):
		pass

	def on_ItemP( self, cc, o ):
		pass

	def on_Die( self, cc, o ):
		o.owner = cc.ch
	#	cc.sn( o ).yuan()
		print cc.ch.name , "死亡"
		callLater( 1, walk, cc, self.scene, 0, self.level )
		callLater( 3, self.reset )

	def on_Hunt( self, cc, o ):

		if o.done == 0:
			print cc.ch.name, "结束", time.time()-self.start
			self.cc.ch.st = None
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
		elif o.done == -1:
			self.start = time.time()
			pass
	
	def pk_auto( self ):
		
		self.lastSend = time.time()
		self.cc.sn( "Pk" ).auto()
					
	def on_Buff( self, cc, o ):
		if len(o.s) > 7 and o.s[0] == cc.ch.id:
		
			getattr(cc.ch,"as")[11] = o.s[7]
			self.do_pk(cc)

		if time.time() - self.lastSend > 10:
			self.pk_auto()
		
class MOVE( Client ):
	user = "xkou"
	pw = "3281044"
	
	ps = [1020, 1021, 1020, 1021, 1020]
	

	def move( self, cc, city ):
		print s1( cc.ch.name ), "move", city
		cc.s( "Move" ).go( city )
		
		p = self.cl.receive()
		if p != city :
			print "move error", city, p

	def onStart( self, cc ):
		ch = login( cc, self.user, self.pw, self.login_user )
		if ch.st:
			ch.st.owner = ch
			print "状态", ch.st
			
		self.cl = SL.channel()
		place = cc.ch.place
		print "当前城", place
		start = False
		for p in self.ps:
			if start:
				self.move( cc, p )

			if p == place:
				start = True
		print s1( cc.ch.name ) , "结束"
	
	def on_Move( self, cc, o ):
		if o.done == 0:
			self.cl.send(o.p)
		

class MAKE( Client ):
	user = ""
	pw = "3281044"
	pick_npc = 2
	pick_scene = 0
	make_npc = 1
	make_scene = 3
	
	do_fuse = True
	fuse_scene = 4
	fuse_npc = 4
	fuse_id = 4
	
	acts = (1, (), 2, () )
	drop_is = (  )
	make_is = ( )
	sell_is = ( 31092, 31098, 31104, 31110 )
	sell_is = ( 30903, 30905, 30927, 30060 )
	pick_idle = 56
	make_count = 5
	def onStart( self, cc ):
		ch = login( cc, self.user, self.pw, self.login_user )
		if ch.st:
			ch.st.owner = ch
			
		callLater( 60, self.checkBag, cc )

		walk( cc, ch.scene, self.pick_scene )
		pick( cc, self.pick_scene, self.pick_npc  )
	
	def checkBag( self, cc ):
		if cc.items.empty() < 10:
			self.onFull( cc )
		callLater( 60, self.checkBag, cc )

	
	def onFull ( self, cc ):
		walk( cc, self.pick_scene, self.make_scene )
		print "acts",self.acts
		cc.s ( cc.items ).sort()
		ch = cc.ch
	
		for actid, ms in self.acts:
			mk = NpcMake( actid, Npc( cc.ch.place, self.make_scene, self.make_npc ) )
			for m in ms:
				print m, cc.items.num( makes[m] )/self.make_count
				for i in range( cc.items.num( makes[m] )/self.make_count ):
					r = cc.s( mk ).go( m, None, False )
					if issub( r,  Failure) : break
					r1 = r[1]
					
					if issub(r1, Item) and  r1.idx in self.sell_is:
						buy = NpcBuy( 0, Npc( cc.ch.place, self.make_scene, self.make_npc-1 ) )
						cc.s( buy ).sell( r1.id, True )

		cc.s ( cc.items ).sort()

		# fuse
		walk( cc, self.make_scene, self.fuse_scene )
		if self.do_fuse:
			fnpc = NpcFuse( self.fuse_id, Npc( cc.ch.place, self.fuse_scene, self.fuse_npc ) )
			for e in cc.items.s:
				if e and type(e) == ItemEg:
					print "熔炼", e.idx
					rs = cc.s( fnpc ).go(e.id )

		walk( cc, self.fuse_scene, self.pick_scene )
		pick( cc, self.pick_scene, self.pick_npc )

	def on_Pick( self, cc, o ):
		
		ch = cc.ch
		if o.i:
			if cc.items.empty() <= self.pick_idle:
				print s1( ch.name ), "包裹满2"
				if cc.items.empty() ==0 :
					cc.sn( cc.items.s[10] ).drop( cc.items.s[10].n )
				callLater( 2, self.onFull, cc )
				return

			if o.i.idx in self.drop_is:
				print s1( ch.name ) , "drop", o.i.idx, o.i.n
				cc.sn( o.i ).drop( o.i.n )
			else:
				cc.onGetItem( o.i )
			pick( cc, self.pick_scene, self.pick_npc )
		
		elif o.i == False:
			print s1( ch.name ), "包裹满"
			
			cc.sn( cc.items.s[10] ).drop( cc.items.s[10].n )
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

			for item in cc.items.s:
				if item == None : continue
				if item.idx not in self.save_items:
					cc.s( self.npcbuy ).sell( item.id, True )
			cc.s( cc.items ).sort()
			callLater( 0.4, self.doit )
			return

		print s1( cc.ch.name), "买入", rs[1].idx

		rs = cc.s( self.npcfuse ).go( rs[1].id )
		if issub( rs, Failure ): return self.doit()
		print s1( cc.ch.name ), "熔炼", rs[0]
		callLater( 0.4, self.doit )

	def onStart( self, cc ):
		self.cc = cc
		ch = login( cc, self.user, self.pw, self.login_user )
		
		self.npcbuy = NpcBuy( 0, Npc( ch.place, self.buy_scene, self.buy_npc ) )
		self.npcfuse = NpcFuse( self.fuse_id, Npc( ch.place, self.fuse_scene, self.fuse_npc ) )
		self.doit( )

		

class HUNT( Client ):
	user = "xkou"
	pw = "3281044"
	hunt_npc = 1
	hunt_scene = 1

	skills = [ (0, 51208, 4.5 ), (100, 51208, 2.7) ] 
	heals = [ (0,50177, 4.5), (8, 56833, 8 ) ]

	def onStart( self, cc ):
		self.cc = cc
		ch = login( cc, self.user, self.pw, self.login_user )
		
		if ch.st :
			ch.st.owner = ch
			if ch.st.t == 0:
				hunt ( cc, self.hunt_scene, self.hunt_npc )
		else:
			walk( cc, ch.scene, self.hunt_scene )
			hunt ( cc, self.hunt_scene, self.hunt_npc )
			self.hunt = object()
			self.cs=None
		cc.pk = 0
		self.mp = 0
		self.hp = 1000
		self.cs ={} 
		callLater( 5, self.useSkill )
		
	
	def petLoop( self ):	
		cc = self.cc
		if cc.pk :
			cc.sn( cc.itemEs.s[20] ).use()
		callLater( 5, self.petLoop )
	
	def useSkill( self ):
		cc = self.cc
		if  cc.pk: 
			for c in self.cs:
				toc = c
				if toc > 0: continue
				break
			else:
				callLater( 5, self.useSkill )
				return
			
			sk = 0
			if self.hp>500:
				for k in self.skills:
					if self.mp >= k[0]  :
						sk = k
			
			else:
				
				for k in self.heals:
					if self.mp >= k[0]:
						sk = k
						toc = cc.ch.id
				

			cc.sn( SkillU( sk[1] ) ).use( toc )
			callLater( sk[2], self.useSkill )
		else:
			callLater( 5, self.useSkill )

	def on_Hunt( self, cc, o ):
		print ">>> hunt", ttime( o.t ), "now:", cc.getServerTime(), long(cc.getServerTime() - ttime( o.t ))
		if o.done == -1:
	#		print "hunt 时间 ", ttime( o.t ), "now:", cc.getServerTime()
	#		print "pk start"
			cc.pk = 1
		elif o.done == 0:
	#		print "结束"
			cc.pk = 0
			cc.s ( cc.items ).sort()
			print "包裹:", cc.items.empty() 
			slwait(1)
			hunt ( cc, self.hunt_scene, self.hunt_npc )


	
	def on_Buff( self , cc, bf ):
		c = bf.s[1]
	#	print bf
		if bf.s and len( bf.s) > 5 and bf.s[5] and len(bf.s[5]) > 1 and c in self.cs :
			r = self.cs[c]
			attrs = []
		#	print "buff.s[5]", bf.s[5]
			for i in range( 0,len( bf.s[5] ), 2 ):
				k = bf.s[5][i]
				v =  bf.s[5][i+1]
				if k == 4:
					if c == cc.ch.id:
						self.hp = v
					attrs.append( "hp: "+  str(v) )
					if v == 0:
						del self.cs[c]
				elif k == 7:
					if c == cc.ch.id:
						self.mp = v
					attrs.append( "mp: " +  str(v) )
					
			print c ,s1(r[1]), " ".join(attrs), "hp:",self.hp,"mp:", self.mp
			
		
	def on_Pk( self, cc, pk ):
		print ">>> PK", "t", ttime(pk.t)
		self.cs ={} 
		for k in pk.cs:
			v = pk.cs[k]
			self.cs[int(k)] = v
			if int(k) > 0 : 
				self.mp = v[8][7]
				self.hp = v[8][4]
				print int(k), s1(v[1]), v[8][4], v[8][7]
		
		
		
			

class PICK(Client):
	user = "xkou2"
	pick_npc = 2
	pick_scene = 0
	buy_npc = 0
	buy_scene = 1
	saves = ( 30951, )
	send_to = "[1服]www".decode("gbk").encode("utf8")
	sends = [  ]
	do_sell = True
	def onStart( self, cc ):
		ch = login( cc, self.user, self.pw, self.login_user )
		for it in cc.items.s:
			if it and it.idx in self.sends and it.n >= 20:
				print s1(ch.name) , "发送邮件"
				mail( cc, self.send_to, [it.id] )
				slwait(0.4)
		if ch.st:
			return
		if ch.gold > 100000:
			mail( cc, self.send_to, [], ch.gold - 100000 )

		walk( cc, ch.scene , self.pick_scene )
		pick( cc, self.pick_scene, self.pick_npc )
		
	def on_Pick( self, cc, o ):
		ch = cc.ch
		if o.done == 0:
			pick( cc, self.pick_scene, self.pick_npc )
		if o.i:
			cc.onGetItem( o.i )
			if o.i.idx in self.sends and o.i.n == 20:
				mail( cc, self.send_to, [o.i.id] )
		if o.i == False:
			print s1(ch.name), "包裹满"
			cc.s( o ).stop()
			if self.do_sell : sell ( cc, self.buy_scene, self.buy_npc, self.saves )
			pick ( cc, self.pick_scene, self.pick_npc )
		




class Hunt_OK(HUNT):
	user = "xkou"
	hunt_npc =3
	hunt_scene = 0

class Make_WWW2( MAKE):
	user = "xkou2"
	pick_npc = 1
	pick_scene = 5
	make_npc = 1
	make_scene = 4
	pick_idle = 30
	make_count = 20
#	acts = ( (4, (31092,31098, 31104, 31110) ), )  # 低
	acts = ( (4, (31096,31102, 31108, 31114) ), )  # 高
#	acts = ( (2, (30903, 30905, 30927)),( 1, (30060,)) )  # 草
	sell_is = ( 30903, 30905, 30927, 30060 )

class Make_WWW( MAKE):
	user = "xkou2"
	pick_npc = 1
	pick_scene = 5
	make_npc = 3
	make_scene = 4
	pick_idle = 50
	make_count = 5
#	acts = ( (4, (31092,31098, 31104, 31110) ), )  # 低
	acts = ( (4, (31096,31102, 31108, 31114) ), )  # 高
	acts = ( (2, (30903, 30905, 30927)),( 1, (30060,)) )  # 草
	sell_is = ( 30903, 30905, 30927, 30060 )

class Make_LIJUN( Make_WWW ):  
	user = "lijun8873"
	pw = "13170132938"
	pick_idle = 45

class Make_TCDJ( MAKE):  # 江陵
	user = "xkou1"
	pick_npc = 5
	pick_scene = 0
	make_npc = 1
	make_scene = 1
	acts = ( (1, ( 30961,30964,30967,30970,30973 )), ) # 铜
	acts = ( (0, ( 30976,30979,30982,30985,30988 )), ) # 铁



class Pick_TCDJ( PICK ):  
	user = "xkou1"
	pick_npc = 2
	pick_scene = 0
	buy_npc = 0
	buy_scene = 1

	saves = ( 30951, 31067, 31815, 31801, 31047 )
	saves = ( 30970, 30971, 31836, 31847 )
#	send_to = "www"
#	sends = [ 31040, 31041, 31042, 31043, 31044 ]
#	sends = [ 31047 ]

class Pick_WWW( PICK ):  
	user = "xkou2"
	pick_npc = 5
	pick_scene = 0
	buy_npc = 0
	buy_scene = 1
	saves = ( 30970, 30971, 31836, 31847 )

class Pick_TT( PICK ):
	user = "xkou3"
	pick_npc = 2
	pick_scene = 0
	buy_npc = 0
	buy_scene = 1
	saves = ( 30970, 30971, 31836, 31847 )
#	send_to = "www"
#	sends = [ 31040, 31041, 31042, 31043, 31044 ]
#	sends = [ 31047 ]



class Pick_KWY1( Pick ):
	user = "cxlby"
	pw = "by121314"
	login_user = 1
	pick_npc = 1
	pick_scene = 5
	buy_npc = 0
	buy_scene = 1
	saves = (  )
	send_to = "[1服]www".decode("gbk").encode("utf8")
	sends = [ 31068, 31074, 31080, 31086] # 31072, 31078, 31084, 31090
#	sends = [ 31047 ]
	sends = [ 30902, 30904, 30926, 30950 ]  # 草
#	ps = [1019,1016,1017,1024,1022] # 从许昌移动
	sends = [ 31072, 31078, 31084, 31090 ]

class Pick_KWY( PICK ):
	user = "cxlby"
	pw = "by121314"
	login_user = 1
	pick_npc = 2
	pick_scene = 0
	buy_npc = 0
	buy_scene = 1
	saves = (  )
	send_to = "[1服]www".decode("gbk").encode("utf8")
	sends = [ 31068, 31074, 31080, 31086] # 31072, 31078, 31084, 31090
#	sends = [ 31047 ]
	sends = [ 30902, 30904, 30926, 30950 ]  # 草
#	ps = [1019,1016,1017,1024,1022] # 从许昌移动
	sends = [ 31072, 31078, 31084, 31090 ] # 高等
	sends = []

class Pick_X4( Pick_KWY ):
	user = "xkou4"
	pw = "3281044"
	login_user = 0
	


class Pick_X5( Pick_X4 ):
	user = "xkou5"

class Pick_X6( Pick_X4 ):
	user = "xkou6"

class Pick_X7( Pick_X4 ):
	user = "xkou7"

class Pick_X8( Pick_X4 ):
	user = "xkou8"

class Pick_X9( Pick_X4 ):
	user = "xkou9"

class Pick_X10( Pick_X4 ):
	user = "xkou10"

class Pick_X11( Pick_X4 ):
	user = "xkou11"

class Pick_X12( Pick_X4 ):
	user = "xkou12"

class Pick_X13( Pick_X4 ):
	user = "xkou13"

class Pick_X14( Pick_X4 ):
	user = "xkou14"

class Pick_X15( Pick_X4 ):
	user = "xkou15"

class Pick_X16( Pick_X4 ):
	user = "xkou16"

class Pick_X17( Pick_X4 ):
	user = "xkou17"

class Pick_X18( Pick_X4 ):
	user = "xkou18"

class Pick_X19( Pick_X4 ):
	user = "xkou19"

class Pick_X20( Pick_X4 ):
	user = "xkou20"

class Pick_X21( Pick_X4 ):
	user = "xkou21"


class Pick_TU( Pick_KWY ):
	user = "tu3206"
	pw = "003206"
	login_user = 0

class Pick_AAA( Pick_KWY ):
	user = "cai"
	login_user = 2

class Pick_LABA( Pick_KWY ):
	user = "liao"
	login_user = 0

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

class Online_X4( ONLINE):
	user = "xkou4"

class Online_X5( ONLINE):
	user = "xkou5"

class Online_X6( ONLINE):
	user = "xkou18"

	def onStart( self, cc ):
		print "ssssssssssssssssssssssssssss"
		ONLINE.onStart( self, cc )
		if cc.ch.st:
			cc.ch.st.owner = cc.ch
			if type(cc.ch.st) == Die :
				cc.sn( cc.ch.st ).yuan()
		
		walk( cc, 0, 4 )


class Online_X7( Online_X6):
	user = "xkou19"


class Online_X8( Online_X6):
	user = "xkou20"

class Online_X9( Online_X6):
	user = "xkou21"

class FB_OK123( X2 ):
	user = "xuke"

class FB_BIRD( X2 ):
	user = "bird"
	ss = [ [], [1] ]

class FB_OK( X2 ):
	user = "xkou"
	login_user = 2
	ss = [ [], [1,1,1],[1,1,0,1] ]
	ss = [ [], [1,1,1,1], [1,1] ]
	ss = [ [], [1,1] ]
	ss = [ [], [1] ]
	level = 0
	sells = (30110, 35520, 35521, 35522, 35523, 35524, 35525, 35526, 35527, 35528, 35529, 35530, 35531, 35532, 35533, 35534, 35535, 35536, 35537, 35538, 35539, 35540, 35541, 35542, 35543, 35544, 35545, 35546, 35547, 35548, 35549, 35550, 35551, 35552, 35553, 35554, 35555, 35556, 35557, 35558, 35559, 35560, 35561, 35562, 35563, 35564, 35565,35736,35737,35738,35739,35740,35741,35742,35743,35744)
	sell_npc = 0

class FB_LMS( X2 ):
	ss = [ [], [1] ]
	user = "zbs99899"
	pw = "6481703"
	level = 0

class FB_FATHER( X2 ):
	user = "cxlby"
	pw = "by121314"
	ss = [ [], [1,1,1],[1,1,0,1] ]
	ss = [ [], [1,1,1,1], [1,1] ]
	ss = [ [], [1,1] ]
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


	
	callLater( 0, work, Pick_TCDJ )
#	run()
#
#	1/0
#
	callLater( 0, work, Make_WWW )
	callLater( 0, work, Pick_KWY )
	callLater( 0, work, Pick_TT )
#	callLater( 0, work, Pick_LABA )
#	callLater( 0, work, Pick_AAA )
#	callLater( 0, work, Pick_TU )
	callLater( 0, work, Pick_X4 )
	callLater( 0, work, Pick_X5 )
	callLater( 0, work, Pick_X6 )
	callLater( 0, work, Pick_X7 )
	callLater( 0, work, Pick_X8 )
	callLater( 0, work, Pick_X9 )
	callLater( 0, work, Pick_X10 )
	callLater( 0, work, Pick_X11 )
	callLater( 0, work, Pick_X12 )
	callLater( 0, work, Pick_X13 )
	callLater( 0, work, Pick_X14 )
	callLater( 0, work, Pick_X15 )
	callLater( 0, work, Pick_X16 )
	callLater( 0, work, Pick_X17 )
	callLater( 0, work, Pick_X18 )
	callLater( 0, work, Pick_X19 )
	callLater( 0, work, Pick_X20 )
	callLater( 0, work, Pick_X21 )

#
#	callLater( 0, work, FB_OK )
#	server2( "s003.tj.game5.cn", 3003, FB_BIRD )
#	callLater( 0, work, Hunt_OK )

#	callLater( 0, work, Fuse_TT )
	
#	server2( "s001.tj.game5.cn", 3001, Online_WWW )
#	server2( "s001.tj.game5.cn", 3001, Online_TCDJ )
#	server2( "s001.tj.game5.cn", 3001, Online_TT )
#	server2( "s001.tj.game5.cn", 3001, Online_OK123 )

	run()
	

