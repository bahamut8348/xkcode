import stackless as SL
import socket as stdsocket

from construct import *

from ctypes import *
from ctypes.wintypes import HANDLE, ULONG, DWORD, BOOL, LPCSTR,LPCWSTR, WinError

import win32file, pywintypes
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
_fileobject = stdsocket._fileobject

_realsocket_old = stdsocket._realsocket


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


def install():
	if stdsocket._realsocket is socket:
		raise StandardError("Still installed")
	stdsocket._realsocket = socket
	stdsocket.socket = stdsocket.SocketType = stdsocket._socketobject = new_socket


class socketmanager:
	def __init__(self):
		self.hiocp = CreateIoCompletionPort(-1, 0, 1, 1)
		self.sockmap={}
		
	def create_socket( self, f, t, p ):
		fake = fake_socket(f, t, p)
#		print "create socket",fake.fileno()
		CreateIoCompletionPort(fake.fileno(), self.hiocp, fake.fileno(), 1)
		self.sockmap[fake.fileno()] = fake
		return fake

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
		ConnectEx( f.fileno(), result.contents.ai_addr, result.contents.ai_addrlen, 0, 0, f.send_len, f.readol )
		#print windll.Ws2_32.WSAGetLastError()
	
	def close_socket( self, f ):
		windll.kernel32.CloseHandle( f.fileno() )
		f.connected = False
		del self.sockmap[f.fileno() ]
	
	def  create_from_socket( self, sock ):
		fake = fake_socket
	
	def accept( self, f ):
		AcceptEx( f.fileno(), f.newsock.fileno(), byref(f.read_buffer),0,32,32, byref(f.recv_len), byref(f.readol) )

	
	def read( self, f):
		
		r = WSARecv( f.fileno(), byref( f.recv_wsabuf ), c_long(1), c_long() , byref(f.recv_flag), byref(f.readol), c_long() )

	def write(self, f ):
		WSASend( f.fileno(), byref( f.send_wsabuf ), c_long(1),  c_long()   ,  c_ulong(), byref(f.writeol), c_long() )
	
	def run(self, timeout = 100):
		n = c_ulong()
		key = c_ulong()
		pol = POINTER(OVERLAPPED)()
		while 1:
			key.value = 0
			r = GetQueuedCompletionStatus( self.hiocp, byref(n), byref(key), byref(pol), timeout )
			print r, pol,  n, key, GetLastError()
			if r == 0:
				if( key.value > 0 ):
					t = pol.contents.Type
					sock = self.sockmap[key.value]
					if t == 3:
						sock.read_ch.send(0)
					self.close_socket(key.value)
				SL.schedule()
				continue
			t = pol.contents.Type
			sock = self.sockmap[key.value]
			if t == 1: 
				if sock.is_server:
					sock.read_ch.send( 1 )
				else:
					sock.read_ch.send( n.value )
			elif t == 2:
				sock.write_ch.send( n.value )
			elif t == 3:
				sock.read_ch.send( 1 )
			SL.schedule()
			
			


sm = socketmanager()

def new_socket( f = AF_INET, t = SOCK_STREAM, p = 0 ):

	return sm.create_socket( f, t, p )

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


_socketobject_old = stdsocket._socketobject




class fake_socket:
	
	def __init__(self, f, t, p ):
		
		self.sock = _realsocket_old(f, t, p)
		
		self.read_buffer_len = c_ulong( 64 )
		self.read_buffer = create_string_buffer(self.read_buffer_len.value)
		self.write_buffer = create_string_buffer(self.read_buffer_len.value)
		
		self.read_ch = SL.channel()
		self.write_ch = SL.channel()
		self.is_server = False

		self.bind_addr=("",0)

		self.send_len = c_ulong()
		self.recv_len = c_ulong()
		
		self.readol = OVERLAPPED()
		self.writeol = OVERLAPPED()

		self.init_readol()
		self.init_writeol()

		self.connected = False
		self.wasConnected  = False

		self.server_addr = None
		
		self.recv_wsabuf = WSABUF()
		self.send_wsabuf = WSABUF()
	
	def init_readol(self):
		ol = self.readol
		ol.InternalHigh = 0
		ol.Internal = 0
		ol.Offset  = 0
		ol.OffsetHigh  = 0
		ol.hEvent = 0
		ol.Type = 1
		

	def init_writeol(self):
		ol = self.writeol
		ol.InternalHigh = 0
		ol.Internal = 0
		ol.Offset  = c_ulong(0)
		ol.OffsetHigh  = 0
		ol.hEvent = 0
		ol.Type = 2
	
	def makefile(self, mode, n ):
		return self.sock.makefile(mode, n)

	
	def connect(self, server ):
		print "connect"
		self.init_readol()
		self.readol.Type = 3
		self.bind(self.bind_addr)
		sm.connect_socket( self, server )
		if 0 == self.read_ch.receive():
			raise error(10061, 'Connection refused')
		self.connected = True
		self.wasConnected = True
	
	@check_still_connected
	def recv(self, size, flag = 0 ):
		print "recv"
		self.read_buffer_len = c_ulong( size )
		self.recv_wsabuf.len = c_ulong(size)
		if size > len( self.read_buffer ): 
			self.read_buffer = create_string_buffer( size )
		self.recv_wsabuf.buf = addressof( self.read_buffer )
		self.init_readol()
		self.recv_flag = c_ulong()
		sm.read( self )
		r = self.read_ch.receive()
		return str(self.read_buffer[:r]) if r else ""
	
	@check_still_connected
	def send(self, buf, flag = 0 ):
		print "send"
		if len(buf) > len(self.write_buffer):
			self.write_buffer = create_string_buffer( buf )
		else:
			self.write_buffer.value = buf
		self.send_wsabuf.len = c_ulong(len(buf))
		self.send_wsabuf.buf = addressof( self.write_buffer )
		sm.write( self  )
		n = self.write_ch.receive()
	
	def sendall(self, buf, flag = 0):
		return self.send(buf, flag)
		
	def fileno(self):
		return self.sock.fileno()
	
	def bind( self, addr ):
		self.bind_addr = addr
		return self.sock.bind( addr )
	
	def listen(self, backlog ):
		self.is_server = True
		return self.sock.listen( backlog )
	
	def close(self):
		self.connected = False
		sm.close_socket(self)
	
	def accept( self ):
		self.init_readol()
		self.newsock = sm.create_socket(AF_INET, SOCK_STREAM,0)
		sm.accept(self)
		r = self.read_ch.receive()
		if r == 1:
			self.newsock.connected = True
			self.newsock.wasConnected = True
			return self.newsock

SL.tasklet(sm.run)(1000)

if __name__ == "__main__":
	
	def startwork( f):
		while 1:
			buf = f.recv(10)
			print `buf`
			f.send( buf )
			SL.schedule()

	def startconnect(f):
		r = f.connect( ("192.168.1.13",80) )
		print "Conn:", r
		pass
	
	def startserver( f ):
		while 1:
			newsock = f.accept()
			print "+++ new", newsock
			SL.tasklet(startwork)(newsock)
			import time
			time.sleep(1)

	sock = new_socket( AF_INET, SOCK_STREAM)
	#sock.bind(("",8000))
	#sock.listen(5)
	SL.tasklet(startconnect)(sock)
	#SL.tasklet(startserver)(sock)
	
	SL.run()