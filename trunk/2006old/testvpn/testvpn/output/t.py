import pyvpn as vpn


c = vpn.Client(1, "1234567", ("192.168.1.128",10002))
c.login("192.168.1.128")

while 1:
	vpn.run()


