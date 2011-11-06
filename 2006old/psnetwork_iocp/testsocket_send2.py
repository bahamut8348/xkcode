from netio import IOCP

import threading, time


if __name__ == "__main__":
	

	def sendbuf( fd ):
		while 1:
			try:
				map( lambda x: iocp.send( fd, "*" * 10240 ) , range(10) )
			except:
				return
			time.sleep(0.1)

	def onAccept( fd, insock, userdata, sockdata ):
		print "+++", insock
		iocp.recv(insock)
		threading.Thread( target = sendbuf, args=(insock,) ).start()
		pass
	
	def onResult ( fd, result, userdata, sockdata, byte ):
		if result != 0:
			print fd, result
		pass
	
	def onData( fd, data, userdata , sockdata ):
		#print data
		
		if data == "":
			print "---", fd
		else:
			iocp.recv(fd)
	
	iocp = IOCP( onAccept = onAccept, onResult = onResult , onData = onData)
	fd = iocp.listen( 5001 )
	
	iocp.run()