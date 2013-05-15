from twisted.internet.protocol import Factory
from twisted.internet.protocol import Protocol
from twisted.internet import reactor

class Echo(Protocol):
	i = -60
	def dataReceived(self, data):
		for d in data.split('^^^^'):
			if d == '' : continue
			i = self.i
			print i, '\t', '0x%02x'%i, '\t',  `d`, '\t',  ['0x%02x'%ord(x) for x in d]
			i = i + 1
			self.i = i
class QOTDFactory(Factory):
	def buildProtocol(self, addr):
		return Echo()

# 8007 is the port you want to run under. Choose something >1024
reactor.listenTCP(8007,QOTDFactory()) 
reactor.run()