import PSClient2 as PSClient
import sys
import thread
import time
import os



uni = unicode
uni = lambda x:x
def cblog(s): 
	#print "==>", `s`
	pass

def int2(s):
	if not s: return 0
	else: return int(s)

def getp(all, *need):
	ret={}
	for e in need:
		ret[e] = u""
		if all.has_key(e):
			ret[e] = all[e]
	
	return ret

def getarg(l, key):
	ret = None
	bFound = False
	for e in l:
		if bFound:
			ret = e
			break
		if e.lower() == key:
			bFound = True
	
	return ret

def readcfg(fn):
	ret={}
	F = open(fn)
	for line in F.readlines():
		line = line.strip()
		try:
			k, v = line.split("=")
		except:
			continue
		k,v = k.strip().lower(), v.strip()
		ret[k] = v
	F.close()
	
	return ret
	
def geterrorcode(a):return self._ps.GetError(a)	

def check(all,ret):
	print "[%s]" % all["idx"],
	print all["desc"],
	expect = eval(all["expect"]+",")
	other = ""
	if expect == ret:
		other= "OK"
	else:
		other= "Failed, Expect %s, but %s" % (all["expect"], str( map(lambda a: "%d-%s" % (a,geterrorcode(a)), ret)))
	print other
	
	F = open("run.log","ab")
	F.write("[%s] [%s] %s %s\r\n" % (sys.argv[0], all["idx"], all['desc'], other) )
	F.close()

def checkret(s,value):
	ch = "+"
	desc = "OK"
	if not value:
		ch="-"
		desc="Error"
	
	print "[%s] %s %s" % (ch,s, desc)
	
class PSEvent:
	def OnValidateResult(self, result ):
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
	def OnLoginMMSResult(self, Result):
		pass
	def OnDispatchGame(self, GameID, MaxUserNum):
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

		
class PS:
	
	_ps = None
		
	def __init__(self, sessionid = 1):
		self._ps = PSClient.PSClient(sessionid)
		self._ps.SetAppInfo(1,1,1,1)
		self._ps.SetLog(cblog)
	
	def getError(self, a):
		return self._ps.GetError(a)
		
	def setLog(self, l):
		self._ps.SetLog(l)
	
	def setAppInfo(self, args):
		args = getp(args,"appid","subid", "majorver","subver")
		self._ps.SetAppInfo( int2(args['appid']), int2(args['subid']), int2(args['majorver']), int2(args['subver']))
		
	def setCallback(self, cbfun):
		self._ps.SetEvent(cbfun)
	
	def login(self, args):
		args = getp(args,"username","password")
		ret = self._ps.Login(args['username'], args['password'])
		return ret
	
	def register(self,args):
		args = getp(args, "username", "password", "nickname","location","city","gender",
			"year","month", "day", "email", "newsletter"
		)
		info = PSClient.DetailInfo()
		info.UserName	=	args['username']
		info.Password	=	args['password']
		info.NickName	=	args['nickname']
		info.Location	=	int2(args['location'])
		info.City		=	args['city']
		info.Gender		=	int2(args['gender'])
		info.Birthday.Year	  = int2(args['year'])
		info.Birthday.Month	  = int2(args['month'])
		info.Birthday.Day		  = int2(args['day'])
		info.Email			= args['email']
		info.bNewsLetter	= int2(args['newsletter'])
		ret = self._ps.Register(info)
		return ret
		
	def modifyUserAccount(self, args):
		args = getp(args, "username", "password", "nickname","location",'city', 'gender',
			"year","month", "day", "email", "newsletter", "AccountID"
		)
		info = PSClient.DetailInfo()
		info.UserName	=	args['username']
		info.Password	=	args['password']
		info.NickName	=	args['nickname']
		info.Location	=	int2(args['location'])
		info.City		=	args['city']
		info.Gender		=	int2(args['gender'])
		info.Birthday.Year	  = int2(args['year'])
		info.Birthday.Month	  = int2(args['month'])
		info.Birthday.Day		  = int2(args['day'])
		info.Email			= args['email']
		info.bNewsLetter	= int2(args['newsletter'])
		ret = self._ps.ModifyUserAccount(0, int2(args['AccountID']),info)
	
	def createRoom(self, args):
		args = getp(args, "callback", "areaid", "nickname", "roomname", "roomtype",
			"status", "password",'maxuser'
		)
		
		ret = self._ps.CreateRoom(int2(args['callback']), int2(args['areaid']), 
			args['nickname'],	args['roomname'], 
			int2(args['roomtype']), int2(args['status']),	args['password'],
			int2(args['maxuser']),
		)
		return ret
		
	def initBuddy(self, args):
		args = getp(args, "callback", "username", 'status')
		ret = self._ps.Init_Buddy( int2(args['callback']), args['username'], int2(args['status']) )
		return ret
	
	def addBuddy(self, args):
		args = getp( args, "callback", "accountid" )
		ret = self._ps.AddBuddy( int2(args['callback']), int2(args['accountid']), "" )
		return ret
		
	def removeBuddy(self, args):
		args = getp( args, "callback", "accountid" )
		ret = self._ps.RemoveBuddy( int2(args['callback']), int2(args['accountid']) )
		return ret		
	
		
	def querySession(self, args):
		args = getp(args, "callback", "sessiontype", "userpsuid","sessionkey",'sessionserverpsuid')
		ret = self._ps.QuerySession( int2(args['callback']), int2(args['sessiontype']),
			args['userpsuid'], int2(args['sessionkey']), args['sessionserverpsuid'],	
		)
	
	def queryFSList(self, args):
		args = getp(args, "callback")
		ret = self._ps.QueryFSList(int2(args['callback']))
		return ret
	
	def joinRoom(self, args):
		args = getp(args, "callback", "chatserver", "roomid", "nickname","password")
		ret = self._ps.JoinRoom(int2(args['callback']), args['chatserver'], int2(args['roomid']),
			args["nickname"], args['password'],
		)
		return ret
	
	def inviteUserToRoom(self, args):
		args = getp(args, "inviter","roomid","username")
		ret = self._ps.InviteUserToRoom(args['inviter'], int2(args['roomid']), args['username'])
		return ret
	
	def queryISList(self, args):
		ret = self._ps.QueryISList();
		return ret
		
	def getLocalPSUID(self):
		return self._ps.GetLocalPSUID()
		
	def changePassword(self, args):
		args = getp(args, "oldpassword", "newpassword", "username")
		ret = self._ps.ChangePassword(args['username'], args['oldpassword'], args['newpassword'])
		return ret
	
	def handleInviteUser(self, args):
		args = getp(args, "chatserver", "inviter", "roomid","result","reason")
		ret = self._ps.HandleInviteUser( args['chatserver'], args['inviter'], int2(args['roomid']), int2(args['result']), args['reason'] )
		return ret
		
	def startGame(self, args):
		args = getp(args, "callback", "roomid")
		ret = self._ps.StartGame(int2(args['callback']), int2(args['roomid']))
		return ret
	
	def queryAreaList(self, args):
		args = getp(args,"areaid")
		ret = self._ps.QueryAreaList(int2(args['areaid']))
		return ret
	
	def queryRoomList(self, args):
		args = getp(args,"areaid" ,"roomtype")
		ret = self._ps.QueryRoomList( int2(args['areaid']), int2(args['roomtype']) )
		return ret
	
	def quitRoom(self, args):
		args = getp(args, "roomid", "callback")
		
		ret = self._ps.QuitRoom(int2(args['callback']), int2(args['roomid']))
		return ret
	
	def chat(self, args):
		args = getp(args, "roomid","touser","content","datatype")
		ret = self._ps.Chat(int2(args['datatype']), int2(args["roomid"]), args['touser'], args['content'])

		return ret
	
		
	def setRoomInfo(self, args):
		args = getp(args, "roomid", "callback", "roominfo")
		ret = self._ps.SetRoomInfo(int2(args['callback']), int2(args['roomid']), args['roominfo'])
		return ret
	
	def setUserInfo(self, args):
		args = getp(args, "roomid", "callback", "userinfo")
		ret = self._ps.SetUserInfo( int2(args['callback']), int2(args['roomid']), args['userinfo'] )
	
	def setIS(self, isaddr):
		#print PSClient.str2addr(isaddr)
		self._ps.SetIS(PSClient.str2addr(isaddr))

	
	def loop(self):
		while 1:
			time.sleep(0.03)
			self.update()
			
			
	def update(self):
		self._ps.Update()
	
	def connectToIS(self):
		self._ps.ConnectToIS()
	
		
	def disconnect(self):
		self._ps.Disconnect()
		
		
def parseTest(fn):
	F = open(fn)
	basename = fn.split("\\")[-1]
	ls = F.readlines()
	d = {"idx":basename.split(".")[0]}
	for e in ls:
		e = e.strip()
		pos = e.find("=")
		if pos  > 0:
			k = e[:pos].strip().lower()
			v = e[pos+1:].strip()
			if v[0]=='"' and v[-1]=='"':
				v = eval(v)
						
			if k!="":
				d[k]= v
			
	
	return d
	
import nt
from excel import excel


class TextCaseLoader:
	all=[]
	count = -1
	
	def __init__(self, path=""):
		self.path = path
		self.now = None
	
	def reload(self):
		self.count = -1
		
	def load(self, path = ""):
		if path: self.path = path
		for e in nt.listdir(self.path):
			fullpath = self.path+e
			if e.split(".")[-1].lower() == 'test':
				self.all.append(parseTest(fullpath))
		
	
	def getnow(self):
		return self.now		
				
	def getnext(self):
		#return "ll", self.count, len(self.all)
		self.count += 1
		if self.count >=len(self.all) : 
			self.now = None
			return None
		self.now = self.all[self.count]
		return self.now

class XlCaseLoader(TextCaseLoader):

	def __init__(self):
		TextCaseLoader.__init__(self)
		
	
	def load(self, path=""):
		xl = excel()
		print "[+]", "Loading Test case from", path
		if path: self.path = path
		xl.open(self.path)
		xl.sheet("testcase")
		self.all = xl.getAll()
		xl.close()
		


CaseLoader = XlCaseLoader

class psevent:
	def OnLobbyServerConn(self, bconn, ls_id, ServerType, ver):
		pass
	
	def OnJoinRoomRecv(self, result, roominfo):
		pass
		
class psevent2:
	def OnLobbyServerConn(self, bconn, ls_id, ServerType, ver):
		pass
	
	


class TestProcessor(PSEvent):
	ready = False
	count = 1
	timeout = 8
	env = None
	report = None
	testname = ""
	ps2 = None
	def __init__(self):
		self.ps = PS()
		self.ps2 = PS(4)
		self.case = CaseLoader()
		self.setup()
		self.setCallback(self)
		self.p = None
		self.Lock = thread.allocate_lock()
		self.init()
		self.report = reportxml()
		
		
	def init(self):
		pass
	
	


	
	def lock(self):
		self.Lock.acquire()
	
	def setReady(self, b= True):
		self.ready = b
	
	def isReady(self):
		return self.ready
	
	def unlock(self):
		self.Lock.release()
	
	def wait(self, timeout = 8):
		_delay = 0.003
		endtime = time.time() + timeout
		while 1:
			self.ontimer(time.time())
			gotit = self.Lock.acquire(0)
			if gotit: return True
			remaintime = endtime - time.time()
			if remaintime <=0:
				self.unlock()
				return False
			_delay = min(_delay * 2, remaintime, .05)
			self.getps().update()
			self.getps2().update()
			time.sleep(_delay)
	
	def geterror(self, code):
		return self.getps().getError(code)
	
	def check(self,ret):
		result = "0"
		all = self.p
		print "[%s]" % all["idx"],
		print all["desc"],
		expect = eval("[" + all["expect"] + "]")
		other = ""
		if expect == ret:
			other= "OK"
			result = "1"
		else:
			other= "Failed, Expect %s, but " % all['expect']
			for e in ret:
				other += " %d - %s" % (e, self.geterror(e))
			#other= "Failed, Expect %s, but %s" % (all["expect"], str( map(lambda a: "%s-%s" % (str(a),str(self.ps2.getError(a))), ret)))
			#other = "Failed, Expect %s, but %s" % (all['expect'], str(ret))
		print other
				
		F = open("run.log","ab")
		F.write("[%s] [%s] %s %s\r\n" % (sys.argv[0], all["idx"], all['desc'], other) )
		F.close()
		expects=[]
		for e in expect:
			expects.append( dict(code=e, desc=self.geterror(e) ))
		
		returncode=[]
		for e in ret:
			returncode.append( dict(code=e, desc=self.geterror(e) ))
			
		params = {}
		for e in self.case.getnow():
			if e in ['idx', 'expect','desc']:
				continue
			params[e] = all[e]
			
		
		self.report.addcase(itemname = self.testname, casefilename= self.case.path, casename= all['desc'], idx=str(all['idx']), expect=expects, returncode =returncode,  params= params, result=result)
		
	def setCallback(self, cb):
		self.ps.setCallback(cb)
	def setCallback2(self, cb):
		self.ps2.setCallback(cb)
	
	def getps(self):
		return self.ps
	
	def getps2(self):
		return self.ps2
	
	def new(self):
		t = self.case.getnext()
		if t:
			n = self.env
			n.update(t)
			self.p = n
		else:
			self.p = t
 	
	def stop(self):
		self.unlock()
	
	def start(self):
		if self.ready:
			self.onReady()
		else:
			self.getps().disconnect()
			self.getps().connectToIS()
	
			
			
	def onReady(self):
		pass
	
	def end(self, code):
		self.check([code])
		self.stop()
	
	def onTimeout(self):
		pass
	def settimeout(self,n=8):
		self.timeout = n

	def updateparam(self, dic):
		for e in self.p:
			if dic.has_key(e) and self.p[e] == "":
				self.p[e] = dic[e]
		for e in dic:
			if not self.p.has_key(e):
				self.p[e] = dic[e]
	
	def ontimer(self, t):
		pass
		
	def loop(self):
		for e in range(self.count):
			print ">> count:",e
			self.report.clearcase(self.case.path)
			while 1:
				w = self.wait(self.timeout)
				if w:
					#OK
					self.new()
					if not self.p:
						self.unlock()
						self.case.reload()
						break
					self.start()
				else:
					#timeout
					self.onTimeout()
			self.report.save()
							
		
	def setup(self):
		cfg = "test.cfg"
		if getarg(sys.argv, "-f"):
			cfg = getarg(sys.argv, "-f")
		c = getarg(sys.argv, "-c")
		if c!=None: self.count = int2(c)
		print "[+] Load Config File:",cfg
		
		all = readcfg(cfg)
	
		defaultis = "tcp://192.168.1.13:7788"
		if all.has_key("isaddr"):
			defaultis = all['isaddr']
		self.getps().setIS( defaultis)
		self.getps2().setIS( defaultis)
		self.env = all
		self.getps().setAppInfo(self.env)
		self.getps2().setAppInfo(self.env)
		print "[+] IS Address:", defaultis
		self.testpath = sys.argv[0][0:-3]+"\\"
		checkret("Load test case from: %s" % self.testpath, 1)
		self.case.load(self.testpath)





class ps2event:
	p = None
	hasconn = 0
	def OnValidateResult(self, byCode):
		#print 'OnvalidateResult', byCode, self.p.getps2().getError(byCode)
		pass
	def OnCreateSessionResult(self, dwCode):
		#print 'OnCreateSesson', dwCode
		assert(dwCode == 105000)
		self.p.getps2().queryAreaList( dict(areaid=1) )
		pass
	def OnConnectIS(self, bConnect, is_id , reason, desp, islist):
		#print bConnect, is_id, reason, desp
		if bConnect:
			print 2,"Connect OK"
		else:
			print 2,"Connect Failed", desp, self.p.getps2().getError(reason)
		self.p.getps2().login( dict(username='ps_test1', password='123123') )
		pass
	def OnAreaListRecv(self, Result, AreaID, al):
		#print "OnAreaListRecv", AreaID
		if (AreaID == 1):
			for e in al:
				#print e
				self.p.getps2().queryAreaList( dict(areaid=e.Id) )
		else:
			self.lastareaid = al[-1].Id
			for  e in al:
				a = Area()
				a.id = e.Id
				self.p.areas[a.id] = a
				self.lastareaid = e.Id
				self.p.getps2().queryRoomList( dict(areaid=a.id, roomtype=2) )
				
		
	def OnRegisterResult(self, errors, names):
		pass
	def OnRoomListRecv(self, Result, AreaID, RoomList, RoomType):
		
		for e in RoomList:
			r = Room()
			r.id = e.Id
			r.chatserver = e.ChatServer
			self.p.areas[AreaID].rooms[r.id] = r
		if AreaID == self.lastareaid:
			if self.hasconn == 0:
				self.hasconn = 1
				self.p.getps().connectToIS()
		
	def OnCreateRoomRecv(self, Callback, Result, Info):
		pass
	def OnJoinRoomRecv(self, Result, Info):
		pass
	def OnRecvChatMsg(self, datatype, RoomID, ToPSUID):
		pass
	def OnQuitRoomResult(self, Result, RoomID, RoomInfo):
		pass
	def OnBuddyListRecv(self, _Result, BuddyList):
		pass
	def OnLoginMMSResult(self, Result):
		pass
	def OnDispatchGame(self, GameID, MaxUserNum):
		pass
	def OnLobbyServerConn(self,bconn, ls_id, dwServerType, ls_ver):
		pass
	def OnQueryNewsRecv(self, Callback, Result, content, AddTime):
		pass
	def OnConfigUserSession(self, Callback, Result, SessionType, userPSUID):
		pass
	def OnQuerySessionResult(self, Callback, Result, SessionType, userPSUID, content):
		pass
	def OnQuerySysinfoRecv(self, Callback, sessionNum, chatRoomNum):
		pass
	def OnUserInvited(self, InviterPSUID, InviterName, RoomName, RoomID,ChatServer,userpass,RoomType):
		pass
	def OnInviteUserReturn(self, InviteeID, InviteeName, Result, RoomID):
		pass
	def OnSetRoomInfoResult(self, Callback, Result, RoomID):
		pass
	def OnRoomInfoChange(self, RoomID, info):
		pass
	def OnSetUserInfoResult(self, Callback, Result, RoomID):
		pass
	def OnGameUserInfoChange(self, RoomID, UserPSUID, Info):
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


class Area:
	id = None
	rooms = {}
class Room:
	id = None
	chatserver = None

class TestProcessor2(TestProcessor):
	cb2 =None
	areas = {}
	
	def OnConnectIS(self, bConn, isid, reason, p, islist):
		if bConn:
			print "1 Connect OK"
			self.getps().login(self.p)
			
		else:
			print "1 Error", self.getps().getError(reason)
			import sys
			sys.abort(0)
			pass
		

	
	def onTimeout(self):
		print "timeout"
		self.setReady(False)
		self.start()
		
	
	def OnValidateResult(self,code):
		print "OnValidateResult", code
		if code == 104060:
			print "validate ok"
		
	
	def OnCreateRoomRecv(self, ret, roomid):
		print "OnCreateRoom result: %d, id: %d" % (ret, roomid.Id)
		self.end(ret)
	
	def dosomething(self):
		pass
		
		
	def OnCreateSessionResult(self, code):
		if 105000 == code:
			print "login ok"
			self.setReady(True)
			self.start()
		else:
			sys.abort()
	
	def start(self):
		
		self.updateparam(self.INFO)
		
		if self.ready:
			self.dosomething()
		else:
			self.timeout = 20
			self.cb2=	ps2event()
			self.cb2.p = self
			self.setCallback2(self.cb2)
			def log2(s):print `s`
			#self.ps2.setLog(log2)
			self.getps2().disconnect()
			self.getps2().connectToIS()
			#self.getps().disconnect()
			#self.getps().connectToIS()

import xml.etree.ElementTree as xml

class reportxml:
	xmlname=""
	def __init__(self, name="report.xml"):
		self.xmlname = name
		self.tree = xml.ElementTree()
		try:
			self.tree.parse(name)
		except:
			self.tree._setroot( xml.Element(u"root") )
		
	
	def addcase(self, itemname, casefilename, casename, idx, expect=[],  returncode =[], createtime=None, params=[], result ="1" ):
		idx = str(idx)
		if createtime == None: createtime = time.strftime("%Y-%m-%d %H:%M:%S")
		e_testitem = None
		ele = self.tree.getroot().findall("testitem")
		if ele:
			for e in ele:
				if e.attrib['filename']==casefilename:
					e.attrib['createtime'] = createtime
					e_testitem = e
					break
		if e_testitem == None:
			e_testitem = xml.Element(u"testitem", name=itemname, filename=casefilename, createtime = createtime)
			self.tree.getroot().append(e_testitem)
	
		e_testcase = xml.Element(u"testcase", index=idx, name=casename, result=result)
		e_testitem.append(e_testcase)
		
		#expect
		e_expect = xml.Element(u"expect")
		e_testcase.append(e_expect)
		for e in expect:
			e_code  = xml.Element(u"code", desc=e['desc'] )
			e_code.text = str(e['code'])
			e_expect.append(e_code)
		
		#returncode
		e_return = xml.Element(u"return")
		e_testcase.append(e_return)
		for e in returncode:
			e_code = xml.Element(u"code", desc = e['desc'])
			e_code.text = str(e['code'])
			e_return.append(e_code)
			
		#params
		e_params = xml.Element(u"params")
		e_testcase.append(e_params)
		for p in params:
			e_param = xml.Element(u"param", name=p)
			e_param.text = str(params[p])
			e_params.append(e_param)
			
		
		pass
	
	def clearcase(self, casepath):
		ele = self.tree.getroot().findall("testitem")
		if ele:
			for e in ele:
				if e.attrib['filename'] == casepath:
					self.tree.getroot().remove(e)
	
	def save(self):
		import codecs
		F= open(self.xmlname,"wb")
		F.write(codecs.BOM_UTF8)
		F.write('<?xml version="1.0" encoding="UTF-8" ?>\n')
		F.write(xml.tostring(self.tree.getroot(),encoding='utf-8'))
		F.close()
		
		
if __name__ == "__main__":
	t = TestProcessor2()
	t.loop()
		