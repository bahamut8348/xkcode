
from PSClient2 import *
import PS

	
class Process(PS.TestProcessor):
	
	INFO = {}
	
	arealist1 = None
	arealist2 = None
	
	roomlist = None

	state = 1
	
	def init(self):
		self.timeout = 109
		
		self.INFO['username'] = u"ps_test2"
		self.INFO['password'] = u"123123"
	def mywork(self):
		# query area 
		self.getps().queryAreaList({"areaid":1})
	
	def OnConnectIS(self, bConn, is_uid, error, msg):
		if bConn:
			self.getps().login(self.INFO)
	
	def OnAreaListRecv(self, result, areaid,  arealist):
		print "OnAreaListRecv"
		#print arealist
		if self.state == 1:
			self.state = 2
			self.getps().queryAreaList({"areaid":arealist[0].Id})
		elif self.state == 2:
			pass
			if  len(arealist) == 0:
				self.getps().queryRoomList({"areaid":3,"roomtype":-1})
			
	
	def OnRoomListRecv(self, result, areaid, roomlist, roomtype):
		print u"areaid:", areaid, u",type", roomtype
		print roomlist[0]
		
	def OnCreateSessionResult(self, code):
		print "[+] OnCreateSessionResult", code, self.getps().getError(code)
		if code == 105000:
			self.mywork()
		pass
	
	def OnValidateResult(self,code):
		print "[+]OnValidateResult", code, self.getps().getError(code)
		if code == 104060:
			print "validate ok"
		
	def start(self):
		print 'start'
		self.getps().connectToIS()
	
	def ontimer(self, t):
		pass
		

if __name__ == "__main__":

	tp = Process( )
	tp.loop()