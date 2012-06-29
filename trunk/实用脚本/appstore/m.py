import socket

F = open("appdns.txt","wb")
for i in range(1, 10):
	filter(lambda x:F.write(x+"\r\n"), socket.gethostbyname_ex("a%d.phobos.apple.com" % i)[2])
F.close()