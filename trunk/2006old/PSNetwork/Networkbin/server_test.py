import PSNetwork as net
import time


class sessioncb:
	
	total = 0
	nn=1024
	last = 0
	send_buffer = "*"*1024
	def OnCreateConnection(self, ss, conn):
		
		print "OnCreateConnect", ss, conn
	
		conn.SendData( self.send_buffer )
		#while 1:
		#	conn.SendData(s)
			#time.sleep(0.01)
			
			
	
		#print "send lenght:", self.nn
		pass
	
	def OnDeleteConnection(self, ss, conn):
		print "OnDeleteConnect",ss, conn, "count:",conn.data
		pass
		
	def OnReceivePacke(self, ss, conn, buff):

		conn.SendData( self.send_buffer )
		pass
		
	def OnJoinFailed(self, ss, conn):
		print "OnJoinFailed", ss, conn
		
		print "ConnectionData:", conn.data
		pass
		
		
cb = sessioncb()

net.CreateSession(3,cb)
net.Run()
print "done"
