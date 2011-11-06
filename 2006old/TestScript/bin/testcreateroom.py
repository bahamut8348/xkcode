
import PS
import sys,time

INFO = {"username":u"ps_test", "password":u"123123",
	"areaid":1, 'maxuser':100, 'roomname':"ll",
}

class CreateRoomTest(PS.TestProcessor):
	testname = "CreateRoom Test"
	def OnConnectIS(self, bConn, isid, reason, p, islist):
		if bConn:
			
			print "Connect OK"
			self.getps().login( dict( username='ps_test', password='123123'))
		else:
			print "Error", self.getps().getError(reason)
			self.onTimeout()
			pass
		
	def end(self, code):
		self.check([code])
		self.stop()
	
	def onTimeout(self):
		print "timeout"
		self.check([3])
		self.setReady(False)
		self.start()
	
	def OnGameUserInfoChange(self, RoomID, UserPSUID, Info):
		pass
	
	def OnValidateResult(self,code):
		print "OnValidateResult", code
		if code == 104060:
			print "validate ok"
			
		else:
			print self.ps.getError(code)
	
	def OnAreaListRecv(self, result, areaid, areas):
		print "OnAreaListRecv", result, areaid, len(areas)
		print areas
		self.setReady(True)
		self.start()
	
	def OnCreateRoomRecv(self, ret, roomid, info):
		print "OnCreateRoom result: %d, id: %d" % (roomid, info.Id)
		self.end(roomid)
		
	
	def OnRoomListRecv(self,areaid, roomtype, rooms, callback):
		#print "rooms",rooms
		pass
		
	
	def OnCreateSessionResult(self, code):
		if 105000 == code:
			print "login ok"
			self.setReady(True)
			self.start()
		else:
			sys.abort()
			
	def OnReady(self):
		self.getps().createRoom(self.p)
		
	def start(self):
		
		self.updateparam(INFO)
		
		if self.ready:
			self.OnReady()	
		else:
			self.getps().disconnect()
			self.getps().connectToIS()
if __name__ == "__main__":
	rt = CreateRoomTest()
	rt.loop()

