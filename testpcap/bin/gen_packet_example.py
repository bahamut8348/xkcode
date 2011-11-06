#!/usr/bin/python

from pydhcplib.dhcp_packet import DhcpPacket
from pydhcplib.type_strlist import strlist
from pydhcplib.type_ipv4 import ipv4


packet = DhcpPacket()

packet.SetOption("op",[1])
#packet.SetOption("domain_name",strlist("anemon.org").list())
packet.SetOption("router",ipv4("1.1.1.1").list()+[6,4,2,1])
packet.SetOption("time_server",[100,100,100,7,6,4,2,1])
packet.SetOption("yiaddr",[0,0,0,0])
packet.SetOption("dhcp_message_type",[1])
packet.SetOption("hlen",[6])
packet.SetOption("chaddr",[1,02,3,4,5,6,0,0,0,0,0,0,0,0,0,0,])
packet.SetOption("htype",[1])



print packet.str()
p = packet.EncodePacket()

import socket as S
s = S.socket(S.AF_INET, S.SOCK_DGRAM, 0)
s.setsockopt( S.SOL_SOCKET,  S.SO_BROADCAST, 1)
s.setsockopt(S.SOL_SOCKET,S.SO_REUSEADDR,1)


s.bind( ('1.1.1.106',68) )

s.sendto(p,("255.255.255.255",67))

