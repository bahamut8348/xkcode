from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor

import code

SERVER = "127.0.0.1"
PORT1 = 5505
PORT2 = 5507


class UserTest(DatagramProtocol):

    def startProtocol(self):
        self.transport.connect( SERVER, PORT1 )
        self.transport.write( code.enc("hello") )
	
    def datagramReceived(self, data, (host, port)):
        print "received %r from %s:%d" % ( data, host, port )

    # Possibly invoked if there is no server listening on the
    # address to which we are sending.
    def connectionRefused(self):
        print "No one listening"

# 0 means any port, we don't care in this case
reactor.listenUDP(0, UserTest())
reactor.run()





import sys

sys.exit()

import pyvpn as VPN
import dpkt



vs = VPN.taps()

def onData( d, k ):
	dd = dpkt.ethernet.Ethernet( d )
	vpn.read()
	print type( dd.data.data )
	print `dd`
	if isinstance( dd.data.data , dpkt.icmp.ICMP ):
		handleICMP( dd )
	
def handleICMP( d ):
	print `d`


vpn = VPN.VPN( vs[0], onData , 1 )
vpn.connect( True )
vpn.setip("2.2.2.2","255.255.255.0")
vpn.read()

while 1:
	VPN.run()