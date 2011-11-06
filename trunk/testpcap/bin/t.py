import winpcap as wincap
import dpkt

print wincap.findalldevs()[1]
print wincap.findalldevs()
#cap = wincap.open( "c:\\dump.cap")
cap = wincap.open()

#cap.setfilter('port 7788 and host 192.168.1.13')

#print wincap.createsrcstr( "","", "c:\\dump.cap", 2 )

cap.setfilter('tcp')
#dump = cap.makedump("c:/dump.cap")
for a, b in cap:
	d = dpkt.ethernet.Ethernet( b )
	#dump.dump(a, b)
	#dump.flush()
	if d.data.data.data.find('info'):
		if isinstance( d.data.data , dpkt.tcp.TCP ):
			print float(a), `d.data`
			break
	

