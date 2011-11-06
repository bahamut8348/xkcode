import PSClient

import thread
#print dir(ps.AreaInfo)

#import threading
import time



def th_createroom(areaid):
	print "start create room, areaid", areaid
	for e in range(10000):
		
		time.sleep(0.1)
		ps.CreateRoom(e, areaid, unicode("nickname"), unicode("roomname"))

class Callback(PSClient.event):
	a = 0
	def OnConnectIS(self, bConnect, a, t, desc):
		
		if bConnect:
			print ">>> OnConnectIS",bConnect, a, t,` desc`
			ps.Login(u"ps_xu.ke",u"psps123")
			print self.a
			self.a+=1
			#ps.Disconnect()
			#print ">>> Disconnect"
			#ps.ConnectToIS()
			pass
		else:
			print "disconnect"
			pass
		
	def OnValidateResult(self, code):
		print code
	
	def OnCreateSessionResult(self, code):
		print "OnCreateSessionResult",code
		ps.Disconnect()
		print ">>> Disconnect"
		ps.ConnectToIS()
		#ps.GetAreaList(1)
		#ps.CreateRoom(0, 0, u"",u"", 0, 0, u"df", 10)
		pass
	
	def OnCreateRoomRecv(self, ret, roomid, callback):
		print callback, ret
		
	def OnAreaListRecv(self, ret, areaid, areas, callback):
		print ret, areaid, callback, areas
		T = threading.Thread(target=th_createroom,args=(areaid,))
		T.start()
		

cb = Callback()


def cblog( s):
	#print "==>", `s`
	pass

ps = PSClient.PSClient()
ps.SetAppInfo(1,1,1,1)
ps.SetCBLog(cblog)
#ps.SetIS(u"tcp://60.28.222.89:7788")
ps.SetIS(u"tcp://192.168.1.13:7788")
#ps.SetIS(u"tcp://127.0.0.1:2003")
ps.SetEventCb(cb)
ps.ConnectToIS()
while 1:
	time.sleep(0.02)
	ps.Update()
"""
for  e in range(10000):
	print e,"\r",
	print ps.register({"username":"ps_xu.ke"})
"""

