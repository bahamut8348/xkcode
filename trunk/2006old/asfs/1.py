# coding: utf-8 

import socket
import threading
import sys, copy, time,ser

iClientCount = 0 #客户端连接数

rootdir="."



def ConnectThread(conn, addr):
	global iClientCount         #引用全局变量
	sock = conn      
	address = copy.copy(addr)
	keep=True
	while keep:
		try:
			msg = sock.recv(1024)
		except socket.error:
			print '>>>A \'receive\' error happened! IP:', address[0],'PORT:', address[1]
			break
	    

		if (msg[0:3].lower()!="get"):break
		
		ran=['','']
		
		if(msg.lower().find("\nrange")>0):
			try:
				ran=ser.getrange(msg.lower())[0]
			except :
				print msg
				print Error
				return
	    
		keep=not ser.getclose(msg)
	    
		filepath=ser.getfilepath(msg)
		filepath=filepath.replace("..","")
		if filepath=='' : 
			fullname=rootdir+filepath
		else:
			fullname="./"+filepath
		buf1=ser.makehead(fullname,ran,keep)
		print ">>>开始发送数据"
		try:
			sock.send(buf1)
			ser.senddata(sock,fullname,ran)
			print ">>>文件发送完毕"
		
			if not keep:break
		except socket.error:
			print '>>>A \'send\' error happened! IP:', address[0], 'PORT:',address[1]
			break

	iClientCount -= 1
	print '>>>Disconnected with', address[0], 'at port', address[1] 
	print '>>>Total number of connections is', iClientCount
	sock.close()

def ListenThread(sock):
    global iClientCount

    while 1:
        if iClientCount >= MAX_CLIENTS:
            time.sleep(1)
            continue
        try:
            conn, addr = sock.accept()
        except socket.error:
            print '>>>An \'accept\' error happened!'
            continue

        iClientCount += 1
        print '>>>Connecting from', addr[0], 'at port', addr[1]
        print '>>>Total number of connections is', iClientCount
        threading.Thread(target=ConnectThread, args=(conn, addr)).start()


if __name__ == '__main__':

    HOST = ''
    PORT = 80
    MAX_CLIENTS = 30

    print '>>>PORT:', PORT
    print '>>>MAX_CLIENTS:', MAX_CLIENTS

    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error:
        print 'Fail to open socket!'
        sys.exit(1)

    print '>>>Socket opened...'

    try:
        sock.bind((HOST, PORT))
    except socket.error:
        print 'Fail to bind port!'
        sock.close()
        sys.exit(1)

    print '>>>Socket binding...'

    try:
        sock.listen(MAX_CLIENTS)
    except socket.error:
        print 'Fail to listen!'
        sock.close()
        sys.exit(1)

    print '>>>Socket listening...'

    threading.Thread(target=ListenThread, args=(sock,)).start()
    
    #此程序不能自己结束:-)
