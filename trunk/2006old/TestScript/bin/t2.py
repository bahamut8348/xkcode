from PSClient2 import *
import PS
ps=PSClient(1)

class messagecb:
	a = 0
	def OnConnectIS(self, bConnect, uid, t, desc):
		
		if bConnect:
			print ">>> OnConnectIS",bConnect, uid, t,` desc`
			print "AccountID:", uid.AccountID, uid.SSID, uid.TimeStamp
			ps.Login(u"ps_xu.ke",u"psps123")
			#ps.Disconnect()
			#ps.QueryNews(0)
		else:
			print "disconnect"
			ps.ConnectToIS()
			pass
		
	def OnValidateResult(self, code):
		print code, `ps.GetError(code)`
		if code == 104064:
			print "ERROR"
		else:
			print ps.GetError(code)
		#ps.Disconnect()
		#ps.ConnectToIS()
		
	def OnQueryNewsRecv(self, callback , result, news, addtime):
		print "addtime", addtime
		print `news.decode("utf16")`
		ps.QueryNews(0)
	def OnUserListRecv(self, *s):
		pass
	def OnCreateSessionResult(self, code):
		print "OnCreateSessionResult",code
		
		#ps.GetAreaList(1)
		ps.CreateRoom(0, 3 , u"d",u"f", 1, 0, u"df", 2)
		#ps.QueryNews(0)
		pass
		
	def OnLobbyServerConn(self, bConnect, ls_uid, ls_type, ls_ver):
		print "LobbyServerConn:", bConnect, ls_uid, ServerType2Str(ls_type), ls_ver
	
	def OnCreateRoomRecv(self, callback, result, info):
		ui = CP_Cmd_GameRoomUserInfo()
		ui.Name = "1"
		ui.Flags = 0xfffff;
		ui.Buffer =  "d"*30
		print "oncreate room", result, ps.GetError(result)
		ps.SetUserInfo(0,info.Id, ui)
	
	def OnGameUserInfoChange(self, RoomID, UserPSUID, UserInfo):
		print UserInfo.Buffer
		ui = CP_Cmd_GameRoomUserInfo()
		ui.Name = str(int(ui.Name)+1)
		ui.Flags = 0xfffff;
		ui.Buffer =  "d"*30
		#print "oncreate room", result, ps.GetError(result)
		ps.SetUserInfo(0,RoomID, ui)
		
		info = CP_Cmd_RoomInfo()
		
		ps.SetRoomInfo(1, RoomID,info)
	
	def OnRoomInfoRecv(self, *s):
		pass
		
	def OnAreaListRecv(self, ret, areaid, areas, callback):
		print ret, areaid, callback, areas
		T = threading.Thread(target=th_createroom,args=(areaid,))
		T.start()

def l(s):
	print s

if __name__ == "__main__":
	cb = messagecb()
	ps.SetCBLog(l)
	ps.SetAppInfo(1,3,1,1)
	ps.SetEventCb(cb)
	ps.SetIS( str2addr("tcp://192.168.1.122:7788") )
	ps.ConnectToIS()
	
	import time
	while 1:
		time.sleep(0.01)
		ps.Update()