#! coding:utf8

from fansy import *
from twisted.python.failure import Failure

def spiritUp( cc ):
	cc.s( "Spirit" ).update( 0 )

def wuUp( cc, chname ):
	while 1:
		cc.sn( "Chat" ).exp( chname, 5000000000000000 )
		while 1:
			r = cc.s( cc.ch ).Exps( 200000000, 200000000, 200000000,0, 200000000 )
			if isinstance( r, Failure ):
				break

if __name__ == "__main__":
	
	def s2( n ):
		return n
		return unicode(n).encode("utf-8")

	def move( cc ):
		ch = cc.ch
		print cc.s( ch ).walk( 1 )
		print cc.s( ch ).walk( 1 )
		print cc.s( ch ).walk( 1 )
		print cc.s( ch ).walk( 0 )

	def onConnect( cc ):
#		u = cc.s("UserNames").signIn( "xuke", "123456" )
		u = cc.s("UserNames").signKey( 121297900, 14000000000,1,"c539fdb99ed7d9d3d899b2451c825536" )
		print cc.user
		cc.s( cc.user ).signCh( 2, 1 )

		ch = cc.ch
		chname = ch.name
		wuUp( cc, chname )
		chname = "星空闪闪"
		chname = "寿司公主"
		chs = ("吸油烟机", ) # "寿司公主", "爹爹爹","吸油烟机","索打索咪咪","女程序",唧唧歪歪
		
		cityn = "下邳"
		

		for chname in chs:
			cc.sn( "Chat" ).lv( chname, 50 )
			cc.sn( "Chat" ).guild( chname, cityn , 8 )
			
			# $sock.send( $ch, 'Exps', vs, this.doneOk, this, [$ch.wulv()] ) // [ 力量，聪慧, 根骨， 统帅， 生命 ]
			ws = [ 
			#	("翡翠龙20星",1) , 
			#	("万里独行20星",1),
			#	("血铜铎20星",1),
			#	("进化石",450 ),
			#	("紫微破片",500 ),
			#	("薤叶芸香",500 ),
			#	("入门级训练书",500),("专家级训练书",100),("大师级训练书",100),("高级聪明豆",100),("伯乐书",100),
			#	("GM的女仆装",1),
			#	("龙骧镇军堰月刀",1),("龙骧镇军碧玉配",1),("龙骧镇军霸王铠",1),("龙骧镇军燕翎盔",1),("龙骧镇军云封肩",1),("龙骧镇军雷魂戒",2),
			#	("彩炼石", 2 ), ("烁光绿虎眼",2), ("烁光绿水晶",2), ("烁光绿玉",2), ("烁光绿翡翠",2),
			#	("烁光五彩琉璃",4), ("烁光五彩水晶",4), ("烁光五彩玉髓",4), ("烁光补天石",4),
			#	("烁光补天石",1),("烁光往生石",1),("烁光五彩水晶",1),("烁光两仪石",1),
			#	("先锋明光铠", 3 ),
			#	("乱世9星",1),
			#	("秘录•单手专精",1),("秘录•双持专精",1),("秘录•防御专精",1),("秘录•恢复专精",1),
			#	("防御卷轴五级",20),("血抗卷轴五级",20),("毒抗卷轴五级",20),("火抗卷轴五级",20),
			#	("龙骧镇军霸王铠",1), ("龙骧镇军燕翎盔",1),("龙骧镇军云封肩",1),("碎龙神甲霸王铠",1), ("碎龙神甲燕翎盔",1),
			#	("绸缎包",3),
			#	("紫微破片",500),
			#	("通行令",1),
			#	("无当战甲8星",1),
			#	("举荐书•五品", 40), 
			#	("举荐书•六品", 40), 
			#	("举荐书•七品", 40),
			#	("举荐书•八品", 40),
			#	("举荐书•九品", 40),

			]
			for w, n in ws:
				pass
				cc.sn( "Chat" ).item( chname, s2(w), n )
			
			cc.sn( "Chat" ).prez( chname, 9999999999 )
			cc.sn( "Chat" ).kn( chname, 4 )
			cc.sn( "Chat" ).job( chname, 23020 )
			cc.sn( "Chat" ).gold( chname, 500000000 )
			for w in range(6):
				cc.sn( "Chat" ).ga( chname, w,  10000000 )

			
			cc.sn( "Chat" ).gz( chname, 12000000 )
			cc.sn( "Chat" ).gf( chname, 12000000 )

	#		cc.sn( "Chat" ).shopUp( 5000, "VIP锦囊", 101, "112" )
		#	cc.sn( "Chat" ).shopDown( 5000, "VIP锦囊" )
	
	def onDisconnect(  ):
		pass
	
	def onNotify( client, d ):
		print "notify",client, d

#	print getLoginStr( "xkou1","3281044")

#	s = server( "1.1.1.11", 3001, onConnect, onNotify, onDisconnect )
	s = server( "s999.tj.game5.cn", 3999, onConnect, onNotify, onDisconnect )
	run()


