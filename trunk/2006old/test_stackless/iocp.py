import stackless as SL
import socket as stdsocket
import msvcrt
import sys


from construct import *

from ctypes import *
from ctypes.wintypes import HANDLE, ULONG, DWORD, BOOL, LPCSTR,LPCWSTR, WinError

import win32file, pywintypes, win32api

if "__all__" in stdsocket.__dict__:
    __all__ = stdsocket.__dict__
    for k, v in stdsocket.__dict__.iteritems():
        if k in __all__:
            globals()[k] = v
        elif k == "EBADF":
            globals()[k] = v
else:
    for k, v in stdsocket.__dict__.iteritems():
        if k.upper() == k:
            globals()[k] = v
    error = stdsocket.error
    timeout = stdsocket.timeout
    # WARNING: this function blocks and is not thread safe.
    # The only solution is to spawn a thread to handle all
    # getaddrinfo requests.  Implementing a stackless DNS
    # lookup service is only second best as getaddrinfo may
    # use other methods.
    getaddrinfo = stdsocket.getaddrinfo

# urllib2 apparently uses this directly.  We need to cater for that.
_fileobject_old = stdsocket._fileobject

_realsocket_old = stdsocket._realsocket
_socketobject_old = stdsocket._socketobject



_ismanagerRunning = False

NULL = c_ulong()

IOC_WS2 = 0x08000000
IOC_OUT = 0x40000000
IOC_IN = 0x80000000
IOC_INOUT = IOC_IN | IOC_OUT
SIO_GET_EXTENSION_FUNCTION_POINTER =  IOC_INOUT | IOC_WS2 | 6
SOL_SOCKET = 0xffff
SO_PROTOCOL_INFOA = 0x2004
SO_PROTOCOL_INFOW = 0x2005
FILE_FLAG_OVERLAPPED = 0x40000000
GENERIC_READ = 0x80000000

SOCKET = SIZE_T = c_uint

def _error_check(result, func, args):
    if result:
        raise WinError()
    return result

def _bool_error_check(result, func, args):
    if not result:
        raise WinError()
    return result

class sockaddr(Structure):
	_fields_ = [
		('sa_family', c_ushort),
		('sa_data', c_char * 14)
	]


class _US(Structure):
	_fields_ = [
		("Offset", DWORD),
		("OffsetHigh", DWORD),
	]

class _U(Union):
	_fields_ = [
		("s", _US),
		("Pointer", c_void_p),
	]

	_anonymous_ = ("s",)

class OVERLAPPED(Structure):
	_fields_ = [
		("Internal", ULONG ),
		("InternalHigh", ULONG ),
		("Offset", DWORD ),
		("OffsetHigh", DWORD ),
		("hEvent", HANDLE ),
		("Type", ULONG),
	]

	
class in_addr(Structure):
	_fields_ = [
		('s_b1', c_ubyte),
		('s_b2', c_ubyte),
		('s_b3', c_ubyte),
		('s_b4', c_ubyte)
	]

class sockaddr_in(Structure):
	_fields_ = [
		('sin_family', c_ushort),
		('sin_port', c_ushort),
		('sin_addr', in_addr),
		('sin_zero', c_char * 8)
	]

class WSABUF(Structure):
	_fields_ = [
		('len', c_ulong),
		('buf', c_ulong)
	]

class GUID(Structure):
	_fields_ = [
		('Data1', c_ulong),
		('Data2', c_ushort),
		('Data3', c_ushort),
		('Data4', c_ubyte * 8)
	]

	def __init__(self, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8):
		self.Data1 = l
		self.Data2 = w1
		self.Data3 = w2
		self.Data4[:] = (b1, b2, b3, b4, b5, b6, b7, b8)

MAX_PROTOCOL_CHAIN = 7
WSAPROTOCOL_LEN = 255
class WSAPROTOCOLCHAIN(Structure):
	_fields_ = [
		('ChainLen', c_int),
		('ChainEntries', DWORD * MAX_PROTOCOL_CHAIN)
	]

class WSAPROTOCOL_INFO(Structure):
	_fields_ = [
		('dwServiceFlags1', DWORD),
		('dwServiceFlags2', DWORD),
		('dwServiceFlags3', DWORD),
		('dwServiceFlags4', DWORD),
		('dwProviderFlags', DWORD),
		('ProviderId', GUID),
		('dwCatalogEntryId', DWORD),
		('ProtocolChain', WSAPROTOCOLCHAIN),
		('iVersion', c_int),
		('iAddressFamily', c_int),
		('iMaxSockAddr', c_int),
		('iMinSockAddr', c_int),
		('iSocketType', c_int),
		('iProtocol', c_int),
		('iProtocolMaxOffset', c_int),
		('iNetworkByteOrder', c_int),
		('iSecurityScheme', c_int),
		('dwMessageSize', DWORD),
		('dwProviderReserved', DWORD),
		('szProtocol', c_char * (WSAPROTOCOL_LEN+1)),
	]
 

class addrinfo(Structure):
    pass

addrinfo._fields_ = [
	('ai_flags', c_int),
	('ai_family', c_int),
	('ai_socktype', c_int),
	('ai_protocol', c_int),
	('ai_addrlen', c_uint),
	('ai_canonname', c_char_p),
	('ai_addr', POINTER(sockaddr)),
	('ai_next', POINTER(addrinfo))
]


WSAID_CONNECTEX = GUID(0x25a207b9,0xddf3,0x4660,0x8e,0xe9,0x76,0xe5,0x8c,0x74,0x06,0x3e)
WSAID_ACCEPTEX = GUID(0xb5367df1,0xcbac,0x11cf,0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92)


WSAIoctl = windll.ws2_32.WSAIoctl
WSAIoctl.argtypes = (SOCKET, DWORD, c_void_p, DWORD, c_void_p, DWORD, POINTER(DWORD), POINTER(OVERLAPPED), c_void_p)
WSAIoctl.restype = c_int
WSAIoctl.errcheck = _error_check


#print win32file.ConnectEx

ConnectExType = WINFUNCTYPE(bool, c_int, POINTER(sockaddr), c_int, DWORD, DWORD, POINTER(DWORD), POINTER(OVERLAPPED))
def _GetConnectExPtr(given_socket=None):
	bogus_sock = given_socket or stdsocket.socket()
	bogus_bytes = DWORD()
	ConnectEx = ConnectExType(0)
	ret = WSAIoctl(
		bogus_sock.fileno(), SIO_GET_EXTENSION_FUNCTION_POINTER, byref(WSAID_CONNECTEX), sizeof(WSAID_CONNECTEX),
		byref(ConnectEx), sizeof(ConnectEx), byref(bogus_bytes), None, None
	)
	return ConnectEx


ConnectEx = _GetConnectExPtr()
GetQueuedCompletionStatus = windll.kernel32.GetQueuedCompletionStatus
CreateIoCompletionPort = windll.kernel32.CreateIoCompletionPort
c_getsockopt = windll.ws2_32.getsockopt
c_getaddrinfo = windll.ws2_32.getaddrinfo
AcceptEx = windll.Mswsock.AcceptEx
WSARecv = windll.ws2_32.WSARecv
WSASend = windll.ws2_32.WSASend
CloseHandle = windll.kernel32.CloseHandle
GetAcceptExSockaddrs  = windll.Mswsock.GetAcceptExSockaddrs
ReadFileEx = windll.kernel32.ReadFileEx
ReadFileEx.argtypes = ( HANDLE, c_void_p, DWORD, POINTER(OVERLAPPED),DWORD )
ReOpenFile = windll.kernel32.ReOpenFile
ReOpenFile.argtypes = ( HANDLE, DWORD, DWORD, DWORD )
ReadFile = windll.kernel32.ReadFile
_get_osfhandle = msvcrt.get_osfhandle



def install():
	if stdsocket._realsocket is socket:
		raise StandardError("Still installed")
	stdsocket._realsocket = socket
	stdsocket.socket = stdsocket.SocketType = stdsocket._socketobject = fake_socket


class object_manager:
	def __init__(self):
		self.hiocp = CreateIoCompletionPort(-1, 0, 1, 1)
		self.objmap={}
		
	def create_object( self,  obj ):
		CreateIoCompletionPort( obj.fileno(), self.hiocp, obj.fileno(), 1 )
		if self.objmap.has_key( obj.fileno() ): 
			raise "has key"
		self.objmap[ obj.fileno() ] = obj
		return obj
	
	def create_file( self, obj, right, sharemode  = 1):
		#h =  _get_osfhandle( obj.fileno() ) 
		#print h
		#h = ReOpenFile( h , c_ulong(right), c_ulong(sharemode), c_ulong(FILE_FLAG_OVERLAPPED) )
		#obj.set_handle( h )
		#print "handle",obj.get_handle()
		CreateIoCompletionPort( obj.get_handle(), self.hiocp, obj.fileno(), 1 )
		if self.objmap.has_key( obj.fileno() ):
			raise "has key"
		self.objmap[ obj.fileno() ] = obj
		return obj
	
		
	
	def connect_socket(self, f, server ):
		prot_info = WSAPROTOCOL_INFO()
		prot_info_len = c_int(sizeof(prot_info))
		c_getsockopt(f.fileno(), SOL_SOCKET, SO_PROTOCOL_INFOA, cast(byref(prot_info), c_char_p), byref(prot_info_len))
		hints = addrinfo()
		hints.ai_family = prot_info.iAddressFamily
		hints.ai_socktype = prot_info.iSocketType
		hints.ai_protocol = prot_info.iProtocol
		result = POINTER(addrinfo)()
		c_getaddrinfo(server[0], str(server[1]), byref(hints), byref(result))
#		print server
		ConnectEx( f.fileno(), result.contents.ai_addr, result.contents.ai_addrlen, 0, 0, f.send_len, f.recvol )
		#print windll.Ws2_32.WSAGetLastError()
	
	def close_socket( self, f ):
		CloseHandle( f.fileno() )
		f.connected = False
		del self.objmap[f.fileno() ]
	
	def close_file(self, f ):
		CloseHandle( f.handle() )
		del self.objmap[f.fileno() ]
	
	def accept( self, f ):
		AcceptEx( f.fileno(), f.newsock.fileno(), byref(f.recv_buffer),0,32,32, byref(f.recv_len), byref(f.recvol) )
	
	def read_file( self, f, length ):
		print "read file:", f.get_handle(), length, f.read_buffer,f.readol
		print ReadFileEx( f.get_handle(), f.read_buffer, length, f.readol , NULL) # byref(f.readol)
		print windll.kernel32.GetLastError(), f.read_buffer.value
	
	def recv( self, f):
		r = WSARecv( f.fileno(), byref( f.recv_wsabuf ), c_long(1), c_long() , byref(f.recv_flag), byref(f.recvol), c_long() )
		print "recv",r


	def send(self, f ):
		WSASend( f.fileno(), byref( f.send_wsabuf ), c_long(1),  NULL   ,  NULL, byref(f.sendol), NULL )
	
	def run(self, timeout = 100):
		n = c_ulong()
		key = c_ulong()
		pol = POINTER(OVERLAPPED)()
		while 1:
			key.value = 0
			r = GetQueuedCompletionStatus( self.hiocp, byref(n), byref(key), byref(pol), c_ulong(timeout) )
			print r, pol,  n, key, GetLastError()
			if r == 0:
				if( key.value > 0 ):
					t = pol.contents.Type
					obj = self.objmap[key.value]
					if t == 4:
						obj.recv_ch.send( 0 )
					self.close_socket(obj)
				SL.schedule()
				continue
			# 1 recv, 2 send, 3 accept, 4 connect, 5 read 6 write
			t = pol.contents.Type
			sock = self.objmap[key.value]
			if t == 1:
				if sock.is_server:
					sock.recv_ch.send( 1 )
				else:
					sock.recv_ch.send( n.value )
			elif t == 2:
				sock.send_ch.send( n.value )
			elif t == 3:
				sock.recv_ch.send( 1 )
			elif t == 4:
				sock.recv_ch.send(r)
			elif t == 5:
				sock.read_ch.send(n.value)

			SL.schedule()
			
			


om = object_manager()



def check_still_connected(f):
    " Decorate socket functions to check they are still connected. "
    def new_f(self, *args, **kwds):
        if not self.connected:
            # The socket was never connected.
            if not self.wasConnected:
                raise error(10057, "Socket is not connected")
            # The socket has been closed already.
            raise error(EBADF, 'Bad file descriptor')
        return f(self, *args, **kwds)
    return new_f


delattr(stdsocket._socketobject,"send")

class fake_socket( _socketobject_old ):
	
	def __init__(self, f=AF_INET, t=SOCK_STREAM, p=0, s=None ):
		
		
		#_socketobject_old.__init__(self,f, t, p, s)
		self._sock = _realsocket_old(f,t,p)
		
		om.create_object( self )
		self.recv_buffer =  create_string_buffer( 64 )
		self.send_buffer =  create_string_buffer( 64 )
		
		self.send_ch = SL.channel( )
		self.recv_ch = SL.channel( )

		self.is_server = False

		self.bind_addr=( "", 0 )

		self.send_len = c_ulong()
		self.recv_len = c_ulong()
		
		self.recvol = OVERLAPPED()
		self.sendol = OVERLAPPED()

		self.init_recvol()
		self.init_sendol()

		self.connected = False
		self.wasConnected  = False

		self.server_addr = None
		
		self.recv_wsabuf = WSABUF()
		self.send_wsabuf = WSABUF()
	
	def init_recvol(self, t = 1 ):
		ol = self.recvol
		ol.InternalHigh = 0
		ol.Internal = 0
		ol.Offset  = 0
		ol.OffsetHigh  = 0
		ol.hEvent = 0
		ol.Type = t
	
	def makefile(self, mode="rw", bufsize = 0):
		return sock_file( self.fileno() )
		

	def init_sendol(self, t = 2 ):
		ol = self.sendol
		ol.InternalHigh = 0
		ol.Internal = 0
		ol.Offset  = c_ulong(0)
		ol.OffsetHigh  = 0
		ol.hEvent = 0
		ol.Type = t
	
	def connect(self, server ):
		
		self.init_recvol(4)
		self.bind(self.bind_addr)
		om.connect_socket( self, server )
		if 0 == self.recv_ch.receive():
			raise error(10061, 'Connection refused')
		self.connected = True
		self.wasConnected = True
	
	@check_still_connected
	def recv(self, size, flag = 0 ):
		self.recv_wsabuf.len = c_ulong(size)
		if size > len( self.recv_buffer ):
			self.recv_buffer = create_string_buffer( size )
		self.recv_wsabuf.buf = addressof( self.recv_buffer )
		self.init_recvol()
		self.recv_flag = c_ulong()
		om.recv( self )
		r = self.recv_ch.receive()
		return str(self.recv_buffer[:r]) if r else ""
	
	#@check_still_connected
	def send(self, buf ):
		if len(buf) > len(self.send_buffer):
			self.send_buffer = create_string_buffer( buf )
		else:
			self.send_buffer.value = buf
		self.send_wsabuf.len = c_ulong(len(buf))
		self.send_wsabuf.buf = addressof( self.send_buffer )
		om.send( self )
		n = self.send_ch.receive()
		return n


	def close(self):
		self.connected = False
		om.close_socket(self)
	
	def accept( self ):
		self.init_recvol(3)
		self.newsock = fake_socket()
		om.accept(self)
		r = self.recv_ch.receive()
		if r == 1:
			self.newsock.connected = True
			self.newsock.wasConnected = True
			localaddr = sockaddr_in()
			remoteaddr = sockaddr_in()
			localaddr_len = c_ulong(16)
			remoteaddr_len = c_ulong(16)
			GetAcceptExSockaddrs( byref( self.recv_buffer),c_ulong(), c_ulong(32),c_ulong(32), byref(localaddr), byref(localaddr_len), byref(remoteaddr), byref(remoteaddr_len) ) 
			return self.newsock, ("%d.%d.%d.%d" % (remoteaddr.sin_addr.s_b4,remoteaddr.sin_addr.s_b3,remoteaddr.sin_addr.s_b2,remoteaddr.sin_addr.s_b1) , stdsocket.ntohs(remoteaddr.sin_port) )
		raise "accept Error"

stdfile = file

import os

	

class fake_file( stdfile ):
	
	readol = OVERLAPPED()
	writeol = OVERLAPPED()
	
	read_buffer = create_string_buffer( 64 )
	write_buffer = create_string_buffer( 64 )
	
	read_ch = SL.channel()  
	write_ch = SL.channel() 
	
	def __init__( self, fn, mode = "r", buffering=-1 ):
		stdfile.__init__( self, fn, mode, buffering )
		if mode.find("r") != -1: right = GENERIC_READ
		else: right = 0x40000000 | GENERIC_READ
		self.set_handle( _get_osfhandle( self.fileno() )  )
		om.create_file( self,  right )
		
		self.write_len = c_ulong()
		self.read_len = c_ulong()

	def set_handle(self, h ):
		self._file_handle = h
	
	def get_handle(self):
		return self._file_handle
	

	
	def init_readol(self, t  ):
		ol = self.readol
		ol.InternalHigh = 0
		ol.Internal = 0
		ol.Offset  = 0
		ol.OffsetHigh  = 0
		ol.hEvent = 0
		ol.Type = 5

	def read( self, size = 0 ):
		#print "read", self.tell()
		if size > 0:
			self.init_readol(1)
			if size > len( self.read_buffer ): 
				self.read_buffer = create_string_buffer( size )
			om.read_file( self, size )
		s = self.read_ch.receive()
		return self.read_buffer[:s]

class sock_file( ):
	def __init__( self, no ):
		print "file:",no
		hpros = win32api.GetCurrentProcess( )
		self.fno =  win32api.DuplicateHandle( hpros, no, hpros, 0, 0, 2)
		om.create_object( self )
		self.buffer = ""
		self.recv_buffer =  create_string_buffer( 64 )
		self.send_buffer =  create_string_buffer( 64 )
		
		self.send_ch = SL.channel( )
		self.recv_ch = SL.channel( )

		self.recvol = OVERLAPPED()
		self.sendol = OVERLAPPED()

		self.recv_wsabuf = WSABUF()
		self.send_wsabuf = WSABUF()
	
	def init_recvol(self, t = 1 ):
		ol = self.recvol
		ol.InternalHigh = 0
		ol.Internal = 0
		ol.Offset  = 0
		ol.OffsetHigh  = 0
		ol.hEvent = 0
		ol.Type = t

	
	def read(self,  size , flag =0 ):
		if size > len(self.recv_buffer):
			self.recv_buffer = create_string_buffer( size )
		self.recv_wsabuf.buf = addressof( self.recv_buffer )
		self.init_recvol()
		self.recv_flag = c_ulong()
		om.recv(self)
		s = self.recv_ch.receive()
		print s
		return self.buffer + self.recv_buffer[:s]

	def readline(self):
		print "readline"
		pos = self.buffer.find("\n")
		if pos  != -1:
			ret = self.buffer[:pos+1]
			self.buffer = self.buffer[pos+1:]
			return ret
		else:
			self.buffer = self.read(1024)
			print self.buffer
			return self.readline()
	
	def fileno(self):
		return int(self.fno)

open = fake_file

SL.tasklet(om.run)(1000)

if __name__ == "__main__":
	import time
	def startwork( f ):
		while 1:
			buf = f.recv(10)
			print `buf`
			f.send( buf )
			
			SL.schedule()

	def startconnect():
		f = fake_socket()
		print f
		r = f.connect( ("192.168.1.13",80) )
		print f
		print "Connected", r
		F = f.makefile()
		while 1:
			F.read(10)
			print 1
			time.sleep(1)
			SL.schedule()
	
	
	def startserver(  ):
		f = fake_socket()
		f.bind(("",8000))
		f.listen(5)
		while 1:
			newsock, addr = f.accept()
			print "+++ new", newsock
			SL.tasklet(startwork)(newsock)
			
			time.sleep(1)

	def readfile():
		F = open("p.txt")
		print F
		print 'content:', F.read(10)

	#sock.bind(("",8000))
	#sock.listen(5)
	#SL.tasklet(startserver)()
	SL.tasklet(startconnect)()
	#SL.tasklet(readfile)()
	SL.run()