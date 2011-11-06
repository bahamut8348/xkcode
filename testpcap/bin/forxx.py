import winpcap as wincap
import dpkt
import re

cap = wincap.open()
cap.setfilter('tcp')
for a, d in cap:
	d = dpkt.ethernet.Ethernet( d )
	# ip, tcp
	try:
		tcp = str(d.data.data.data)
	except:
		continue
	if tcp.find("GET ") == 0:
		ma = re.findall('host:(.*)\s', tcp, re.I)
		url = tcp[4:tcp.find("\n")-10]
		
		print ma[0].strip() + url