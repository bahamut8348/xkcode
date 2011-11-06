import psevent.PSEvent


class PSEvent2(PSEvent):
	c = None
	
	def send(self, ):
		pass
	
	def OnValidateResult(self, result ):
		self.c.send
		pass
	def OnCreateSessionResult(self, result):
		pass
	def OnConnectIS(self, bConnect, is_id, Reason, desp, pislist):
		pass
	def OnAreaListRecv(self, Result, AreaID, areaList):
		pass
	def OnRegisterResult(self, errors, names):
		pass
	def OnRoomListRecv(self, Result, AreaID, RoomList, RoomType):
		pass
	def OnCreateRoomRecv(self, callback, result, pRoomInfo):
		pass
	def OnJoinRoomRecv(self, Result, pRoomInfo):
		pass
	def OnRecvChatMsg(self, datatype, roomid, serverid, toUser, fromUser, msg):
		pass
	def OnQuitRoomResult(self, callback, result, roomid, pRoomInfoo):
		pass
	def OnLoginMMSResult(self, Result, matchserver):
		pass
	def OnDispatchGame(self, gameid, matchserver, strroombuffer, userlist):
		pass
	def OnLobbyServerConn(self, bShow, ls_id, dwServerType, ls_ver):
		pass
	def OnQueryNewsRecv(self, Callback, Result, msg, AddTime):
		pass
	def OnConfigUserSession(self, Callback, Result, SessionType, userPSUID):
		pass
	def OnQuerySessionResult(self, Callback, Result, SessionType, userPSUID, sessioninfo):
		pass
	def OnQuerySysinfoRecv(self, Callback, sessionNum, chatRoomNum):
		pass
	def OnQuerySysinfoRecv(self, callback, info):
		pass
	def OnUserInvited(self, roomid, roomserver, inviterUser, roomName, toroomid, toChatServerID, usepass, password, roomType, invitebuffer ):
		pass
	def OnInviteUserReturn(self, roomid, roomserver, inviteeUser, result, toroomid, toChatServerID, inviteebuffer ):
		pass
	def OnSetRoomInfoResult(self, callback, result, roomid, serverid):
		pass
	def OnRoomInfoChange(self, roomid, RoomInfo):
		pass
	def OnSetUserInfoResult(self, Callback, Result, RoomID):
		pass
	def OnRoomUserInfoChange(self, roomid, userid, UserInfo):
		pass
	def OnStartGameResult(self, callback, result, roomid):
		pass
	def OnStartGameRecv(self, roomid, startgameinfo):
		pass
	def OnMatchGameResult(self, result, roomid, gameid, token, saddr):
		pass
	def OnQuerySession(self, Callback, result, Psuid, SessionType, sessioninfo):
		pass
	def QueryBuddySession(self, Callback, BuddyPSUID, SessionType):
		pass
	def OnQueryAccountInfoRecv(self, dwCallback, Result, UserInfo):
		pass
	def OnModifyAccountInfoRecv(self, dwCallback, errors ):
		pass
	def OnPasswordChangeRecv(self, dwCallback, errors):
		pass
	def OnSendPacketErrorRecv(self, result ):
		pass
	def OnUserListRecv(self, roomid, syncType, userList):
		pass
	def OnISListRecv(self, Result, IsList):
		pass
	def OnFSListRecv(self, Callback, FsList):
		pass
	def OnUserInfoRecv(self, Callback, Result, RoomID, UserInfo, buffer ):
		pass
	def OnRoomInfoRecv(self, callback, result, roominfo, buffer):
		pass
	def OnDeleteBuddyRes(self, Callback, Result, BuddyAccoutID, desp):
		pass
	def OnAddBuddyRes(self, Callback, Result, BuddyAccoutID, desp):
		pass
	def OnBuddyListRecv(self, TotalNum, BuddyList):
		pass
	def OnBuddyStateChange(self, AccountID, ChangeType, Status, buffer):
		pass
	def OnAddBuddyReq(self, Callback, NickName, Reason):
		pass
	def OnConfigBuddyStatusRes(self, Callback, Result):
		pass
	def OnRoomUserInfoRecv(self, callback, result, roomid, serverid, userinfo, buffer ):
		pass
	def OnInit_Buddy(self, callback, result, buddyserverid):
		pass


