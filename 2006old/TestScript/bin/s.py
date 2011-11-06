from PSClient2 import *

c = PSClient(1) 


class as():
	ls = None
	def start(self):
		self.ls = LobbyServer()
		ls = self.ls
		ls.Type = 2
		ls.Name = ""
		ls.RegEvent(self)
		ls.SetMLS("tcp://192.168.1.13:2000")
		ls.Start()
	def OnConnectMLS(self, result):
		print "OnConnectMLS", result, GetError(result)
		
	def loop(self):
		while 1:
			self.ls.Update()
	


if __name__ == "__main__":
	print SERVERTYPE.ST_CS
	#s = as()
	#s.start()
	#s.loop()
