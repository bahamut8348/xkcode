import PSClient2 as PSClient

import thread
#print dir(ps.AreaInfo)

#import threading
import time



def th_createroom(areaid):
	print "start create room, areaid", areaid
	for e in range(10000,20000):
		
		time.sleep(0.1)
		#ps.CreateRoom(e, areaid, unicode("nickname"), unicode("roomname"))

nn = 4000

class Callback():
    
	a = 0
	
	def OnRegisterResult(self, a, b):
		global nn
		print "OnRegisterResult", a, b
		info = PSClient.DetailInfo()
		info.UserName	=	"ps_F%d" % nn
		info.Password	=	"xxsdfs"
		info.NickName	=	"sdfdf"
		info.Location	=	1
		info.City		=	"2sd"
		info.Gender		=	1
		info.Birthday.Year	  = 1986
		info.Birthday.Month	  = 10
		info.Birthday.Day		  = 2
		info.Email			= ''
		info.bNewsLetter	= 1
		ps.Register(info)
		nn+=1
		pass

	def OnConnectIS(self, bConnect, a, t, desc):
		
		if bConnect:
			ps.Login(u"ps_test1",u"psps123")
			#ps.Disconnect()
			#ps.QuerySysInfo(9)
			ps2.ConnectToIS()
			pass
		else:
			print "disconnect"
			ps.ConnectToIS()
			pass
		
	def OnValidateResult(self, code):
		print code
		if code == 104064:
			print "ERROR"
		#ps.Disconnect()
		#ps.ConnectToIS()
	
	def OnCreateSessionResult(self, code):
		print "OnCreateSessionResult",code
		#ps.QuitRoom(1,3)
		#print "sdddddddddddddddddddddddddddddddddddddddd"
		#ps.Disconnect()
		#ps.ConnectToIS()
		#ps.GetAreaList(1)
		#ps.CreateRoom(0, 0, u"",u"", 0, 0, u"df", 10)
		
		pass
	
	def OnQuerySysinfoRecv(self, callback, session, chatroom):
		print "OnQuerySysinfoRecv", callback, session, chatroom
		
	def OnLobbyServerConn(self, bConnect, ls_uid, ls_type, ls_ver):
		print "LobbyServerConn:", bConnect, ls_uid, PSClient.ServerType2Str(ls_type), ls_ver
	

	
	def OnCreateRoomRecv(self, ret, roomid, callback):
		print callback, ret
		
	def OnAreaListRecv(self, ret, areaid, areas, callback):
		print ret, areaid, callback, areas
		T = threading.Thread(target=th_createroom,args=(areaid,))
		T.start()
		

cb = Callback()


import WConio
import functools


def cblog(s, name):
	print "[%s] %s" % (name, s)
	pass

log1 = functools.partial(cblog, name='C1')
log2 = functools.partial(cblog, name='C2')



ps = PSClient.PSClient(sessionid=3)
ps2 = PSClient.PSClient(sessionid=2)
ps.SetAppInfo(1,3,1,1)
ps2.SetAppInfo(1,3,1,1)
ps.SetCBLog(log1)
ps2.SetCBLog(log2)


islist = []
#islist.append( PSClient.str2addr("tcp://192.168.1.13:7788") )
islist.append( PSClient.str2addr("tcp://60.28.222.89:7789") )
#islist.append( PSClient.str2addr("tcp://60.28.222.89:7788") )
islist.append( PSClient.str2addr("tcp://74.53.50.5:7789") )
ps.InitIS(islist)
ps2.InitIS(islist)
ps.SetIS( PSClient.str2addr("tcp://192.168.1.13:7788") )
ps2.SetIS( PSClient.str2addr("tcp://192.168.1.13:7788") )
ps.SetEventCb(cb)
ps2.SetEventCb(cb)
ps.ConnectToIS()

while 1:
	time.sleep(0.01)
	ps.Update()
	ps2.Update()
"""
for  e in range(10000):
	print e,"\r",
	print ps.register({"username":"ps_xu.ke"})
"""

