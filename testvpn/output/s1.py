import socket
import select, time
# two port
import code 

class User :
	def __init__ ( self, name, ip ):
		self.name = name
		self.addr = ip
	
	def __str__( self):
		return "Test User %s, %s" % (self.name, self.addr ) 

class Server:
	
	def __init__( self ):
		self.s1 = self.newsock( 5505 ) # test
		self.s2 = self.newsock( 5507 ) # main
		
		self.test_socks = {}
		self.login_socks = {}

		self.users = {}
	
	def del_user( self, u ):
		del self.users[u]
		print "del_user", u

	def add_user( self, u ):
		if u.name in self.users:
			self.del_user( u.name )
		
		self.users[u.name] = u
		print "add_user", u.name

	def newsock( self, port ):
		sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM , 0 )
		sock.bind( ("", port ) )
		return sock
	
	def run_once( self ):
		sl = select.select( [self.s1, self.s2],[],[], 2 )
		for s in sl[0]:
			if s == self.s1 : 
				self.handle_s1( s )
			elif s == self.s2 : 
				self.handle_s2( s )
	
	def handle_s1( self, sock ):
		data ,conn = sock.recvfrom( 2048 )
		print "sock 1", data, conn
		d = code.dec(data)
		name = d.u
		user = User( name , conn )
		self.test_socks[ name ] = user
		print "test user", user
	
	def handle_s2( self, sock ):
		pass

	def run( self ):
		while 1:
			self.run_once( )

if __name__ == "__main__" :
	ser = Server( )
	print "server start"
	ser.run( )

