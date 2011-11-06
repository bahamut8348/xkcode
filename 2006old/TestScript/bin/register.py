import PSClient2 as PSClient
import time

info = PSClient.DetailInfo()
info.UserName	=	""
info.Password	=	"123123"
info.NickName	=	"sdfdf"
info.Location	=	1
info.City		=	"2sd"
info.Gender		=	1
info.Birthday.Year	  = 1986
info.Birthday.Month	  = 10
info.Birthday.Day		  = 2
info.Email			= ''
info.bNewsLetter	= 1


nn=1
class Callback():
	a = 0
	
	def OnRegisterResult(self, a, b):
		global nn
		print "OnRegisterResult", a, b
		if a:
			print map( lambda x: ps.GetError(x), a)
		info = PSClient.DetailInfo()
		info.UserName	=	"ps_XXX%d" % nn
		info.Password	=	"123123"
		info.NickName	=	"sdfdf"
		info.Location	=	1
		info.City		=	"ssd"
		info.Gender		=	1
		info.Birthday.Year	  = 0
		info.Birthday.Month	  = 0
		info.Birthday.Day		  = 0
		info.Email			= ''
		info.bNewsLetter	= 1
		ps.Register(info)
		nn+=1
		if nn==200:
			1/0
		pass

	def OnConnectIS(self, bConnect, a, t, desc):
		
		if bConnect:
			ps.Register(info)
		else:
			print "disconnect"
			ps.ConnectToIS(PSClient.DetailInfo())
			pass

	def OnLobbyServerConn(self, bConnect, ls_uid, ls_type, ls_ver):
		print "LobbyServerConn:", bConnect, ls_uid, PSClient.ServerType2Str(ls_type), ls_ver
	

cb = Callback()

ps = PSClient.PSClient(sessionid=3)
ps.SetAppInfo(1,3,1,1)
#ps.SetCBLog(log1)
ps.SetIS( PSClient.str2addr("tcp://192.168.1.13:7788") )
ps.SetEventCb(cb)
ps.ConnectToIS()

while 1:
	time.sleep(0.01)
	ps.Update()

