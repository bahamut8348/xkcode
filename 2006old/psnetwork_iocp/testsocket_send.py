from netio import IOCP

import threading
import time

if __name__ == "__main__":

	
	
	def sendbuf( fd ):
		a = 1
		while 1:
			map( lambda x: iocp.send(fd, "t" * 1024, a), range(100) )
			a+=1
			time.sleep(0.01)
		#iocp.callLater(0.001, sendbuf, fd, "test" )

	def onData( fd, data, userdata , sockdata):
		print "onData:", fd, data, sockdata, userdata
		if data: iocp.recv(fd, time.time())

	def onResult( fd, result, userdata, sockdata, byte ):
		#print result, userdata,"\r",
		if result != 0:
			print "onResult", fd, result

	iocp = IOCP( onData, onResult )
	fd = iocp.socket( ( "192.168.1.46",5001 ), "test")
	iocp.recv(fd)
	threading.Thread( target = sendbuf, args = (fd,) ).start()
	#iocp.callLater(1, sendbuf, fd, "1" )
	iocp.run()


		


