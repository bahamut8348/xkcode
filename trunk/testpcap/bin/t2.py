#! coding: utf-8

import winpcap as wincap
import dpkt
import socket
from random import Random
from ctypes import *
from pydhcplib.dhcp_packet import DhcpPacket

WSASocket = windll.ws2_32.WSASocketA
setsockopt = windll.ws2_32.setsockopt
send = windll.ws2_32.send

R = Random()
R.seed()

# gateway mac
mac = "00:24:2c:e7:f8:85"
macaddr="\x00\x24\x2c\xe7\xf8\x85"
s_selfip = "192.168.10.102"
gwip = "192.168.10.1"
dnsip="124.207.162.136"
selfmac="\1\2\3\4\5\6"
import socket

def tonip(s):
	s1 =  socket.inet_aton( s )
	return s1

selfip = tonip( s_selfip )
dnsip = tonip( dnsip )

dhcpip = 1

def getdhcpip():
	dhcpip+=1
	return "1.2.3." + str(dhcpip)



apid = 4
lanid = 2

def tip( s ):
	return ".".join(map( lambda x:str(ord(x)), s))


gwmac = open("gwmac", "rb").read( )

def jj( *args ):
	a = map( lambda x: str(x),  args )
	return "+".join( a )

def genxid():
	#return 0xfb9fa56f
	decxid = R.randint(0,0xffffffff)
	xid = []
	decxid = 0xfb9fa796
	for i in xrange(4):
		xid.insert(0, decxid & 0xff)
		decxid = decxid >> 8
	return xid

class PortMgr:
	def __init__( self ):
		self.inout = {}
		self.outin = {}
		self.minp = 100
		self.maxp = 30000
		self.cp = self.minp
	
	def newcp(self):
		self.cp +=1
		if self.cp >= self.maxp:
			self.cp = self.minp

		return self.cp

	def handleOut( self,srcAddr, srcPort, dstAddr, dstPort  ):
		k = jj( srcAddr , srcPort, dstAddr, dstPort )
		k2 = jj( dstAddr, dstPort )
		if k in self.inout:
			print "返回端口:", self.inout[k]
			return self.inout[k]
		else:
			while 1:
				p = self.cp
				k3 = jj( dstAddr,dstPort,p)
				if k3 in self.outin:
					p = self.newcp()
					continue
				else:
					print "设置新的端口:", p
					self.outin[k3] = srcAddr, srcPort
					self.inout[k] = p
					return p
					

				
	def handleIn(self,  srcAddr, srcPort, dstAddr, dstPort ):
		da, dp = self.outin[ jj( srcAddr, srcPort, dstPort ) ]
		return da, dp


def getcap( i ):
	return wincap.open( wincap.findalldevs( )[i] )

class Router:
	def __init__(self):
		self.tp = PortMgr()
		self.up = PortMgr()
		socket.socket()
		#self.send_s = socket.socket(socket.AF_INET, socket.SOCK_RAW, 255)
		#self.send_s =  wincap.rsocket()
	
	def getselfip( self):
		
		pak = DhcpPacket()
		pak.SetOption("op", [1] )
		#pak.SetOption("time_server", [100,100,100,7,6,4,2,1])
		pak.SetOption("yiaddr", [0,0,0,0])
		pak.SetOption("giaddr", [0,0,0,0])
		pak.SetOption("ciaddr", [0,0,0,0])
		pak.SetOption("dhcp_message_type",[1])
		pak.SetOption("hlen",[6])
		pak.SetOption("flags",[1<<7,0] )
#		pak.SetOption("chaddr",[1,2,3,4,5,6,0,0,0,0,0,0,0,0,0,0] )
		pak.SetOption("chaddr",[0, 0x24, 0x2c, 0xf2, 0x01, 0xe2 ] + [ 0 for x in range(10)] )
		pak.SetOption("xid",genxid( ))
		pak.SetOption("htype",[1])
		pak.SetOption("host_name",[ 0x28, 0x27,0x29])
		
		pak.SetOption("request_ip_address", [1,1,1,111] )
		pak.SetOption("parameter_request_list",[1,28,2,3,15,6,12,40,41,42])
		
		#d = dpkt.dhcp.DHCP( chaddr = selfmac, xid=00332,op=dpkt.dhcp.DHCPDISCOVER, opts=( (dpkt.dhcp.DHCP_OPT_REQ_IP,""),) )
		d = pak.EncodePacket()
	
		i = 300  - len(d)
		u = dpkt.udp.UDP( dport=67, sport=68, data=d + "\x00" * i)
		u.ulen = len(u)
		i = dpkt.ip.IP( dst="\xff"*4, src="\x00"*4, data = u, p=dpkt.ip.IP_PROTO_UDP, ttl=128, tos = 0x10 )
		i.len = len(i)
		e = dpkt.ethernet.Ethernet( dst="\xff" * 6, src=macaddr, data = i )
		print `e`
		cap = getcap( lanid )
		p = "ffffffffffff00242ce7f885080045100148000000001011a99600000000ffffffff0044004301343cda0101060087ee9dc00000800000000000000000000000000000000000000c29f201e2000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006382536332040101016f350101370a011c02030f060c28292aff0000000000000000000000000000000000000000000000000000000000000000000000000000"
		ps = ""
		for s in range( len(p)/ 2 ):
			ps += eval("'\\x"+p[s*2]+p[s*2+1]+"'") 
		ps = e
		cap.send( str(ps) )

	def ap_func( self ):
		global selfmac
		def sendlan( o ):
			o.dst = gwmac
			o.src = selfmac
			print "发送出去"
			print `o`
			print "发送结束"
			#lancap.send( o )
			#self.send_s.sendto(str(o),0,(gwip,53) )
			print "结果:", wincap.sendto( self.send_s, str(o), (gwip,53)  )
		
		print "开始AP线程."
		lancap = getcap( lanid )
		apcap  = getcap( apid  )
		for a, b in apcap:
			d = dpkt.ethernet.Ethernet( b )
			#d.dst = gwmac
			if isinstance ( d.data, dpkt.ip6.IP6): continue
			print `d`
			ip = d.data
			if isinstance( d.data, dpkt.arp.ARP ):
				print "ARP包"
				#sendlan( d )
				continue
			if ip.dst == "\xff" * 6:
				# 广播包，直接转
				print "广播包"
				d.src = selfmac
				lancap.send( d )
			if isinstance( d.data.data, dpkt.udp.UDP ):
				if not selfmac: selfmac = d.dst
				udp = ip.data
				print "UDP 包",tip(ip.src),ip.data.sport,"==>", tip(ip.dst),udp.dport
				if udp.dport == 67:
					print "DHCP"
					pak = DhcpPacket()
					pak.DecodePacket( udp.data )
					print pak.str()
					if pak.GetOption("dhcp_message_type")
					continue
				p = self.up.handleOut( ip.src, ip.data.sport, ip.dst, ip.data.dport )
				d.data.src = selfip
				if( ip.data.dport == 53 ):
					d.data.dst = dnsip
				d.data.sum = 0
				d.data.id = 0
			#	sendlan( d )
			
	def lan_func(self ):
		lancap = getcap( lanid )
		apcap = getcap( apid )
		lancap.setfilter(  )




if __name__ == "__main__":
	r = Router()
	r.ap_func()
#	r.getselfip()
	pass
