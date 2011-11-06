import socket
from ctypes import *
import win32file, pywintypes
import time
import struct


"""
from construct import *
sockaddr = Struct("sockaddr",
	ULInt16("family"),
	ULInt16("port"),
	String("addr",4),
	String("data", 8),
)
"""


class IOCP:

	class ListenSocket:

		def __init__(self, port, data = None ):
			ol = pywintypes.OVERLAPPED()
			ol.Internal = 0
			ol.InternalHigh  = 0
			ol.Offset  = 0
			ol.OffsetHigh  = 0
			ol.hEvent = 0
			self.sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM)
			self.sock.bind(("",port))
			self.sock.listen(5)
			self.sock.setblocking(0)
			fd = self.sock.fileno()
			self.ol = ol
			self.ol.object =[ self.sock ]
			self.buffer = win32file.AllocateReadBuffer(64)
			self.data = data
			self.insock = None
		
		def socket(self):
			return self.sock

	
	def __init__(self, onData = None, onResult = None, onAccept = None ):
		# 	def onData( fd, data, userdata , sockdata):
		#  def onResult( fd, result, userdata, sockdata, byte ):
		#  def onAccept( fd, insock, userdata, sockdata ):
		self.hiocp = win32file.CreateIoCompletionPort(-1,0,1,1)

		ol = pywintypes.OVERLAPPED()
		ol.Internal = 0
		ol.InternalHigh  = 0
		ol.Offset  = 0
		ol.OffsetHigh  = 0
		ol.hEvent = 0

		self.ol = ol
		self.timerol = pywintypes.OVERLAPPED()
		self.timerol.object = [1,1,1]

		self.allfun = []
		
		self.onresult = onResult
		self.ondata = onData
		self.onaccept = onAccept

		self.fds = {}
		self.infds = {}

		self.stop = 0

		import threading
		#threading.Thread(target = self.runtimer ).start()

	def listen(self, port, data = None ):
		sock = self.ListenSocket( port, data )
		win32file.CreateIoCompletionPort( sock.socket().fileno(), self.hiocp, 0,1)
		self.infds[sock.socket()] = sock
		sock.insock = socket.socket( socket.AF_INET, socket.SOCK_STREAM)
		win32file.AcceptEx( sock.socket(), sock.insock, sock.buffer,  sock.ol )


	def addsocket( self, sock, data = None ):
		fd = sock.fileno()

		win32file.CreateIoCompletionPort( fd, self.hiocp, 0,1)

		sendol = pywintypes.OVERLAPPED()
		sendol.Internal = 0
		sendol.InternalHigh  = 0
		sendol.Offset  = 0
		sendol.OffsetHigh  = 0
		sendol.hEvent = 0

		recvol = pywintypes.OVERLAPPED()
		recvol.Internal = 0
		recvol.InternalHigh  = 0
		recvol.Offset  = 0
		recvol.OffsetHigh  = 0
		recvol.hEvent = 0

		self.fds[sock] = [sendol, recvol , win32file.AllocateReadBuffer(1024000), data]
		
		
	def socket(self, (server, port ), data = 1):
		sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM)
		sock.connect( (server,port) )
		sock.setblocking(0)
		self.addsocket(sock, data)
		return sock
		
	
	def callLater(self, timeout, fun, *param ):
		self.allfun.append( [timeout+time.time(), fun, param ] )

	def runtimer(self):
		while not self.stop:
			for e in self.allfun:
				if e[0]<=time.time():
					self.timerol.object=[time.time(), e[1], e[2] ]
					self.postevent()
					self.allfun.remove( e )
			time.sleep(0.001)
	
	def send(self, fd,  data, userdata = None ):
		self.fds[fd][0].object = [userdata, fd]
		return win32file.WSASend( fd, data, self.fds[fd][0],0 )
	
	def recv( self, fd, userdata = None ):
		self.fds[fd][1].object = [userdata, fd, self.fds[fd][2], 1 ]
		win32file.WSARecv( fd, self.fds[fd][2], self.fds[fd][1], 0 )

	def postevent(self):
		self.timerol.object[1]( *self.timerol.object[2])
		#win32file.PostQueuedCompletionStatus( self.hiocp, 10 , 2, self.timerol )
	
	def run(self):
		try:
			while 1:
				rc,b,c,ol = win32file.GetQueuedCompletionStatus( self.hiocp, 1000 )
				if ol:
					#print rc,b,c,ol
					otype = len(ol.object)
					if otype == 2 :
						[userdata, fd] = ol.object
						if self.onresult : self.onresult( fd, rc, userdata, self.fds[fd][3], b )
					elif otype == 1:
						#print rc, b,c,ol
						assert rc == 0
						sock = ol.object[0]
						self.addsocket( self.infds[sock].insock )
						if self.onaccept : self.onaccept( sock,self.infds[sock].insock , 0, self.infds[sock].data )
	
						self.infds[sock].insock = socket.socket( socket.AF_INET, socket.SOCK_STREAM)
						win32file.AcceptEx( sock, self.infds[sock].insock, self.infds[sock].buffer,  self.infds[sock].ol )
					
					elif otype == 3:
						ol.object[1](*ol.object[2])
					elif otype == 4 :
						fd = ol.object[1]
						if self.ondata : self.ondata( fd, ol.object[2][:b], ol.object[0], self.fds[fd][3] )
						if b == 0:
							win32file.CloseHandle( fd.fileno() )
							del self.fds[fd]

		except KeyboardInterrupt, r:
			self.stop = 1
			

if __name__ == "__main__":
	
	def onAccept( fd, insock, userdata, sockdata ):
		print "new connection:", fd, insock, sockdata
		iocp.recv(insock)
	def onData( fd, data, userdata , sockdata):
		#print fd, data
		iocp.recv(fd)

	iocp = IOCP( onAccept = onAccept, onData = onData )
	iocp.listen(2, "sockdata_test")
	iocp.run()
