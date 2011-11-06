from twisted.internet import stdio, reactor, protocol
from twisted.protocols import basic


class EchoProtocol( protocol.Protocol ):
	def dataReceived(self, data):
		#self.transport.write("haha")
		pass
	
class EchoServerFactory(protocol.ServerFactory):
	protocol  = EchoProtocol


from netio import IOCP


if __name__ == "__main__":
	def onAccept( fd, insock, userdata, sockdata ):
		iocp.recv(insock)
		print "+++",  insock
		
	def onData( fd, data, userdata , sockdata):
		#print fd, data
		if data:
			iocp.recv(fd)
		pass

	port = 5001
	iocp = IOCP( onAccept = onAccept, onData = onData )
	iocp.listen(port, "sockdata_test")
	print "start", port
	iocp.run()
