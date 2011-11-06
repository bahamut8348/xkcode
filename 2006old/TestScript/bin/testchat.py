import PSClient as _ps
import PS
import win32event, win32pipe, win32api, win32process
import sys, time

EVENT_NAME = u"testchatevent"
PIPE_NAME	= u"\\\\.\\pipe\\testchatpipe"

user1 = u"ps_test", u"123123"
user2 = u"ps_xu.ke", u"psps123"

class TestChat(PS.TestProcessor):
	
	seq = 0
	event = None
	pipe	= None
	user=None
	def init(self):
		try:
			self.event = win32event.OpenEvent(win32event.EVENT_ALL_ACCESS,0,EVENT_NAME)
		except Exception, r:
			print r[2]
		if self.event:
			self.seq = 2
			self.user = user2
			self.settimeout(1000000)
		else:
			self.seq = 1
			self.settimeout(1000000)
			self.user = user1
			self.event = win32event.CreateEvent(None, False, False, EVENT_NAME)
			self.pipe  = win32pipe.CreateNamedPipe(PIPE_NAME,3,0,10,1024,1024,100,None)
			si = win32process.STARTUPINFO()
			si.lpTitle = "test"
			si.dwFlags = 0
			#win32process.CreateProcess(None,"psrun.exe " + sys.argv[0],None,None,0,0x10,None,None,si)
			
		print "user",self.seq
	
	def onTimeout(self):
		print 'timeout'
	
	def OnConnectIS(self, bConn, isid, reason, p):
		self.getps().login({"username":self.user[0], "password":self.user[1]})
	
	def OnCreateRoomRecv(self, ret, roomid, callback):
		if ret==0:
			print "CreateRoom OK, ID:", roomid
			self.setReady(True)
			print "wait for user2 "
		else:
			print "create Room Error"
	
	def OnUserListRecv(self, roomid, type, users):
		print "Get Users",users
		
		
	def OnAreaListRecv(self, result, areaid, areas, callback):
		print areas
		if areaid==1:
			
			self.areaid = areas[0][0]
			if self.seq == 1:
				self.getps().createRoom( {'roomname':'testroom','areaid':self.areaid })
			elif self.seq==2:
				self.getps().getAreaList({ "areaid":2 })
	
	def OnRoomListRecv(self,areaid, roomtype, rooms, callback):
		print "rooms", rooms

	
	def OnCreateSessionResult(self, code):
		if code == 105000:
			self.getps().getAreaList({ "areaid":1 })
		else:
			print "login error"
	
	def start(self):
		if not self.isReady():
			self.getps().connectToIS()
		else:
			pass
	
			
tc =TestChat()
tc.loop()

	