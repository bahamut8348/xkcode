from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor
from twisted.application.internet import MulticastServer

class	MulticastServerUDP(DatagramProtocol):
	def startProtocol(self):
		print 'Started Listening'
		# Join	a specific multicast	group, which is the IP we will respond	to
		#self.transport.joinGroup('255.255.255.255')
		reactor.callLater( 0 , self.query )
	def query(self):
		self.transport.write("\xff\xff\xff\xffinfostring\x00",("192.168.1.255",27015))
		reactor.callLater( 5 , self.query )

	def datagramReceived(self, datagram, address):
		# The uniqueID check is to ensure	we	only service requests from
		# ourselves
		print address
		for e in datagram.split("\\"):
			print e
		if datagram == 'UniqueID':
			print	"Server Received:" +	repr(datagram)
			self.transport.write("data", address)

# Note that	the join	function	is	picky	about	having a	unique object
# on which to call join.  To avoid using startProtocol, the	following is
# sufficient:
#reactor.listenMulticast(8005, MulticastServerUDP()).join('224.0.0.1')

# Listen	for multicast on 224.0.0.1:8005
reactor.listenMulticast(27015, MulticastServerUDP())
reactor.run()
