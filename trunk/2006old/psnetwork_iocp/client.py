from PSNetwork import *
import time



class sessioncb:
	n = 0
	def OnCreateConnection(self, ss, conn):
		print "OnCreateConnect", ss, conn
		#conn.SendData("*"*1024)
		pass
	
	def OnDeleteConnection(self, ss, conn):
		print "OnDeleteConnect",  ss, conn
		pass
		
	def OnReceivePacke(self, ss, conn, buff):
		#print "OnRecv, length:", len(buff)
		self.n += 1
		if (self.n % 50) == 0:
			print "packet got",self.n
		conn.SendData(buff)
		pass
		
	def OnJoinFailed(self, ss, conn):
		print "OnJoinFailed",  ss, conn
		pass

		
cb = sessioncb()
man = Manager()
session = man.CreateSession(4,10, cb)

session.JoinSession( str2addr("tcp://192.168.1.128:7788"), 0, [], 3 )


while 1:
	man.Update()
	time.sleep(0.01)