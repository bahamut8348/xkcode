import pyvpn

ts =  pyvpn.taps()


def data_got( data, k ):
	#print `data`
	print k[1]
	k[0]()
	

def doread():
	vpn.read()

def doread2():
	vpn2.read()

vpn  = pyvpn.VPN(ts[0], data_got, [doread,"10.2.2.2"] )
vpn.setip("10.2.2.2","255.255.255.0")
vpn.connect(1)
doread()

vpn2  = pyvpn.VPN(ts[1], data_got, [doread,"10.2.2.3"] )
vpn2.setip("10.2.2.3","255.255.255.0")
vpn2.connect(1)
doread2()

while 1:
	pyvpn.run()
	
