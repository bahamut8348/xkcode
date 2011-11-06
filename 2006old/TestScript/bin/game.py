from PSClient2 import *
import time
from psevent import PSEvent

def l(x):print x

class GameServer(PSEvent):
	def __init__(self, sessionid = 2):
		self.ps = PSClient(sessionid)
		self.ps.SetEvent(self)
		self.ps.SetIS(Address("tcp://192.168.1.132:7788"))
		self.ps.SetAppInfo(1,3,0,0)
		self.ps.SetLog(l)
		self.func = self.dowork()
		self.func.next()
		
		self.game = {}
	
	def OnCreateSessionResult(self, result):
		#self.sendtest()
		pass
	
	def OnConnectIS(self, *result):
		if result[0]:
			self.func.send( result )
		else:
			self.ps.ConnectToIS()
	
	def OnLoginMMSResult(self, *result):
		self.func.send( result )
		
	
	def OnDispatchGame(self, gameid, matchserver,  roombuffer , userlist):
		self.game[gameid] = (matchserver, roombuffer, userlist)
		print "]", "send dispatch response"
		self.ps.HandleDispatchGame(107000, gameid, 917528, [e+1 for e in range(len(userlist))], SessionAddress() )
		self.sendtest( len(userlist) , userlist)
	
	def sendtest(self, number = 6, userlist = None):
		alluser = []
		if userlist:
			alluser = userlist
		else:
			for e in range(1,number):
				report = GameUserReport()
				report.AccountID = 1000 * e
				report.Buffer = "1" * (e+1)
				alluser.append(report)
			
		self.ps.ReportGameResult(1,917528L, alluser)
		
		
	def dowork(self):
		self.ps.ConnectToIS()
		yield 1
		self.ps.LoginToMMS(10)
		Result, matchserver = yield 1
		print "MatchServer" , matchserver
		yield 1
		
	def update(self):
		self.ps.Update()
		
		

if __name__ == "__main__":
	gs = GameServer()
	while 1:
		gs.update()
		time.sleep(0.01)
		