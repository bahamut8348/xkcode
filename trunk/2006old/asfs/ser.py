# coding: utf-8

import re,string, urllib,os,sys,time



copyright=r"<br /><br /><hr />&copy;<i>KegerX Server 1.0.0</i><br>"

error=r"<title>404 错误</title><h1>错误 404 :<br />没有找到请求的资源!</h1>"

error+=copyright


def getfilectime(filename):
	gmt =time.localtime(os.path.getmtime(filename))
	return time.strftime("%Y-%m-%d %H:%M", gmt)


def getfilemtime(filename):
	gmt =time.localtime(os.path.getmtime(filename))
	return time.strftime("%a, %d %b %Y %H:%M:%S", gmt)


def getnowtime():
	gmt =time.localtime()
	return time.strftime("%a, %d %b %Y %H:%M:%S", gmt)

def getfilesize(filename):
	
	if os.path.isdir(filename):
		buf="       &lt;DIR&gt;         "
		buf=buf.replace(" ","&nbsp;")
	else:
		buf="%21d" % os.path.getsize(filename)
		buf=buf.replace(" ","&nbsp;")
		
	return buf
	
def getdirlist(filename):
	
	header='<meta http-equiv="Content-Type" content="text/html; charset=gb2312">'
	header+="<title>文件浏览</title><h1>文件浏览:</h1><br>"

	buf=header
	
	buf+='<h5>&nbsp;&nbsp;&nbsp;<a href="../">%s</a> 的目录:<br /><br /></h5>'% filename
	
	href=""
	allfile=[]
	if (len(sys.argv) is 1) or filename<>"." :
		allfile=os.listdir(filename)
	else:
		for eachitem in sys.argv[1:]:
			import glob
			flist=glob.glob(eachitem)
			for e in flist:
				allfile.append(os.path.basename(e))
			#if os.path.isdir(eachitem) or os.path.isfile(eachitem):
			#	allfile.append(os.path.basename(eachitem))

	for eachfile in allfile:
	
		href=filename+"/"+eachfile
		if os.path.isdir(href):
			buf+=getfilectime(href)+getfilesize(href)
			buf=buf+'&nbsp;&nbsp;<a href="%s" >%s</a><br/>' % (eachfile+"/",eachfile)
		else:
			buf+=getfilectime(href)+getfilesize(href)
			buf=buf+'&nbsp;&nbsp;<a href="%s" >%s</a><br/>' % (eachfile,eachfile)
	
	return buf+copyright

def getfilepath(str):
	
	p=re.compile("get /+(\S*) http",re.I)
	ma=p.findall(str)
	fileurl=ma[0]
	
	filepath=fileurl.replace("/","\\")
	filepath=urllib.unquote(filepath)
	try:
		filepath = filepath.decode('utf-8').encode('gb2312')
	except:
		pass
		
	print filepath
	return filepath
	
	
def getrange(str):

	p=re.compile("range: bytes=(\d+)-(\d*)")
	ma=p.findall(str)
	return ma
	
def getclose(str):

	if(str.lower().find("connection: close")>0):return True
	return False
	

def makehead(filename,ran,iskeep):
	buf0="HTTP/1.1 {status}\r\n"+\
		"Server: Microsoft-IIS/6.0\r\n"+\
		"X-Powered-By: ASP.NET\r\n"+\
		"Date: {date} GMT\r\n"+\
		"Accept-Ranges: bytes\r\n"+\
		"Content-Type: {contenttype}\r\n"+\
		"Last-Modified: {mtime} GMT\r\n"+\
		"Content-Length: {datalen}\r\n"+\
		"Connection: {keep}\r\n"+\
		"{contentrange}\r\n"
		
	if iskeep:
		buf0 = buf0.replace("{keep}", "Keep-Alive")
	else:
		buf0 = buf0.replace("{keep}", "Close")
	
	
	lastmtime=""
	if os.path.exists(filename):
		if os.path.isdir(filename):
			filesize=len(getdirlist(filename))
			lastmtime = getnowtime()
		else:
			filesize=os.path.getsize(filename)
			lastmtime = getfilemtime(filename)
		
		

		range=""
		if ran[0]=='':
			buf0=buf0.replace("{datalen}","%d" % filesize)
			buf0=buf0.replace("{status}","200 OK")
	
		elif ran[0]!='' and ran[1]=='':
			buf0=buf0.replace("{datalen}","%d" % (filesize-int(ran[0])))
			range="Content-Range: bytes %d-%d/%d\r\n" % (int(ran[0]),(filesize-1),filesize)
			buf0=buf0.replace("{status}","206 Partial content")
			
		elif ran[0]!='' and ran[1]!='':
			buf0=buf0.replace("{datalen}","%d" % (int(ran[1])-int(ran[0])+1))
			range="Content-Range: bytes %d-%d/%d\r\n" % (int(ran[0]),int(ran[1]),filesize)
			buf0=buf0.replace("{status}","206 Partial content")
			
		buf0=buf0.replace("{contenttype}",checktype(filename))
		buf0=buf0.replace("{contentrange}",range)
		
		buf0=buf0.replace("{mtime}",lastmtime)
		buf0=buf0.replace("{date}", getnowtime())
		
		#buf0=buf0.replace("{contentrange}","")
		#print buf0
		return buf0
			
		
	#文件不存在
	else:
		buf0=buf0.replace("{status}","404")
		buf0=buf0.replace("{mtime}",getnowtime());
		buf0=buf0.replace("{contenttype}","text/html; charset=gb2312")
		buf0=buf0.replace("{datalen}","%d" % len(error))
		buf0=buf0.replace("{contentrange}","")
	
	return buf0
	


def checktype(filename):
	
	if os.path.isdir(filename):
		return "text/html"
	
	extname=""

	extnames=[ 
		('text/html','.htm','.html'),
		('application/octet-stream','.exe'),
		('video/x-ms-asf','.asf','.asr','.asx','.voc'),
		('image/bmp','.bmp'),
		('text/plain','.c','.cpp','.py','.txt'),
		('application/msword','.doc','.dot'),
		('application/hta','.hta'),
		('image/jpeg','.jpe','.jpeg','.jpg'),
		('message/rfc822','.mht','.mhtml'),
		('video/uicktime','.mov'),
		('image/png','.png'),
		('image/gif','.gif')
		
		
	]
	name=os.path.basename(filename)
	pos1=name.rfind(".")
	
	#get extend name
	if pos1 is -1: extname= name
	else: extname=name[pos1:].lower()
	
	for eachextname in extnames:
		if extname in eachextname[1:]:
			return eachextname[0]
			
	return 'application/octet-stream'




def senddata(sock,filename,ran):
	if not os.path.exists(filename):
		sock.send(error)
	
		return
		
	elif os.path.isdir(filename):
		sock.send(getdirlist(filename))
		return
	
		
	rf=file(filename,"rb")
	if ran[0]=='':
		while(1):
			buf=rf.read(512000)
			if len(buf)>0:
				sock.send(buf)
			else: break
		rf.close()
		
	elif  ran[0]!='' and ran[1]=='':
		rf.seek(int(ran[0]))
		while(1):
			buf=rf.read(10240)
			if len(buf)>0:
				sock.send(buf)
			else: break
		rf.close()
	
	elif  ran[0]!='' and ran[1]!='':
		rf.seek(int(ran[0]))
		endpos=int(ran[1])
		totallen=endpos-int(ran[0])+1
		sendlen=10240
		while(1):
			if totallen<sendlen:
				sendlen=totallen
			else:
				totallen=totallen-sendlen
				
			buf=rf.read(sendlen)
			if len(buf)>0:
				try:
					sock.send(buf)
				except socket.error:
					break
			else: break
		rf.close()
		

	return

if __name__=="__main__":
	
	print checktype(".")
	gmt =time.localtime(os.path.getctime("sdfsdf"))
	print time.strftime("%Y-%m-%d %H:%M:%S", gmt),getfilesize("rr.rar")
	