import _socket
import thread
import bencode



def waitfordata(talk, sock=None):
	if sock == None: sock = talk.sock
	while 1:
		if talk.stop:
			break
		data = sock.recv(4)
		datalen = int(data)
		data = sock.recv(datalen)
		ret = bencode.bdecode(data)
		talk.OnCmd(ret['cmd'], ret['data'], ret['name'])
		
		
def handleconnect(talk):
	while (1):
		if talk.stop:
			break
		sock, addr = talk.sock.accept()
		print "new", addr
		talk.clients.append([sock,addr])
		thread.start_new_thread(waitfordata, (talk,sock) )


class Talk:
	
	sock = None
	name = ""
	clients=[]
	stop = False
	buf = ""
	def __init__(self, name):
		self.sock = _socket.socket(_socket.AF_INET, _socket.SOCK_STREAM)
		self.name = name
		
		
	def create(self, port):
		self.sock.bind(("",port))
		self.sock.listen(5)
		thread.start_new_thread(handleconnect, (self,) )
		
	
	def open(self, port, server="127.0.0.1"):
		self.sock.connect((server,port))
		thread.start_new_thread(waitfordata, (self,) )
	
	def sendcmd(self, cmd, data):
		self.rawsend({'name':self.name,'cmd':cmd,"data":data})
	
	def rawsend(self, obj):
		data = bencode.bencode(obj)
		l = len(data)
		buf = "%04d%s" % (l, data)
		if self.clients:
			for s in self.clients:
				s[0].send(buf)
		else:
			self.sock.send(buf)
	
	def OnCmd(self, cmd, data):
		pass



class xktalk(Talk):
	def OnCmd(self, cmd, data, name):
		print "OnCmd", cmd, data, name
		
if __name__ == "__main__":
	t = xktalk('xk')
	t.create(1000)
	import time
	time.sleep(1000)
		