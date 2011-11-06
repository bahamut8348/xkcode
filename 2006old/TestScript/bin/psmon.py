from psmon_conf import *


from PSClient2 import *
import getopt, sys, time, smtplib, os
from email.mime.text import MIMEText
import urllib2
import xml.etree.ElementTree as tree


def cblog( s):
	print "==>", s
	pass


isaddr = ""
appname = ""

class Allevent:
	ps = None
	result = None

	def SetResult(self, ret):
		self.result = ret

	def OnConnectIS(self, bConnect, a, t, desc, islist):
		if bConnect:
			self.ps.Login(LOGIN_USERNAME, LOGIN_PASSWORD)
		else:
			self.SetResult(t)
	
	def OnValidateResult(self, code):
		if code == 104060:
			pass
		else:
			print "Validate ERROR", self.ps.GetError(code)
			self.SetResult(code)
	
	def OnCreateSessionResult(self, code):
		#print "OnCreateSessionResult",code
		self.SetResult(code)
		
	
	def OnLobbyServerConn(self, bConnect, ls_uid, ls_type, ls_ver):
		pass
		#print "LobbyServerConn:", bConnect, ls_uid, PSClient.ServerType2Str(ls_type), ls_ver

class ServerTest:
	def __init__(self):
		self.cb = Allevent()
		self.ps = PSClient(1)
		#self.ps.SetLog(cblog)
		
		self.cb.ps = self.ps
		
	def Login(self, isip, username, password, appid, subid):
		self.ps.SetAppInfo(appid,subid,1,1)
		self.ps.SetIS( str2addr(isip) )
		self.ps.SetEvent(self.cb)
		self.ps.ConnectToIS()

	def GetResult(self):
		start = time.time()
		while 1:
			self.Update()
			if self.cb.result != None:
				break
			if time.time() - start > 50:
				self.cb.SetResult(104065)
				break
			time.sleep(.01)
		return self.cb.result
	
	def End(self):
		self.cb.result = None
		self.ps.Disconnect()
	
	def Update(self):
		self.ps.Update()

def usage():
	print "psmon [-n appname] [-i isaddr] [-a appid] [-s subid] [-t tsaddr]\n eg: psmon -n tj -i tcp://192.168.1.13:7788 -a 1 -s 2 -t 192.168.1.13:1999"

class SendMail:
	
	def send(self,title, content):
		smtp = smtplib.SMTP(SMTP_SERVER)
		smtp.login(SMTP_USERNAME, SMTP_PASSWORD)
		t = time.gmtime()
		msg = MIMEText(content + "\r\n\r\nby PSMon. \nPossibilitySpace\n%d-%d-%d %d:%d:%d\r\n" % t[:6])
		msg['Subject'] = title + isaddr
		msg['From'] = SMTP_FROMADDR
		msg['To'] = ";".join(NOTIFY_EMAIL)

		smtp.sendmail(SMTP_FROMADDR, NOTIFY_EMAIL, msg.as_string())
		smtp.quit()

class GameServerMonitor:
	desc = "[Server Monitor]"
	needResult = 1
	ErrorDesc = ""
	initser = set(['11', '10', '15', '14', '17', '16', '1', '3', '2', '5', '4', '6', '8','19'])
	code = 1
	def __init__(self):
		pass
	
	def start(self):
		F = None
		try:
			F = urllib2.urlopen("http://%s/TS?ServerType=18&PSUID=0-0-0-0-0000000000000000&Operation=1" % TSADDR)
		except:
			self.code = 0
			self.ErrorDesc = "Transfer Server Down!!"
			return
		tr = tree.parse(F)
		eles = tr.findall('//LobbyServerInfo/ServerType')
		sts = set( map (lambda x: x.text, eles) )
		if sts == self.initser:
			self.code = 1
			self.ErrorDesc = "All Server OK"
		else:
			print sts, self.initser
			self.code = 0
			self.ErrorDesc = "No Server", self.initser.difference(sts)
			
	def waitfinish(self):
		return self.code


class LoginMonitor:
	desc = "[Login Monitor]"
	needResult = 105000
	ErrorDesc = ""
	
	def __init__(self):
		self.st = ServerTest()

	def start(self):
		self.st.Login( isaddr, LOGIN_USERNAME, LOGIN_PASSWORD, appid, subid)
	def waitfinish(self):
		ret = self.st.GetResult()
		self.ErrorDesc = self.st.ps.GetError(ret)
		self.st.ps.Disconnect()
		self.st.End()
		return ret

class TaskStatus:
	error = False
	mailtime = 0
	errorcount=0
	wellcount=0

def mlog( s ):
	print "[%s] [%s] %s" % (time.asctime(), appname , s)

class Monitor:
	task=[]
	
	def addtask(self, o):
		self.task.append((o,TaskStatus()))

	def __init__(self):
		self.addtask(LoginMonitor())
		#self.addtask(GameServerMonitor())
	
	def senderrormail(self, moni, s):
		s.error = True
		s.wellcount = 0
		s.errorcount += 1
		if time.time()- s.mailtime > 5*60 and s.errorcount> 4:
			s.mailtime = time.time()
			s.errorcount = 0
			mlog( "Send Error Mail" )
			sm = SendMail()
			sm.send("[Error] %s" % moni.desc, "Error %s %s" % (moni.desc, moni.ErrorDesc) )
		pass
	
	def sendokmail(self, moni, s):
		s.error = False
		s.errorcount = 0
		s.wellcount += 1
		
		if s.wellcount>4:
			s.wellcount = 0
			sm = SendMail()
			mlog( "Send Ok Mail" )
			sm.send("[Well] %s" % moni.desc, "" )
		pass


	def start(self):
		while 1:
			for e in self.task:
				moni, status = e
				moni.start()
				ret = moni.waitfinish()
				mlog( "%s Result %d" % (moni.desc, ret) )
				error = False
				if ret != moni.needResult:
					error = True
					mlog("%s %s %s, Error Count: %d" % ( moni.desc, "error", moni.ErrorDesc, status.errorcount ) )
				else:
					mlog("%s %s" % ( moni.desc, "ok" ))
				if status.error != error:
					if error == True: self.senderrormail(moni, status)
					elif error == False:  self.sendokmail(moni,status)
				else:
					if error == True:
						self.senderrormail(moni, status)
						
				
			
			time.sleep(INTERVAL)




if __name__ == "__main__":
	
	try:
		opts, args = getopt.getopt(sys.argv[1:], "n:hi:a:s:", [])
	except getopt.GetoptError, err:
		print str(err)
		usage()
		sys.exit(2)

	isaddr = "tcp://192.168.1.13:7788"
	appid = 1
	subid = 3
	appname = "d"
	TSADDR = "192.168.1.13:1999"

	for o, a in opts:
		if o == "-i":
			isaddr = a
		elif o =="-h":
			usage()
			sys.exit()
		elif o == "-a":
			appid = int(a)
		elif o == '-s':
			subid = int(a)
		elif o == '-t':
			TSADDR = a
		elif o == "-n":
			appname = a
		else:
			assert False, "unhandled option"
	print "IS:", isaddr
	print "AppID:", appid
	print "SubID:", subid
	print "TS:", TSADDR
	print "UserName:", LOGIN_USERNAME
	if appname:
		os.system("title "+ appname + " - Possibility Space")
	mon = Monitor()
	mon.start()

	



	
	
