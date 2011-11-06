from PS import PS
import PSClient2 as psclient

import time
from random import *

import functools


class pscallback:
	ps = None
	args = None
	arealist = None
	joinedrooms = None
	name = None
	def OnLobbyServerConn(self, bconn, lsid, servertype, serverver):
		pass
	
	def OnConnectIS(self, bconn, isid, reason, desps):
		print self.name, "onconnect", bconn
		if bconn:
			self.ps.login(self.args)
	
	def OnValidateResult(self, code):
		pass
	
	def OnCreateSessionResult(self, code):
		#self.args['logined'] = True
		self.ps.queryAreaList( dict(areaid=1) )
		print "CreateSession", code, self.name
	
	def OnRoomListRecv(self, result, areaid,  roomlist, roomtype):
		for e in roomlist:
			r = Room()
			r.id = e.Id
			r.chatserver = e.ChatServer
			self.arealist[areaid].rooms[e.Id] = r
			#print r.chatserver
		
		self.args['logined'] = True
		print self.name,"OK"
	
	def OnJoinRoomRecv(self, ret, info):
		if  ret == 106010:
			r = Room()
			r.id = info.Id
			r.chatserver = info.ChatServer
			self.joinedrooms[r.id] = r
	
	def OnCreateRoomRecv(self, callback, ret, info):
		if ret == 106000:
			r = Room()
			r.id = info.Id
			r.chatserver = info.ChatServer
			self.joinedrooms[r.id] = r
	
	def OnRoomInfoChange(self, roomid, info):
		pass
	
	def OnUserListRecv(self, roomid, synctype, UserList):
		#for e in UserList:
			#print e
		pass
	
	def OnQuitRoomResult(self, ret, roomid, roominfo):
		if ret==106050:
			del self.joinedrooms[roomid]
	
	def OnRecvChatMsg(self, datatype, roomid, to, fromuser , msg):
		#print "got a msg"
		#self.ps.
		pass
	
	def OnGameUserInfoChange(self, roomid, userpsuid, gameuserinfo):
		#print "info change"
		pass
	
	def OnGameCommandRecv(self, roomid, sender, gamecmd):
		#print "game command recv"
		pass
		
	def OnAreaListRecv(self, result, areaid, alist):
		if areaid == 1:
			for e in alist:
				a = Area()
				a.id = e.Id
				self.arealist[a.id]=a
				self.ps.queryAreaList( dict(areaid=a.id) )
		else:
			for e in alist:
				a = Area()
				a.id = e.Id
				self.arealist[a.id] = a
				self.ps.queryRoomList( dict(areaid=a.id, roomtype=-1) )

class User:
	uid = 0

class Room:
	id = 0
	users = []
	chatserver = None
    
	

class Area:
	id = 0
	rooms = {}

def mylog(s, name):
	#print 
	pass


def getrand(l, base=0):
	try:
		return l[randint(base,len(l)-1)]
	except:
		return 0
		

	
def update_areaid(args, arealist, joinedrooms):
	args['areaid'] = getrand(arealist.keys())

def update_areaidroomid(args, arealist, joinedrooms):
	update_areaid(args, arealist, joinedrooms)
	args['roomid'] = getrand( arealist[ args['areaid'] ].rooms.keys() )
	args['chatserver'] = arealist[ args['areaid'] ].rooms[args['roomid']].chatserver
	args['roomname'] = "%d" % int(time.time() * 100)
	args['nickname'] = "%d" % int(time.time() * 100)

def update_joinedroomid(args, arealist, joinedrooms):
	args['roomid'] = getrand(joinedrooms.keys())

def update_content(args, arealist, joinedrooms):
	update_joinedroomid( args, arealist, joinedrooms)
	args['content']=time.asctime()
		


def clientinstance(sessionid, u="ps_test", p="123123", cblog=None):
	cb = pscallback()
	cb.name = u
	arealist = {}
	joinedrooms = {}
	args = dict(appid=1, subid=3, majorver=0, subver = 0, logined = False)
	args.update( dict(callback=0,nickname=u,roomname="",roomtype=2,usepassword=0,maxuser=10) )
	args.update( dict(touser=psclient.PSUID(), datatype=1) )
	ps = PS(sessionid)
	cb.ps = ps
	cb.args = args
	cb.arealist = arealist
	cb.joinedrooms = joinedrooms
	yield 1
	if cblog: ps.setLog(cblog)
	ps.setAppInfo(args)
	args.update(dict(username=u, password=p))
	ps.setIS('tcp://192.168.1.122:7788')
	ps.setCallback(cb)
	ps.connectToIS()
	print u, "ConnectTo"
	commands = dict(      createRoom= "0,update_areaid")
	commands.update( dict(joinRoom  = "4,update_areaidroomid") )
	commands.update( dict(quitRoom  = "0,update_joinedroomid") )
	commands.update( dict(chat      = "20,update_content") )
	selstr = "".join(map(lambda x: ("%s," % x) * int(commands[x].split(",")[0]),  commands.keys() ) ).split(",")
	#print selstr
	while 1:
		oper = yield cb.args['logined']
		if oper == 0:
			break
		else:
			ps.update()
			
		if args['logined']:
			#print "login ok",'haha'
			#select a commands
			cmd = getrand( selstr[:-1] )
			#ps.quitRoom(args)
			#print cmd
			#continue
			for e in commands[cmd].split(",")[1:]:
				if e:
					eval("%s(args, arealist, joinedrooms)" % e)
			eval('ps.%s(args)' % cmd)
			
			
			
	
	print "finish", sessionid
		
		
if __name__ == "__main__":
	cs=[]
	for e in range(700,780):
		log = functools.partial(mylog, name=str(e))
		ci = clientinstance(e, "ps_F%d" % e, "xxsdfs", log)
		ci.next()
		ci.send(2)
		cs.append(ci)
		time.sleep(0.001)
	#ci2 = clientinstance(3, "ps_")
	while 1:
		for e in cs:
			if e.send(2):
				time.sleep(1)
			else:
				time.sleep(0.01)
			
			
		
	ci.send(0)
		