import PSNetwork as net
import time


class sessioncb:
	
	total = 0
	nn=1024
	last = 0
	send_buffer = "*"*1024
	def OnCreateConnection(self, ss, conn):
		print "OnCreateConnect", ss, conn
	
	def OnDeleteConnection(self, ss, conn):
		print "OnDeleteConnect",ss, conn, "count:",conn.data
		pass
		
	def OnReceivePacke(self, ss, conn, buff):
		print "OnReceivePacke"
		pass
		
	def OnJoinFailed(self, ss, conn):
		print "OnJoinFailed", ss, conn
		
		print "ConnectionData:", conn.data
		pass
		
		
cb = sessioncb()

sess = net.CreateSession(3,cb)
sess.OpenPort(net.str2addr("tcp://0.0.0.0:7788"),10000)

net.Run()

print "done"
