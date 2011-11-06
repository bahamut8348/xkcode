from PSNetwork import *



class sessioncb:
	
	total = 0
	nn=1024
	def OnCreateConnection(self, ss, conn):
		
		print "OnCreateConnect", ss, conn
		#conn.SendData("*" * self.nn)
		#print "send lenght:", self.nn
		pass
	
	def OnDeleteConnection(self, ss, conn):
		print "OnDeleteConnect",ss, conn, "count:",conn.data
		pass
		
	def OnReceivePacke(self, ss, conn, buff):
		print "OnReceivePacke", conn, len(buff)
		self.ss.SendPacket(buff)
		#conn.Close(True)
		#ss.JoinSession( str2addr("tcp://192.168.1.63:7788") , 1 , [self.total] )
		#self.total+=1
		
		pass
		
	def OnJoinFailed(self, ss, conn):
		print "OnJoinFailed", ss, conn
		
		print "ConnectionData:", conn.data
		pass
		
		
cb = sessioncb()

man = Manager()
import time
print man

#for e in range(1, 65535):
all = []
for e in range(1,3700):
	sess = man.CreateSession(e,10, cb)
	all.append(sess)
	sess.JoinSession( str2addr("tcp://192.168.1.66:7788") , 0 , [],3)
	time.sleep(0.01)

print "all Joined"
#	sess.JoinSession( str2addr("udp://192.168.1.13:8080") , 0 , [])

#man.ReleaseSession(sess)

import time
while 1:
	man.Update()
	time.sleep(0.01)
print "done"
