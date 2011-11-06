import PSNetwork as net
import time



class sessioncb:
	n = 0
	def OnCreateConnection(self, ss, conn):
		print "OnCreateConnect", ss, conn
		conn.SendData("*"*1024)
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

def loop_call(n):
	print n
	net.CallLater(1, loop_call, n+1)

net.CallLater(1,loop_call, 1)
	
cb = sessioncb()
session = net.CreateSession(3, cb)

session.JoinSession( net.str2addr("tcp://192.168.1.128:7788"), [] )

net.Run( )