import os
import winpcap as wincap
import dpkt 
import threading

def getmac():
	cap = wincap.open()
	cap.setfilter("icmp")
	for a , b in cap:
		d = dpkt.ethernet.Ethernet( b )
		if d.data.data.type == 0:
			print `d`
			open("gwmac","wb").write( d.src )
			print "MAC:", `d.src`
			break


if __name__ == "__main__":
	T = threading.Thread( target = getmac )
	T.start()
	os.system("ping 163.com")
	
	