import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#sock.connect(( 'www.xfocus.net',443 ) )
sock.connect(( '127.0.0.1',300 ) )
ssl = socket.ssl(sock)
ssl.write("get / http/1.1\nhost: www.xfocus.net\n\n")
print ssl.read(1024)