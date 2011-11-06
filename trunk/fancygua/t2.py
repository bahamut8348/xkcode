#! coding:utf-8

import jot
import socket
import struct

import stackless as SL

import fansy as FAN

from fansy import *



if __name__ == "__main__":
	
	def onConnect( cc ):
		u = cc.s("UserNames").signIn( "xuke", "123456" )
		chs = filter( lambda x : isinstance( x, Ch ), u )
	#	print "chs", len( chs )
		v = cc.s( cc.user ).signCh( 0, 1 )
		ch = cc.ch
		ch.items = v[2]
		ch.items.s = v[3]
		cc.name = ch.name
		print ch.name, ch.place, ch.scene
		
		if ch.st :
			ch.st.owner = ch
			print "×´Ì¬", ch.st, issub( ch.st, St )
			if issub( ch.st, Pk ):
				pass
			else:
				cc.sn( ch.st ).stop()

		
		npc = Npc( ch.place, 0, 6 )
		pick = NpcPick( 0, npc )
			
	#	cc.sn( pick ).go()
		
		npc = Npc( ch.place, 0, 5 )
		hunt = NpcHunt( 0, npc )
		cc.sn ( hunt ).go()
		
		
	
	def onNotify( cc, o ):
		ch = cc.ch
	#	print "notify", o
		if issub( o, Pick ) and o.done != 0:
			if o.i : 				
				cc.onGetItem( o.i )
			if cc.ch.items.isFull():
				print "°ü¹üÒÑÂú"
			else:
				cc.sn( o ).go()
		elif issub( o, Hunt ) :
			print ">> Hunt", o.done
			cc.sn( o ).go()
		
		elif issub( o, Pk ):
			print ">>", "Pk"
			cc.sn( o ).go( 0, 0 )
		elif issub( o, Err ):
			print "Error",o.hint
		else:
			print "\nNotify:", o
			

	s = FAN.server( "1.1.1.11", 3001, onConnect, onNotify )
	FAN.run()

