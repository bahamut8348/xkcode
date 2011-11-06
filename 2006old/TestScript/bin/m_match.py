import PSClient2 as PSClient

import time
import pymssql


class db:
	
	def __init__(self):
		self.con = pymssql.connect(host='localhost',user='sa',password='sa',database='match')
		self.cur = self.con.cursor()

	def execute(self, sql):
		self.cur.execute(sql)
		self.con.commit()

	def add(self, roomid):
		sql = "insert into result (roomid) values(%d)" % roomid
		self.execute(sql)
		
	def remove(self, roomid):
		sql = "delete from result where roomid = %d" % roomid
		self.execute(sql)
		

class ps2event:
	p = None
	username=""
	password=""
	cs = None
	self.db = db()
	
	def OnValidateResult(self, byCode):
		print "Validate", byCode, self.p.GetError(byCode)
		pass
		
	def OnCreateSessionResult(self, dwCode):
		print "session,", dwCode, self.username
		if dwCode == 105000:
			#self.p.JoinRoom(0, self.cs, 1048587, "test %s" % self.username, 0,"")
			self.p.CreateRoom(0, 3, self.username,  self.username+"'s room", 1, 0,"",5)
		
	
	def OnConnectIS(self, bConnect, is_id , reason, desp):
		print "Con [%s]" % self.username
		print self.p
		self.p.Login(self.username, self.password)
		pass
		
	def OnRegisterResult(self, errors, names):
		pass
	def OnRoomListRecv(self, Result, AreaID, RoomList, RoomType):
		pass
		
	def OnCreateRoomRecv(self, Callback, Result, Info):
		if 106000 ==  Result:
			self.p.StartGame(0,Info.Id)
			self.db.add(RoomID)
			
			
	def OnJoinRoomRecv(self, Result, Info):
		print Result, self.username, self.p.GetError(Result)
		pass
	def OnRecvChatMsg(self, datatype, roomid, to, fromuser , msg):
		pass
	def OnQuitRoomResult(self, Result, RoomID, RoomInfo):
		pass
	def OnBuddyListRecv(self, _Result, BuddyList):
		pass
	def OnLoginMMSResult(self, Result):
		pass
	def OnDispatchGame(self, GameID, MaxUserNum):
		pass
	def OnLobbyServerConn(self,bconn, ls_id, dwServerType, ls_ver):
		if dwServerType==524288:
			self.cs = ls_id
			print "chat Server", self.cs
		pass
	def OnQueryNewsRecv(self, Callback, Result, content, AddTime):
		pass
	def OnConfigUserSession(self, Callback, Result, SessionType, userPSUID):
		pass
	def OnQuerySessionResult(self, Callback, Result, SessionType, userPSUID, content):
		pass
	def OnQuerySysinfoRecv(self, Callback, sessionNum, chatRoomNum):
		pass
	def OnUserInvited(self, InviterPSUID, RoomName, RoomID, ChatServer, usepass, RoomType):
		pass
	def OnInviteUserReturn(self, InviteeID, InviteeName, Result, RoomID):
		pass
	def OnSetRoomInfoResult(self, Callback, Result, RoomID):
		pass
	def OnRoomInfoChange(self, RoomID, info):
		pass
	def OnSetUserInfoResult(self, Callback, Result, RoomID):
		pass
	def OnGameUserInfoChange(self, RoomID, UserPSUID, Info):
		pass
	def OnSendGameCommandResult(self, Callback, Result, RoomID):
		pass
	def OnGameCommandRecv(self, RoomID, Sender, cmd):
		pass
	def OnStartGameResult(self, Callback, Result, RoomID):
		pass
	def OnStartGameRecv(self, RoomID, StartGameInfo):
		pass
	def OnMatchGameResult(self, Result, RoomID, GameID, Token, saddr):
		print self.username, "match:", self.p.GetError(Result), GameID 
		self.db.remove(RoomID)
		self.p.QuitRoom(0,RoomID)
		self.p.CreateRoom(0, 3, self.username,  self.username+"'s room", 1, 0,"",5)
		pass
	def OnQuerySession(self, Callback, result, Psuid, SessionType, content):
		pass
	def QueryBuddySession(self, Callback, BuddyPSUID, SessionType):
		pass
	def OnDeleteBuddyRes(self, Callback, Result, BuddyAccoutID):
		pass
	def OnAddBuddyRes(self, Callback, Result, std, BuddyAccID, BuddyUid):
		pass
	def OnBuddyStateChange(self, ChangeType, BuddyPSUID, BuddyAccountID):
		pass
	def OnAddBuddyReq(self, Callback, name, name2, BuddyID, BuddyAccID):
		pass
	def OnQueryAccountInfoRecv(self, dwCallback, Result):
		pass
	def OnModifyAccountInfoRecv(self, dwCallback, Errors):
		pass
	def OnPasswordChangeRecv(self, dwCallback, errors):
		pass
	def OnSendPacketErrorRecv(self, Result):
		pass
	def OnUserListRecv(self, RoomID, SyncType,UserList):
		pass

def mlog(s):
	print `s`

def myclient(session = 1, username="", password="" ):
	ev = ps2event()
	client = PSClient.PSClient(session)
	ev.p = client
	ev.username = username
	ev.password = password
	client.SetAppInfo(1,3,1,1)
	client.SetEventCb( ev )
	#client.SetCBLog(mlog)
	client.SetIS( PSClient.str2addr("tcp://192.168.1.145:7788") )
	client.ConnectToIS()
	yield 2
	while 1:
		client.Update()
		yield 2
	

if __name__ == "__main__":
	
#	d =db()
#	d.add(9)
#	d.add(110)
#	d.remove(110)
#	#0/0
	
	all=[]
	
	for e in range(500,550):
		client = myclient(e, 'ps_F%d' %e , u"xxsdfs")
		client.next()
		time.sleep(0.1)
		all.append(client)
	while 1:
		for client in all:
			client.next()
		
		time.sleep(0.01)
		
		