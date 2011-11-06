from PSClient2 import *

import time

total = 0

class Ps:
	ps = None
	n = 0
	bconn = False
	def __init__(self, sid):
		self.ps = PSClient(sid)
		ps = self.ps
		ps.SetIS( str2addr("tcp://192.168.1.63:7788") )
		ps.SetAppInfo(1,3,1,1)
		ps.ConnectToIS()
		ps.SetEventCb(self)
	

	def OnLobbyServerConn(self, *s):
		#print s
		pass
	
	def OnConnectIS(self, bConn, is_id, desc, reason):
		self.bconn = bConn
	

if __name__ == "__main__":
	all = []
	for e in range(1,2):
		c = Ps(e)
		all.append(c)
	print "all create"
	
	while 1:
		for c in all:
			c.ps.Update()
			if c.bconn == False:
				break
		else:
			break
	print "all Connected"
	while 1:		
		for c in all:
			c.ps.Login("Username","password")
		#	print total
			total += 1
			if total%10000==0:
				print total
		
	
	
	
