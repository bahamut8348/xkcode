from pyPort import *
import time

def disp():
	all = []
	for e in GetConns():
		if e.RemotePort == 7788 and e.State== 5:
			all.append(e)
			#print e.State, e.State == 5
		
	print "Connecting Number:",len(all)
		
if __name__ == "__main__":
	while 1:
		disp()
		time.sleep(1)