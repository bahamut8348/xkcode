import stackless as SL
from libarp import tomac, ether_header, arp_pack,query_arp
import winpcap as pcap
import time

cap = pcap.open()

def waitreq():
	for t, data in cap:
		d = str(data)
		C = ether_header.parse(d)
		if C.ether_type != 0x0806: 
			SL.schedule()
			continue
		C = arp_pack.parse(d)
		if C.opcode != 2 : 
			SL.schedule()
			continue
		F=open(time.strftime("%y-%m-%d")+".log","ab")
		F.write("%s %s\r\n" % (C.sender_mac, C.sender_ip) )
		print C.sender_ip
		F.close()
		SL.schedule()


def queryall():
	for a in range(0,255):
		query_arp("192.168.1.%d" % a )
		SL.schedule()
	print "done"

SL.tasklet(waitreq)()
SL.tasklet(queryall)()

SL.run()


