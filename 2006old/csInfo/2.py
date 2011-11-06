from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor

from construct import *

byte=ULInt8
dword = ULInt32

Players = Struct("Players",
	byte("Number"),
	Repeater(0,64,
		Struct("PlayerInfo",
			byte("Id"),
			CString("Name"),
			dword("Frag"),
			dword("Time"),
		)
	)
)

ServerInfo = Struct("ServerInfo",
	CString("Host"),
	CString("Name"),
	CString("Map"),
	CString("Mod"),
	CString("ModName"),
	byte("PlayerNumber"),
	byte("Max"),
)

host, port = "219.146.191.99", 37015
host, port = "192.168.1.59", 27015

def tostr(s):
	return s.decode("utf-8").encode("gbk")

class QueryList(DatagramProtocol):
	def startProtocol(self):
		self.queryBase()

	def queryBase( self ):
		self.transport.write("\xff\xff\xff\xff\infostring\0",("255.255.255.255",27015) )
	
	def datagramReceived(self, data, (host, port)):
		#print "received %r from %s:%d" % (data, host, port)
		print `data`
		if data[4] == "m" :
			self.onbaseinfo( data[5:] )

		if data[4] == 'D':
			self.onplayer( data[5:] )
	

class Helloer(DatagramProtocol):

	def startProtocol(self):
		self.transport.connect( host, port)
		#print "we can only send to %s now" % str((host, port))
		self.querybase()
		reactor.callLater( 3, self.querybase )
		self.queryPlayer()
		reactor.callLater( 3, self.queryPlayer )
	

		
	
	def queryPlayer(self):
		self.sendcmd( 0x55 )
		reactor.callLater( 3, self.queryPlayer )

	def sendcmd( self, cmd ):
		self.transport.write("\xFF\xFF\xFF\xFF" + chr(cmd) ) # no need for address
		pass
	
	def querybase(self):
		self.transport.write("\xFF\xFF\xFF\xFF\x54") # no need for address
		pass
	
	def onplayer( self, data ):
		num = data[0]
		print Players.parse(data)
	  
	def datagramReceived(self, data, (host, port)):
		#print "received %r from %s:%d" % (data, host, port)
		#print `data`
		if data[4] == "m" :
			self.onbaseinfo( data[5:] )
		
		if data[4] == 'D':
			self.onplayer( data[5:] )

	def onbaseinfo( self, data ):
		print ServerInfo.parse(data)
    # Possibly invoked if there is no server listening on the
    # address to which we are sending.
	def connectionRefused(self):
		print "No one listening"

# 0 means any port, we don't care in this case
reactor.listenUDP(27015, Helloer())
#reactor.listenUDP(27016, QueryList())
reactor.run()
