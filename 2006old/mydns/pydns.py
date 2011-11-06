

from twisted.internet.protocol import Factory, Protocol
from twisted.internet import reactor
from twisted.names import client, server, dns
import socket

matchdb={"blogspot.com":"72.14.207.190",
	#"farm1.static.flickr.com":"68.142.232.116",
	#"farm1.static.flickr.com":"127.0.0.1",
	#"farm2.static.flickr.com":"69.147.90.156",
	#"zh.wikipedia.org":"145.97.39.132",
}

class SpelDnsReolver(client.Resolver):
	def filterAnswers(self, message):
		name = str(message.queries[0].name).lower()
		print name
		if message.trunc:
			return self.queryTCP(message.queries).addCallback(self.filterAnswers)
		else:
			if(len(message.answers) == 0):
				if matchdb.has_key(name):
					pass
				else:
					return self.queryUDP(message.queries).addCallback(self.filterAnswers)
		for e in matchdb:
			if name.find(e)>-1:
				for ans in message.answers:
					if ans.type == 1: 
						print e, ans.payload.dottedQuad(),"-->", matchdb[e]
						ans.payload.address = socket.inet_aton(matchdb[e])
						
				break
		return (message.answers, message.authority, message.additional)

verbosity = 0
resolver = SpelDnsReolver(servers=[('219.141.140.10', 53)])
f = server.DNSServerFactory(clients=[resolver], verbose=verbosity)
p = dns.DNSDatagramProtocol(f)
f.noisy = p.noisy = verbosity

reactor.listenUDP(53, p)
reactor.listenTCP(53, f)
reactor.run()
