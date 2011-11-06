from PSNetwork import *
import time

man = Manager()

def send_data( conn ):
	if conn:	
		if conn.SendData( sessioncb.send_buffer ):
			man.CallLater(0.5, send_data, conn )

class sessioncb:
	
	total = 0
	nn=1024
	last = 0
	send_buffer = "*"*250
	def OnCreateConnection(self, ss, conn):
		print "+++",  conn
		send_data(conn)
		pass
	

	def OnDeleteConnection(self, ss, conn):
		print "---", conn
		pass
		
	def OnReceivePacke(self, ss, conn, buff):
		#print "OnReceivePacke", conn, len(buff)
		#conn.SendData(buff)
		#conn.SendData(buff)
		#conn.Close(True)
		#ss.JoinSession( str2addr("tcp://192.168.1.63:7788") , 1 , [self.total] )
		#self.total+=1
		#conn.SendData( self.send_buffer )
		pass
		
	def OnJoinFailed(self, ss, conn):
		print "OnJoinFailed", ss, conn
		
		print "ConnectionData:", conn.data
		pass

	
		
		
cb = sessioncb()

print man

#for e in range(1, 65535):
sess = man.CreateSession(3,cb)
for e in range(50):
	sess.JoinSession( str2addr("udp://192.168.1.63:9999"), "test" )
#	sess.JoinSession( str2addr("udp://192.168.1.13:8080") , 0 , [])

man.Run()

print "done"
