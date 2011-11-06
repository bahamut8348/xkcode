import PS
import sys,time, os

class Event2(PS.ps2event):
	
	def OnCreateRoomRecv(self, callback, Result, info):
		self.p.OnCreateRoomRecv(callback, Result, info)
				

class testJoinRoom(PS.TestProcessor2):
	
	INFO = {"username":u"ps_test", "password":u"123123",
		"areaid":1, 'maxuser':100, 'roomname':"ll",
	}
	myareaid = 3
	room_usepass		= None
	room_hasJoined		= None
	room_maxuser		= None
	room_canJoin		= None
	testname = "Join Room Test"
	
	def dosomething(self):
		room = self.areas.values()[0].rooms.values()[0]
		self.p['roomid'] = room.id
		self.p['chatserver'] = room.chatserver
		desc = self.p['desc']
		if desc == u"success":
			self.p['usepass'] = True
			self.getps().joinRoom(self.p)
		elif desc == u"user has exist":
			self.p['roomid'], self.p['chatserver'] = self.room_hasJoined.Id, self.room_hasJoined.ChatServer
			self.getps().joinRoom(self.p)
		elif desc == u"wrong password":
			self.p['roomid'], self.p['chatserver'] = self.room_usepass.Id, self.room_usepass.ChatServer
			self.getps().joinRoom(self.p)
		elif desc == u"reach max user":
			self.p['roomid'], self.p['chatserver'] = self.room_maxuser.Id, self.room_maxuser.ChatServer
			self.getps().joinRoom(self.p)
		else:
			self.getps().joinRoom(self.p)
	
	def OnGameUserInfoChange(self, userid, uid, info):
		pass
	
	def OnCreateSessionResult(self, code):
		
		if 105000 == code:
			ev = Event2()
			ev.p = self
			self.setCallback2(ev)
			print "login ok", "areaid", self.areas.keys()[1]
			params = {'maxuser':100,"callback":1, "areaid": self.areas.keys()[1], "roomtype": 1, "usepass":True, "password":"11"}
			self.getps2().createRoom( params )
			params["callback"]	= 2
			params["usepass"]		= False
			params["maxuser"]		= 3
			self.getps().createRoom( params )
			params['callback']	= 3
			params['usepass']		= False
			params["maxuser"]		= 1
			self.getps2().createRoom( params )
		else:
			os.abort()
	
	def OnCreateRoomRecv(self, callback, result, info):
		if callback == 1:
			self.room_usepass = info
		if callback == 2:
			self.room_hasJoined = info
		if callback == 3:
			self.room_maxuser = info

		if self.room_usepass and self.room_hasJoined and self.room_maxuser :
			self.getps().queryAreaList({"areaid":1})
	
	def OnUserListRecv(self, roomid, synctype, userlist):
		pass

		
	def OnAreaListRecv(self, result, areaid, arealist):
		
		if len(arealist) == 0:
			self.getps().queryRoomList({"areaid":areaid, "roomtype":-1})
		else:
			area = arealist[0]
			self.p['areaid']=area.Id
			self.p['roomtype'] = -1
			self.getps().queryAreaList(self.p)
	
	def OnRoomListRecv(self, result, areaid, roomlist, roomtype):
		
		
		self.setReady(True)
		self.start()

	
	def OnJoinRoomRecv(self, result, roominfo):
		self.end(result)
	
if __name__ == "__main__":
	tj = testJoinRoom()
	tj.loop()
		