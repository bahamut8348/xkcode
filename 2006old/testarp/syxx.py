#000FE21E92B6
import winpcap as pcap
import dpkt
import socket, time

from construct import *

print "sdf"

short=UBInt16
byte = UBInt8

class MacAddrAdapter( Adapter ):
	def _encode(self, s, context ):
		return eval("'\\x" + "\\x".join([ s[x:x+2] for x in range(0,12,2)])+"'")
	
	def _decode(self, obj, context):
		return  "".join( ( ( "%02X" % ord(x)) for x in obj) )

class IpAddressAdapter(Adapter):
	def _encode(self, obj, context):
		return "".join(chr(int(b)) for b in obj.split("."))
	def _decode(self, obj, context):
		return ".".join(str(ord(b)) for b in obj)

def IpAddress(name): return IpAddressAdapter(Bytes(name, 4))

def MacAddr(name): return  MacAddrAdapter(Bytes(name,6))

mac = Struct("mac",
	Embed(MetaField("data",6)),
)

ip = Struct("ip",
	Embed(MetaField("data", 4 )),
)

ether_header = Struct("ether",
	MacAddr("dst"),
	MacAddr("src"),
	short("ether_type"),
)

arp_pack = Struct("arp",
	Embed(ether_header)	,
	short("hardtype"),
	short("proto"),
	byte("hardsize"),
	byte("protosize"),
	short("opcode"),
	MacAddr( "sender_mac"),
	IpAddress( "sender_ip"),
	MacAddr( "target_mac"),
	IpAddress( "target_ip"),
)


devs = pcap.findalldevs()
cap = pcap.open()
#for a, data in cap:
#	print `dpkt.ethernet.Ethernet(data).data`
def tomac(s):
	return eval("'\\x" + "\\x".join([ s[x:x+2] for x in range(0,12,2)])+"'")

def test():
	srcmac = "0019e24daa0c"
	srcmac = "003018A2E825"
	dstmac = "ffffffffffff"
	#dstmac = tomac("0019e24daa0c")
	#dstmac = tomac('0013026aaa35')

	buf = arp_pack.build( Container(
			dst= dstmac,
			src= srcmac,
			hardtype = 1,
			ether_type = 0x0806,
			proto = 0x0800,
			hardsize = 6,
			protosize = 4,
			opcode = 2,
			sender_mac = srcmac,
			sender_ip = "192.168.1.16",
			target_mac = dstmac,
			target_ip = "192.168.1.255",
			data = "\x00" * 18,
		)
	)
	print `buf`
	cap.send(buf)

def send_arp( ip, mac ):
	srcmac = mac
	dstmac = "ffffffffffff"
	#dstmac = tomac("0019e24daa0c")
	#dstmac = tomac('0013026aaa35')

	buf = arp_pack.build( Container(
			dst= dstmac,
			src= srcmac,
			hardtype = 1,
			ether_type = 0x0806,
			proto = 0x0800,
			hardsize = 6,
			protosize = 4,
			opcode = 2,
			sender_mac = srcmac,
			sender_ip = ip,
			target_mac = dstmac,
			target_ip = "192.168.1.255",
			data = "\x00" * 18,
		)
	)
	
	print `buf`

def query_arp( ipaddr ):
	srcmac = "003018a6b297"
	dstmac = "ffffffffffff"
	buf = arp_pack.build( Container(
		dst= dstmac,
		src= srcmac,
		hardtype = 1,
		ether_type = 0x0806,
		proto = 0x0800,
		hardsize = 6,
		protosize = 4,
		opcode = 1,
		sender_mac = srcmac,
		sender_ip = "192.168.1.128",
		target_mac = "0"*12,
		target_ip = ipaddr,
		data = "\x00" * 18,
		)
	)
	
	
	gateway = '\xff\xff\xff\xff\xff\xff\x00\x0f\xe2\x1e\x92\xb6\x08\x06\x00\x01\x08\x00\x06\x04\x00\x02\x00\x0f\xe2\x1e\x92\xb6\xac\x16\x14\x01\xff\xff\xff\xff\xff\xff\xc0\xa8\x01\xff'


if __name__ == "__main__":
	#query_arp('192.168.1.35')
	import time
	while 1:
		send_arp("172.22.20.1", "000FE21E92B6")
		time.sleep(1000)
		