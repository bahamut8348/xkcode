import PSClient2 as PSClient

import time

class ps2event:
	p = None
	username=""
	password=""
	cs = None
	def OnValidateResult(self, byCode):
		print "Validate", byCode, self.p.GetError(byCode)
		pass
		
	def OnCreateSessionResult(self, dwCode):
		print "session,", dwCode, self.username
		if dwCode == 105000:
			#self.p.JoinRoom(2, 458773, 1048656, "test %s" % self.username,"")
			self.p.JoinLobbyRoom(40)
		
	
	def OnConnectIS(self, bConnect, is_id , reason, desp, islist):
		if not bConnect:
			self.p.ConnectToIS()
		else:
			
			print "Con [%s]" % self.username
			print self.p
			self.p.Login(self.username, self.password)
			pass
		
	def OnRegisterResult(self, errors, names):
		pass
	def OnRoomListRecv(self, Result, AreaID, RoomList, RoomType):
		pass
		
	def OnCreateRoomRecv(self, Callback, Result, Info):
		pass
	def OnJoinRoomRecv(self, Result, Info):
		print Result, self.username, self.p.GetError(Result)
		pass
	def OnRecvChatMsg(self, datatype, roomid, serverid, to, fromuser , msg):
		if fromuser == 1059:
			self.p.Chat(datatype,roomid,0,msg)
		pass
	def OnQuitRoomResult(self, Callback,  Result, RoomID, RoomInfo):
		pass
	def OnBuddyListRecv(self, _Result, BuddyList):
		pass
	def OnLoginMMSResult(self, Result):
		pass
	def OnDispatchGame(self, GameID, MaxUserNum):
		pass
	def OnLobbyServerConn(self,bconn, ls_id, dwServerType, ls_ver):
		if dwServerType in ( 524288, 64 ):
			self.cs = ls_id
			print "chat Server", self.cs, dwServerType
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
	def OnGameUserInfoChange(*p):
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
	client.SetAppInfo(1,2,1,1)
	client.SetEvent( ev )
	#client.SetLog(mlog)
	client.SetIS( PSClient.str2addr("tcp://66.98.248.34:7789") )
	client.ConnectToIS()
	yield 2
	while 1:
		client.Update()
		yield 2
	

if __name__ == "__main__":
	
	all=[]
	
	for e in range(200,400):
		client = myclient(e, 'ps_test%d' % e , u"123123")
		client.next()
		time.sleep(0.001)
		all.append(client)
	while 1:
		for client in all:
			client.next()
		
			time.sleep(0.002)
		
		