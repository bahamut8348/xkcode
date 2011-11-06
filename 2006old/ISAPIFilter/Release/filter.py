import socket
import time

def FilterProc(context, code, a):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect( ("127.0.0.1",9191) )
	sock.send("%d\r\n" % code)
	
	
	
	try:
		all_header = context.ServerVariable( "ALL_RAW") 
		sock.send(all_header + "\r\n")
		context.AddHeaders("Server: xk Web Server\r\n")
		context.Function(0,"404 Not Found", "Content-Length: 14\r\nContent-Type: text/html\r\nConnection: Close\r\n\r\n","")
		context.Write("File Not Found")
	except:
		sock.send("Error\r\n")
		
	sock.close()
		
	del sock
	
	return 0x8000000