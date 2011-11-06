from PSNetwork import *
import time



class sessioncb:
	n = 0
	def OnCreateConnection(self, ss, conn):
		print "OnCreateConnect", ss, conn
		pass
	
	def OnDeleteConnection(self, ss, conn):
		print "OnDeleteConnect",  ss, conn
		pass
		
	def OnReceivePacke(self, ss, conn, buff):
		#print "OnRecv, length:", len(buff), buff
		self.n += 1
		if self.n % 10000 == 0:
			print self.n
		conn.SendData(buff)
		pass
		
	def OnJoinFailed(self, ss, conn):
		print "OnJoinFailed",  ss, conn
		pass

		
cb = sessioncb()
man = Manager()
session = man.CreateSession(3,10, cb)
print session
session.OpenPort( str2addr("tcp://0.0.0.0:7788"), 0)
session.OpenPort( str2addr("udp://0.0.0.0:7788"), 0)

while 1:
	man.Update()
	try:
		time.sleep(0.001)
	except:
		break
	
del man

print "End"
time.sleep(3)